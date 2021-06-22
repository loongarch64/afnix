// ---------------------------------------------------------------------------
// - Hmac.cpp                                                                -
// - afnix:sec module - hashed mac class implementation                      -
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

#include "Hmac.hpp"
#include "Sha1.hpp"
#include "Vector.hpp"
#include "Exception.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - private section                                                       -
  // -------------------------------------------------------------------------

  // HMAC algorithm constants
  static const String HMAC_ALGO_NAME = "HMAC";
  // HMAC inner pad byte
  static const t_byte HMAC_IPAD_XVAL = 0x36;
  // HMAC outer pad byte
  static const t_byte HMAC_OPAD_XVAL = 0x5C;

  // this procedure initialize the hmac key
  static t_byte* hmac_init_mkey (Hasher* hash, 
				 const Key& mkey, const t_byte bpad) {
    // do nothing without a hasher
    if (hash == nullptr) return nullptr;
    // reset the hasher
    hash->reset ();
    // get the key and hasher info
    long ksiz = mkey.getsize  ();
    long hsiz = hash->getsize ();
    // create a working buffer
    t_byte* xbuf = new t_byte[hsiz];
    for (long i = 0; i < hsiz; i++) xbuf[i] = bpad;
    // case 1: ksiz == hsiz
    if (ksiz == hsiz) {
      // update the working buffer
      for (long i = 0; i < hsiz; i++) xbuf[i] ^= mkey.getbyte (i);
    }
    // case 2: ksiz > hsiz
    if (ksiz > hsiz) {
      // create a key buffer
      t_byte* kbuf = new t_byte[ksiz];
      for (long i = 0; i < ksiz; i++) kbuf[i] = mkey.getbyte (i);
      // hash the key
      hash->process (kbuf, ksiz);
      hash->finish  ();
      // update the working buffer
      long hlen = hash->gethlen ();
      if (hlen < hsiz) {
	for (long i = 0;    i < hlen; i++) xbuf[i] ^= hash->getbyte (i);
	for (long i = hlen; i < hsiz; i++) xbuf[i] ^= 0x00;
      } else {
	for (long i = 0; i < hsiz; i++) xbuf[i] ^= hash->getbyte (i);
      }
      // clean the buffer and hasher
      delete [] kbuf;
      hash->reset ();
    }
    // case 3: ksiz < hsiz
    if (ksiz < hsiz) {
      for (long i = 0;    i < ksiz; i++) xbuf[i] ^= mkey.getbyte (i);
      for (long i = ksiz; i < hsiz; i++) xbuf[i] ^= 0x00;
    }
    return xbuf;
  }

  // this procedure initialize a hasher object from a key
  static void hmac_init (Hasher* hash, const Key& mkey) {
    // do nothing without a hasher
    if (hash == nullptr) return;
    // initialize the hasher key
    long    hsiz = hash->getsize ();
    t_byte* kbuf = hmac_init_mkey (hash, mkey, HMAC_IPAD_XVAL);
    // process the key buffer
    hash->process (kbuf, hsiz);
    // done
    delete [] kbuf;
  }

  // this procedure complete the hasher object from a key
  static void hmac_finish (Hasher* hash, const Key& mkey) {
    // do nothing without a hasher
    if (hash == nullptr) return;
    // get the hasher info
    long hsiz = hash->getsize ();
    long hlen = hash->gethlen ();
    long xlen = hsiz + hlen;
    // create a working buffer
    t_byte* xbuf = new t_byte[xlen];
    // update the working buffer
    hash->finish ();
    for (long i = hsiz, j = 0; i < xlen; i++, j++) {
      xbuf[i] = hash->getbyte (j);
    }
    // initialize the hasher key
    t_byte* kbuf = hmac_init_mkey (hash, mkey, HMAC_OPAD_XVAL);
    // update the working buffer
    for (long i = 0; i < hsiz; i++) xbuf[i] = kbuf[i];
    // process the buffer
    hash->process (xbuf, xlen);
    // finish processing
    hash->finish ();
    // done
    delete [] kbuf;
    delete [] xbuf;
  }

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a hmac by key

  Hmac::Hmac (const Key& mkey) : Mac (HMAC_ALGO_NAME, mkey) {
    Object::iref (p_hash = new Sha1);
  }

  // create a hmac by key and hasher

  Hmac::Hmac (const Key& mkey, Hasher* hash) : Mac (HMAC_ALGO_NAME, mkey) {
    p_hash = (hash == nullptr) ? new Sha1 : hash;
    Object::iref (p_hash);
  }

  // destroy this hmac

  Hmac::~Hmac (void) {
    Object::dref (p_hash);
  }

  // return the class name

  String Hmac::repr (void) const {
    return "Hmac";
  }

  // reset this hmac

  void Hmac::reset (void) {
    wrlock ();
    try {
      hmac_init (p_hash, d_mkey);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return the hmac length

  long Hmac::gethlen (void) const {
    rdlock ();
    try {
      long result = p_hash->gethlen ();
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return the hmac result length

  long Hmac::getrlen (void) const {
    rdlock ();
    try {
      long result = p_hash->getrlen ();
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return the hmac byte value by index

  t_byte Hmac::getbyte (const long index) const {
    rdlock ();
    try {
      t_byte result = p_hash->getbyte (index);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // return the hmac value as a relatif

  Relatif Hmac::gethval (void) const {
    rdlock ();
    try {
      Relatif result = p_hash->gethval ();
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // return the hmac result value as a relatif

  Relatif Hmac::getrval (void) const {
    rdlock ();
    try {
      Relatif result = p_hash->getrval ();
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
    
  // return the formatted hmac code

  String Hmac::format (void) const {
    rdlock ();
    try {
      String result = p_hash->format ();
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // process a message by data
  
  void Hmac::process (const t_byte* data, const long size) {
    wrlock ();
    try {
      p_hash->process (data, size);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }
 
  // process a message with a buffer
  
  void Hmac::process (Buffer& buf) {
    wrlock ();
    try {
      p_hash->process (buf);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // process a message with an input stream

  void Hmac::process (InputStream& is) {
    wrlock ();
    try {
      p_hash->process (is);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // finish processing the hmac

  void Hmac::finish (void) {
    wrlock ();
    try {
      hmac_finish (p_hash, d_mkey);
      unlock ();
    } catch (...) {
      reset ();
      unlock ();
      throw;
    }
  }

  // -------------------------------------------------------------------------
  // - object section                                                        -
  // -------------------------------------------------------------------------

  // create a new object in a generic way
  
  Object* Hmac::mknew (Vector* argv) {
    long argc = (argv == nullptr) ? 0 : argv->length ();
    // check for 1 argument
    if (argc == 1) {
      Object* obj = argv->get (0);
      // check for a key
      Key* kobj = dynamic_cast <Key*> (obj);
      if (kobj != nullptr) return new Hmac (*kobj);
      throw Exception ("type-error", "invalid object with hmac constructor",
                       Object::repr (obj));
    }
    // check for 2 arguments
    if (argc == 2) {
      // load first object
      Object* obj = argv->get (0);
      // check for a key
      Key* kobj = dynamic_cast <Key*> (obj);
      if (kobj == nullptr) {
	throw Exception ("type-error", "invalid object with hmac constructor",
			 Object::repr (obj));
      }
      // load second object
      obj = argv->get (1);
      // check fo a hasher
      Hasher* hobj = dynamic_cast <Hasher*> (obj);
      if (hobj == nullptr) {
	throw Exception ("type-error", "invalid object with hmac constructor",
			 Object::repr (obj));
      }
      // create the hmac
      return new Hmac (*kobj, hobj);
    }
    // invalid arguments
    throw Exception ("argument-error", "too many arguments with key");
  }
}
