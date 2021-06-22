// ---------------------------------------------------------------------------
// - Rc2.cpp                                                                 -
// - afnix:sec module - RC2 block cipher class implementation                -
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

#include "Rc2.hpp"
#include "Vector.hpp"
#include "Integer.hpp"
#include "Cryptics.hxx"
#include "QuarkZone.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - private section                                                       -
  // -------------------------------------------------------------------------

  // RC2 block cipher constants
  static const char* RC2_ALGO_NAME = "RC2";
  static const long  RC2_BLOK_SIZE = 8;
  static const long  RC2_WORD_SIZE = RC2_BLOK_SIZE / 2;

  // RC2 effective key constants
  static const long  RC2_EKEY_BITS = 1024;
  static const long  RC2_EKEY_SIZE = RC2_EKEY_BITS / 8;
  static const long  RC2_EKEY_WLEN = RC2_EKEY_SIZE / 2;

  // RC2 PI constant
  static t_byte RC2_PI[256]={
			     0xd9, 0x78, 0xf9, 0xc4, 0x19, 0xdd, 0xb5, 0xed,
			     0x28, 0xe9, 0xfd, 0x79, 0x4a, 0xa0, 0xd8, 0x9d,
			     0xc6, 0x7e, 0x37, 0x83, 0x2b, 0x76, 0x53, 0x8e,
			     0x62, 0x4c, 0x64, 0x88, 0x44, 0x8b, 0xfb, 0xa2,
			     0x17, 0x9a, 0x59, 0xf5, 0x87, 0xb3, 0x4f, 0x13,
			     0x61, 0x45, 0x6d, 0x8d, 0x09, 0x81, 0x7d, 0x32,
			     0xbd, 0x8f, 0x40, 0xeb, 0x86, 0xb7, 0x7b, 0x0b,
			     0xf0, 0x95, 0x21, 0x22, 0x5c, 0x6b, 0x4e, 0x82,
			     0x54, 0xd6, 0x65, 0x93, 0xce, 0x60, 0xb2, 0x1c,
			     0x73, 0x56, 0xc0, 0x14, 0xa7, 0x8c, 0xf1, 0xdc,
			     0x12, 0x75, 0xca, 0x1f, 0x3b, 0xbe, 0xe4, 0xd1,
			     0x42, 0x3d, 0xd4, 0x30, 0xa3, 0x3c, 0xb6, 0x26,
			     0x6f, 0xbf, 0x0e, 0xda, 0x46, 0x69, 0x07, 0x57,
			     0x27, 0xf2, 0x1d, 0x9b, 0xbc, 0x94, 0x43, 0x03,
			     0xf8, 0x11, 0xc7, 0xf6, 0x90, 0xef, 0x3e, 0xe7,
			     0x06, 0xc3, 0xd5, 0x2f, 0xc8, 0x66, 0x1e, 0xd7,
			     0x08, 0xe8, 0xea, 0xde, 0x80, 0x52, 0xee, 0xf7,
			     0x84, 0xaa, 0x72, 0xac, 0x35, 0x4d, 0x6a, 0x2a,
			     0x96, 0x1a, 0xd2, 0x71, 0x5a, 0x15, 0x49, 0x74,
			     0x4b, 0x9f, 0xd0, 0x5e, 0x04, 0x18, 0xa4, 0xec,
			     0xc2, 0xe0, 0x41, 0x6e, 0x0f, 0x51, 0xcb, 0xcc,
			     0x24, 0x91, 0xaf, 0x50, 0xa1, 0xf4, 0x70, 0x39,
			     0x99, 0x7c, 0x3a, 0x85, 0x23, 0xb8, 0xb4, 0x7a,
			     0xfc, 0x02, 0x36, 0x5b, 0x25, 0x55, 0x97, 0x31,
			     0x2d, 0x5d, 0xfa, 0x98, 0xe3, 0x8a, 0x92, 0xae,
			     0x05, 0xdf, 0x29, 0x10, 0x67, 0x6c, 0xba, 0xc9,
			     0xd3, 0x00, 0xe6, 0xcf, 0xe1, 0x9e, 0xa8, 0x2c,
			     0x63, 0x16, 0x01, 0x3f, 0x58, 0xe2, 0x89, 0xa9,
			     0x0d, 0x38, 0x34, 0x1b, 0xab, 0x33, 0xff, 0xb0,
			     0xbb, 0x48, 0x0c, 0x5f, 0xb9, 0xb1, 0xcd, 0x2e,
			     0xc5, 0xf3, 0xdb, 0x47, 0xe5, 0xa5, 0x9c, 0x77,
			     0x0a, 0xa6, 0x20, 0x68, 0xfe, 0x7f, 0xc1, 0xad
  };

  // this procedure expand the key into an effective key
  static void rc2_key_expand (t_word* rkey, const Key& key, const long klen) {
    // basic check as usual
    if (rkey == nullptr) return;
    // reset the key
    for (long k = 0L; k < RC2_EKEY_WLEN; k++) rkey[k] = nilw;  
    // check the supplied key size
    long blen = key.getsize ();
    if (blen >= RC2_EKEY_SIZE) {
      throw Exception ("key-error", "invalid key size");
    }
    // load the byte key
    t_byte bkey[RC2_EKEY_SIZE];
    for (long i = 0; i < blen; i++) bkey[i] = key.getbyte (i);
    // compute effective key length
    long t8 = (klen + 7) / 8;
    long te = 1 << (8 + klen - 8 * t8);
    t_byte tm = 0xFF % te;
    // key expansion first loop
    for (long i = blen; i < RC2_EKEY_SIZE; i++) {
      t_byte k = bkey[i-1] + bkey[i-blen];
      bkey[i] = RC2_PI[k];
    }
    bkey[RC2_EKEY_SIZE - t8] = RC2_PI[bkey[RC2_EKEY_SIZE - t8] & tm];
    // key expansion second loop
    for (long i = RC2_EKEY_SIZE - t8 - 1; i >= 0; i--) {
      bkey[i] = RC2_PI[bkey[i+1] ^ bkey[i+t8]];
    }
    // map the word key
    for (long i = 0, j = 0; i < RC2_EKEY_SIZE-1; i += 2, j++) {
      rkey[j] = bkey[i+1];
      rkey[j] <<= 8;
      rkey[j] |= bkey[i];
    }
  }

  // this procedure bind an input buffer to the state word
  static inline void rc2_bi_to_r (t_word* r, const t_byte* bi) {
    // check result and input buffer
    if ((r == nullptr) || (bi == nullptr)) return;
    // bind the word from the input bytes
    for (long i = 0, j = 0; i < RC2_BLOK_SIZE; i += 2, j++) {
      r[j] = bi[i+1];
      r[j] <<= 8;
      r[j] |= bi[i];
    }
  }

  // this procedure bind a state word to an output buffer
  static inline void rc2_r_to_bo (t_byte* bo, const t_word* r) {
    // check result and buffer
    if ((bo == nullptr) || (r == nullptr)) return;
    // bind the byte from the words
    for (long i = 0, j = 0; i < RC2_BLOK_SIZE; i += 2, j++) {
      bo[i]   = (t_byte) r[j];
      bo[i+1] = (t_byte) (r[j] >> 8);
    }
  }

  // this procedure do an encryption mix up
  static long rc2_enc_mix_up (t_word* r, const t_word* ekey, 
			      const long j, const long i) {
    // compute index
    long i1 = i - 1; long i2 = i - 2; long i3 = i - 3;
    if (i1 < 0) i1 += 4;
    if (i2 < 0) i2 += 4;
    if (i3 < 0) i3 += 4;
    r[i] = r[i] + ekey[j] + (r[i1] & r[i2]) + ((~r[i1]) & r[i3]);
    // compute rotation index
    long k =  (i == 0) ? 1 : (i == 1) ? 2 : (i == 2) ? 3 : 5;
    r[i] = wrotl (r[i], k);
    // update index result
    return j + 1;
  }

  // this procedure performs an encryption mixing round
  static long rc2_enc_mix (t_word* r, const t_word* ekey, const long j) {
    // mix up: 0
    long result = rc2_enc_mix_up (r, ekey, j, 0);
    // mix up: 1
    result = rc2_enc_mix_up (r, ekey, result, 1);
    // mix up: 2
    result = rc2_enc_mix_up (r, ekey, result, 2);
    // mix up: 3
    result = rc2_enc_mix_up (r, ekey, result, 3);
    // here it is
    return result;
  }

  // this procedure do an encryption mash up
  static void rc2_enc_mash_up (t_word* r, const t_word* ekey, const long i) {
    // compute index
    long i1 = i - 1;
    if (i1 < 0) i1 += 4;
    // compute mash up
    r[i] = r[i] + ekey[r[i1] & 63];
  }

  // this procedure performs an encryption mash round
  static void rc2_enc_mash (t_word* r, const t_word* ekey) {
    rc2_enc_mash_up (r, ekey, 0);
    rc2_enc_mash_up (r, ekey, 1);
    rc2_enc_mash_up (r, ekey, 2);
    rc2_enc_mash_up (r, ekey, 3);
  }

  // this procedure performs an encryption round
  static void rc2_enc (t_word* r, const t_word* ekey) {
    // 0: initialize index
    long j = 0;
    // 1: five mixing round
    j = rc2_enc_mix (r, ekey, j);
    j = rc2_enc_mix (r, ekey, j);
    j = rc2_enc_mix (r, ekey, j);
    j = rc2_enc_mix (r, ekey, j);
    j = rc2_enc_mix (r, ekey, j);
    // 2: one mashing round
    rc2_enc_mash (r, ekey);
    // 3: six mixing round
    j = rc2_enc_mix (r, ekey, j);
    j = rc2_enc_mix (r, ekey, j);
    j = rc2_enc_mix (r, ekey, j);
    j = rc2_enc_mix (r, ekey, j);
    j = rc2_enc_mix (r, ekey, j);
    j = rc2_enc_mix (r, ekey, j);
    // 4: one mashing round
    rc2_enc_mash (r, ekey);
    // 5: five mixing rounds
    j = rc2_enc_mix (r, ekey, j);
    j = rc2_enc_mix (r, ekey, j);
    j = rc2_enc_mix (r, ekey, j);
    j = rc2_enc_mix (r, ekey, j);
    j = rc2_enc_mix (r, ekey, j);
    // basic verififcation
    if (j != 64) {
      throw Exception ("internal-error", "invalid rc2 round index");
    }
  }

  // this procedure do a decryption mix up
  static long rc2_dec_mix_up (t_word* r, const t_word* ekey, 
			      const long j, const long i) {
    // compute rotation index
    long k =  (i == 0) ? 1 : (i == 1) ? 2 : (i == 2) ? 3 : 5;
    r[i] = wrotr (r[i], k);
    // compute index
    long i1 = i - 1; long i2 = i - 2; long i3 = i - 3;
    if (i1 < 0) i1 += 4;
    if (i2 < 0) i2 += 4;
    if (i3 < 0) i3 += 4;
    r[i] = r[i] - ekey[j] - (r[i1] & r[i2]) - ((~r[i1]) & r[i3]);
    // update index result
    return j - 1;
  }

  // this procedure performs a decryption mixing round
  static long rc2_dec_mix (t_word* r, const t_word* ekey, const long j) {
    // mix up: 3
    long result = rc2_dec_mix_up (r, ekey, j, 3);
    // mix up: 2
    result = rc2_dec_mix_up (r, ekey, result, 2);
    // mix up: 1
    result = rc2_dec_mix_up (r, ekey, result, 1);
    // mix up: 0
    result = rc2_dec_mix_up (r, ekey, result, 0);
    // here it is
    return result;
  }

  // this procedure do a decryption mash up
  static void rc2_dec_mash_up (t_word* r, const t_word* ekey, const long i) {
    // compute index
    long i1 = i - 1;
    if (i1 < 0) i1 += 4;
    // compute mash up
    r[i] = r[i] - ekey[r[i1] & 63];
  }

  // this procedure performs a decryption mash round
  static void rc2_dec_mash (t_word* r, const t_word* ekey) {
    rc2_dec_mash_up (r, ekey, 3);
    rc2_dec_mash_up (r, ekey, 2);
    rc2_dec_mash_up (r, ekey, 1);
    rc2_dec_mash_up (r, ekey, 0);
  }

  // this procedure perform an decryption round
  static void rc2_dec (t_word* r, const t_word* ekey) {
    // 0: initialize index
    long j = 63;
    // 1: five mixing round
    j = rc2_dec_mix (r, ekey, j);
    j = rc2_dec_mix (r, ekey, j);
    j = rc2_dec_mix (r, ekey, j);
    j = rc2_dec_mix (r, ekey, j);
    j = rc2_dec_mix (r, ekey, j);
    // 2: one mashing round
    rc2_dec_mash (r, ekey);
    // 3: six mixing round
    j = rc2_dec_mix (r, ekey, j);
    j = rc2_dec_mix (r, ekey, j);
    j = rc2_dec_mix (r, ekey, j);
    j = rc2_dec_mix (r, ekey, j);
    j = rc2_dec_mix (r, ekey, j);
    j = rc2_dec_mix (r, ekey, j);
    // 4: one mashing round
    rc2_dec_mash (r, ekey);
    // 5: five mixing rounds
    j = rc2_dec_mix (r, ekey, j);
    j = rc2_dec_mix (r, ekey, j);
    j = rc2_dec_mix (r, ekey, j);
    j = rc2_dec_mix (r, ekey, j);
    j = rc2_dec_mix (r, ekey, j);
    // basic verififcation
    if (j != -1) {
      throw Exception ("internal-error", "invalid rc2 round index");
    }
  }

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a cipher by key

  Rc2::Rc2 (const Key& key) : ModeCipher (RC2_ALGO_NAME, RC2_BLOK_SIZE) {
    // initialize the effective key length
    d_klen = RC2_EKEY_BITS;
    // initialize the cipher
    p_rkey = new t_word[RC2_EKEY_WLEN];
    // set the key
    setkey (key);
  }

  // create a cipher by key and flag

  Rc2::Rc2 (const Key& key, 
	    const bool rflg) : ModeCipher (RC2_ALGO_NAME, RC2_BLOK_SIZE) {
    // initialize the effective key length
    d_klen = RC2_EKEY_BITS;
    // initialize the cipher
    p_rkey = new t_word[RC2_EKEY_WLEN];
    // set the key
    setkey (key);
    // set the reverse flag
    setrflg (rflg);
  }

  // destroy this cipher

  Rc2::~Rc2 (void) {
    delete [] p_rkey;
  }
  
  // return the class name

  String Rc2::repr (void) const {
    return "Rc2";
  }

  // reset this cipher
  
  void Rc2::reset (void) {
    wrlock ();
    try {
      // reset the mode cipher
      ModeCipher::reset ();
      // key reset
      for (long k = 0L; k < RC2_EKEY_WLEN; k++) p_rkey[k] = nilw;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // clear this cipher
  
  void Rc2::clear (void) {
    wrlock ();
    try {
      // clear the mode cipher
      ModeCipher::clear ();
      // expand the key
      rc2_key_expand (p_rkey, d_ckey, d_klen);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set the cipher key

  void Rc2::setkey (const Key& key) {
    wrlock ();
    try {
      // set the base key
      ModeCipher::setkey (key);
      // expand the key
      rc2_key_expand (p_rkey, d_ckey, d_klen);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    } 
  }

  // set the effective key length

  void Rc2::setklen (const long klen) {
    wrlock ();
    try {
      if ((klen <= 0) || (klen > RC2_EKEY_BITS)) {
	throw Exception ("key-error", "invalid effective key size");
      }
      // set the key and clear
      d_klen = klen;
      clear ();
      // unlock and return
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the effective key length

  long Rc2::getklen (void) const {
    rdlock ();
    try {
      long result = d_klen;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // encode a block buffer into another one

  void Rc2::encode (t_byte* bo, const t_byte* bi) {
    wrlock ();
    try {
      // bind the round state
      t_word r[RC2_WORD_SIZE] = {0x0000, 0x0000, 0x0000, 0x0000};
      rc2_bi_to_r (r, bi);
      // encode the state round
      rc2_enc (r, p_rkey);
      // bind the output buffer
      rc2_r_to_bo (bo, r);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // decode a block buffer into another one

  void Rc2::decode (t_byte* bo, const t_byte* bi) {
    wrlock ();
    try {
      // bind the round state
      t_word r[RC2_WORD_SIZE] = {0x0000, 0x0000, 0x0000, 0x0000};
      rc2_bi_to_r (r, bi);
      // decode the state round
      rc2_dec (r, p_rkey);
      // bind the output buffer
      rc2_r_to_bo (bo, r);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // -------------------------------------------------------------------------
  // - object setion                                                         -
  // -------------------------------------------------------------------------

  // the quark zone
  static const long QUARK_ZONE_LENGTH = 2;
  static QuarkZone  zone (QUARK_ZONE_LENGTH);

  // the object supported quarks
  static const long QUARK_SETKLEN = zone.intern ("set-effective-key");
  static const long QUARK_GETKLEN = zone.intern ("get-effective-key");

  // create a new object in a generic way
  
  Object* Rc2::mknew (Vector* argv) {
    long argc = (argv == nullptr) ? 0 : argv->length ();
    // check for 1 argument
    if (argc == 1) {
      // check for a key
      Object* obj = argv->get (0);
      Key*    key = dynamic_cast <Key*> (obj);
      if (key != nullptr) return new Rc2 (*key);
      throw Exception ("argument-error", "invalid arguments with RC2");
    }
    // check for 2 arguments
    if (argc == 2) {
      // check for a key
      Object* obj = argv->get (0);
      Key*    key  = dynamic_cast <Key*> (obj);
      if (key == nullptr) {
	throw Exception ("argument-error", "invalid arguments with RC2");
      }
      // get the reverse flag and object
      bool rflg = argv->getbool (1);
      return new Rc2 (*key, rflg);
    }
    throw Exception ("argument-error", "too many arguments with RC2");
  }

  // return true if the given quark is defined

  bool Rc2::isquark (const long quark, const bool hflg) const {
    rdlock ();
    try {
      if (zone.exists (quark) == true) {
	unlock ();
	return true;
      }
      bool result = hflg ? ModeCipher::isquark (quark, hflg) : false;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // apply this object with a set of arguments and a quark
  
  Object* Rc2::apply (Evaluable* zobj, Nameset* nset, const long quark,
		      Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();
    
    // check for 0 argument
    if (argc == 0) {
      if (quark == QUARK_GETKLEN) return new Integer (getklen ());
    }
    // check for 1 argument
    if (argc == 1) {
      if (quark == QUARK_SETKLEN) {
	long klen = argv->getlong (0);
	setklen (klen);
	return nullptr;
      }
    }
    // call the block cipher method
    return ModeCipher::apply (zobj, nset, quark, argv);
  }
}
