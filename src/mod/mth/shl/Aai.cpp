// ---------------------------------------------------------------------------
// - Aai.cpp                                                                 -
// - afnix:mth module - abstract array interface implementation              -
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

#include "Aai.hpp"
#include "Vector.hpp"
#include "Integer.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a default array

  Aai::Aai (void) {
    d_size = 0LL;
    d_strd = 1L;
  }

  // create an array by size

  Aai::Aai (const t_long size) {
    // check the size
    if (size < 0LL) {
      throw Exception ("size-error", "invalid abstract array size");
    }
    d_size = size;
    d_strd = 1L;
  }

  // create a array by size and stride

  Aai::Aai (const t_long size, const long strd) {
    // check the size
    if ((size < 0LL) || (strd < 0L)) {
      throw Exception ("size-error", "invalid abstract array size");
    }
    d_size = size;
    d_strd = strd;
  }

  // copy construct this array

  Aai::Aai (const Aai& that) {
    that.rdlock ();
    try {
      d_size = that.d_size;
      d_strd = that.d_strd;
      that.unlock ();
    } catch (...) {
      that.unlock ();
      throw;
    }
  }
  
  // copy move this array

  Aai::Aai (Aai&& that) noexcept {
    that.wrlock ();
    try {
      // move base serial
      Serial::operator = (static_cast<Serial&&>(that));
      // move locally
      d_size = that.d_size; that.d_size = 0LL;
      d_strd = that.d_strd; that.d_strd = 1L;
      that.unlock ();
    } catch (...) {
      d_size = 0LL;
      d_strd = 0LL;
      that.unlock ();
    } 
  }

  // assign a array to this one

  Aai& Aai::operator = (const Aai& that) {
    // check for self-assignation
    if (this == &that) return *this;
    // lock and assign
    wrlock ();
    that.rdlock ();
    try {
      d_size = that.d_size;
      d_strd = that.d_strd;
      that.unlock ();
      unlock ();
      return *this;
    } catch (...) {
      that.unlock ();
      unlock ();
      throw;
    }
  }
  
  // move a array into this one

  Aai& Aai::operator = (Aai&& that) noexcept {
    // check for self-move
    if (this == &that) return *this;
    // lock and move
    wrlock ();
    that.wrlock ();
    try {
      // assign base serial
      Serial::operator = (static_cast<Serial&&>(that));
      // move locally
      d_size = that.d_size; that.d_size = 0LL;
      d_strd = that.d_strd; that.d_strd = 1L;
      unlock ();
      that.unlock ();
    } catch (...) {
      d_size = 0LL;
      d_strd = 0LL;
      unlock ();
      that.unlock ();
    }
    return *this;
  }
  
  // get the array size

  t_long Aai::getsize (void) const {
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

  // get the array stride

  long Aai::getstrd (void) const {
    rdlock ();
    try {
      long result = d_strd;
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
  static const long QUARK_RESET     = zone.intern ("reset");
  static const long QUARK_CLEAR     = zone.intern ("clear");
  static const long QUARK_GETSIZE   = zone.intern ("get-size");
  static const long QUARK_GETSTRD   = zone.intern ("get-stride");


  // return true if the given quark is defined

  bool Aai::isquark (const long quark, const bool hflg) const {
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
  
  Object* Aai::apply (Evaluable* zobj, Nameset* nset, const long quark,
		      Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();

    // dispatch 0 argument
    if (argc == 0) {
      if (quark == QUARK_GETSIZE)  return new Integer (getsize ());
      if (quark == QUARK_GETSTRD)  return new Integer (getstrd ());
      if (quark == QUARK_RESET) {
	reset ();
	return nullptr;
      }
      if (quark == QUARK_CLEAR) {
	clear ();
	return nullptr;
      }
    }
    // call the serial object
    return Serial::apply (zobj, nset, quark, argv);
  }
}
