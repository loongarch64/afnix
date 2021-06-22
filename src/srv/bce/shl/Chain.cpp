// ---------------------------------------------------------------------------
// - Chain.cpp                                                          -
// - afnix:bce service - blockchain class implementation                     -
// ---------------------------------------------------------------------------
// - This program is free software;  you can redistribute it  and/or  modify -
// - it provided that this copyright notice is kept intact.                  -
// -                                                                         -
// - This program  is  distributed in  the hope  that it will be useful, but -
// - without  any  warranty;  without  even   the   implied    warranty   of -
// - merchantability or fitness for a blockicular purpose.  In no event shall -
// - the copyright holder be liable for any  direct, indirect, incidental or -
// - special damages arising in any way out of the use of this software.     -
// ---------------------------------------------------------------------------
// - copyright (c) 1999-2021 amaury darsch                                   -
// ---------------------------------------------------------------------------

#include "Date.hpp"
#include "Chain.hpp"
#include "Bcesid.hxx"
#include "Utility.hpp"
#include "Integer.hpp"
#include "Evaluable.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - private section                                                       -
  // -------------------------------------------------------------------------

  // the chain information plist
  static const String PN_CHN_CTIM = "PN-CHN-CTIM";
  static const String PI_CHN_CTIM = "CHAIN CREATION TIME";
  static const String PN_CHN_MTIM = "PN-CHN-MTIM";
  static const String PI_CHN_MTIM = "CHAIN MODIFICATION TIME";
  static const String PN_CHN_RLEN = "PN-CHN-RLEN";
  static const String PI_CHN_RLEN = "CHAIN REQUEST LENGTH";
  
  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a default chain

  Chain::Chain (void) {
    d_ctim = 0L;
    d_mtim = 0L;
    p_rstk = nullptr;
    p_lgdr = nullptr;
  }
  
  // copy construct this chain

  Chain::Chain (const Chain& that) {
    that.rdlock ();
    try {
      d_ctim = that.d_ctim;
      d_mtim = that.d_mtim;
      Object::iref (p_rstk = that.p_rstk);
      Object::iref (p_lgdr = that.p_lgdr);
      that.unlock ();
    } catch (...) {
      that.unlock ();
      throw;
    }
  }
  
  // copy move this chain

  Chain::Chain (Chain&& that) noexcept : Serial (static_cast<Serial&&>(that)) {
    that.wrlock ();
    try {
      d_ctim = that.d_ctim; that.d_ctim = 0L;
      d_mtim = that.d_mtim; that.d_mtim = 0L;
      p_rstk = that.p_rstk; that.p_rstk = nullptr;
      p_lgdr = that.p_lgdr; that.p_lgdr = nullptr;
    } catch (...) {
      d_ctim = 0L;
      d_mtim = 0L;
      p_rstk = nullptr;
      p_lgdr = nullptr;
    }
    that.unlock ();
  }

  // destroy this chain

  Chain::~Chain (void) {
    Object::dref (p_lgdr);
  }
  
  // assign a chain to this one

  Chain& Chain::operator = (const Chain& that) {
    // check for self assignation
    if (this == &that) return *this;
    // lock and assign
    wrlock ();
    that.rdlock ();
    try {
      d_ctim = that.d_ctim;
      d_mtim = that.d_mtim;
      Object::iref (that.p_rstk); Object::dref (p_rstk); p_rstk = that.p_rstk;
      Object::iref (that.p_lgdr); Object::dref (p_lgdr); p_lgdr = that.p_lgdr;
      unlock ();
      that.unlock ();
      return *this;
    } catch (...) {
      unlock ();
      that.unlock ();
      throw;
    }
  }
  
  // move a chain to this one

  Chain& Chain::operator = (Chain&& that) noexcept {
    // check for self move
    if (this == &that) return *this;
    // lock and assign
    wrlock ();
    that.wrlock ();
    try {
      // assign base object
      Serial::operator = (static_cast<Serial&&>(that));
      // move locally
      d_ctim = that.d_ctim; that.d_ctim = 0L;
      d_mtim = that.d_mtim; that.d_mtim = 0L;
      Object::dref (p_rstk); p_rstk = that.p_rstk; that.p_rstk = nullptr;
      Object::dref (p_lgdr); p_lgdr = that.p_lgdr; that.p_lgdr = nullptr;
    } catch (...) {
      d_ctim = 0L;
      d_mtim = 0L;
      p_rstk = nullptr;
      p_lgdr = nullptr;
    }
    unlock ();
    that.unlock ();
    return *this;
  }
  
  // return the chain class name

  String Chain::repr (void) const {
    return "Chain";
  }

  // return a clone of this object

  Object* Chain::clone (void) const {
    return new Chain (*this);
  }
  
  // return the serial did

  t_word Chain::getdid (void) const {
    return SRL_DEOD_BCE;
  }

  // return the serial sid
  
  t_word Chain::getsid (void) const {
    return SRL_BCHN_SID;
  }

  // serialize this 

  void Chain::wrstream (OutputStream& os) const {
    rdlock ();
    try {
      Serial::wrlong (d_ctim, os);
      Serial::wrlong (d_mtim, os);
      if (p_rstk == nullptr) {
	Serial::wrnilid (os);
      } else {
	p_rstk->serialize(os);
      }
      if (p_lgdr == nullptr) {
	Serial::wrnilid (os);
      } else {
	p_lgdr->serialize(os);
      }
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // deserialize this chain

  void Chain::rdstream (InputStream& is) {
    wrlock ();
    try {
      d_ctim = Serial::rdlong (is);
      d_mtim = Serial::rdlong (is);
      Object::iref (p_rstk = dynamic_cast<Vector*>(Serial::deserialize(is)));
      Object::iref (p_lgdr = dynamic_cast<Ledger*>(Serial::deserialize(is)));
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the chain information plist

  Plist Chain::getplst (void) const {
    rdlock ();
    try {
      // create a result plist
      Plist result;
      // fill the times
      if (d_ctim > 0) {
	result.add (PN_CHN_CTIM, PI_CHN_CTIM, Date(d_ctim).toiso(true));
      }
      if (d_mtim > 0) {
	result.add (PN_CHN_MTIM, PI_CHN_MTIM, Date(d_mtim).toiso(true));
      }
      // add the requests
      long rlen = (p_rstk == nullptr) ? 0L : p_rstk->length ();
      result.add (PN_CHN_RLEN, PI_CHN_RLEN, (t_long) rlen);
      for (long k = 0L; k < rlen; k++) {
	// get the request
	auto rqst = dynamic_cast<Request*>(p_rstk->get (k));
	if (rqst == nullptr) continue;
	// merge in the result plist
	result = result.merge (rqst->getplst (), k);
      }
      // add the ledger
      if (p_lgdr != nullptr) result+= p_lgdr->getplst ();
      // here it is
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // get the chain creation time

  t_long Chain::getctim (void) const {
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

  // get the chain modification time

  t_long Chain::getmtim (void) const {
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

  // get the ledger object

  Ledger* Chain::getlgdr (void) {
    rdlock ();
    try {
      Ledger* result = p_lgdr;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // get the ledger object

  const Ledger* Chain::getlgdr (void) const {
    rdlock ();
    try {
      Ledger* result = p_lgdr;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // push a request to the stack

  void Chain::push (Request* rqst) {
    // chech for nil
    if (rqst == nullptr) return;
    wrlock ();
    try {
      // add the request
      if (p_rstk == nullptr) p_rstk = new Vector;
      p_rstk->add (rqst);
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

  // process the request stack by signing key

  bool Chain::process (const Key& skey, Logger* logr) {
    wrlock ();
    try {
      bool status = true;
      while ((p_rstk != nullptr) && (p_rstk->empty () == false)) {
	// get the request
	auto rqst = dynamic_cast<Request*>(p_rstk->pop ());
	if ((rqst == nullptr) || (status == false)) continue;
	status = status & validate (*rqst, logr);
	if (status == false) continue;
	// default processing
	status = status && process (*rqst, skey, logr);
      }
      // restore chain state with bad status
      unlock ();
      return status;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // -------------------------------------------------------------------------
  // - protected section                                                     -
  // -------------------------------------------------------------------------

  // validate a request

  bool Chain:: validate (const Request&, Logger*) const {
    return true;
  }
  
  // process a request by signing key

  bool Chain::process (const Request& rqst, const Key& skey, Logger* logr) {
    wrlock ();
    try {
      bool status = true;
      long code = rqst.getcode ();
      // check for a link code
      if (code == Request::TR_CODE_LINK) {
	if (p_lgdr == nullptr) p_lgdr = new Ledger;
	status = p_lgdr->link (rqst.gettran (), skey);
      } else {
	if (logr != nullptr) {
	  String mesg = "[afnix] invalid request code to process ";
	  mesg += Utility::tostring (code);
	  logr->add (Logger::MLVL_XERR, mesg);
	}
	status = false;
      }
      // done
      unlock ();
      return status;
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
  static const long QUARK_PUSH    = zone.intern ("push");
  static const long QUARK_GETLGDR = zone.intern ("get-ledger");
  static const long QUARK_GETCTIM = zone.intern ("get-creation-time");
  static const long QUARK_GETMTIM = zone.intern ("get-modification-time");

  // create a new object in a generic way

  Object* Chain::mknew (Vector* argv) {
    long argc = (argv == nullptr) ? 0 : argv->length ();
    // create a default chain
    if (argc == 0) return new Chain;
    // argument error
    throw Exception ("argument-error",
		     "too many argument with chain constructor");
  }

  // return true if the given quark is defined

  bool Chain::isquark (const long quark, const bool hflg) const {
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
  
  Object* Chain::apply (Evaluable* zobj, Nameset* nset, const long quark,
			Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();
    
    // check for 0 argument
    if (argc == 0) {
      if (quark == QUARK_GETCTIM) return new Integer (getctim ());
      if (quark == QUARK_GETMTIM) return new Integer (getmtim ());
      if (quark == QUARK_GETLGDR) {
	rdlock ();
	try {
	  Object* result = getlgdr ();
	  zobj->post (result);
	  unlock ();
	  return result;
	} catch (...) {
	  unlock ();
	  throw;
	}
      }
    }
    // check for 1 argument
    if (argc == 1) {
      if (quark == QUARK_PUSH) {
	Object* obj = argv->get (0);
	auto rqst = dynamic_cast <Request*> (obj);
	if (rqst == nullptr) {
	  throw Exception ("type-error", "invalid object for chain push",
			   Object::repr (obj));
	}
	push (rqst);
	return nullptr;
      }
    }
    // call the serial method
    return Serial::apply (zobj, nset, quark, argv);
  }
}
