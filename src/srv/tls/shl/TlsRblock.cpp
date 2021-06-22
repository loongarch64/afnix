// ---------------------------------------------------------------------------
// - TlsRblock.cpp                                                           -
// - afnix:tls service - tls record block class implementation               -
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
#include "Integer.hpp"
#include "TlsRblock.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create an empty block

  TlsRblock::TlsRblock (void) {
    p_brcd = nullptr;
    d_bpos = 0L;
    d_blen = 0L;
  }

  // create a record block by record, position and length
  
  TlsRblock::TlsRblock (TlsRecord* brcd, const long bpos, const long blen) {
    Object::iref (p_brcd = brcd);
    d_bpos = bpos;
    d_blen = blen;
    // check position/length consistency
    long rlen = (p_brcd == nullptr) ? 0L : p_brcd->length ();
    if ((bpos < 0L) || (bpos > rlen) || (blen < 0L) || ((bpos + blen) > rlen)) {
      throw Exception ("tls-error", 
		       "invalid tls record block position-length");
    }
  }

  // copy construct this record block

  TlsRblock::TlsRblock (const TlsRblock& that) {
    that.rdlock ();
    try {
      Object::iref (p_brcd = that.p_brcd);
      d_bpos = that.d_bpos;
      d_blen = that.d_blen;
      that.unlock ();
    } catch (...) {
      that.unlock ();
      throw;
    }
  }

  // destroy this record block
  
  TlsRblock::~TlsRblock (void) {
    Object::dref(p_brcd);
  }

  // assign a record block to this one

  TlsRblock& TlsRblock::operator = (const TlsRblock& that) {
    // check for self-assignation
    if (this == &that) return *this;
    // lock and assign
    wrlock ();
    that.rdlock ();
    try {
      Object::iref (that.p_brcd);
      Object::dref (p_brcd);
      p_brcd = that.p_brcd;
      d_bpos = that.d_bpos;
      d_blen = that.d_blen;
      unlock ();
      that.unlock ();
      return *this;
    } catch (...) {
      unlock ();
      that.unlock ();
      throw;
    }
  }

  // return the class name
  
  String TlsRblock::repr (void) const {
    return "TlsRblock";
  }

  // clone this object

  Object* TlsRblock::clone (void) const {
    return new TlsRblock (*this);
  }

  // reset the block

  void TlsRblock::reset (void) {
    wrlock ();
    try {
      if (p_brcd != nullptr) p_brcd->reset ();
      d_bpos = 0L;
      d_blen = 0L;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the block type

  t_byte TlsRblock::gettype (void) const {
    rdlock ();
    try {
      t_byte result = (p_brcd == nullptr) ? nilc : p_brcd->gettype ();
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the block length

  long TlsRblock::length (void) const {
    rdlock ();
    try {
      long result = d_blen;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get a block byte by position

  t_byte TlsRblock::getbyte (const long pos) const {
    rdlock ();
    try {
      long     bidx = d_bpos + pos;
      t_byte result = (p_brcd == nullptr) ? nilc : p_brcd->getbyte (bidx);
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

  // create a new object in a generic way

  Object* TlsRblock::mknew (Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();

    // check for 0 argument
    if (argc == 0) return new TlsRblock;
    // check for 3 arguments
    if (argc == 3) {
      Object*    obj = argv->get (0);
      TlsRecord* rcd = dynamic_cast<TlsRecord*> (obj);
      if ((rcd == nullptr) && (obj != nullptr)) {
	throw Exception ("type-error", "invalid object as tls record",
			 Object::repr (obj));
      }
      long   bpos = argv->getlong (1);
      long   blen = argv->getlong (2);
      return new TlsRblock (rcd, bpos, blen);
    }
    // too many arguments
    throw Exception ("argument-error", 
                     "too many argument with tls record block");
  }
}
