// ---------------------------------------------------------------------------
// - SessionSet.cpp                                                          -
// - afnix:csm service - session set class implementation                    -
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
#include "Integer.hpp"
#include "Evaluable.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"
#include "SessionSet.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create an empty session set

  SessionSet::SessionSet (void) {
    reset ();
  }

  // return the object class name

  String SessionSet::repr (void) const {
    return "SessionSet";
  }

  // reset this session set
  
  void SessionSet::reset (void) {
    wrlock ();
    try {
      d_hash.reset ();
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the number of session in the session set

  long SessionSet::length (void) const {
    rdlock ();
    try {
      long result = d_hash.length ();
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
	
  // return true if the session set is empty

  bool SessionSet::empty (void) const {
    rdlock ();
    try {
      bool result = d_hash.empty ();
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // check if a session exists by hash id

  bool SessionSet::exists (const String& hid) const {
    rdlock ();
    try {
      bool result = d_hash.exists (hid);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // add a session object

  void SessionSet::add (Session* sobj) {
    // check for nil first
    if (sobj == nullptr) return;
    // lock and add
    wrlock ();
    try {
      String hid = sobj->getshid ();
      if (exists (hid) == true) {
	throw Exception ("session-error", "duplicate session in add");
      }
      d_hash.add (hid, sobj);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get a session by index

  Session* SessionSet::get (const long idx) const {
    rdlock ();
    try {
      Session* result = dynamic_cast <Session*> (d_hash.getobj (idx));
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get a session by hash id

  Session* SessionSet::lookup (const String& hid) const {
    rdlock ();
    try {
      Session* result = dynamic_cast <Session*> (d_hash.lookup (hid));
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // remove a session by hash id

  void SessionSet::remove (const String& hid) {
    wrlock ();
    try {
      d_hash.remove (hid);
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
  static const long QUARK_ZONE_LENGTH = 7;
  static QuarkZone  zone (QUARK_ZONE_LENGTH);

  // the object supported quarks
  static const long QUARK_ADD     = zone.intern ("add");
  static const long QUARK_GET     = zone.intern ("get");
  static const long QUARK_REMOVE  = zone.intern ("remove");
  static const long QUARK_LOOKUP  = zone.intern ("lookup");
  static const long QUARK_LENGTH  = zone.intern ("length");
  static const long QUARK_EMPTYP  = zone.intern ("empty-p");
  static const long QUARK_EXISTSP = zone.intern ("exists-p");

  // create a new object in a generic way

  Object* SessionSet::mknew (Vector* argv) {
    long argc = (argv == nullptr) ? 0 : argv->length ();
    // check for 0 argument
    if (argc == 0) return new SessionSet;
    // invalid arguments
    throw Exception ("argument-error",
                     "too many argument with session set constructor");
  }

  // return true if the given quark is defined

  bool SessionSet::isquark (const long quark, const bool hflg) const {
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
  
  Object* SessionSet::apply (Evaluable* zobj, Nameset* nset, const long quark,
			  Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();
    
    // check for 0 argument
    if (argc == 0) {
      if (quark == QUARK_EMPTYP) return new Boolean (empty  ());
      if (quark == QUARK_LENGTH) return new Integer (length ());
    }
    // check for 1 argument
    if (argc == 1) {
      if (quark == QUARK_ADD) {
	Object*   obj = argv->get (0);
	Session* sobj = dynamic_cast <Session*> (obj);
	if ((sobj == nullptr) && (obj != nullptr)) {
	  throw Exception ("type-error", "invalid object with add",
			   Object::repr (obj));
	}
	add (sobj);
	return nullptr;
      }
      if (quark == QUARK_GET) {
	rdlock ();
	try {
	  long idx = argv->getlong (0);
	  Session* so = get (idx);
	  zobj->post (so);
	  unlock ();
	  return so;
	} catch (...) {
	  unlock ();
	  throw;
	}
      }
      if (quark == QUARK_LOOKUP) {
	rdlock ();
	try {
	  String hid = argv->getstring (0);
	  Session* so = lookup (hid);
	  zobj->post (so);
	  unlock ();
	  return so;
	} catch (...) {
	  unlock ();
	  throw;
	}
      }
      if (quark == QUARK_REMOVE) {
	String hid = argv->getstring (0);
	remove (hid);
	return nullptr;
      }
      if (quark == QUARK_EXISTSP) {
	String hid = argv->getstring (0);
	return new Boolean (exists (hid));
      }
    }
    // call the object method
    return Object::apply (zobj, nset, quark, argv);
  }
}
