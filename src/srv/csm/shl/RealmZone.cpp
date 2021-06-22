// ----------------------------------------------------------------------------
// - RealmZone.cpp                                                            -
// - afnix:csm service - workzone realm class implementation                  -
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
#include "Crypto.hpp"
#include "LocalZone.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"
#include "RealmZone.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - private section                                                       -
  // -------------------------------------------------------------------------

  // the realm localspace name
  static const String RZ_ZONE_NAME = "realm";
  // the realm localspace info
  static const String RZ_ZONE_INFO = "realm localspace";

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a default realm

  RealmZone::RealmZone (void) {
    Object::iref (p_zone = new LocalZone);
    p_zone->setname (RZ_ZONE_NAME); p_zone->setinfo (RZ_ZONE_INFO);
  }
  
  // create a realm by root

  RealmZone::RealmZone (const String& root) {
    Object::iref (p_zone = new LocalZone (root, RZ_ZONE_NAME, RZ_ZONE_INFO));
  }

  // create a realm by root and name

  RealmZone::RealmZone (const String& root, const String& name) {
    Object::iref (p_zone = new LocalZone (root, name, RZ_ZONE_INFO));
  }

  // create a realm by root, name and info

  RealmZone::RealmZone (const String& root,
			const String& name, const String& info) {
    Object::iref (p_zone = new LocalZone (root, name, info));
  }
  
  // create a realm by localzone
  
  RealmZone::RealmZone (WorkZone* zone) {
    Object::iref (p_zone = zone);
  }

  // destroy this realm

  RealmZone::~RealmZone (void) {
    Object::dref (p_zone);
  }

  // get the class name

  String RealmZone::repr (void) const {
    return "RealmZone";
  }

  // get the realm name

  String RealmZone::getname (void) const {
    rdlock ();
    try {
      String result;
      if (p_zone != nullptr) result = p_zone->getname ();
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // get the realm info

  String RealmZone::getinfo (void) const {
    rdlock ();
    try {
      String result;
      if (p_zone != nullptr) result = p_zone->getinfo ();
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // check if a kid exists

  bool RealmZone::exists (const String& kid) const {
    rdlock ();
    try {
      // check for existence
      bool result = p_zone->exists (kid);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // update the localzone with an authority

  void RealmZone::update (const Authority& auth) {
    wrlock ();
    OutputStream* os = nullptr;
    try {
      // get the authority hid
      String path = auth.getkid().tostring();
      // get an output stream by uri
      OutputStream* os = p_zone->getos (path);
      if (os == nullptr) {
	throw Exception ("realm-error", "nil stream for authority write");
      }
      // write the authority - the stream is closed by the writer
      auth.toxml (os);
      unlock ();
    } catch (...) {
      delete os;
      unlock ();
      throw;
    }
  }

  // create an authority by name and credential

  void RealmZone::create (const String& name, const Credential& cred) {
    wrlock ();
    try {
      // check if the authority exists
      if (exists (name) == true) {
	throw Exception ("realm-error", "authority already exists", name);
      }
      // create an authority by name
      Authority auth = name; auth.setcred (cred);
      // update the realm and unlock
      update (auth);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // validate an authority by rid and credential

  bool RealmZone::validate (const String& kid, const Credential& cred) const {
    rdlock ();
    try {
      // check if the authority exists
      if (p_zone->exists (kid) == false) {
	unlock ();
	return false;
      }
      // get an input stream by kid
      InputStream* is = p_zone->getis (kid);
      // load the authority
      Authority auth(kid, is);
      // validate the authority
      bool result = auth.validate (cred);
      // unlock and return - the input stream is cleaned by the reader
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // get the realm root

  String RealmZone::getroot (void) const {
    rdlock ();
    try {
      String result;
      LocalZone* zone = dynamic_cast<LocalZone*> (p_zone);
      if (zone != nullptr) result = zone->getroot ();
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
  static const long QUARK_ZONE_LENGTH = 1;
  static QuarkZone  zone (QUARK_ZONE_LENGTH);
  
  // the object supported quarks
  static const long QUARK_GETROOT = zone.intern ("get-root");
  
  // create a new object in a generic way

  Object* RealmZone::mknew (Vector* argv) {
    long argc = (argv == nullptr) ? 0 : argv->length ();

    // check for 0 argument
    if (argc == 0) return new RealmZone;
    // check for 1 argument
    if (argc == 1) {
      Object* obj = argv->get (0);
      // check for a string
      String* sobj = dynamic_cast <String*> (obj);
      if (sobj != nullptr) return new RealmZone (*sobj);
      // check for a workzone
      WorkZone* zobj = dynamic_cast <WorkZone*> (obj);
      if (zobj != nullptr) return new RealmZone (zobj);
      // invalid object
      throw Exception ("type-error", 
                       "invalid object with realm constructor",
                       Object::repr (obj)); 
    }
    // check for 2 arguments
    if (argc == 2) {
      String root = argv->getstring (0);
      String name = argv->getstring (1);
      return new RealmZone (root, name);
    }
    // check for 3 arguments
    if (argc == 3) {
      String root = argv->getstring (0);
      String name = argv->getstring (1);
      String info = argv->getstring (2);
      return new RealmZone (root, name, info);
    }
    // invalid arguments
    throw Exception ("argument-error", 
                     "invalid arguments with realm constructor");
  }
  
  // return true if the given quark is defined

  bool RealmZone::isquark (const long quark, const bool hflg) const {
    rdlock ();
    try {
      if (zone.exists (quark) == true) {
	unlock ();
	return true;
      }
      bool result = hflg ? Realm::isquark (quark, hflg) : false;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // apply this object with a set of arguments and a quark
  
  Object* RealmZone::apply (Evaluable* zobj, Nameset* nset, const long quark,
			    Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();
    
    // dispatch 0 argument
    if (argc == 0) {
      if (quark == QUARK_GETROOT) return new String (getroot ());
    }
    // call the realm methods
    return Realm::apply (zobj, nset, quark, argv);
  }
}
