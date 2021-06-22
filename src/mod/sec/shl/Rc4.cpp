// ---------------------------------------------------------------------------
// - Rc4.cpp                                                                 -
// - afnix:sec module - RC4 serial cipher class implementation               -
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

#include "Rc4.hpp"
#include "Vector.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - private section                                                       -
  // -------------------------------------------------------------------------

  // RC4 serial cipher constants
  static const char* RC4_ALGO_NAME = "RC4";

  // RC4 s-box constants
  static const long  RC4_SBOX_SIZE = 256;                          

  // this procedure expand the key into an effective key
  static void rc4_key_expand (t_byte* sbox, const Key& key) {
    // basic check as usual
    if (sbox == nullptr) return;
    // initialize the sbox
    for (long i = 0; i < RC4_SBOX_SIZE; i++) sbox[i] = (t_byte) i;
    // fill the skey
    t_byte skey[RC4_SBOX_SIZE];
    long klen = key.getsize ();
    for (long i = 0; i < RC4_SBOX_SIZE; i++) {
      skey[i] = key.getbyte (i % klen);
    }
    // update the sbox
    long j = 0;
    for (long i = 0; i < RC4_SBOX_SIZE; i++) {
      j = (j + sbox[i] + skey[i]) % 256;
      t_byte t = sbox[i];
      sbox[i]  = sbox[j];
      sbox[j]  = t;
    }
  }

  // this procedure compute the next byte from the s-box
  static t_byte rc4_get_k (t_byte* sbox, long sidx[2]) {
    // compute indexes
    sidx[0] = (sidx[0] + 1) % RC4_SBOX_SIZE;
    sidx[1] = (sidx[1] + sbox[sidx[0]]) % RC4_SBOX_SIZE;
    // store temporary
    t_byte t = sbox[sidx[0]];
    // update the sbox
    sbox[sidx[0]] = sbox[sidx[1]];
    sbox[sidx[1]] = t;
    // get result index
    long kidx = (sbox[sidx[0]] + sbox[sidx[1]]) % 256;
    return sbox[kidx];
  }

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a cipher by key

  Rc4::Rc4 (const Key& key) : SerialCipher (RC4_ALGO_NAME) {
    // initialize the s-box
    p_sbox = new t_byte[RC4_SBOX_SIZE];
    // set the key
    setkey (key);
  }

  // create a cipher by key and flag

  Rc4::Rc4 (const Key& key, const bool rflg) : SerialCipher (RC4_ALGO_NAME) {
    // initialize the s-box
    p_sbox = new t_byte[RC4_SBOX_SIZE];
    // set the key
    setkey (key);
    // set the reverse flag
    setrflg (rflg);
  }

  // destroy this cipher

  Rc4::~Rc4 (void) {
    delete [] p_sbox;
  }
  
  // return the class name

  String Rc4::repr (void) const {
    return "Rc4";
  }

  // reset this cipher
  
  void Rc4::reset (void) {
    wrlock ();
    try {
      // reset the serial cipher
      SerialCipher::reset ();
      // reset the sbox indexex
      d_sidx[0] = 0;
      d_sidx[1] = 0;
      // reset the sbox
      for (long i = 0; i < RC4_SBOX_SIZE; i++) p_sbox[i] = nilc;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // clear this cipher
  
  void Rc4::clear (void) {
    wrlock ();
    try {
      // clear the serial cipher
      SerialCipher::clear ();
      // reset the sbox indexex
      d_sidx[0] = 0;
      d_sidx[1] = 0;
      // expand the key
      rc4_key_expand (p_sbox, d_ckey);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set the cipher key

  void Rc4::setkey (const Key& key) {
    wrlock ();
    try {
      // set the base key
      SerialCipher::setkey (key);
      // reset the sbox indexex
      d_sidx[0] = 0;
      d_sidx[1] = 0;
      // expand the key
      rc4_key_expand (p_sbox, d_ckey);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    } 
  }
  
  // encode a block buffer into another one

  t_byte Rc4::encode (const t_byte bi) {
    wrlock ();
    try {
      // get the byte for the s-box
      t_byte k = rc4_get_k (p_sbox, d_sidx);
      // compute the result
      t_byte result = bi ^ k;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // decode a block buffer into another one

  t_byte Rc4::decode (const t_byte bi) {
    wrlock ();
    try {
      // get the byte for the s-box
      t_byte k = rc4_get_k (p_sbox, d_sidx);
      // compute the result
      t_byte result = bi ^ k;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // -------------------------------------------------------------------------
  // - object setion                                                         -
  // -------------------------------------------------------------------------

  // create a new object in a generic way
  
  Object* Rc4::mknew (Vector* argv) {
    long argc = (argv == nullptr) ? 0 : argv->length ();
    // check for 1 argument
    if (argc == 1) {
      // check for a key
      Object* obj = argv->get (0);
      Key*    key = dynamic_cast <Key*> (obj);
      if (key != nullptr) return new Rc4 (*key);
      throw Exception ("argument-error", "invalid arguments with RC4");
    }
    // check for 2 arguments
    if (argc == 2) {
      // check for a key
      Object* obj = argv->get (0);
      Key*    key  = dynamic_cast <Key*> (obj);
      if (key == nullptr) {
	throw Exception ("argument-error", "invalid arguments with RC4");
      }
      // get the reverse flag and object
      bool rflg = argv->getbool (1);
      return new Rc4 (*key, rflg);
    }
    throw Exception ("argument-error", "too many arguments with RC4");
  }
}
