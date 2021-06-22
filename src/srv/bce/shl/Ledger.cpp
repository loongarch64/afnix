// ---------------------------------------------------------------------------
// - Ledger.cpp                                                              -
// - afnix:bce service - blockchain ledger class implementation              -
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
#include "Bcesid.hxx"
#include "Ledger.hpp"
#include "Boolean.hpp"
#include "Integer.hpp"
#include "Evaluable.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - private section                                                       -
  // -------------------------------------------------------------------------

  // the ledger information plist
  static const String PN_LGR_CTIM = "PN-LGR-CTIM";
  static const String PI_LGR_CTIM = "LEDGER CREATION TIME";
  static const String PN_LGR_MTIM = "PN-LGR-MTIM";
  static const String PI_LGR_MTIM = "LEDGER MODIFICATION TIME";
  static const String PN_LGR_BLEN = "PN-LGR-BLEN";
  static const String PI_LGR_BLEN = "LEDGER BLOCK LENGTH";
  
  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a default ledger

  Ledger::Ledger (void) {
    d_ctim = 0L;
    d_mtim = 0L;
    p_blks = nullptr;
  }
  
  // copy construct this ledger

  Ledger::Ledger (const Ledger& that) {
    that.rdlock ();
    try {
      d_ctim = that.d_ctim;
      d_mtim = that.d_mtim;
      Object::iref (p_blks = that.p_blks);
      that.unlock ();
    } catch (...) {
      that.unlock ();
      throw;
    }
  }
  
  // copy move this ledger

  Ledger::Ledger (Ledger&& that) noexcept :
    Serial (static_cast<Serial&&>(that)) {
    that.wrlock ();
    try {
      d_ctim = that.d_ctim; that.d_ctim = 0L;
      d_mtim = that.d_mtim; that.d_mtim = 0L;
      p_blks = that.p_blks; that.p_blks = nullptr;
    } catch (...) {
      d_ctim = 0L;
      d_mtim = 0L;
      p_blks = nullptr;
    }
    that.unlock ();
  }

  // destroy this ledger

  Ledger::~Ledger (void) {
    Object::dref (p_blks);
  }
  
  // assign a ledger to this one

  Ledger& Ledger::operator = (const Ledger& that) {
    // check for self assignation
    if (this == &that) return *this;
    // lock and assign
    wrlock ();
    that.rdlock ();
    try {
      d_ctim = that.d_ctim;
      d_mtim = that.d_mtim;
      Object::iref (that.p_blks); Object::dref (p_blks); p_blks = that.p_blks;
      unlock ();
      that.unlock ();
      return *this;
    } catch (...) {
      unlock ();
      that.unlock ();
      throw;
    }
  }
  
  // move a ledger to this one

  Ledger& Ledger::operator = (Ledger&& that) noexcept {
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
      Object::dref (p_blks); p_blks = that.p_blks; that.p_blks = nullptr;
    } catch (...) {
      d_ctim = 0L;
      d_mtim = 0L;
      p_blks = nullptr;
    }
    unlock ();
    that.unlock ();
    return *this;
  }
  
  // return the ledger class name

  String Ledger::repr (void) const {
    return "Ledger";
  }

  // return a clone of this object

  Object* Ledger::clone (void) const {
    return new Ledger (*this);
  }
  
  // return the serial did

  t_word Ledger::getdid (void) const {
    return SRL_DEOD_BCE;
  }

  // return the serial sid
  
  t_word Ledger::getsid (void) const {
    return SRL_LGDR_SID;
  }

  // serialize this 

  void Ledger::wrstream (OutputStream& os) const {
    rdlock ();
    try {
      Serial::wrlong (d_ctim, os);
      Serial::wrlong (d_mtim, os);
      if (p_blks == nullptr) {
	Serial::wrnilid (os);
      } else {
	p_blks->serialize(os);
      }
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // deserialize this ledger

  void Ledger::rdstream (InputStream& is) {
    wrlock ();
    try {
      d_ctim = Serial::rdlong (is);
      d_mtim = Serial::rdlong (is);
      Object::iref (p_blks = dynamic_cast<Vector*>(Serial::deserialize(is)));
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // get the ledger information plist

  Plist Ledger::getplst (void) const {
    rdlock ();
    try {
      // create a result plist
      Plist result;
      // fill the times
      if (d_ctim > 0) {
	result.add (PN_LGR_CTIM, PI_LGR_CTIM, Date(d_ctim).toiso(true));
      }
      if (d_mtim > 0) {
	result.add (PN_LGR_MTIM, PI_LGR_MTIM, Date(d_mtim).toiso(true));
      }
      // add the blocks
      long blen = (p_blks == nullptr) ? 0L : p_blks->length ();
      result.add (PN_LGR_BLEN, PI_LGR_BLEN, (t_long) blen);
      for (long k = 0L; k < blen; k++) {
	// get the block
	auto blok = dynamic_cast<Block*>(p_blks->get (k));
	if (blok == nullptr) continue;
	// merge in the result plist
	result = result.merge (blok->getplst (), k);
      }
      // here it is
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // get the ledger creation time

  t_long Ledger::getctim (void) const {
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

  // get the ledger modification time

  t_long Ledger::getmtim (void) const {
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

  // get the ledger length

  long Ledger::length (void) const {
    rdlock ();
    try {
      long result = (p_blks == nullptr) ? 0L : p_blks->length ();;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get a block by index

  Block* Ledger::get (const long bidx) const {
    rdlock ();
    try {
      auto result = (p_blks == nullptr)
	? nullptr
	: dynamic_cast<Block*>(p_blks->get(bidx));
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // link a transaction to the ledger

  bool Ledger::link (Transaction* tran, const Key& skey) {
    // check for null
    if (tran == nullptr) return false;
    // lock and add
    wrlock ();
    try {
      // check for a block ledger
      if (p_blks == nullptr) Object::iref (p_blks = new Vector);
      // create the block by transaction
      Block* blok = new Block (tran);
      // add the block to the ledger
      p_blks->add (blok);
      // update the times
      if (d_ctim == 0L) {
	d_ctim = Time::gettclk ();
	d_mtim = d_ctim;
      } else {
	d_mtim = Time::gettclk ();
      }
      unlock ();
      return true;
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
  static const long QUARK_GET     = zone.intern ("get");
  static const long QUARK_LINK    = zone.intern ("link");
  static const long QUARK_LENGTH  = zone.intern ("length");
  static const long QUARK_GETCTIM = zone.intern ("get-creation-time");
  static const long QUARK_GETMTIM = zone.intern ("get-modification-time");

  // create a new object in a generic way

  Object* Ledger::mknew (Vector* argv) {
    long argc = (argv == nullptr) ? 0 : argv->length ();
    // create a default ledger
    if (argc == 0) return new Ledger;
    // argument error
    throw Exception ("argument-error",
		     "too many argument with ledger constructor");
  }

  // return true if the given quark is defined

  bool Ledger::isquark (const long quark, const bool hflg) const {
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
  
  Object* Ledger::apply (Evaluable* zobj, Nameset* nset, const long quark,
			Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();
    
    // check for 0 argument
    if (argc == 0) {
      if (quark == QUARK_LENGTH)  return new Integer (length ());
      if (quark == QUARK_GETCTIM) return new Integer (getctim ());
      if (quark == QUARK_GETMTIM) return new Integer (getmtim ());
    }
    // check for 1 argument
    if (argc == 1) {
      if (quark == QUARK_GET) {
	rdlock ();
	try {
	  long bidx = argv->getlong (0);
	  Object* result = get (bidx);
	  zobj->post (result);
	  unlock ();
	  return result;
	} catch (...) {
	  unlock ();
	  throw;
	}
      }
    }
    // check for 2 arguments
    if (argc == 2) {
      if (quark == QUARK_LINK) {
	Object* obj = argv->get (0);
	auto tran = dynamic_cast <Transaction*> (obj);
	if (tran == nullptr) {
	  throw Exception ("type-error", "invalid object for ledger link",
			   Object::repr (obj));
	}
	obj = argv->get (1);
	auto skey = dynamic_cast <Key*> (obj);
	if (skey == nullptr) {
	  throw Exception ("type-error", "invalid object for ledger link",
			   Object::repr (obj));
	}
	
	return new Boolean (link (tran, *skey));
      }
    }
    // call the serial method
    return Serial::apply (zobj, nset, quark, argv);
  }
}
