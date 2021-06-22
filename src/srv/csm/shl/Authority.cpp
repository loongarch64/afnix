// ---------------------------------------------------------------------------
// - Authority.cpp                                                           -
// - afnix:csm service - authority class implementation                      -
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
#include "Authority.hpp"
#include "Authority.hxx"
#include "QuarkZone.hpp"
#include "Exception.hpp"
#include "InputOutput.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - private section                                                       -
  // -------------------------------------------------------------------------
  
  // the blob request plist
  static const String PN_BLB_HASH = "PN-BLB-HASH";
  static const String PI_BLB_HASH = "BLOB HASH CREDENTIAL";

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------
  
  // create a default authority

  Authority::Authority (void) {
    p_cred = nullptr;
  }
  
  // create an authority by uri

  Authority::Authority (const Uri& uri) {
    p_cred = nullptr;
    // create an authority reader
    AuthorityReader ard(uri);
    // extract an authority from the xml
    Authority auth = ard.xmlto ();
    // copy locally
    this->operator = (auth);
  }

  // create a authority by name

  Authority::Authority (const String& name) : Identity (name) {
    p_cred = nullptr;
  }

  // create a authority by name and info

  Authority::Authority (const String& name,
			const String& info) : Identity (name, info) {
    p_cred = nullptr;
  }
  
  // create a authority by rid, name and info
  
  Authority::Authority (const String& rid, const String& name,
			const String& info) : Identity (rid, name, info) {
    p_cred = nullptr;
  }
  
  // create an authority by name and input stream

  Authority::Authority (const String& name, InputStream* is) {
    p_cred = nullptr;
    // create an authority reader
    AuthorityReader ard(name, is);
    // extract an authority from the xml
    Authority auth = ard.xmlto ();
    // copy locally
    this->operator = (auth);
  }

  // copy construct this authority

  Authority::Authority (const Authority& that) {
    that.rdlock ();
    try {
      // copy the base identity
      Identity::operator = (that);
      // copy locally
      Object::iref (p_cred = that.p_cred);
      that.unlock ();
    } catch (...) {
      that.unlock ();
      throw;
    }
  }

  // destroy this authority

  Authority::~Authority (void) {
    Object::dref (p_cred);
  }
  
  // assign a authority to this one

  Authority& Authority::operator = (const Authority& that) {
    // check for self-assignation
    if (this == &that) return *this;
    // lock and assign
    wrlock ();
    that.rdlock ();
    try {
      // copy the base identity
      Identity::operator = (that);
      // copy locally
      Object::iref (that.p_cred); Object::dref (p_cred); p_cred = that.p_cred;
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

  String Authority::repr (void) const {
    return "Authority";
  }

  // return a clone of this object

  Object* Authority::clone (void) const {
    return new Authority (*this);
  }
  
  // return the serial did

  t_word Authority::getdid (void) const {
    return SRL_DEOD_CSM;
  }

  // return the serial sid

  t_word Authority::getsid (void) const {
    return SRL_AUTH_SID;
  }
  
  // serialize this authority

  void Authority::wrstream (OutputStream& os) const {
    rdlock ();
    try {
      // serialize the base identity
      Identity::wrstream (os);
      // serialize locally
      if (p_cred == nullptr) {
	Serial::wrnilid (os);
      } else {
	p_cred->serialize (os);
      }
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // deserialize this authority

  void Authority::rdstream (InputStream& is) {
    wrlock ();
    try {
      // deserialize the base identity
      Identity::rdstream (is);
      // deserialize locally
      Object::dref (p_cred);
      Object::iref (p_cred =
		    dynamic_cast <Credential*> (Serial::deserialize (is)));
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // request an information list

  Plist Authority::getplst (void) const {
    rdlock ();
    try {
      Plist result = Identity::getplst ();
      if (p_cred != nullptr) {
        for (long k = 0L; k < p_cred->length (); k++) {
          result.add (PN_BLB_HASH, PI_BLB_HASH, p_cred->getcval(k));
        }
      }
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set the authority credential

  void Authority::setcred (const Credential& cred) {
    wrlock ();
    try {
      Object::dref (p_cred);
      Object::iref (p_cred = new Credential (cred));
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // get the authority credential

  Credential* Authority::getcred (void) const {
    rdlock ();
    try {
      Credential* result = p_cred;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // authority to xml by uri

  void Authority::toxml (const Uri& uri) const {
    rdlock ();
    try {
      // create an authority writer by string uri
      AuthorityWriter awr (*this, uri);
      // write the authority
      awr.write ();
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }
  // authority to xml by string uri

  void Authority::toxml (const String& suri) const {
    rdlock ();
    try {
      // create an authority writer by string uri
      AuthorityWriter awr (*this, suri);
      // write the authority
      awr.write ();
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // authority to xml by output stream

  void Authority::toxml (OutputStream* os) const {
    rdlock ();
    try {
      // create an authority writer by string uri
      AuthorityWriter awr (*this, os);
      // write the authority
      awr.write ();
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // validate a credential object for acceptance

  bool Authority::validate (const Credential& cred) const {
    rdlock ();
    try {
      bool result = (p_cred == nullptr) ? false : p_cred->validate (cred);
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
  static const long QUARK_TOXML   = zone.intern ("to-xml");
  static const long QUARK_VALIDP  = zone.intern ("valid-p");
  static const long QUARK_SETCRED = zone.intern ("set-credential");
  static const long QUARK_GETCRED = zone.intern ("get-credential");

  // create a new object in a generic way

  Object* Authority::mknew (Vector* argv) {
    long argc = (argv == nullptr) ? 0 : argv->length ();

    // check for 1 argument
    if (argc == 1) {
      Object* obj = argv->get (0);
      // check for a uri
      auto uri = dynamic_cast<Uri*>(obj);
      if (uri != nullptr) return new Authority(*uri);
      // check for a string
      auto name = dynamic_cast<String*>(obj);
      if (name != nullptr) return new Authority(*name);
      // invalid object
      throw Exception ("type-error", "invalid object for authority constructor",
                       Object::repr (obj));
    }
    // check for 2 arguments
    if (argc == 2) {
      String name = argv->getstring (0);
      Object* obj = argv->get (1);
      // check for a string
      auto info = dynamic_cast<String*>(obj);
      if (info != nullptr) return new Authority(name, *info);
      // check for an input stream
      auto is = dynamic_cast<InputStream*>(obj);
      if (is != nullptr) return new Authority(name, is);
      // invalid object
      throw Exception ("type-error", "invalid object for authority constructor",
                       Object::repr (obj));

    }
    // check for 3 arguments
    if (argc == 3) {
      String  rid = argv->getstring (0);
      String name = argv->getstring (1);
      String info = argv->getstring (2);
      return new Authority (rid, name, info);
    }
    // invalid arguments
    throw Exception ("argument-error", 
                     "invalid arguments with authority constructor");
  }

  // return true if the given quark is defined

  bool Authority::isquark (const long quark, const bool hflg) const {
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
  
  Object* Authority::apply (Evaluable* zobj, Nameset* nset, const long quark,
			    Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();
    
    // dispatch 0 argument
    if (argc == 0) {
      if (quark == QUARK_GETCRED) return getcred ();
    }
    if (argc == 1) {
      if (quark == QUARK_SETCRED) {
        // check for a string vector
        Object*  obj = argv->get (0);
        Credential* cred = dynamic_cast <Credential*> (obj);
        if (cred == nullptr) {
          throw Exception ("type-error", "invalid object with set-credential",
                           Object::repr (obj));
        }
        setcred (*cred);
        return nullptr;
      }
      if (quark == QUARK_VALIDP) {
        // check for a credential object
        Object*  obj = argv->get (0);
        Credential* cred = dynamic_cast <Credential*> (obj);
        if (cred == nullptr) {
          throw Exception ("type-error", "invalid object with valid-p",
                           Object::repr (obj));
        }
        return new Boolean (validate (*cred));
      }
      if (quark == QUARK_TOXML) {
        Object* obj = argv->get (0);
        // check for an uri
        auto uri = dynamic_cast <Uri*> (obj);
        if (uri != nullptr) {
          toxml(*uri);
          return nullptr;
        }
        // check for a string uri
        auto suri = dynamic_cast <String*> (obj);
        if (suri != nullptr) {
          toxml(*suri);
          return nullptr;
        }
        // check for an output string
        auto os = dynamic_cast <OutputStream*> (obj);
        if (os != nullptr) {
          toxml(os);
          return nullptr;
        }
        // invalid object
        throw Exception ("type-error", "invalid object for to-xml",
                         Object::repr (obj));
      }
    }
    // call the identity methods
    return Identity::apply (zobj, nset, quark, argv);
  }
}
