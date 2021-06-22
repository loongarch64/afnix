// ---------------------------------------------------------------------------
// - Avi.cpp                                                                 -
// - afnix:mth module - abstract vector interface implementation             -
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

#include "Avi.hpp"
#include "Vector.hpp"
#include "Boolean.hpp"
#include "Integer.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"
 
namespace afnix {

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a default vector

  Avi::Avi (void) {
    d_size = 0LL;
  }

  // create a vector by size

  Avi::Avi (const t_long size) {
    // check the size
    if (size < 0LL) {
      throw Exception ("size-error", "invalid abstract vector size");
    }
    d_size = size;
  }
  
  // copy construct this vector

  Avi::Avi (const Avi& that) {
    that.rdlock ();
    try {
      d_size = that.d_size;
      that.unlock ();
    } catch (...) {
      that.unlock ();
      throw;
    }
  }
  
  // copy move this vector

  Avi::Avi (Avi&& that) noexcept : Serial (static_cast<Serial&&>(that)) {
    that.wrlock ();
    try {
      // copy move base serial
      Serial::operator = (static_cast<Serial&&>(that));
      // copy move locally
      d_size = that.d_size; that.d_size = 0LL;
      that.unlock ();
    } catch (...) {
      d_size = 0LL;
      that.unlock ();
    }
  }

  // assign a vector to this one

  Avi& Avi::operator = (const Avi& that) {
    // check for self-assignation
    if (this == &that) return *this;
    // lock and assign
    wrlock ();
    that.rdlock ();
    try {
      d_size = that.d_size;
      that.unlock ();
      unlock ();
      return *this;
    } catch (...) {
      that.unlock ();
      unlock ();
      throw;
    }
  }
  
  // move a vector into this one

  Avi& Avi::operator = (Avi&& that) noexcept {
    // check for self-move
    if (this == &that) return *this;
    // lock and move
    wrlock ();
    that.wrlock ();
    try {
      // move base serial
      Serial::operator = (static_cast<Serial&&>(that));
      // move locally
      d_size = that.d_size; that.d_size = 0LL;
      unlock ();
      that.unlock ();
    } catch (...) {
      d_size = 0LL;
      unlock ();
      that.unlock ();
    }
    return *this;
  }

  // return the vector size

  t_long Avi::getsize (void) const {
    rdlock ();
    try {
      t_long result = d_size;
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

  static const long QUARK_CLEAR   = zone.intern ("clear");
  static const long QUARK_RESET   = zone.intern ("reset");
  static const long QUARK_ISNILP  = zone.intern ("nil-p");
  static const long QUARK_GETSIZE = zone.intern ("get-size");

  // return true if the given quark is defined

  bool Avi::isquark (const long quark, const bool hflg) const {
    rdlock ();
    try {
      if (zone.exists (quark) == true){
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
  
  Object* Avi::apply (Evaluable* zobj, Nameset* nset, const long quark,
                      Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0L : argv->length ();

    // dispatch 0 argument
    if (argc == 0L) {
      if (quark == QUARK_ISNILP)  return new Boolean (isnil ());
      if (quark == QUARK_GETSIZE) return new Integer (getsize ());
      if (quark == QUARK_CLEAR) {
	clear ();
	return nullptr;
      }
      if (quark == QUARK_RESET) {
	reset ();
	return nullptr;
      }
    }
    // call the serial object
    return Serial::apply (zobj, nset, quark, argv);
  }
}

