// ----------------------------------------------------------------------------
// - Realm.cpp                                                                -
// - afnix:csm service - realm class implementation                           -
// ----------------------------------------------------------------------------
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

#include "Realm.hpp"
#include "Vector.hpp"
#include "Boolean.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - object section                                                        -
  // -------------------------------------------------------------------------
  
  // the quark zone
  static const long QUARK_ZONE_LENGTH = 5;
  static QuarkZone  zone (QUARK_ZONE_LENGTH);

  // the object supported quarks
  static const long QUARK_CREATE  = zone.intern ("create");
  static const long QUARK_UPDATE  = zone.intern ("update");
  static const long QUARK_VALIDP  = zone.intern ("valid-p");
  static const long QUARK_EXISTSP = zone.intern ("exists-p");
  static const long QUARK_GETINFO = zone.intern ("get-info");

  // return true if the given quark is defined

  bool Realm::isquark (const long quark, const bool hflg) const {
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
  
  Object* Realm::apply (Evaluable* zobj, Nameset* nset, const long quark,
			Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();
    
    // dispatch 0 argument
    if (argc == 0) {
      if (quark == QUARK_GETINFO) return new String (getinfo ());
    }
    // dispatch 1 argument
    if (argc == 1) {
      if (quark == QUARK_EXISTSP) {
	String rid = argv->getstring (0);
	return new Boolean (exists (rid));
      }
      if (quark == QUARK_UPDATE) {
	Object* obj = argv->get (0);
	auto*  auth = dynamic_cast <Authority*> (obj);
	if (auth == nullptr) {
	  throw Exception ("type-error", "invalid object with update",
			   Object::repr (obj));
	}
	update (*auth);
	return nullptr;
      }
    }
    // dispatch 2 arguments
    if (argc == 2) {
      if (quark == QUARK_CREATE) {
	// the authority name
	String name = argv->getstring (0);
	// check for a credential object
	Object*  obj = argv->get (1);
	Credential* cred = dynamic_cast <Credential*> (obj);
	if (cred == nullptr) {
	  throw Exception ("type-error", "invalid object with create",
			   Object::repr (obj));
	}
	create (name, *cred);
	return nullptr;
      }
      if (quark == QUARK_VALIDP) {
	// the authority rid
	String rid = argv->getstring (0);
	// check for a credential object
	Object*  obj = argv->get (1);
	Credential* cred = dynamic_cast <Credential*> (obj);
	if (cred == nullptr) {
	  throw Exception ("type-error", "invalid object with valid-p",
			   Object::repr (obj));
	}
	return new Boolean (validate (rid, *cred));
      }
    }
    // call the nameable methods
    return Nameable::apply (zobj, nset, quark, argv);
  }
}
