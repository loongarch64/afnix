// ---------------------------------------------------------------------------
// - Identity.cpp                                                            -
// - afnix:csm service - identity class implementation                       -
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

#include "Csmsid.hxx"
#include "Vector.hpp"
#include "Crypto.hpp"
#include "Boolean.hpp"
#include "Identity.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"
#include "InputOutput.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - private section                                                       -
  // -------------------------------------------------------------------------

  // the identity plist
  static const String PN_IDY_SMOD = "PN-IDY-SMOD";
  static const String PI_IDY_SMOD = "IDENTITY SIGNATURE MODE";
  static const String PV_IDY_AMOD = "ACTIVE";
  static const String PV_IDY_IMOD = "INACTIVE";
  
  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------
  
  // create a default identity

  Identity::Identity (void) {
    p_sign = nullptr;
    setrid (d_kid.tostring ());
  }

  // create a identity by name

  Identity::Identity (const String& name) : Bloc (name) {
    p_sign = nullptr;
    setrid (d_kid.tostring ());
  }

  // create a identity by name and info

  Identity::Identity (const String& name,
		      const String& info) : Bloc (name, info) {
    p_sign = nullptr;
    setrid (d_kid.tostring ());
  }
  
  // create a identity by rid, name and info

  Identity::Identity (const String& rid, const String& name,
		      const String& info) : Bloc (rid, name, info) {
    p_sign = nullptr;
  }
  
  // copy construct this identity

  Identity::Identity (const Identity& that) {
    that.rdlock ();
    try {
      // copy the base bloc
      Bloc::operator = (that);
      // copy locally
      Object::iref (p_sign = that.p_sign);
      that.unlock ();
    } catch (...) {
      that.unlock ();
      throw;
    }
  }

  // destroy this identity

  Identity::~Identity (void) {
    Object::dref (p_sign);
  }
  
  // assign a identity to this one

  Identity& Identity::operator = (const Identity& that) {
    // check for self-assignation
    if (this == &that) return *this;
    // lock and assign
    wrlock ();
    that.rdlock ();
    try {
      // copy the base bloc
      Bloc::operator = (that);
      // copy locally
      Object::iref (that.p_sign); Object::dref (p_sign); p_sign = that.p_sign;
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

  String Identity::repr (void) const {
    return "Identity";
  }

  // return a clone of this object

  Object* Identity::clone (void) const {
    return new Identity (*this);
  }

  // return the serial did

  t_word Identity::getdid (void) const {
    return SRL_DEOD_CSM;
  }

  // return the serial sid

  t_word Identity::getsid (void) const {
    return SRL_IDTY_SID;
  }
  
  // serialize this identity

  void Identity::wrstream (OutputStream& os) const {
    rdlock ();
    try {
      // serialize the base bloc
      Bloc::wrstream (os);
      // serialize locally
      if (p_sign == nullptr) {
	Serial::wrnilid (os);
      } else {
	p_sign->serialize (os);
      }
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // deserialize this identity

  void Identity::rdstream (InputStream& is) {
    wrlock ();
    try {
      //deserialize the base bloc
      Bloc::rdstream (is);
      // deserialize locally
      Object::dref (p_sign);
      Object::iref (p_sign =
		    dynamic_cast <Signature*> (Serial::deserialize (is)));
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // request an information list

  Plist Identity::getplst (void) const {
    rdlock ();
    try {
      // get the base plist
      Plist result = Bloc::getplst ();
      // add the signature mode flag
      String smod = verify () ? PV_IDY_AMOD : PV_IDY_IMOD;
      result.add (PN_IDY_SMOD, PI_IDY_SMOD, smod);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // serialize the identity object for signature
  
  void Identity::wrsign (OutputStream& os) const {
    rdlock ();
    try {
      Blob::wrstream (os);
      unlock();
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // sign this identity

  bool Identity::sign (const Key& skey) {
    wrlock ();
    Signer* sngr = nullptr;
    try {
      // create a signer by key
      sngr = Crypto::mksigner (skey);
      // compute the signature if possible
      if (sngr != nullptr) {
        Object::dref (p_sign);
        p_sign = new Signature (sngr->compute (this));
        Object::iref (p_sign);
      }
      delete sngr;
      // get final status
      bool result = (p_sign == nullptr) ? false : true;
      unlock ();
      return result;
    } catch (...) {
      delete sngr;
      unlock ();
      throw;
    }
  }

  // verify this identity

  bool Identity::verify (void) const {
    rdlock ();
    Signer* sngr = nullptr;
    try {
      // create a signer by key
      if (p_sign != nullptr) sngr = Crypto::mksigner (*p_sign);
      // verify the signature if possible
      bool result = (sngr != nullptr) ? sngr->verify(*p_sign, this) : false;
      delete sngr;
      // unlock and stat
      unlock ();
      return result;
    } catch (...) {
      delete sngr;
      unlock ();
      throw;
    }
  }

  // -------------------------------------------------------------------------
  // - object section                                                        -
  // -------------------------------------------------------------------------
  
  // create a new object in a generic way

  Object* Identity::mknew (Vector* argv) {
    long argc = (argv == nullptr) ? 0 : argv->length ();

    // check for 1 argument
    if (argc == 1) {
      String name = argv->getstring (0);
      return new Identity (name);
    }
    // check for 2 arguments
    if (argc == 2) {
      String name = argv->getstring (0);
      String info = argv->getstring (1);
      return new Identity (name, info);
    }
    // check for 3 arguments
    if (argc == 3) {
      String  rid = argv->getstring (0);
      String name = argv->getstring (1);
      String info = argv->getstring (2);
      return new Identity (rid, name, info);
    }
    // invalid arguments
    throw Exception ("argument-error", 
                     "invalid arguments with identity constructor");
  }

  // return true if the given quark is defined

  bool Identity::isquark (const long quark, const bool hflg) const {
    rdlock ();
    try {
      bool result = hflg ? Signable::isquark (quark, hflg) : false;
      if (result == false) {
        result =  hflg ? Bloc::isquark (quark, hflg) : false;
      }
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // apply this object with a set of arguments and a quark
  
  Object* Identity::apply (Evaluable* zobj, Nameset* nset, const long quark,
			   Vector* argv) {
    // check the signable method
    if (Signable::isquark (quark, true) == true) {
      return Signable::apply (zobj, nset, quark, argv);
    }
    // call the bloc methods
    return Bloc::apply (zobj, nset, quark, argv);
  }
}
