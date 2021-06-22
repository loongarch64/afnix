// ---------------------------------------------------------------------------
// - Principal.cpp                                                           -
// - afnix:csm service - principal class implementation                      -
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
#include "Evaluable.hpp"
#include "Principal.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"
#include "InputOutput.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------
  
  // create a default principal

  Principal::Principal (void) {
    p_skey = nullptr;
  }
  
  // create a principal by name

  Principal::Principal (const String& name) : Identity (name) {
    p_skey = nullptr;
  }

  // create a principal by name and info
  
  Principal::Principal (const String& name,
			const String& info) : Identity (name, info) {
    p_skey = nullptr;
  }
  
  // create a principal by rid, name and info
  
  Principal::Principal (const String& rid, const String& name,
			const String& info) : Identity (rid, name, info) {
    p_skey = nullptr;
  }
  
  // copy construct this principal

  Principal::Principal (const Principal& that) {
    that.rdlock ();
    try {
      // copy the base identity
      Identity::operator = (that);
      // copy locally
      Object::iref (p_skey = that.p_skey);
      that.unlock ();
    } catch (...) {
      that.unlock ();
      throw;
    }
  }

  // destroy this principal

  Principal::~Principal (void) {
    Object::dref (p_skey);
  }
  
  // assign a principal to this one

  Principal& Principal::operator = (const Principal& that) {
    // check for self-assignation
    if (this == &that) return *this;
    // lock and assign
    wrlock ();
    that.rdlock ();
    try {
      // copy the base identity
      Identity::operator = (that);
      // copy locally
      Object::iref (that.p_skey); Object::dref (p_skey); p_skey = that.p_skey;
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

  String Principal::repr (void) const {
    return "Principal";
  }

  // return a clone of this object

  Object* Principal::clone (void) const {
    return new Principal (*this);
  }
  
  // return the serial did

  t_word Principal::getdid (void) const {
    return SRL_DEOD_CSM;
  }

  // return the serial sid

  t_word Principal::getsid (void) const {
    return SRL_PCPL_SID;
  }
  
  // serialize this principal

  void Principal::wrstream (OutputStream& os) const {
    rdlock ();
    try {
      // serialize the base identity
      Identity::wrstream (os);
      // serialize locally
      if (p_skey == nullptr) {
	Serial::wrnilid (os);
      } else {
	p_skey->serialize (os);
      }
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // deserialize this principal

  void Principal::rdstream (InputStream& is) {
    wrlock ();
    try {
      // deserialize the base identity
      Identity::rdstream (is);
      // deserialize locally
      Object::dref (p_skey);
      Object::iref (p_skey = dynamic_cast <Key*> (Serial::deserialize (is)));
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // verify the principal
  
  bool Principal::verify (void) const {
    rdlock ();
    try {
      // check for a key
      bool result = (p_skey == nullptr) ? false : true;
      // verify the identity
      if (result == true) result = Identity::verify ();
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // sign the principal with a new key

  bool Principal::sign (void) {
    wrlock ();
    Key* skey = nullptr;
    try {
      skey = Crypto::mkkey (Crypto::XKEY_SGN);
      bool result = (skey == nullptr) ? false : sign (*skey);
      delete skey;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
    
  // sign this principal by key

  bool Principal::sign (const Key& skey) {
    wrlock ();
    try {
      // sign the identity
      if (Identity::sign(skey) == false) {
	Object::dref (p_sign); p_sign = nullptr;
	Object::dref (p_skey); p_skey = nullptr;
	return false;
      }
      // save the key
      Object::dref (p_skey);
      Object::iref (p_skey = new Key(skey));
      unlock ();
      return true;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // sign a signable object

  bool Principal::sign (Signable* sgnl) const {
    rdlock ();
    try {
      bool result = (p_skey == nullptr)
	? false
	: (sgnl == nullptr) ? false : sgnl->sign (*p_skey);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // get the principal signing key

  Key* Principal::getskey (void) const {
    rdlock ();
    try {
      Key* result = p_skey;
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
  static const long QUARK_SIGN    = zone.intern ("sign");
  static const long QUARK_GETSKEY = zone.intern ("get-key");

  // create a new object in a generic way

  Object* Principal::mknew (Vector* argv) {
    long argc = (argv == nullptr) ? 0 : argv->length ();

    // check for 0 argument
    if (argc == 0) return new Principal;
    // check for 1 argument
    if (argc == 1) {
      String name = argv->getstring (0);
      return new Principal (name);
    }
    // check for 2 arguments
    if (argc == 2) {
      String name = argv->getstring (0);
      String info = argv->getstring (1);
      return new Principal (name, info);
    }
    // check for 3 arguments
    if (argc == 3) {
      String  rid = argv->getstring (0);
      String name = argv->getstring (1);
      String info = argv->getstring (2);
      return new Principal (rid, name, info);
    }
    // invalid arguments
    throw Exception ("argument-error", 
                     "invalid arguments with principal constructor");
  }

  // return true if the given quark is defined

  bool Principal::isquark (const long quark, const bool hflg) const {
    rdlock ();
    try {
      if (zone.exists (quark) == true) {
	unlock ();
	return true;
      }
      bool result = hflg ? Identity::isquark (quark, hflg) : false;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // apply this object with a set of arguments and a quark
  
  Object* Principal::apply (Evaluable* zobj, Nameset* nset, const long quark,
			    Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();
    
    // check for 0 argument
    if (argc == 0) {
      if (quark == QUARK_SIGN) return new Boolean (sign ());
      if (quark == QUARK_GETSKEY) {
	rdlock ();
	try {
	  Object* skey = getskey ();
	  zobj->post (skey);
	  unlock ();
	  return skey;
	} catch (...) {
	  unlock ();
	  throw;
	}
      }
    } 
    // check for 1 argument
    if (argc == 1) {
      if (quark == QUARK_SIGN) {
        Object* obj = argv->get (0);
	// check for a signable
        auto sgnl = dynamic_cast<Signable*>(obj);
        if (sgnl != nullptr) return new Boolean (sign (sgnl));
	// check for a key
        auto skey = dynamic_cast<Key*>(obj);
        if (skey != nullptr) return new Boolean (sign (*skey));
	// invalid object
	throw Exception ("type-error", "invalid object for principal sign",
			 Object::repr (obj));
      }
    }
    // call the identity methods
    return Identity::apply (zobj, nset, quark, argv);
  }
}
