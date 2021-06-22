// ---------------------------------------------------------------------------
// - Notary.cpp                                                              -
// - afnix:csm service - notary class implementation                         -
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

#include "Visa.hpp"
#include "Vector.hpp"
#include "Notary.hpp"
#include "Notary.hxx"
#include "Boolean.hpp"
#include "Evaluable.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"

namespace afnix {
  
  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------
  
  // create a default notary

  Notary::Notary (void) {
    p_relm = nullptr;
  }

  // create a notary by uri

  Notary::Notary (const Uri& uri) {
    // create a notary reader
    NotaryReader nrd(uri);
    // extract a notary from the xml
    Notary ntry = nrd.xmlto ();
    // update the object
    *this = ntry;
  }

  // create a notary by name

  Notary::Notary (const String& name) : Principal (name) {
    p_relm = nullptr;
  }

  // create a notary by name and info

  Notary::Notary (const String& name,
		  const String& info) : Principal (name, info) {
    p_relm = nullptr;
  }
  
  // create a notary by name and input stream

  Notary::Notary (const String& name, InputStream* is) {
    // create a notary reader
    NotaryReader nrd(name, is);
    // extract a notary from the xml
    Notary ntry = nrd.xmlto ();
    // update the object
    *this = ntry;
  }

  // copy construct this notary

  Notary::Notary (const Notary& that) {
    that.rdlock ();
    try {
      // copy base object
      Principal::operator = (that);
      // copy locally
      Object::iref (p_relm = that.p_relm);
      that.unlock ();
    } catch (...) {
      that.unlock ();
      throw;
    }
  }
  
  // destroy this notary

  Notary::~Notary (void) {
    Object::dref (p_relm);
  }

  // assign a notary to this one

  Notary& Notary::operator = (const Notary& that) {
    // check for self-assignation
    if (this == &that) return *this;
    // lock and assign
    wrlock ();
    that.rdlock ();
    try {
      // assign base object
      Principal::operator = (that);
      // assign locally
      Object::iref (p_relm = that.p_relm);
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

  String Notary::repr (void) const {
    return "Notary";
  }

  // return a clone of this object

  Object* Notary::clone (void) const {
    return new Notary (*this);
  }
  
  // set the notary realm

  void Notary::setrelm (Realm* relm) {
    wrlock ();
    try {
      Object::iref (relm); Object::dref (p_relm);
      p_relm = relm;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the notary realm

  Realm* Notary::getrelm (void) const {
    rdlock ();
    try {
      Realm* result = p_relm;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // notary to xml by uri

  void Notary::toxml (const Uri& uri) const {
    rdlock ();
    try {
      // create a notary by string uri
      NotaryWriter nwr (*this, uri);
      // write the notary
      nwr.write ();
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // notary to xml by string uri

  void Notary::toxml (const String& suri) const {
    rdlock ();
    try {
      // create a notary by string uri
      NotaryWriter nwr (*this, suri);
      // write the notary
      nwr.write ();
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // notary to xml by output stream

  void Notary::toxml (OutputStream* os) const {
    rdlock ();
    try {
      // create a notary by string uri
      NotaryWriter nwr (*this, os);
      // write the notary
      nwr.write ();
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // validate an act

  bool Notary::validate (const Act& act) const {
    rdlock ();
    try {
      bool result = true;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // validate an identity

  bool Notary::validate (const Identity& idty) const {
    rdlock ();
    try {
      bool result = true;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // validate a principal

  bool Notary::validate (const Principal& auth) const {
    rdlock ();
    try {
      bool result = true;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // deliver an act to an identity by name and credential

  Act* Notary::deliver (const String& name, const Credential& cred,
			const Identity& rqst) const {
    rdlock ();
    try {
      // validate the requester
      if (validate (rqst) == false) {
	unlock ();
	return nullptr;
      }
      // use the realm to validate by name
      if ((p_relm == nullptr) || (p_relm->validate (name, cred) == false)) {
	unlock ();
	return nullptr;
      }
      // create an act which validate this request
      Identity user = name;
      Act* act = new Visa (*this, rqst, user);
      unlock ();
      return act;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // -------------------------------------------------------------------------
  // - object section                                                        -
  // -------------------------------------------------------------------------
  
  // the quark zone
  static const long QUARK_ZONE_LENGTH = 5;
  static QuarkZone  zone (QUARK_ZONE_LENGTH);

  // the object supported quarks
  static const long QUARK_TOXML   = zone.intern ("to-xml");
  static const long QUARK_VALIDP  = zone.intern ("valid-p");
  static const long QUARK_SETRELM = zone.intern ("set-realm");
  static const long QUARK_GETRELM = zone.intern ("get-realm");
  static const long QUARK_DELIVER = zone.intern ("deliver");
  
  // create a new object in a generic way

  Object* Notary::mknew (Vector* argv) {
    long argc = (argv == nullptr) ? 0 : argv->length ();

    // check for 0 argument
    if (argc == 0) return new Notary;
    // check for 1 argument
    if (argc == 1) {
      Object* obj = argv->get (0);
      // check for a uri
      auto uri = dynamic_cast<Uri*>(obj);
      if (uri != nullptr) return new Notary(*uri);
      // check for a string
      auto name = dynamic_cast<String*>(obj);
      if (name != nullptr) return new Notary(*name);
      // invalid object
      throw Exception ("type-error", "invalid object for notary constructor",
		       Object::repr (obj));
    }
    // check for 2 arguments
    if (argc == 2) {
      String name = argv->getstring (0);
      Object* obj = argv->get (1);
      // check for a string
      auto info = dynamic_cast<String*>(obj);
      if (info != nullptr) return new Notary(name, *info);
      // check for an input stream
      auto is = dynamic_cast<InputStream*>(obj);
      if (is != nullptr) return new Notary(name, is);
      // invalid object
      throw Exception ("type-error", "invalid object for notary constructor",
		       Object::repr (obj));
    }
    // invalid arguments
    throw Exception ("argument-error", 
                     "invalid arguments with notary constructor");
  }

  // return true if the given quark is defined

  bool Notary::isquark (const long quark, const bool hflg) const {
    rdlock ();
    try {
      if (zone.exists (quark) == true) {
	unlock ();
	return true;
      }
      bool result = hflg ? Principal::isquark (quark, hflg) : false;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // apply this object with a set of arguments and a quark
  
  Object* Notary::apply (Evaluable* zobj, Nameset* nset, const long quark,
			 Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();
    
    // dispatch 0 argument
    if (argc == 0) {
      if (quark == QUARK_GETRELM) {
	rdlock ();
	try {
	  Object* result = getrelm ();
	  zobj->post (result);
	  unlock ();
	  return result;
	} catch (...) {
	  unlock ();
	  throw;
	}
      }
    }
    // dispatch 1 argument
    if (argc == 1) {
      if (quark == QUARK_SETRELM) {
	Object* obj = argv->get (0);
	Realm*  relm = dynamic_cast <Realm*> (obj);
	if (relm == nullptr) {
	  throw Exception ("type-error", "invalid object for set-realm",
			   Object::repr (obj));
	}
	setrelm (relm);
        return nullptr;
      }
      if (quark == QUARK_VALIDP) {
	Object* obj = argv->get (0);
	// check for an act
	Act* act = dynamic_cast <Act*> (obj);
	if (act != nullptr) return new Boolean (validate (*act));
	// check for a principal
	Principal* pcpl = dynamic_cast <Principal*> (obj);
	if (pcpl != nullptr) return new Boolean (validate (*pcpl));
	// check for an identity
	Identity* idty = dynamic_cast <Identity*> (obj);
	if (idty != nullptr) return new Boolean (validate (*idty));
	// invalid object
	throw Exception ("type-error", "invalid object for valid-p",
			 Object::repr (obj));
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
    // dispatch 3 arguments
    if (argc == 3) {
      if (quark == QUARK_DELIVER) {
	String name  = argv->getstring (0);
	// get the credential
	Object* obj = argv->get (1);
	Credential* cred = dynamic_cast <Credential*> (obj);
	if (cred == nullptr) {
	  throw Exception ("type-error", "invalid credential object",
			   Object::repr (obj));
	}
	// get the identity
	obj = argv->get (2);
	Identity* idty = dynamic_cast <Identity*> (obj);
	if (idty == nullptr) {
	  throw Exception ("type-error", "invalid identity object",
			   Object::repr (obj));
	}
	return deliver (name, *cred, *idty);
      }
    }
    // call the principal methods
    return Principal::apply (zobj, nset, quark, argv);
  }
}
