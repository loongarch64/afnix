// ---------------------------------------------------------------------------
// - WorkZone.cpp                                                            -
// - afnix:csm module - abstract workzone class implementation               -
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

#include "Vector.hpp"
#include "Boolean.hpp"
#include "WorkZone.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - private section                                                       -
  // -------------------------------------------------------------------------

  // default workzone name, info and flags
  static const String WZ_NAME_DEF = "";
  static const String WZ_INFO_DEF = "";
  static const String WZ_SCHM_URI = "zone";

  // -------------------------------------------------------------------------
  // - public section                                                       -
  // -------------------------------------------------------------------------

  // get the workzone uri scheme

  String WorkZone::getschm (void) {
    return WZ_SCHM_URI;
  }

  // map a name to a zone uri
  
  String WorkZone::tozuri (const String& name) {
    String result = WZ_SCHM_URI + ":///" + name;
    return result;
  }
  
  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a default workzone

  WorkZone::WorkZone (void) {
    d_name = WZ_NAME_DEF;
    d_info = WZ_NAME_DEF;
  }

  // create a workzone by name

  WorkZone::WorkZone (const String& name) {
    d_name = name;
  }

  // create a workzone by name and info

  WorkZone::WorkZone (const String& name, const String& info) {
    d_name = name;
    d_info = info;
  }

  // get the workzone name

  String WorkZone::getname (void) const {
    rdlock ();
    try {
      String result = d_name;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set the workzone name

  void WorkZone::setname (const String& name) {
    wrlock ();
    try {
      d_name = name;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the workzone info

  String WorkZone::getinfo (void) const {
    rdlock ();
    try {
      String result = d_info;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set the workzone info

  void WorkZone::setinfo (const String& info) {
    wrlock ();
    try {
      d_info = info;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // check if an entity exists by string uri

  bool WorkZone::exists (const String& suri) const {
    rdlock ();
    try {
      // map the uri string
      String sval = Uri::isuri (suri) ? suri : tonuri (suri);
      // map the string to uri
      Uri uri (sval);
      // check by uri
      bool result = exists (uri);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // rename an entity by string uri

  bool WorkZone::rename (const String& suri, const String& puri) {
    wrlock ();
    try {
      // map the uri string
      String nval = Uri::isuri (suri) ? suri : tonuri (suri);
      String pval = Uri::isuri (puri) ? puri : tonuri (puri);
      // map the string to uri
      Uri nuri (nval); Uri turi (pval);
      // check by uri
      bool result = rename (nuri, turi);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // remove an entity by string uri

  bool WorkZone::remove (const String& suri) {
    wrlock ();
    try {
      // map the uri string
      String sval = Uri::isuri (suri) ? suri : tonuri (suri);
      // map the string to uri
      Uri uri (sval);
      // check by uri
      bool result = remove (uri);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // get an input stream by string uri

  InputStream* WorkZone::getis (const String& suri) const {
    rdlock ();
    try {
      // map the uri string
      String sval = Uri::isuri (suri) ? suri : tonuri (suri);
      // map the string to uri
      Uri uri (sval);
      // get the input stream
      InputStream* is = getis (uri);
      unlock ();
      return is;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get an output stream by string uri

  OutputStream* WorkZone::getos (const String& suri) const {
    rdlock ();
    try {
      // map the uri string
      String sval = Uri::isuri (suri) ? suri : tonuri (suri);
      // map the string to uri
      Uri uri (sval);
      // get the input stream
      OutputStream* os = getos (uri);
      unlock ();
      return os;
    } catch (...) {
      unlock ();
      throw;
    }
  }
 
  // -------------------------------------------------------------------------
  // - object section                                                        -
  // -------------------------------------------------------------------------

  // the quark zone
  static const long QUARK_ZONE_LENGTH = 12;
  static QuarkZone  zone (QUARK_ZONE_LENGTH);

  // the object supported quarks
  static const long QUARK_CLEAN    = zone.intern ("clean");
  static const long QUARK_GETIS    = zone.intern ("get-input-stream");
  static const long QUARK_GETOS    = zone.intern ("get-output-stream");
  static const long QUARK_TONURI   = zone.intern ("to-normalized-uri");
  static const long QUARK_RENAME   = zone.intern ("rename");
  static const long QUARK_REMOVE   = zone.intern ("remove");
  static const long QUARK_EXISTSP  = zone.intern ("exists-p");
  static const long QUARK_SETNAME  = zone.intern ("set-name");
  static const long QUARK_GETINFO  = zone.intern ("get-info");
  static const long QUARK_SETINFO  = zone.intern ("set-info");
  static const long QUARK_GETELST  = zone.intern ("get-entity-list");
  static const long QUARK_TOEPTBL  = zone.intern ("to-entity-table");

  // return true if the given quark is defined

  bool WorkZone::isquark (const long quark, const bool hflg) const {
    rdlock ();
    try {
      if (zone.exists (quark) == true) {
	unlock ();
	return true;
      }
      bool result = hflg ? Nameable::isquark (quark, hflg) : false;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // apply this object with a set of arguments and a quark
  
  Object* WorkZone::apply (Evaluable* zobj, Nameset* nset, const long quark,
			   Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();
    
    // check for 0 argument
    if (argc == 0) {
      if (quark == QUARK_CLEAN)   return new Boolean (clean ());
      if (quark == QUARK_GETINFO) return new String (getinfo ());
      if (quark == QUARK_GETELST) return getelst ();
      if (quark == QUARK_TOEPTBL) return toeptbl ();
    }
    // check for 1 argument
    if (argc == 1) {
      if (quark == QUARK_SETNAME) {
	String name = argv->getstring (0);
	setname (name);
	return nullptr;
      }
      if (quark == QUARK_SETINFO) {
	String info = argv->getstring (0);
	setinfo (info);
	return nullptr;
      }
      if (quark == QUARK_GETELST) {
	String idty = argv->getstring (0);
	return getelst (idty);
      }
      if (quark == QUARK_EXISTSP) {
	Object* obj = argv->get (0);
	// check for a a uri
	Uri* uri = dynamic_cast <Uri*> (obj);
	if (uri != nullptr) return new Boolean (exists (*uri));
	// check for a string
	String* sobj = dynamic_cast <String*> (obj);
	if (sobj != nullptr) return new Boolean (exists (*sobj));
	// invalid object
	throw Exception ("type-error", "invalid object for workzone exists",
			 Object::repr (obj));
      }
      if (quark == QUARK_REMOVE) {
	Object* obj = argv->get (0);
	// check for a uri
	Uri* uri = dynamic_cast <Uri*> (obj);
	if (uri != nullptr) return new Boolean (remove (*uri));
	// check for a string
	String* sobj = dynamic_cast <String*> (obj);
	if (sobj != nullptr) return new Boolean (remove (*sobj));
	// invalid object
	throw Exception ("type-error", "invalid object for workzone remove",
			 Object::repr (obj));
      }
      if (quark == QUARK_TONURI) {
	String name = argv->getstring (0);
	return new String (tonuri (name));
      }
      if (quark == QUARK_GETIS) {
	Object* obj = argv->get (0);
	// check for a a uri
	Uri* uri = dynamic_cast <Uri*> (obj);
	if (uri != nullptr) return getis (*uri);
	// check for a string
	String* sobj = dynamic_cast <String*> (obj);
	if (sobj != nullptr) return getis (*sobj);
	// invalid object
	throw Exception ("type-error", 
			 "invalid object for workzone input stream",
			 Object::repr (obj));
      }
      if (quark == QUARK_GETOS) {
	Object* obj = argv->get (0);
	// check for a a uri
	Uri* uri = dynamic_cast <Uri*> (obj);
	if (uri != nullptr) return getos (*uri);
	// check for a string
	String* sobj = dynamic_cast <String*> (obj);
	if (sobj != nullptr) return getos (*sobj);
	// invalid object
	throw Exception ("type-error", 
			 "invalid object for workzone output stream",
			 Object::repr (obj));
      }      
    }
    if (argc == 2) {
      if (quark == QUARK_RENAME) {
	Object* obj = argv->get (0);
	// check for a uri
	auto suri = dynamic_cast <Uri*> (obj);
	if (suri != nullptr) {
	  obj = argv->get (1);
	  auto turi = dynamic_cast <Uri*> (obj);
	  if (turi != nullptr) return new Boolean (rename (*suri, *turi));
	}
	// check for a string
	String* sobj = dynamic_cast <String*> (obj);
	if (sobj != nullptr) {
	  String puri = argv->getstring (1);
	  return new Boolean (rename (*sobj, puri));
	}
	// invalid object
	throw Exception ("type-error", "invalid objects for workzone rename");
      }
    }
    // call the nameable method
    return Nameable::apply (zobj, nset, quark, argv);
  }
}
