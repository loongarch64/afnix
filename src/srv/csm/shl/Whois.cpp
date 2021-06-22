// ---------------------------------------------------------------------------
// - Whois.cpp                                                               -
// - afnix:csm service - domain whois class implementation                   -
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

#include "Date.hpp"
#include "Whom.hxx"
#include "Whois.hpp"
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

  // create a nil domain whois

  Whois::Whois (void) {
    d_rid.clear ();
    d_ctim = 0L;
    d_mtim = 0L;
    d_dlen = 0L;
  }

  // copy construct this domain whois

  Whois::Whois (const Whois& that) {
    that.rdlock ();
    try {
      // copy base part
      Taggable::operator = (that);
      // copy locally
      d_rid  = that.d_rid;
      d_ctim = that.d_ctim;
      d_mtim = that.d_mtim;
      d_dlen = that.d_dlen;
      that.unlock ();
    } catch (...) {
      that.unlock ();
      throw;
    }
  }

  // copy construct this domain whis

  Whois& Whois::operator = (const Whois& that) {
    // check for self-assignation
    if (this == &that) return *this;
    // lock and assign
    wrlock ();
    that.rdlock ();
    try {
      // copy base object
      Taggable::operator = (that);
      // copy locally
      d_rid  = that.d_rid;
      d_ctim = that.d_ctim;
      d_mtim = that.d_mtim;
      d_dlen = that.d_dlen;
      unlock ();
      that.unlock ();
      return *this;
    } catch (...) {
      unlock ();
      that.unlock ();
      throw;
    }
  }

  // return the domain whois class name

  String Whois::repr (void) const {
    return "Whois";
  }

  // return the serial did

  t_word Whois::getdid (void) const {
    return SRL_DEOD_CSM;
  }

  // return the serial sid

  t_word Whois::getsid (void) const {
    return SRL_WOIS_SID;
  }
  
  // serialize this domain whois

  void Whois::wrstream (OutputStream& os) const {
    rdlock ();
    try {
      // serialize the taggable
      Taggable::wrstream (os);
      // serialize locally
      d_rid.wrstream (os);
      Serial::wrlong (d_ctim, os);
      Serial::wrlong (d_mtim, os);
      Serial::wrlong (d_dlen, os);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // deserialize this domain whois

  void Whois::rdstream (InputStream& is) {
    wrlock ();
    try {
      // deserialize the taggable
      Taggable::rdstream (is);
      // deserialize locally
      d_rid.rdstream (is);
      d_ctim = Serial::rdlong (is);
      d_mtim = Serial::rdlong (is);
      d_dlen = Serial::rdlong (is);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // check if the whois domain is valid

  bool Whois::valid (void) const {
    rdlock ();
    try {
      bool result = (d_rid.isnil () == true) ? false : true;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the domain id

  String Whois::getrid (void) const {
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

  // get the creation time

  t_long Whois::getctim (void) const {
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
  
  // get the modification time

  t_long Whois::getmtim (void) const {
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
  
  // get the domain length

  long Whois::length (void) const {
    rdlock ();
    try {
      long result = d_dlen;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // merge a domain whois

  void Whois::merge (const Whois& wois) {
    wrlock ();
    try {
      // check for initial whois
      if (d_rid.isnil () == true) {
	*this = wois;
	unlock ();
	return;
      }
      // check for valid rid
      if (d_rid != wois.d_rid) {
	throw Exception ("whois-error", "inconsistent rid in merge", d_rid);
      }
      // check creation time
      if (wois.d_ctim < d_ctim) d_ctim = wois.d_ctim;
      // check modification time
      if (wois.d_mtim > d_mtim) d_mtim = wois.d_mtim;
      // update the length
      d_dlen += wois.d_dlen;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the whois plist

  Plist Whois::getplst (void) const {
    rdlock ();
    try {
      // get the local plist
      Plist result;
      if (d_name.isnil () == false) {
	result.add (PN_PRT_NAME, PI_PRT_NAME, d_name);
      }
      if (d_info.isnil () == false) {
	result.add (PN_PRT_INFO, PI_PRT_INFO, d_info);
      }
      result.add (PN_DMN_XRID, PI_DMN_XRID, d_rid);
      if (d_ctim > 0) {
	result.add (PN_DMN_CTIM, PI_DMN_CTIM, Date(d_ctim).toiso(true));
      }
      if (d_mtim > 0) {
	result.add (PN_DMN_MTIM, PI_DMN_MTIM, Date(d_mtim).toiso(true));
      }
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
  static const long QUARK_ZONE_LENGTH = 6;
  static QuarkZone  zone (QUARK_ZONE_LENGTH);

  // the object supported quarks
  static const long QUARK_LENGTH  = zone.intern ("length");
  static const long QUARK_VALIDP  = zone.intern ("valid-p");
  static const long QUARK_GETRID  = zone.intern ("get-rid");
  static const long QUARK_GETPLST = zone.intern ("get-plist");
  static const long QUARK_GETCTIM = zone.intern ("get-creation-time");
  static const long QUARK_GETMTIM = zone.intern ("get-modification-time");

  // create a new object in a generic way

  Object* Whois::mknew (Vector* argv) {
    long argc = (argv == nullptr) ? 0 : argv->length ();
    // check for 0 argument
    if (argc == 0) return new Whois;
    // invalid arguments
    throw Exception ("argument-error",
                     "too many argument with domain constructor");
  }

  // return true if the given quark is defined
  
  bool Whois::isquark (const long quark, const bool hflg) const {
    rdlock ();
    try {
      if (zone.exists (quark) == true) {
	unlock ();
	return true;
      }
      bool result = hflg ? Taggable::isquark (quark, hflg) : false;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // apply this object with a set of arguments and a quark
  
  Object* Whois::apply (Evaluable* zobj, Nameset* nset, const long quark,
			Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();
    
    // check for 0 argument
    if (argc == 0) {
      if (quark == QUARK_GETRID)  return new String  (getrid  ());
      if (quark == QUARK_LENGTH)  return new Integer (length  ());
      if (quark == QUARK_VALIDP)  return new Boolean (valid   ());
      if (quark == QUARK_GETCTIM) return new Integer (getctim ());
      if (quark == QUARK_GETMTIM) return new Integer (getmtim ());
      if (quark == QUARK_GETPLST) return new Plist   (getplst ());
    }
    // call the taggable method
    return Taggable::apply (zobj, nset, quark, argv);
  }
}
