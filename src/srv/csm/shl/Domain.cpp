// ---------------------------------------------------------------------------
// - Domain.cpp                                                              -
// - afnix:csm service - blob domain class implementation                    -
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

#include "Time.hpp"
#include "Domain.hpp"
#include "Csmsid.hxx"
#include "Vector.hpp"
#include "Boolean.hpp"
#include "Integer.hpp"
#include "Evaluable.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a nil domain

  Domain::Domain (void) {
    d_rid.clear ();
    d_ctim = 0L;
    d_mtim = 0L;
  }

  // create a domain by name

  Domain::Domain (const String& name) : Collection (name) {
    d_rid.clear ();
    d_ctim = 0L;
    d_mtim = 0L;
  }

  // create a domain by name and info

  Domain::Domain (const String& name,
		  const String& info) : Collection (name, info) {
    d_rid.clear ();
    d_ctim = 0L;
    d_mtim = 0L;
  }
  
  // create a domain by name, info and rid

  Domain::Domain (const String& name, const String& info,
		  const String& rid) : Collection (name, info) {
    d_rid = rid;
    d_ctim = 0L;
    d_mtim = 0L;
  }

  // copy construct this domain

  Domain::Domain (const Domain& that) {
    that.rdlock ();
    try {
      // copy base part
      Collection::operator = (that);
      // copy locally
      d_rid  = that.d_rid;
      d_ctim = that.d_ctim;
      d_mtim = that.d_mtim;
      that.unlock ();
    } catch (...) {
      that.unlock ();
      throw;
    }
  }

  // copy move this domain

  Domain::Domain (Domain&& that) noexcept :
    Collection (static_cast<Collection&&>(that)) {
    that.wrlock ();
    try {
      d_rid  = that.d_rid;  that.d_rid.clear ();
      d_ctim = that.d_ctim; that.d_ctim = 0L;
      d_mtim = that.d_mtim; that.d_mtim = 0L;
    } catch (...) {
      d_rid.clear ();
    }
    that.unlock ();
  }
  
  // copy construct this domain

  Domain& Domain::operator = (const Domain& that) {
    // check for self-assignation
    if (this == &that) return *this;
    // lock and assign
    wrlock ();
    that.rdlock ();
    try {
      // copy base collection
      Collection::operator = (that);
      // copy locally
      d_rid  = that.d_rid;
      d_ctim = that.d_ctim;
      d_mtim = that.d_mtim;
      unlock ();
      that.unlock ();
      return *this;
    } catch (...) {
      unlock ();
      that.unlock ();
      throw;
    }
  }

  // move a domain to this one
  
  Domain& Domain::operator = (Domain&& that) noexcept {
    // check for self move
    if (this == &that) return *this;
    // lock and assign
    wrlock ();
    that.wrlock ();
    try {
      // assign base object
      Collection::operator = (static_cast<Collection&&>(that));
      // move locally
      d_rid  = that.d_rid;  that.d_rid.clear ();
      d_ctim = that.d_ctim; that.d_ctim = 0L;
      d_mtim = that.d_mtim; that.d_mtim = 0L;
    } catch (...) {
      d_rid.clear ();
    }
    unlock ();
    that.unlock ();
    return *this;
  }
  
  // return the domain class name

  String Domain::repr (void) const {
    return "Domain";
  }

  // return the serial did

  t_word Domain::getdid (void) const {
    return SRL_DEOD_CSM;
  }

  // return the serial sid

  t_word Domain::getsid (void) const {
    return SRL_DOMN_SID;
  }
  
  // serialize this domain

  void Domain::wrstream (OutputStream& os) const {
    rdlock ();
    try {
      // serialize the collection
      Collection::wrstream (os);
      // serialize locally
      d_rid.wrstream (os);
      Serial::wrlong (d_ctim, os);
      Serial::wrlong (d_mtim, os);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // deserialize this domain

  void Domain::rdstream (InputStream& is) {
    wrlock ();
    try {
      // deserialize the collection
      Collection::rdstream (is);
      // deserialize locally
      d_rid.rdstream (is);
      d_ctim = Serial::rdlong (is);
      d_mtim = Serial::rdlong (is);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // add a part (as a blob) in this domain

  Uuid Domain::addpart (Part* part) {
    wrlock ();
    try {
      auto blob = dynamic_cast<Blob*>(part);
      if (blob == nullptr) {
	throw Exception ("domain-error", "invalid object to add as blob",
			 Object::repr (part));
      }
      // get the blob rid and validate
      String rid = blob->getrid ();
      if ((d_rid.isnil () == false) && (d_rid != rid)) {
	throw Exception ("domain-error", 
			 "invalid rid for registered domain", rid);
      }
      // add the blob in the collection
      Uuid kid = Collection::addpart (part);
      // update the times
      if (d_ctim == 0L) {
        d_ctim = Time::gettclk ();
        d_mtim = d_ctim;
      } else {
        d_mtim = Time::gettclk ();
      }
      // unlock and return the key id
      unlock ();
      return kid;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set a part (as a blob) in this domain

  bool Domain::setpart (Part* part) {
    wrlock ();
    try {
      auto blob = dynamic_cast<Blob*>(part);
      if (blob == nullptr) {
	unlock ();
	return false;
      }
      // get the blob rid and validate
      String rid = blob->getrid ();
      if ((d_rid.isnil () == false) && (d_rid != rid)) {
	unlock ();
	return false;
      }
      // paranoid creation time check
      if (d_ctim == 0L) {
	throw Exception ("domain-error", "nil creation time in set-part");
      }
      // set the blob in the collection
      bool result = Collection::setpart (part);
      // update the modification time
      if (result == true) d_mtim = Time::gettclk ();
      // unlock and return the key id
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the domain creation time

  t_long Domain::getctim (void) const {
    rdlock ();
    try {
      t_long result = d_ctim;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // get the domain modification time

  t_long Domain::getmtim (void) const {
    rdlock ();
    try {
      t_long result = d_mtim;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // validate a rid by domain rid

  bool Domain::isrid (const String& rid) const {
    rdlock ();
    try {
      bool result = (d_rid == rid);
      unlock ();
      return result;
    } catch  (...) {
      unlock ();
      throw;
    }
  }

  // set the registration id

  void Domain::setrid (const String& rid) {
    wrlock ();
    try {
      if (d_rid != rid) {
	// get the set length
	long slen = length ();
	// loop in the set
	for (long k = 0L; k < slen; k++) {
	  Object* obj = getat (k);
	  auto blob = dynamic_cast <Blob*> (obj);
	  if (blob == nullptr) {
	    throw Exception ("internal-error", "invalid object in domain",
			     Object::repr (obj));
	  }
	  blob->setrid(rid);
	}
      }
      d_rid = rid;
      // update the times
      if (d_ctim == 0L) {
        d_ctim = Time::gettclk ();
        d_mtim = d_ctim;
      } else {
        d_mtim = Time::gettclk ();
      }
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // get the registration id

  String Domain::getrid (void) const {
    rdlock ();
    try {
      String result = d_rid;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the domain whois

  Whois Domain::whois (void) const {
    rdlock ();
    try {
      Whois wois;
      wois.setname (getname (), getinfo ());
      wois.d_rid  = getrid  ();
      wois.d_ctim = getctim ();
      wois.d_mtim = getmtim ();
      wois.d_dlen = length  ();
      unlock ();
      return wois;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // -------------------------------------------------------------------------
  // - object section                                                        -
  // -------------------------------------------------------------------------

  // the quark zone
  static const long QUARK_ZONE_LENGTH = 6;
  static QuarkZone  zone (QUARK_ZONE_LENGTH);

  // the object supported quarks
  static const long QUARK_WHOIS   = zone.intern ("whois");
  static const long QUARK_ISRIDP  = zone.intern ("rid-p");
  static const long QUARK_SETRID  = zone.intern ("set-rid");
  static const long QUARK_GETRID  = zone.intern ("get-rid");
  static const long QUARK_GETCTIM = zone.intern ("get-creation-time");
  static const long QUARK_GETMTIM = zone.intern ("get-modification-time");

  // create a new object in a generic way

  Object* Domain::mknew (Vector* argv) {
    long argc = (argv == nullptr) ? 0 : argv->length ();
    // check for 0 argument
    if (argc == 0) return new Domain;
    // check for 1 argument
    if (argc == 1) {
      String name = argv->getstring (0);
      return new Domain (name);
    }
    // check for 2 arguments
    if (argc == 2) {
      String name = argv->getstring (0);
      String info = argv->getstring (1);
      return new Domain (name, info);
    }
    // check for 3 arguments
    if (argc == 3) {
      String name = argv->getstring (0);
      String info = argv->getstring (1);
      String  rid = argv->getstring (2);
      return new Domain (name, info, rid);
    }
    throw Exception ("argument-error",
                     "too many argument with domain constructor");
  }

  // return true if the given quark is defined

  bool Domain::isquark (const long quark, const bool hflg) const {
    rdlock ();
    try {
      if (zone.exists (quark) == true) {
	unlock ();
	return true;
      }
      bool result = hflg ? Collection::isquark (quark, hflg) : false;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // apply this object with a set of arguments and a quark
  
  Object* Domain::apply (Evaluable* zobj, Nameset* nset, const long quark,
			 Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();
    
    // check for 0 argument
    if (argc == 0) {
      if (quark == QUARK_WHOIS)   return new Whois   (whois   ());
      if (quark == QUARK_GETRID)  return new String  (getrid  ());
      if (quark == QUARK_GETCTIM) return new Integer (getctim ());
      if (quark == QUARK_GETMTIM) return new Integer (getmtim ());
    }
    // check for 1 argument
    if (argc == 1) {
      if (quark == QUARK_ISRIDP) {
	String rid = argv->getstring (0);
	return new Boolean (isrid (rid));
      }
      if (quark == QUARK_SETRID) {
	String rid = argv->getstring (0);
	setrid (rid);
	return nullptr;
      }
    }
    // call the collection method
    return Collection::apply (zobj, nset, quark, argv);
  }
}
