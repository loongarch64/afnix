// ---------------------------------------------------------------------------
// - Xextn.cpp                                                               -
// - afnix:tls service - X509 extension class implementation                 -
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

#include "Xextn.hpp"
#include "Vector.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a default extension

  Xextn::Xextn (void) {
    p_eon = nullptr;
    reset ();
  }

  // create an extension by eid and node

  Xextn::Xextn (const String& eid, AsnNode* eon) {
    p_eon = nullptr;
    set (eid, false, eon);
  }

  // create an extension by eid, flag and node

  Xextn::Xextn (const String& eid, const bool ecf, AsnNode* eon) {
    p_eon = nullptr;
    set (eid, ecf, eon);
  }
  // copy construct this object

  Xextn::Xextn (const Xextn& that) {
    that.rdlock ();
    try {
      d_eid = that.d_eid;
      d_ecf = that.d_ecf;
      Object::iref (p_eon = that.p_eon);
      that.unlock ();
    } catch (...) {
      that.unlock ();
      throw;
    }
  }

  // destroy this extension

  Xextn::~Xextn (void) {
    Object::dref (p_eon);
  }

  // return the object class name

  String Xextn::repr (void) const {
    return "Xextn";
  }

  // return a clone of this object
  
  Object* Xextn::clone (void) const {
    return new Xextn (*this);
  }

  // assign an object to this one

  Xextn& Xextn::operator = (const Xextn& that) {
    // check for self-assignation
    if (this == &that) return *this;
    // lock and assign
    wrlock ();
    that.rdlock ();
    try {
      // assign locally
      d_eid = that.d_eid;
      d_ecf = that.d_ecf;
      Object::iref (that.p_eon);
      Object::dref (p_eon);
      p_eon = that.p_eon;
      // unlock and return
      unlock ();
      that.unlock ();
      return *this;
    } catch (...) {
      unlock ();
      that.unlock ();
      throw;
    }
  }

  // reset the extension

  void Xextn::reset (void) {
    wrlock ();
    try {
      d_eid = "";
      d_ecf = false;
      Object::dref (p_eon); p_eon = nullptr;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the extension identifier

  String Xextn::geteid (void) const {
    rdlock ();
    try {
      String result = d_eid;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the extension critical flag

  bool Xextn::getecf (void) const {
    rdlock ();
    try {
      bool result = d_ecf;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set the extension by oid and node

  void Xextn::set (const String& eid, const bool ecf, AsnNode* eon) {
    wrlock ();
    try {
      // set the oid and flag
      d_eid = eid;
      d_ecf = ecf;
      // protect and set the parameters
      Object::iref (eon);
      Object::dref (p_eon);
      p_eon = eon;
      // unlock and return
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // -------------------------------------------------------------------------
  // - object section                                                        -
  // -------------------------------------------------------------------------

  // the quark zone
  static const long QUARK_ZONE_LENGTH = 2;
  static QuarkZone  zone (QUARK_ZONE_LENGTH);

  // the object supported quarks
  static const long QUARK_SET    = zone.intern ("set");
  static const long QUARK_GETEID = zone.intern ("get-identifier");

  // create a new object in a generic way
 
  Object* Xextn::mknew (Vector* argv) {
    long argc = (argv == nullptr) ? 0 : argv->length ();

    // create a default object
    if (argc == 0) return new Xextn;
    // too many arguments
    throw Exception ("argument-error",
                     "too many argument with extension constructor");
  }

  // return true if the given quark is defined

  bool Xextn::isquark (const long quark, const bool hflg) const {
    rdlock ();
    if (zone.exists (quark) == true) {
      unlock ();
      return true;
    }
    bool result = hflg ? Object::isquark (quark, hflg) : false;
    unlock ();
    return result;
  }
  
  // apply this object with a set of arguments and a quark

  Object* Xextn::apply (Evaluable* zobj, Nameset* nset, const long quark,
			Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();
    
    // check for 0 argument
    if (argc == 0) {
      if (quark == QUARK_GETEID) return new String (geteid ());
    }
    // check for 3 arguments
    if (argc == 3) {
      if (quark == QUARK_SET) {
	String  eid = argv->getstring (0);
	bool    ecf = argv->getbool   (1);
	Object* obj = argv->get (2);
	AsnNode* eon = dynamic_cast <AsnNode*> (obj);
	if ((obj != nullptr) && (eon == nullptr)) {
	  throw Exception ("type-error", 
			   "invalid object as extension parameter",
			   Object::repr (obj));
	}
	set (eid, ecf, eon);
	return nullptr;
      }
    }
    // call the object method
    return Object::apply (zobj, nset, quark, argv);
  }
}
