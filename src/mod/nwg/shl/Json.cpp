// ---------------------------------------------------------------------------
// - Json.cpp                                                                -
// - afnix:csm service - json class implementation                           -
// ---------------------------------------------------------------------------
// - This program is free software;  you can redistribute it  and/or  modify -
// - it provided that this copyright notice is kept intact.                  -
// -                                                                         -
// - This program  is  distributed in  the hope  that it will be useful, but -
// - without  any  warranty;  without  even   the   implied    warranty   of -
// - merchantability or fitness for a particular purpose.  In no event shall -
// - the copyright holder be liable for any  direct, indirect, incidental or -
// - special damages arising in any way out of the use of this software.     -
// ---------------------------------------------------------------------------
// - copyright (c) 1999-2021 amaury darsch                                   -
// ---------------------------------------------------------------------------

#include "Set.hpp"
#include "Json.hpp"
#include "Plist.hpp"
#include "Vector.hpp"
#include "Boolean.hpp"
#include "HashTable.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - private section                                                       -
  // -------------------------------------------------------------------------

  // forward object stringify
  static bool json_stry (Buffer& jbuf, Object* obj);
  
  // stringify a literal into a buffer
  static bool json_stry (Buffer& jbuf, const Literal& lobj) {
    jbuf.add(lobj.toliteral());
    return true;
  }

  // stringify a vector into a buffer
  static bool json_stry (Buffer& jbuf, const Vector& vobj) {
    // initialize the array
    jbuf.add ('[');
    // loop in the vector
    long vlen = vobj.length ();
    for (long k = 0L; k < vlen; k++) {
      Object* obj = vobj.get (k);
      if (obj == nullptr) continue;
      // add separator
      if (k > 0L) jbuf.add (',');
      // add the object
      if (json_stry (jbuf, obj) == false) return false;
    }
    // finish the array
    jbuf.add (']');
    return true;
  }

  // stringify a set into a buffer
  static bool json_stry (Buffer& jbuf, const Set& sobj) {
    // initialize the array
    jbuf.add ('[');
    // loop in the set
    long slen = sobj.length ();
    for (long k = 0L; k < slen; k++) {
      Object* obj = sobj.get (k);
      if (obj == nullptr) continue;
      // add separator
      if (k > 0L) jbuf.add (',');
      // add the object
      if (json_stry (jbuf, obj) == false) return false;
    }
    // finish the array
    jbuf.add (']');
    return true;
  }

  // stringify a hashtable into a buffer
  static bool json_stry (Buffer& jbuf, const HashTable& hobj) {
    // initialize the object
    jbuf.add ('{');
    // loop in the table
    long hlen = hobj.length ();
    for (long k = 0L; k < hlen; k++) {
      String  key = hobj.getkey (k);
      Object* obj = hobj.getobj (k);
      if ((key.isnil () == true) || (obj == nullptr)) continue;
      // add separator
      if (k > 0L) jbuf.add (',');
      // add the key
      jbuf.add (key.toliteral());
      jbuf.add (':');
      // add the object
      if (json_stry (jbuf, obj) == false) return false;
    }
    // finish the object
    jbuf.add ('}');
    return true;
  }

  // stringify a plist into a buffer
  static bool json_stry (Buffer& jbuf, const Plist& pobj) {
    // initialize the object
    jbuf.add ('{');
    // loop in the plist
    long plen = pobj.length ();
    for (long k = 0L; k < plen; k++) {
      Property* prop = pobj.get (k);
      if (prop == nullptr) continue;
      // get the property value as aliteral
      String   name = prop->getname ();
      Literal* lobj = prop->toliteral ();
      if ((name.isnil() == true) || (lobj == nullptr)) continue;
      // add separator
      if (k > 0L) jbuf.add (',');
      // add the name
      jbuf.add (name.toliteral());
      jbuf.add (':');
      // add the literal
      if (json_stry (jbuf, *lobj) == false) {
	delete lobj;
	return false;
      }
      delete lobj;
    }
    // finish the object
    jbuf.add ('}');
    return true;
  }

  // stringify a printtable into a buffer
  static bool json_stry (Buffer& jbuf, const PrintTable& ptbl) {
    // initialize the object
    jbuf.add ('[');
    // loop in the rows
    long rows = ptbl.getrows ();
    for (long i = 0L; i < rows; i++) {
      if (i > 0L) jbuf.add (',');
      jbuf.add ('[');
      // loop in the columns
      long cols = ptbl.getcols ();
      for (long j = 0L; j < cols; j++) {
	if (j > 0L) jbuf.add(',');
	String sval = ptbl.get (i, j);
	jbuf.add (sval.toliteral());
      }
      jbuf.add(']');
    }
    jbuf.add(']');
    return true;
  }
  
  // stringify an object into a buffer
  static bool json_stry (Buffer& jbuf, Object* obj) {
    // check for nil
    if (obj == nullptr) return false;
    // check for a literal
    auto lobj = dynamic_cast<Literal*>(obj);
    if (lobj != nullptr) return json_stry (jbuf, *lobj);
    // check for a vector
    auto vobj = dynamic_cast<Vector*>(obj);
    if (vobj != nullptr) return json_stry (jbuf, *vobj);
    // check for a set
    auto sobj = dynamic_cast<Set*>(obj);
    if (sobj != nullptr) return json_stry (jbuf, *sobj);
    // check for a hashtable
    auto hobj = dynamic_cast<HashTable*>(obj);
    if (hobj != nullptr) return json_stry (jbuf, *hobj);
    // check for a plist
    auto pobj = dynamic_cast<Plist*>(obj);
    if (pobj != nullptr) return json_stry (jbuf, *pobj);
    // check for a printtable
    auto tobj = dynamic_cast<PrintTable*>(obj);
    if (tobj != nullptr) return json_stry (jbuf, *tobj);
    // nothing to convert
    return false;
  }
  
  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a default json
  
  Json::Json (void) {
    reset ();
  }

  // return the class name

  String Json::repr (void) const {
    return "Json";
  }

  // reset this object

  void Json::reset (void) {
    wrlock ();
    try {
      d_jbuf.reset ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the buffer string value

  String Json::tostring (void) const {
    rdlock ();
    try {
      String result = d_jbuf.tostring ();
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // stringify an object into the json buffer

  bool Json::stringify (Object* obj) {
    wrlock ();
    try {
      // stringify the object
      bool result = json_stry(d_jbuf, obj);
      // reset the buffer in case of error
      if (result == false) d_jbuf.reset ();
      // unlock and return
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // -------------------------------------------------------------------------
  // - object section                                                        -
  // -------------------------------------------------------------------------

  // the quark zone
  static const long QUARK_ZONE_LENGTH = 3;
  static QuarkZone  zone (QUARK_ZONE_LENGTH);

  // the object supported quarks
  static const long QUARK_RESET = zone.intern ("reset");
  static const long QUARK_TOSTR = zone.intern ("to-string");
  static const long QUARK_STRGY = zone.intern ("stringify");

  // create a new object in a generic way

  Object* Json::mknew (Vector* argv) {
    long argc = (argv == nullptr) ? 0 : argv->length ();
    // check for 0 argument
    if (argc == 0) return new Json;
    throw Exception ("argument-error",
                     "too many argument with json constructor");
  }

  // return true if the given quark is defined

  bool Json::isquark (const long quark, const bool hflg) const {
    rdlock ();
    try {
      if (zone.exists (quark) == true) {
	unlock ();
	return true;
      }
      bool result = hflg ? Object::isquark (quark, hflg) : false;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // apply this object with a set of arguments and a quark
  
  Object* Json::apply (Evaluable* zobj, Nameset* nset, const long quark,
		       Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();
    
    // check for 0 argument
    if (argc == 0) {
      if (quark == QUARK_TOSTR) return new String (tostring ());
      if (quark == QUARK_RESET) {
	reset ();
	return nullptr;
      }
    }
    // check for 1 argument
    if (argc == 1) {
      if (quark == QUARK_STRGY) {
	Object* obj = argv->get (0);
	return new Boolean (stringify (obj));
      }
    }
    // call the object method
    return Object::apply (zobj, nset, quark, argv);
  }
}
