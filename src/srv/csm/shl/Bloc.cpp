// ----------------------------------------------------------------------------
// - Bloc.cpp                                                                 -
// - afnix:csm service - plist blob class implementation                      -
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

#include "Bloc.hpp"
#include "Csmsid.hxx"
#include "Vector.hpp"
#include "Boolean.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"

namespace afnix {
  
  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------
  
  // create a default plist blob

  Bloc::Bloc (void) {
    p_cnds = nullptr;
  }

  // create a plist blob by name

  Bloc::Bloc (const String& name) : Blob (name) {
    p_cnds = nullptr;
  }

  // create a plist blob by name and info

  Bloc::Bloc (const String& name, const String& info) : Blob (name, info) {
    p_cnds = nullptr;
  }

  // create a plist blob by rid, name and info

  Bloc::Bloc (const String& rid, const String& name,
	      const String& info) : Blob (rid, name, info) {
    p_cnds = nullptr;
  }

  // copy construct this plist blob

  Bloc::Bloc (const Bloc& that) {
    that.rdlock ();
    try {
      // copy the base blob
      Blob::operator = (that);
      // copy locally
      Object::iref (p_cnds = that.p_cnds);
      that.unlock ();
    } catch (...) {
      that.unlock ();
      throw;
    }
  }

  // destroy this bloc

  Bloc::~Bloc (void) {
    Object::dref (p_cnds);
  }
  
  // assign a plist blob to this one

  Bloc& Bloc::operator = (const Bloc& that) {
    // check for self-assignation
    if (this == &that) return *this;
    // lock and assign
    wrlock ();
    that.rdlock ();
    try {
      // copy the base blob
      Blob::operator = (that);
      // copy locally
      Object::iref (that.p_cnds); Object:: dref (p_cnds); p_cnds = that.p_cnds;
      unlock ();
      that.unlock ();
      return *this;
    } catch (...) {
      unlock ();
      that.unlock ();
      throw;
    }
  }

  // get the class name

  String Bloc::repr (void) const {
    return "Bloc";
  }

  // return a clone of this object

  Object* Bloc::clone (void) const {
    return new Bloc (*this);
  }
  
  // return the serial did

  t_word Bloc::getdid (void) const {
    return SRL_DEOD_CSM;
  }

  // return the serial sid

  t_word Bloc::getsid (void) const {
    return SRL_BLOC_SID;
  }
  
  // serialize this plist blob

  void Bloc::wrstream (OutputStream& os) const {
    rdlock ();
    try {
      // serialize the base object
      Blob::wrstream (os);
      // serialize locally
      if (p_cnds == nullptr) {
	Serial::wrnilid (os);
      } else {
	p_cnds->serialize (os);
      }
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // deserialize this plist blob

  void Bloc::rdstream (InputStream& is) {
    wrlock ();
    try {
      //deserialize the base object
      Blob::rdstream (is);
      // deserialize locally
      p_cnds = dynamic_cast <HashTable*> (Serial::deserialize (is));
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // request an information list

  Plist Bloc::getplst (void) const {
    rdlock ();
    try {
      // get the base plist
      Plist result = Blob::getplst ();
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // check is this bloc is swappable

  bool Bloc::isswap (Part* part) const {
    rdlock ();
    try {
      // check base part
      bool status = Blob::isswap (part);
      // check for a bloc
      if (status == true) {
	// check for a bloc
	auto bloc = dynamic_cast<Bloc*>(part);
	if (bloc == nullptr) status = false;
      }
      unlock ();
      return status;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // add a conditional to the bloc

  void Bloc::addcond (const Conditional& cond) {
    wrlock ();
    try {
      // check for a table of conditionals
      if (p_cnds == nullptr) p_cnds = new HashTable;
      // add the conditional by name
      String name = cond.getname ();
      if (name.isnil () == true) {
	throw Exception ("bloc-erro", "invalid unmaed conditional to add");
      }
      p_cnds->add (name, new Conditional (cond));
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get a conditional by name

  Conditional* Bloc::getcond (const String& name) const {
    rdlock ();
    try {
      // check for null
      if (p_cnds == nullptr) {
	unlock ();
	return nullptr;
      }
      // try to find by name
      auto result = dynamic_cast<Conditional*> (p_cnds->get (name));
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // get a vector of conditionals

  Vector* Bloc::getcnds (void) const {
    rdlock ();
    try {
      Vector* result = (p_cnds == nullptr) ? nullptr : p_cnds->getvobj ();
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
  static const long QUARK_ZONE_LENGTH = 2;
  static QuarkZone  zone (QUARK_ZONE_LENGTH);

  // the object supported quarks
  static const long QUARK_GETCNDS = zone.intern ("get-conditionals");
  static const long QUARK_ADDCOND = zone.intern ("add-conditional");
  // create a new object in a generic way

  Object* Bloc::mknew (Vector* argv) {
    long argc = (argv == nullptr) ? 0 : argv->length ();

    // check for 0 argument
    if (argc == 0) return new Bloc;
    // check for 1 argument
    if (argc == 1) {
      String name = argv->getstring (0);
      return new Bloc (name);
    }
    // check for 2 arguments
    if (argc == 2) {
      String name = argv->getstring (0);
      String info = argv->getstring (1);
      return new Bloc (name, info);
    }
    // check for 3 arguments
    if (argc == 3) {
      String  rid = argv->getstring (0);
      String name = argv->getstring (1);
      String info = argv->getstring (2);
      return new Bloc (rid, name, info);
    }
    // invalid arguments
    throw Exception ("argument-error", 
                     "invalid arguments with bloc constructor");
  }

  // return true if the given quark is defined

  bool Bloc::isquark (const long quark, const bool hflg) const {
    rdlock ();
    try {
      if (zone.exists (quark) == true) {
	unlock ();
	return true;
      }
      bool result = hflg ? Blob::isquark (quark, hflg) : false;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // apply this object with a set of arguments and a quark
  
  Object* Bloc::apply (Evaluable* zobj, Nameset* nset, const long quark,
		       Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();
    
    // dispatch 0 argument
    if (argc == 0) {
      if (quark == QUARK_GETCNDS) return getcnds ();
    }
    // dispatch 1 argument
    if (argc == 1) {
      if (quark == QUARK_ADDCOND) {
	Object* obj = argv->get (1);
        auto   cond = dynamic_cast <Conditional*> (obj);
        if (cond == nullptr) {
	  throw Exception ("type-error", "invalid object with add-conditional",
			   Object::repr (obj));
        }
        addcond (*cond);
        return nullptr;
      }
    }
    // call the blob methods
    return Blob::apply (zobj, nset, quark, argv);
  }
}
