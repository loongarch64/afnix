// ---------------------------------------------------------------------------
// - Act.cpp                                                                 -
// - afnix:csm service - notary act class implementation                     -
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

#include "Act.hpp"
#include "Csmsid.hxx"
#include "Vector.hpp"
#include "Integer.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------
  
  // create a default act

  Act::Act (void) {
    d_sgnt = 0LL;
    d_tvlb = 0LL;
    d_tvla = 0LL;
  }

  // create an act by identity

  Act::Act (const Identity& idty, const Identity& rqst) {
    d_idty = idty;
    d_rqst = rqst;
    d_sgnt = 0LL;
    d_tvlb = 0LL;
    d_tvla = 0LL;
  }

  // copy construct an act

  Act::Act (const Act& that) {
    that.rdlock ();
    try {
      // copy localy
      d_idty = that.d_idty;
      d_rqst = that.d_rqst;
      d_sgnt = that.d_sgnt;
      d_tvlb = that.d_tvlb;
      d_tvla = that.d_tvla;
      d_sign = that.d_sign;
      that.unlock ();
    } catch (...) {
      that.unlock ();
      throw;
    }
  }
  
  // assign an act to this one

  Act& Act::operator = (const Act& that) {
    // check for self-assignation
    if (this == &that) return *this;
    // lock and assign
    wrlock ();
    that.rdlock ();
    try {
      // copy localy
      d_idty = that.d_idty;
      d_rqst = that.d_rqst;
      d_sgnt = that.d_sgnt;
      d_tvlb = that.d_tvlb;
      d_tvla = that.d_tvla;
      d_sign = that.d_sign;
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

  String Act::repr (void) const {
    return "Act";
  }
  
  // return a clone of this object

  Object* Act::clone (void) const {
    return new Act (*this);
  }

  // return the serial did

  t_word Act::getdid (void) const {
    return SRL_DEOD_CSM;
  }

  // return the serial sid

  t_word Act::getsid (void) const {
    return SRL_XACT_SID;
  }
  
  // serialize this identity

  void Act::wrstream (OutputStream& os) const {
    rdlock ();
    try {
      d_idty.wrstream (os);
      d_rqst.wrstream (os);
      Serial::wrlong (d_sgnt, os);
      Serial::wrlong (d_tvlb, os);
      Serial::wrlong (d_tvla, os);
      d_sign.wrstream (os);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // deserialize this identity

  void Act::rdstream (InputStream& is) {
    wrlock ();
    try {
      d_idty.rdstream (is);
      d_rqst.rdstream (is);
      d_sgnt = Serial::rdlong (is);
      d_tvlb = Serial::rdlong (is);
      d_tvla = Serial::rdlong (is);
      d_sign.rdstream (is);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // set the validity before time
  
  void Act::settvlb (const t_long tvlb) {
    wrlock ();
    try {
      d_tvlb = tvlb;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the validity before time

  t_long Act::gettvlb (void) const {
    rdlock ();
    try {
      t_long result = d_tvlb;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // set the validity after time
  
  void Act::settvla (const t_long tvla) {
    wrlock ();
    try {
      d_tvla = tvla;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the validity after time

  t_long Act::gettvla (void) const {
    rdlock ();
    try {
      t_long result = d_tvla;
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
  static const long QUARK_ZONE_LENGTH = 4;
  static QuarkZone  zone (QUARK_ZONE_LENGTH);

  // the object supported quarks
  static const long QUARK_SETTVLB = zone.intern ("set-before-validity-time");
  static const long QUARK_GETTVLB = zone.intern ("get-before-validity-time");
  static const long QUARK_SETTVLA = zone.intern ("set-after-validity-time");
  static const long QUARK_GETTVLA = zone.intern ("get-after-validity-time");

  // create a new object in a generic way

  Object* Act::mknew (Vector* argv) {
    long argc = (argv == nullptr) ? 0 : argv->length ();

    // check for 0 argument
    if (argc == 0) return new Act;
    // check for 2 arguments
    if (argc == 2) {
      // get the identity
      Object* obj = argv->get (0);
      auto idty = dynamic_cast <Identity*> (obj);
      if (idty == nullptr) {
	throw Exception ("type-error", "invalid object for act constructor",
			 Object::repr (obj));
      }
      // get the identity
      obj = argv->get (1);
      auto rqst = dynamic_cast <Identity*> (obj);
      if (rqst == nullptr) {
	throw Exception ("type-error", "invalid object for act constructor",
			 Object::repr (obj));
      }
      return new Act (*idty, *rqst);
    }
    // invalid arguments
    throw Exception ("argument-error", 
                     "invalid arguments with act constructor");
  }

  // return true if the given quark is defined

  bool Act::isquark (const long quark, const bool hflg) const {
    rdlock ();
    try {
      if (zone.exists (quark) == true) {
	unlock ();
	return true;
      }
      bool result = hflg ? Serial::isquark (quark, hflg) : false;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // apply this object with a set of arguments and a quark
  
  Object* Act::apply (Evaluable* zobj, Nameset* nset, const long quark,
			 Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();
    
    // dispatch 0 argument
    if (argc == 0) {
      if (quark == QUARK_GETTVLB) return new Integer (gettvlb ());
      if (quark == QUARK_GETTVLA) return new Integer (gettvla ());
    }
      // dispatch 1 argument
    if (argc == 1) {
      if (quark == QUARK_SETTVLB) {
	t_long tvlb = argv->getlong (0);
	settvlb (tvlb);
	return nullptr;
      }
      if (quark == QUARK_SETTVLA) {
	t_long tvla = argv->getlong (0);
	settvla (tvla);
	return nullptr;
      }
    }
    // call the serial methods
    return Serial::apply (zobj, nset, quark, argv);
  }
}
