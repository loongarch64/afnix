// ---------------------------------------------------------------------------
// - Transaction.cpp                                                         -
// - afnix:bce service - block chain transaction class implementation        -
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

#include "Vector.hpp"
#include "Boolean.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"
#include "Transaction.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - private section                                                       -
  // -------------------------------------------------------------------------

  // the transaction information plist
  static const String PN_TRN_NAME = "PN-TRN-NAME";
  static const String PI_TRN_NAME = "TRANSACTION NAME";
  static const String PN_TRN_INFO = "PN-TRN-INFO";
  static const String PI_TRN_INFO = "TRANSACTION INFO";
  static const String PN_TRN_UUID = "PN-TRN-UUID";
  static const String PI_TRN_UUID = "TRANSACTION UUID";

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a default transaction

  Transaction::Transaction (void) {
    d_uuid.random ();
  }
  
  // create an transaction by name

  Transaction::Transaction (const String& name) : Taggable (name) {
    d_uuid.random ();
  }

  // create an transaction by name and info

  Transaction::Transaction (const String& name,
			    const String& info) : Taggable (name, info) {
    d_uuid.random ();
  }

  // copy construct this transaction

  Transaction::Transaction (const Transaction& that) {
    that.rdlock ();
    try {
      // copy base taggable
      Taggable::operator = (that);
      // copy locally
      d_uuid = that.d_uuid;
      that.unlock ();
    } catch (...) {
      that.unlock ();
      throw;
    }
  }
  
  // copy move this transaction

  Transaction::Transaction (Transaction&& that) noexcept :
    Taggable (static_cast<Taggable&&>(that)) {
    that.wrlock ();
    try {
      d_uuid = that.d_uuid; that.d_uuid.reset ();
    } catch (...) {
      d_uuid.reset ();
    }
    that.unlock ();
  }
  
  // assign a transaction to this one

  Transaction& Transaction::operator = (const Transaction& that) {
    // check for self assignation
    if (this == &that) return *this;
    // lock and assign
    wrlock ();
    that.rdlock ();
    try {
      // copy base taggable
      Taggable::operator = (that);
      // copy locally
      d_uuid = that.d_uuid;
      unlock ();
      that.unlock ();
      return *this;
    } catch (...) {
      unlock ();
      that.unlock ();
      throw;
    }
  }
  
  // move a transaction to this one

  Transaction& Transaction::operator = (Transaction&& that) noexcept {
    // check for self assignation
    if (this == &that) return *this;
    // lock and assign
    wrlock ();
    that.wrlock ();
    try {
      // assign base object
      Taggable::operator = (static_cast<Taggable&&>(that));
      // move locally
      d_uuid = that.d_uuid; that.d_uuid.reset ();
    } catch (...) {
      d_uuid.reset ();
    }
    unlock ();
    that.unlock ();
    return *this;
  }
  
  // serialize this transaction

  void Transaction::wrstream (OutputStream& os) const {
    rdlock ();
    try {
      // serialize the taggable
      Taggable::wrstream (os);
      // serialize locally
      d_uuid.wrstream (os);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // deserialize this transaction

  void Transaction::rdstream (InputStream& is) {
    wrlock ();
    try {
      // deserialize the taggable
      Taggable::rdstream (is);
      // deserialize locally
      d_uuid.rdstream (is);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the transaction plist

  Plist Transaction::getplst (void) const {
    rdlock ();
    try {
      Plist result (d_name, d_info);
      result.add (PN_TRN_NAME, PI_TRN_NAME, d_name);
      result.add (PN_TRN_INFO, PI_TRN_INFO, d_info);
      result.add (PN_TRN_UUID, PI_TRN_UUID, d_uuid.tostring ());
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
  static const long QUARK_ZONE_LENGTH = 1;
  static QuarkZone  zone (QUARK_ZONE_LENGTH);

  // the object supported quarks
  static const long QUARK_GETPLST = zone.intern ("get-plist");

  // return true if the given quark is defined

  bool Transaction::isquark (const long quark, const bool hflg) const {
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
  
  Object* Transaction::apply (Evaluable* zobj, Nameset* nset, const long quark,
			      Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();
    
    // check for 0 argument
    if (argc == 0) {
      if (quark == QUARK_GETPLST) return new Plist  (getplst ());
    }
    // call the taggable method
    return Taggable::apply (zobj, nset, quark, argv);
  }
}
