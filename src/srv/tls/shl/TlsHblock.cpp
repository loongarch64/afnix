// ---------------------------------------------------------------------------
// - TlsHblock.cpp                                                           -
// - afnix:tls service - tls block handshake class implementation            -
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
#include "TlsTypes.hxx"
#include "TlsHblock.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create an empty block
  
  TlsHblock::TlsHblock (void) {
    p_brcd = nullptr;
    d_type = nilc;
    d_bpos = 0L;
    d_blen = 0L;
  }

  // create a block block by record, position, type and length
  
  TlsHblock::TlsHblock (TlsRecord* brcd, const t_byte type, const long bpos,
			const long blen) {
    Object::iref (p_brcd = brcd);
    d_type = type;
    d_bpos = bpos;
    d_blen = blen;
    // check record consistency
    if ((brcd != nullptr) && (brcd->gettype () != TLS_TYPE_HSK)) {
      throw Exception ("tls-error", 
		       "invalid record type as tls handshake block");
    }
    // check position/length consistency
    long rlen = (p_brcd == nullptr) ? 0L : p_brcd->length ();
    if ((bpos < 0L) || (bpos > rlen) || (blen < 0L) || ((bpos + blen) > rlen)) {
      throw Exception ("tls-error", 
		       "invalid tls handshake block position-length");
    }
  }

  // copy construct this block

  TlsHblock::TlsHblock (const TlsHblock& that) {
    that.rdlock ();
    try {
      Object::iref (p_brcd = that.p_brcd);
      d_type = that.d_type;
      d_bpos = that.d_bpos;
      d_blen = that.d_blen;
      that.unlock ();
    } catch (...) {
      that.unlock ();
      throw;
    }
  }

  // destroy this handshake block
  
  TlsHblock::~TlsHblock (void) {
    Object::dref(p_brcd);
  }

  // assign a block to this one

  TlsHblock& TlsHblock::operator = (const TlsHblock& that) {
    // check for self-assignation
    if (this == &that) return *this;
    // lock and assign
    wrlock ();
    that.rdlock ();
    try {
      Object::iref (that.p_brcd);
      Object::dref (p_brcd);
      p_brcd = that.p_brcd;
      d_type = that.d_type;
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
  
  String TlsHblock::repr (void) const {
    return "TlsHblock";
  }

  // clone this object

  Object* TlsHblock::clone (void) const {
    return new TlsHblock (*this);
  }

  // reset the block

  void TlsHblock::reset (void) {
    wrlock ();
    try {
      if (p_brcd != nullptr) p_brcd->reset ();
      d_type = nilc;
      d_bpos = 0L;
      d_blen = 0L;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the block handshake type

  t_byte TlsHblock::gettype (void) const {
    rdlock ();
    try {
      t_byte result = d_type;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the block length

  long TlsHblock::length (void) const {
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

  t_byte TlsHblock::getbyte (const long pos) const {
    rdlock ();
    try {
      long     bidx = d_bpos + pos + 4L;
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

  Object* TlsHblock::mknew (Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();

    // check for 0 argument
    if (argc == 0) return new TlsHblock;
    // check for 4 argument
    if (argc == 4) {
      Object*    obj = argv->get (0);
      TlsRecord* rcd = dynamic_cast<TlsRecord*> (obj);
      if ((rcd == nullptr) && (obj != nullptr)) {
	throw Exception ("type-error", "invalid object as tls record",
			 Object::repr (obj));
      }
      t_byte type = argv->getbyte (1);
      long   bpos = argv->getlong (2);
      long   blen = argv->getlong (3);
      return new TlsHblock (rcd, type, bpos, blen);
    }
    // too many arguments
    throw Exception ("argument-error", 
                     "too many argument with tls block handshake");
  }
}
