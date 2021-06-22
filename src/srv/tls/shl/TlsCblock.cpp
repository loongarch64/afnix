// ---------------------------------------------------------------------------
// - TlsCblock.cpp                                                           -
// - afnix:tls service - tls cipher block class implementation               -
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
#include "TlsCblock.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create an empty block

  TlsCblock::TlsCblock (void) {
    p_data = nullptr;
    p_bmac = nullptr;
    p_hmac = nullptr;
  }

  // create a cipher block by record and mac 
  
  TlsCblock::TlsCblock (TlsRecord* brcd, Hmac* hmac) {
    Object::iref (p_hmac = hmac);
    long hlen = (hmac == nullptr) ? 0L : hmac->getrlen ();
    long dlen = (brcd == nullptr) ? 0L : brcd->length () - hlen;
    Object::iref (p_data = new TlsRblock (brcd, 0L, dlen));
  }

  // create a cipher block by record blocks

  TlsCblock::TlsCblock (TlsBlock* data, TlsBlock* bmac) {
    Object::iref (p_data = data);
    Object::iref (p_bmac = bmac);
    p_hmac = nullptr;
  }
  
  // create a cipher block by record blocks and mac

  TlsCblock::TlsCblock (TlsBlock* data, TlsBlock* bmac, Hmac* hmac) {
    Object::iref (p_data = data);
    Object::iref (p_bmac = bmac);
    Object::iref (p_hmac = hmac);
  }
  
  // copy construct this cipher block

  TlsCblock::TlsCblock (const TlsCblock& that) {
    that.rdlock ();
    try {
      Object::iref (p_data = that.p_data);
      Object::iref (p_bmac = that.p_bmac);
      Object::iref (p_hmac = that.p_hmac);
      that.unlock ();
    } catch (...) {
      that.unlock ();
      throw;
    }
  }

  // destroy this cipher block
  
  TlsCblock::~TlsCblock (void) {
    reset ();
  }

  // assign a cipher block to this one

  TlsCblock& TlsCblock::operator = (const TlsCblock& that) {
    // check for self-assignation
    if (this == &that) return *this;
    // lock and assign
    wrlock ();
    that.rdlock ();
    try {
      Object::iref (that.p_data);
      Object::iref (that.p_bmac);
      Object::iref (that.p_hmac);
      Object::dref (p_data);
      Object::dref (p_bmac);
      Object::dref (p_hmac);
      p_data = that.p_data;
      p_bmac = that.p_bmac;
      p_hmac = that.p_hmac;
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
  
  String TlsCblock::repr (void) const {
    return "TlsCblock";
  }

  // clone this object

  Object* TlsCblock::clone (void) const {
    return new TlsCblock (*this);
  }

  // reset the block

  void TlsCblock::reset (void) {
    wrlock ();
    try {
      Object::dref (p_data); p_data = nullptr;
      Object::dref (p_bmac); p_bmac = nullptr;
      Object::dref (p_hmac); p_hmac = nullptr;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the block type

  t_byte TlsCblock::gettype (void) const {
    rdlock ();
    try {
      t_byte result = (p_data == nullptr) ? nilc : p_data->gettype ();
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the block length

  long TlsCblock::length (void) const {
    rdlock ();
    try {
      long result = (p_data == nullptr) ? 0L : p_data->length ();
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get a block byte by position

  t_byte TlsCblock::getbyte (const long pos) const {
    rdlock ();
    try {
      t_byte result = (p_data == nullptr) ? nilc : p_data->getbyte (pos);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get a clone of the cipher data block

  TlsBlock* TlsCblock::getdata (void) const {
    rdlock();
    try {
      TlsBlock* result = (p_data == nullptr) ? nullptr :
	dynamic_cast<TlsBlock*> (p_data->clone());
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

  Object* TlsCblock::mknew (Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();

    // check for 0 argument
    if (argc == 0) return new TlsCblock;
    // check for 2 arguments
    if (argc == 2) {
      // get the record
      Object*     obj = argv->get (0);
      TlsRecord* brcd = dynamic_cast<TlsRecord*> (obj);
      if ((brcd == nullptr) && (obj != nullptr)) {
	throw Exception ("type-error", "invalid object as tls record",
			 Object::repr (obj));
      }
      // get the hmac
      obj = argv->get (1);
      Hmac* hmac = dynamic_cast<Hmac*> (obj);
      if ((hmac == nullptr) && (obj != nullptr)) {
	throw Exception ("type-error", "invalid object as tls hmac",
			 Object::repr (obj));
      }
      return new TlsCblock (brcd, hmac);
    }
    // check for 3 arguments
    if (argc == 3) {
      // get the block data
      Object*     obj = argv->get (0);
      TlsBlock* data = dynamic_cast<TlsBlock*> (obj);
      if ((data == nullptr) && (obj != nullptr)) {
	throw Exception ("type-error", "invalid object as tls record",
			 Object::repr (obj));
      }
      // get the block mac
      obj = argv->get (1);
      TlsBlock* bmac = dynamic_cast<TlsBlock*> (obj);
      if ((bmac == nullptr) && (obj != nullptr)) {
	throw Exception ("type-error", "invalid object as tls record",
			 Object::repr (obj));
      }
      // get the hmac
      obj = argv->get (2);
      Hmac* hmac = dynamic_cast<Hmac*> (obj);
      if ((hmac == nullptr) && (obj != nullptr)) {
	throw Exception ("type-error", "invalid object as tls hmac",
			 Object::repr (obj));
      }
      return new TlsCblock (data, bmac, hmac);
    }
    // too many arguments
    throw Exception ("argument-error", 
                     "too many argument with tls cipher block");
  }
}
