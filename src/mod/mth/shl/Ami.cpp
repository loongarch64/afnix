// ---------------------------------------------------------------------------
// - Ami.cpp                                                                 -
// - afnix:mth module - abstract matrix interface implementation             -
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

#include "Ami.hpp"
#include "Vector.hpp"
#include "Boolean.hpp"
#include "Integer.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a default matrix

  Ami::Ami (void) {
    d_rsiz = 0LL;
    d_csiz = 0LL;
  }

  // create a square matrix by size

  Ami::Ami (const t_long size) {
    // check the size
    if (size < 0LL) {
      throw Exception ("size-error", "invalid abstract matrix size");
    }
    d_rsiz = size;
    d_csiz = size;
  }

  // create a matrix by size

  Ami::Ami (const t_long rsiz, const t_long csiz) {
    // check the size
    if ((rsiz < 0LL) || (csiz < 0LL)) {
      throw Exception ("size-error", "invalid abstract matrix size");
    }
    d_rsiz = rsiz;
    d_csiz = csiz;
  }

  // copy construct this matrix

  Ami::Ami (const Ami& that) {
    that.rdlock ();
    try {
      d_rsiz = that.d_rsiz;
      d_csiz = that.d_csiz;
      that.unlock ();
    } catch (...) {
      that.unlock ();
      throw;
    }
  }
  
  // copy move this matrix

  Ami::Ami (Ami&& that) noexcept {
    that.wrlock ();
    try {
      // move base serial
      Serial::operator = (static_cast<Serial&&>(that));
      // move locally
      d_rsiz = that.d_rsiz; that.d_rsiz = 0LL;
      d_csiz = that.d_csiz; that.d_csiz = 0LL;
      that.unlock ();
    } catch (...) {
      d_rsiz = 0LL;
      d_csiz = 0LL;
      that.unlock ();
    } 
  }

  // assign a matrix to this one

  Ami& Ami::operator = (const Ami& that) {
    // check for self-assignation
    if (this == &that) return *this;
    // lock and assign
    wrlock ();
    that.rdlock ();
    try {
      d_rsiz = that.d_rsiz;
      d_csiz = that.d_csiz;
      that.unlock ();
      unlock ();
      return *this;
    } catch (...) {
      that.unlock ();
      unlock ();
      throw;
    }
  }
  
  // move a matrix into this one

  Ami& Ami::operator = (Ami&& that) noexcept {
    // check for self-move
    if (this == &that) return *this;
    // lock and move
    wrlock ();
    that.wrlock ();
    try {
      // assign base serial
      Serial::operator = (static_cast<Serial&&>(that));
      // move locally
      d_rsiz = that.d_rsiz; that.d_rsiz = 0LL;
      d_csiz = that.d_csiz; that.d_csiz = 0LL;
      unlock ();
      that.unlock ();
    } catch (...) {
      d_rsiz = 0LL;
      d_csiz = 0LL;
      unlock ();
      that.unlock ();
    }
    return *this;
  }
  
  // get the matrix row size

  t_long Ami::getrsiz (void) const {
    rdlock ();
    try {
      t_long result = d_rsiz;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the matrix column size

  t_long Ami::getcsiz (void) const {
    rdlock ();
    try {
      t_long result = d_csiz;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return true if the matrix is square

  bool Ami::issquare (void) const {
    rdlock ();
    try {
      bool result = (d_rsiz == d_csiz);
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
  static const long QUARK_ZONE_LENGTH = 5;
  static QuarkZone  zone (QUARK_ZONE_LENGTH);

  // the object supported quarks
  static const long QUARK_RESET     = zone.intern ("reset");
  static const long QUARK_CLEAR     = zone.intern ("clear");
  static const long QUARK_GETRSIZ   = zone.intern ("get-row-size");
  static const long QUARK_GETCSIZ   = zone.intern ("get-col-size");
  static const long QUARK_SQUAREP   = zone.intern ("square-p");


  // return true if the given quark is defined

  bool Ami::isquark (const long quark, const bool hflg) const {
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
  
  Object* Ami::apply (Evaluable* zobj, Nameset* nset, const long quark,
		      Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();

    // dispatch 0 argument
    if (argc == 0) {
      if (quark == QUARK_GETRSIZ)  return new Integer (getrsiz ());
      if (quark == QUARK_GETCSIZ)  return new Integer (getcsiz ());
      if (quark == QUARK_SQUAREP)  return new Boolean (issquare ());
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
