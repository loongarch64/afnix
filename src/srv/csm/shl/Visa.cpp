// ---------------------------------------------------------------------------
// - Visa.cpp                                                                -
// - afnix:csm service - visa class implementation                           -
// ---------------------------------------------------------------------------
// - This program is free software;  you can redistribute it  and/or  modify -
// - it provided that this copyright notice is kept intvisa.                  -
// -                                                                         -
// - This program  is  distributed in  the hope  that it will be useful, but -
// - without  any  warranty;  without  even   the   implied    warranty   of -
// - merchantability or fitness for a particular purpose.  In no event shall -
// - the copyright holder be liable for any  direct, indirect, incidental or -
// - special damages arising in any way out of the use of this software.     -
// ---------------------------------------------------------------------------
// - copyright (c) 1999-2021 amaury darsch                                   -
// ---------------------------------------------------------------------------

#include "Visa.hpp"
#include "Csmsid.hxx"
#include "Vector.hpp"
#include "Integer.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------
  
  // create an visa by identities

  Visa::Visa (const Identity& idty, const Identity& rqst,
	      const Identity& user) : Act (idty, rqst) {
    d_user = user;
  }

  // copy construct an visa

  Visa::Visa (const Visa& that) {
    that.rdlock ();
    try {
      // copy the base act
      Act::operator = (that);
      // copy localy
      d_user = that.d_user;
      that.unlock ();
    } catch (...) {
      that.unlock ();
      throw;
    }
  }
  
  // assign an visa to this one

  Visa& Visa::operator = (const Visa& that) {
    // check for self-assignation
    if (this == &that) return *this;
    // lock and assign
    wrlock ();
    that.rdlock ();
    try {
      // copy the base act
      Act::operator = (that);
      // copy localy
      d_user = that.d_user;
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

  String Visa::repr (void) const {
    return "Visa";
  }
  
  // return a clone of this object

  Object* Visa::clone (void) const {
    return new Visa (*this);
  }

  // return the serial did

  t_word Visa::getdid (void) const {
    return SRL_DEOD_CSM;
  }

  // return the serial sid

  t_word Visa::getsid (void) const {
    return SRL_VISA_SID;
  }
  
  // serialize this identity

  void Visa::wrstream (OutputStream& os) const {
    rdlock ();
    try {
      // serialize the base object
      Act::wrstream (os);
      // serialize locally
      d_user.wrstream (os);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // deserialize this identity

  void Visa::rdstream (InputStream& is) {
    wrlock ();
    try {
      // deserialize the base object
      Act::rdstream (is);
      // deserialize locally
      d_user.rdstream (is);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // get the user identity

  Identity Visa::getuser (void) const {
    rdlock ();
    try {
      Identity result = d_user;
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
  static const long QUARK_GETUSER = zone.intern ("get-user-identity");

  // create a new object in a generic way

  Object* Visa::mknew (Vector* argv) {
    long argc = (argv == nullptr) ? 0 : argv->length ();

    // check for 0 argument
    if (argc == 0) return new Visa;
    // check for 3 arguments
    if (argc == 3) {
      // get the identities
      Object* obj = argv->get (0);
      auto idty = dynamic_cast <Identity*> (obj);
      if (idty == nullptr) {
	throw Exception ("type-error", "invalid object for visa constructor",
			 Object::repr (obj));
      }
      obj = argv->get (1);
      auto rqst = dynamic_cast <Identity*> (obj);
      if (rqst == nullptr) {
	throw Exception ("type-error", "invalid object for visa constructor",
			 Object::repr (obj));
      }
      obj = argv->get (2);
      auto user = dynamic_cast <Identity*> (obj);
      if (rqst == nullptr) {
	throw Exception ("type-error", "invalid object for visa constructor",
			 Object::repr (obj));
      }
      return new Visa (*idty, *rqst, *user);
    }
    // invalid arguments
    throw Exception ("argument-error", 
                     "invalid arguments with visa constructor");
  }

  // return true if the given quark is defined

  bool Visa::isquark (const long quark, const bool hflg) const {
    rdlock ();
    try {
      if (zone.exists (quark) == true) {
	unlock ();
	return true;
      }
      bool result = hflg ? Act::isquark (quark, hflg) : false;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // apply this object with a set of arguments and a quark
  
  Object* Visa::apply (Evaluable* zobj, Nameset* nset, const long quark,
		       Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();
    
    // dispatch 0 argument
    if (argc == 0) {
      if (quark == QUARK_GETUSER) return new Identity (getuser ());
    }
    // call the act methods
    return Act::apply (zobj, nset, quark, argv);
  }
}
