// ---------------------------------------------------------------------------
// - Block.cpp                                                               -
// - afnix:bce service - block chain block class implementation              -
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
#include "Block.hpp"
#include "Bcesid.hxx"
#include "Crypto.hpp"
#include "Vector.hpp"
#include "Integer.hpp"
#include "Evaluable.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - private section                                                       -
  // -------------------------------------------------------------------------

  // the block information plist
  static const String PN_BLK_STMP = "PN-BLK-STMP";
  static const String PI_BLK_STMP = "BLOCK TIMESTAMP";

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a default block

  Block::Block (void) {
    d_stmp = 0L;
    d_hash = 0L;
    p_tran = nullptr;
    p_sign = nullptr;
  }
  
  // create an block by transaction

  Block::Block (Transaction* tran) {
    d_stmp = Time::gettclk ();
    d_hash = 0L;
    Object::iref (p_tran = tran);
    p_sign = nullptr;
  }

  // copy construct this block

  Block::Block (const Block& that) {
    that.rdlock ();
    try {
      d_stmp = that.d_stmp;
      d_hash = that.d_hash;
      Object::iref (p_tran = that.p_tran);
      Object::iref (p_sign = that.p_sign);
      that.unlock ();
    } catch (...) {
      that.unlock ();
      throw;
    }
  }
  
  // copy move this block

  Block::Block (Block&& that) noexcept {
    that.wrlock ();
    try {
      // move base object
      Signable::operator = (static_cast<Signable&&>(that));
      // copy move locally
      d_stmp = that.d_stmp; that.d_stmp = 0L;
      d_hash = that.d_hash; that.d_hash = 0L;
      p_tran = that.p_tran; that.p_tran = nullptr;
      p_sign = that.p_sign; that.p_sign = nullptr;
    } catch (...) {
      d_stmp = 0L;
      d_hash = 0L;
      p_tran = nullptr;
      p_sign = nullptr;
    }
    that.unlock ();
  }

  // destroy this block

  Block::~Block (void) {
    Object::dref (p_tran);
    Object::dref (p_sign);
  }
  
  // assign a block to this one

  Block& Block::operator = (const Block& that) {
    // check for self assignation
    if (this == &that) return *this;
    // lock and assign
    wrlock ();
    that.rdlock ();
    try {
      Object::iref (that.p_tran); Object::dref (p_tran);
      Object::iref (that.p_sign); Object::dref (p_sign);
      d_stmp = that.d_stmp;
      d_hash = that.d_hash;
      p_tran = that.p_tran;
      p_sign = that.p_sign;
      unlock ();
      that.unlock ();
      return *this;
    } catch (...) {
      unlock ();
      that.unlock ();
      throw;
    }
  }
  
  // move a block to this one

  Block& Block::operator = (Block&& that) noexcept {
    // check for self move
    if (this == &that) return *this;
    // lock and assign
    wrlock ();
    that.wrlock ();
    try {
      // assign base object
      Signable::operator = (static_cast<Signable&&>(that));
      // move locally
      d_stmp = that.d_stmp; that.d_stmp = 0L;
      d_hash = that.d_hash; that.d_hash = 0L;
      p_tran = that.p_tran; that.p_tran = nullptr;
      p_sign = that.p_sign; that.p_sign = nullptr;
    } catch (...) {
      d_stmp = 0L;
      d_hash = 0L;
      p_tran = nullptr;
      p_sign = nullptr;
    }
    unlock ();
    that.unlock ();
    return *this;
  }
  
  // return the block class name

  String Block::repr (void) const {
    return "Block";
  }

  // return a clone of this object

  Object* Block::clone (void) const {
    return new Block (*this);
  }
  
  // return the serial did

  t_word Block::getdid (void) const {
    return SRL_DEOD_BCE;
  }

  // return the serial sid
  
  t_word Block::getsid (void) const {
    return SRL_BLOK_SID;
  }

  // serialize this block

  void Block::wrstream (OutputStream& os) const {
    rdlock ();
    try {
      Serial::wrlong (d_stmp, os);
      d_hash.wrstream (os);
      if (p_tran == nullptr) {
	Serial::wrnilid (os);
      } else {
	p_tran->serialize(os);
      }
      if (p_sign == nullptr) {
	Serial::wrnilid (os);
      } else {
	p_sign->serialize(os);
      }
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // deserialize this block

  void Block::rdstream (InputStream& is) {
    wrlock ();
    try {
      d_stmp = Serial::rdlong (is);
      d_hash.rdstream (is);
      Object::iref (p_tran =
		    dynamic_cast<Transaction*>(Serial::deserialize(is)));
      Object::iref (p_sign =
		    dynamic_cast<Signature*>(Serial::deserialize(is)));
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // serialize this signable object

  void Block::wrsign (OutputStream& os) const {
    rdlock ();
    try {
      Serial::wrlong (d_stmp, os);
      d_hash.wrstream (os);
      if (p_tran == nullptr) {
	Serial::wrnilid (os);
      } else {
	p_tran->serialize(os);
      }
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // sign this block

  bool Block::sign (const Key& skey) {
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
  
  // verify this block

  bool Block::verify (void) const {
    wrlock ();
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

  // get the block information plist

  Plist Block::getplst (void) const {
    rdlock ();
    try {
      // create a result plist
      Plist result;
      // set the block uuid and timestamp
      result.add (PN_BLK_STMP, PI_BLK_STMP, Date(d_stmp).toiso(true));
      if (p_tran != nullptr) result += p_tran->getplst ();
      // here it is
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // get the block time stamp

  t_long Block::getstmp (void) const {
    rdlock ();
    try {
      t_long result = d_stmp;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the block transaction

  Transaction* Block::gettran (void) const {
    rdlock ();
    try {
      Transaction* result = p_tran;
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
  static const long QUARK_GETSTMP = zone.intern ("get-timestamp");
  static const long QUARK_GETTRAN = zone.intern ("get-transaction");

  // create a new object in a generic way

  Object* Block::mknew (Vector* argv) {
    long argc = (argv == nullptr) ? 0 : argv->length ();
    // create a default block
    if (argc == 0) return new Block;
    // check for 1 argument
    if (argc == 1) {
      Object* obj = argv->get (0);
      auto tran = dynamic_cast <Transaction*> (obj);
      if (tran == nullptr) {
        throw Exception ("type-error", "invalid object for block constructor",
                         Object::repr (obj));
      }
      return new Block (tran);
    }
    throw Exception ("argument-error",
                     "too many argument with block constructor");
  }

  // return true if the given quark is defined

  bool Block::isquark (const long quark, const bool hflg) const {
    rdlock ();
    try {
      if (zone.exists (quark) == true) {
	unlock ();
	return true;
      }
      bool result = hflg ? Signable::isquark (quark, hflg) : false;
      if (result == false) {
	result = hflg ? Serial::isquark (quark, hflg) : false;
      }
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // apply this object with a set of arguments and a quark
  
  Object* Block::apply (Evaluable* zobj, Nameset* nset, const long quark,
			Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();
    
    // check for 0 argument
    if (argc == 0) {
      if (quark == QUARK_GETSTMP) return new Integer (getstmp ());
      if (quark == QUARK_GETTRAN) {
	rdlock ();
	try {
	  Object* result = gettran ();
	  zobj->post (result);
	  unlock ();
	  return result;
	} catch (...) {
	  unlock ();
	  throw;
	}
      }
    }
    // check the signable method
    if (Signable::isquark (quark, true) == true) {
      return Signable::apply (zobj, nset, quark, argv);
    }
    // call the serial method
    return Serial::apply (zobj, nset, quark, argv);
  }
}
