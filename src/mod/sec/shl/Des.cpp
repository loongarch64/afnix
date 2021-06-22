// ---------------------------------------------------------------------------
// - Des.cpp                                                                 -
// - afnix:sec module - des (data encryption standard) class implementation  -
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

#include "Des.hpp"
#include "Vector.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - private section                                                       -
  // -------------------------------------------------------------------------

  // des block cipher constants
  static const char* DES_ALGO_NAME = "DES";
  static const long  DES_BLOK_SIZE = 8;

  // des valid key constants
  static const long  DES_VKEY_BITS = 64;
  static const long  DES_VKEY_SIZE = DES_VKEY_BITS / 8;

  // des states constants
  static const long  DES_ROUND_NUM = 16;

  // the permuted choice PC-1 table (conforming to FIPS PUB 46-3)
  static const long  DES_TBPC1_LEN = 56;
  static const long  DES_TBPC1 []  = {
    57, 49, 41, 33, 25, 17, 9,
    1,  58, 50, 42, 34, 26, 18,
    10, 2,  59, 51, 43, 35, 27,
    19, 11, 3,  60, 52, 44, 36,
    63, 55, 47, 39, 31, 23, 15,
    7,  62, 54, 46, 38, 30, 22,
    14, 6,  61, 53, 45, 37, 29,
    21, 13, 5,  28, 20, 12, 4
  };

  // the permuted choice PC-2 table (conforming to FIPS PUB 46-3)
  static const long  DES_TBPC2_LEN = 48;
  static const long  DES_TBPC2 []  = {
    14, 17, 11, 24, 1,  5,
    3,  28, 15, 6,  21, 10,
    23, 19, 12, 4,  26, 8,
    16, 7,  27, 20, 13, 2,
    41, 52, 31, 37, 47, 55,
    30, 40, 51, 45, 33, 48,
    44, 49, 39, 56, 34, 53,
    46, 42, 50, 36, 29, 32
  };

  // the key schedule shift value
  static const long DES_HALVE_ROT [] = {1,1,2,2,2,2,2,2,1,2,2,2,2,2,2,1};

  // the initial permutation (conforming to FIPS PUB 46-3)
  static const long  DES_TBLIP_LEN = 64;
  static const long  DES_TBLIP []  = {
    58, 50, 42, 34, 26, 18, 10, 2,
    60, 52, 44, 36, 28, 20, 12, 4,
    62, 54, 46, 38, 30, 22, 14, 6,
    64, 56, 48, 40, 32, 24, 16, 8,
    57, 49, 41, 33, 25, 17, 9,  1,
    59, 51, 43, 35, 27, 19, 11, 3,
    61, 53, 45, 37, 29, 21, 13, 5,
    63, 55, 47, 39, 31, 23, 15, 7
  };

  // the reverse permutation (conforming to FIPS PUB 46-3)
  static const long  DES_TBLRP_LEN = 64;
  static const long  DES_TBLRP []  = {
    40, 8, 48, 16, 56, 24, 64, 32,
    39, 7, 47, 15, 55, 23, 63, 31,
    38, 6, 46, 14, 54, 22, 62, 30,
    37, 5, 45, 13, 53, 21, 61, 29,
    36, 4, 44, 12, 52, 20, 60, 28,
    35, 3, 43, 11, 51, 19, 59, 27,
    34, 2, 42, 10, 50, 18, 58, 26,
    33, 1, 41, 9,  49, 17, 57, 25
  };

  // the feistel expansion table (conforming to FIPS PUB 46-3)
  static const long  DES_TBLFE_LEN = 48;
  static const long  DES_TBLFE []  = {
    32, 1,  2,  3,  4,  5,
    4,  5,  6,  7,  8,  9,
    8,  9,  10, 11, 12, 13,
    12, 13, 14, 15, 16, 17,
    16, 17, 18, 19, 20, 21,
    20, 21, 22, 23, 24, 25,
    24, 25, 26, 27, 28, 29,
    28, 29, 30, 31, 32, 1
  };
  
  // the feistel sbox mapping function
  static const long  DES_BOXSM []  = {
    0,  16, 1,  17, 2,  18, 3,  19, 4,  20, 5,  21, 6,  22, 7,  23,
    8,  24, 9,  25, 10, 26, 11, 27, 12, 28, 13, 29, 14, 30, 15, 31,
    32, 48, 33, 49, 34, 50, 35, 51, 36, 52, 37, 53, 38, 54, 39, 55,
    40, 56, 41, 57, 42, 58, 43, 59, 44, 60, 45, 61, 46, 62, 47, 63
  };

  // the feistel sbox S1 (conforming to FIPS PUB 46-3)
  static const t_byte DES_BOXS1 []  = {
    14, 4,  13, 1,  2,  15, 11, 8,  3,  10, 6,  12, 5,  9,  0,  7,
    0,  15, 7,  4,  14, 2,  13, 1,  10, 6,  12, 11, 9,  5,  3,  8,
    4,  1,  14, 8,  13, 6,  2,  11, 15, 12, 9,  7,  3,  10, 5,  0,
    15, 12, 8,  2,  4,  9,  1,  7,  5,  11, 3,  14, 10, 0,  6,  13
  };

  // the feistel sbox S2 (conforming to FIPS PUB 46-3)
  static const t_byte DES_BOXS2 []  = {
    15, 1,  8,  14, 6,  11, 3,  4,  9,  7,  2,  13, 12, 0,  5,  10,
    3,  13, 4,  7,  15, 2,  8,  14, 12, 0,  1,  10, 6,  9,  11, 5,
    0,  14, 7,  11, 10, 4,  13, 1,  5,  8,  12, 6,  9,  3,  2,  15,
    13, 8,  10, 1,  3,  15, 4,  2,  11, 6,  7,  12, 0,  5,  14, 9
  };

  // the feistel sbox S3 (conforming to FIPS PUB 46-3)
  static const t_byte DES_BOXS3 []  = {
    10, 0,  9,  14, 6,  3,  15, 5,  1,  13, 12, 7,  11, 4,  2,  8,
    13, 7,  0,  9,  3,  4,  6,  10, 2,  8,  5,  14, 12, 11, 15, 1,
    13, 6,  4,  9,  8,  15, 3,  0,  11, 1,  2,  12, 5,  10, 14, 7,
    1,  10, 13, 0,  6,  9,  8,  7,  4,  15, 14, 3,  11, 5,  2,  12
  };

  // the feistel sbox S4 (conforming to FIPS PUB 46-3)
  static const t_byte DES_BOXS4 []  = {
    7,  13, 14, 3,  0,  6,  9,  10, 1,  2,  8,  5,  11, 12, 4,  15,
    13, 8,  11, 5,  6,  15, 0,  3,  4,  7,  2,  12, 1,  10, 14, 9,
    10, 6,  9,  0,  12, 11, 7,  13, 15, 1,  3,  14, 5,  2,  8,  4,
    3,  15, 0,  6,  10, 1,  13, 8,  9,  4,  5,  11, 12, 7,  2,  14
  };

  // the feistel sbox S5 (conforming to FIPS PUB 46-3)
  static const long  DES_BOXS5 []  = {
    2,  12, 4,  1,  7,  10, 11, 6,  8,  5,  3,  15, 13, 0,  14, 9,
    14, 11, 2,  12, 4,  7,  13, 1,  5,  0,  15, 10, 3,  9,  8,  6,
    4,  2,  1,  11, 10, 13, 7,  8,  15, 9,  12, 5,  6,  3,  0,  14,
    11, 8,  12, 7,  1,  14, 2,  13, 6,  15, 0,  9,  10, 4,  5,  3
  };

  // the feistel sbox S6 (conforming to FIPS PUB 46-3)
  static const long  DES_BOXS6 []  = {
    12, 1,  10, 15, 9,  2,  6,  8,  0,  13, 3,  4,  14, 7,  5,  11,
    10, 15, 4,  2,  7,  12, 9,  5,  6,  1,  13, 14, 0,  11, 3,  8,
    9,  14, 15, 5,  2,  8,  12, 3,  7,  0,  4,  10, 1,  13, 11, 6,
    4,  3,  2,  12, 9,  5,  15, 10, 11, 14, 1,  7,  6,  0,  8,  13
  };

  // the feistel sbox S7 (conforming to FIPS PUB 46-3)
  static const long  DES_BOXS7 []  = {
    4,  11, 2,  14, 15, 0,  8,  13, 3,  12, 9,  7,  5,  10, 6,  1,
    13, 0,  11, 7,  4,  9,  1,  10, 14, 3,  5,  12, 2,  15, 8,  6,
    1,  4,  11, 13, 12, 3,  7,  14, 10, 15, 6,  8,  0,  5,  9,  2,
    6,  11, 13, 8,  1,  4,  10, 7,  9,  5,  0,  15, 14, 2,  3,  12
  };

  // the feistel sbox S8 (conforming to FIPS PUB 46-3)
  static const long  DES_BOXS8 []  = {
    13, 2,  8,  4,  6,  15, 11, 1,  10, 9,  3,  14, 5,  0,  12, 7,
    1,  15, 13, 8,  10, 3,  7,  4,  12, 5,  6,  11, 0,  14, 9,  2,
    7,  11, 4,  1,  9,  12, 14, 2,  0,  6,  10, 13, 15, 3,  5,  8,
    2,  1,  14, 7,  4,  10, 8,  13, 15, 12, 9,  0,  3,  5,  6,  11
  };

  // the feistel pbox (conforming to FIPS PUB 46-3)
  static const long  DES_FPBOX_LEN = 32;
  static const long  DES_FPBOX []  = {
    16, 7,  20, 21, 29, 12, 28, 17,
    1,  15, 23, 26, 5,  18, 31, 10,
    2,  8,  24, 14, 32, 27, 3,  9,
    19, 13, 30, 6,  22, 11, 4,  25    
  };

  // this procedure rotates a 28 bits halves
  static inline t_quad des_rot_halve (const t_quad x, const long rval) {
    t_quad result = x;
    for (long i = 0; i < rval; i++) {
      t_quad qval = result << 1;
      if ((qval & 0x10000000UL) != nilq) qval |= 0x00000001UL;
      result = qval & 0x0FFFFFFFUL;
    }
    return result;
  }

  // this procedure bind the key to an octa
  static inline t_octa des_key_to_octa (const Key& key) {
    // check the key size
    if (key.getbits () != DES_VKEY_BITS) {
      throw Exception ("des-error", "invalid key type or size with des");
    }
    // bind the key to an octa
    t_octa result = 0x0000000000000000ULL;
    for (long i = 0; i < DES_VKEY_SIZE; i++) {
      if (i > 0) result <<= 8; 
      result |= key.getbyte (i);
    }
    return result;
  }

  // this procedure permutes an octa key from the PC-1 table
  static inline t_octa des_key_pc_1 (const t_octa okey) {
    // the key is permuted from the PC-1 table - the result is
    // defined with 56 bits, lsb to msb reading from left to write
    t_octa result = 0x0000000000000000ULL;
    for (long i = 0; i <  DES_TBPC1_LEN; i++) {
      // shift the result
      if (i > 0) result <<= 1;
      // get the shift index and mask
      long   sidx = 64 - DES_TBPC1[i];
      t_octa mask = (okey >> sidx) & 0x0000000000000001ULL;
      // set the result
      result |= mask;
    }
    return result;
  }

  // this procedure permutes an octa key from the PC-2 table
  static inline t_octa des_key_pc_2 (const t_octa okey) {
    // the key is permuted from the PC-2 table - the result is
    // defined with 48 bits, lsb to msb reading from left to write
    t_octa result = 0x0000000000000000ULL;
    for (long i = 0; i <  DES_TBPC2_LEN; i++) {
      // shift the result
      if (i > 0) result <<= 1;
      // get the shift index and mask
      long   sidx = 56 - DES_TBPC2[i];
      t_octa mask = (okey >> sidx) & 0x0000000000000001ULL;
      // set the result
      result |= mask;
    }
    return result;
  }

  // this procedure expand the key into the round subkeys
  static void des_key_expand (t_octa* rkey, const Key& key) {
    // check arguments
    if (rkey == nullptr) return;
    // reset the round key
    for (long k = 0L; k < DES_ROUND_NUM; k++) rkey[k] = nilo;
    // basic check as usual
    if (rkey == nullptr) return;
    // bind the key to an octa
    t_octa okey = des_key_to_octa (key);
    // generate the permuted key from PC-1
    t_octa pkey = des_key_pc_1 (okey);
    // prepare the left and right 28 bits halves
    t_quad lkh = (t_quad) ((pkey >> 28) & 0x000000000FFFFFFFULL);
    t_quad rkh = (t_quad) (pkey & 0x000000000FFFFFFFULL);
    // rotate the left and right halves - and update the keys
    for (long i = 0; i < DES_ROUND_NUM; i++) {
      // rotate the halves
      t_quad nlkh = des_rot_halve (lkh, DES_HALVE_ROT[i]);
      t_quad nrkh = des_rot_halve (rkh, DES_HALVE_ROT[i]);
      // build the 56 bits subkey
      t_octa skey = (((t_octa) nlkh) << 28) | ((t_octa) nrkh);
      // set the permuted key from PC-2
      rkey[i] = des_key_pc_2 (skey);
      // bind previous halves
      lkh = nlkh; rkh = nrkh;
    }
  }

  // this procedure bind an input buffer to an octa
  static inline t_octa des_bi_to_octa (const t_byte* bi) {
    // bind the buffer to an octa
    t_octa result = 0x0000000000000000ULL;
    for (long i = 0; i < DES_BLOK_SIZE; i++) {
      if (i > 0) result <<= 8; 
      result |= (t_octa) bi[i];
    }
    return result;
  }

  // this procedure bind an octa to an output buffer
  static inline void des_octa_to_bo (t_byte* bo, const t_octa x) {
    t_octa oval = x;
    for (long i = 0; i < DES_BLOK_SIZE; i++) {
      if (i > 0) oval >>= 8; 
      bo[DES_BLOK_SIZE-i-1] = (t_byte) (oval & 0x00000000000000FFULL);
    }
  }

  // this procedure permutes an octa from the initial permutation table
  static inline t_octa des_ip (const t_octa x) {
    // the key is permuted from the ip table - the result is
    // defined with 64 bits, lsb to msb reading from left to write
    t_octa result = 0x0000000000000000ULL;
    for (long i = 0; i <  DES_TBLIP_LEN; i++) {
      // shift the result
      if (i > 0) result <<= 1;
      // get the shift index and mask
      long   sidx = 64 - DES_TBLIP[i];
      t_octa mask = (x >> sidx) & 0x0000000000000001ULL;
      // set the result
      result |= mask;
    }
    return result;
  }

  // this procedure permutes an octa from the reverse permutation table
  static inline t_octa des_rp (const t_octa x) {
    // the key is permuted from the rp table - the result is
    // defined with 64 bits, lsb to msb reading from left to write
    t_octa result = 0x0000000000000000ULL;
    for (long i = 0; i <  DES_TBLRP_LEN; i++) {
      // shift the result
      if (i > 0) result <<= 1;
      // get the shift index and mask
      long   sidx = 64 - DES_TBLRP[i];
      t_octa mask = (x >> sidx) & 0x0000000000000001ULL;
      // set the result
      result |= mask;
    }
    return result;
  }

  // this procedure expands a quad into a 48 bits octa
  static inline t_octa des_feistel_expand (const t_quad x) {
    // the quad is expanded from the feistel expansion table
    t_octa result = nilo;
    for (long i = 0; i < DES_TBLFE_LEN; i++) {
      // shift the result
      if (i > 0) result <<= 1;
      // get the shift index and mask
      long   sidx = 32 - DES_TBLFE[i];
      t_octa mask = (x >> sidx) & 0x0000000000000001ULL;
      // set the result
      result |= mask;
    }
    return result;
  }

  // this procedure process an octa through the sbox
  static inline t_quad des_feistel_sbox (const t_octa x) {
    // initialize the result
    t_quad result = nilq;
    // extract sbox 6 bits input
    long sidx = (x >> 42) & 0x0000003F;
    // extract sbox index
    long midx = DES_BOXSM[sidx];
    // extract sbox S1 value
    result |= DES_BOXS1[midx];
    // shift result
    result <<= 4;
    // extract sbox 6 bits input
    sidx = (x >> 36) & 0x0000003F;
    // extract sbox index
    midx = DES_BOXSM[sidx];
    // extract sbox S2 value
    result |= DES_BOXS2[midx];
    // shift result
    result <<= 4;
    // extract sbox 6 bits input
    sidx = (x >> 30) & 0x0000003F;
    // extract sbox index
    midx = DES_BOXSM[sidx];
    // extract sbox S3 value
    result |= DES_BOXS3[midx];
    // shift result
    result <<= 4;
    // extract sbox 6 bits input
    sidx = (x >> 24) & 0x0000003F;
    // extract sbox index
    midx = DES_BOXSM[sidx];
    // extract sbox S4 value
    result |= DES_BOXS4[midx];
    // shift result
    result <<= 4;
    // extract sbox 6 bits input
    sidx = (x >> 18) & 0x0000003F;
    // extract sbox index
    midx = DES_BOXSM[sidx];
    // extract sbox S5 value
    result |= DES_BOXS5[midx];
    // shift result
    result <<= 4;
    // extract sbox 6 bits input
    sidx = (x >> 12) & 0x0000003F;
    // extract sbox index
    midx = DES_BOXSM[sidx];
    // extract sbox S6 value
    result |= DES_BOXS6[midx];
    // shift result
    result <<= 4;
    // extract sbox 6 bits input
    sidx = (x >> 6) & 0x0000003F;
    // extract sbox index
    midx = DES_BOXSM[sidx];
    // extract sbox S7 value
    result |= DES_BOXS7[midx];
    // shift result
    result <<= 4;
    // extract sbox 6 bits input
    sidx = x & 0x0000003F;
    // extract sbox index
    midx = DES_BOXSM[sidx];
    // extract sbox S8 value
    result |= DES_BOXS8[midx];
    // final result
    return result;
  }

  // this procedure permutes a quad with the pbox table
  static inline t_quad des_feistel_pbox (const t_quad x) {
    // the quad is permuted from the feistel pbox
    t_quad result = 0x00000000UL;
    for (long i = 0; i < DES_FPBOX_LEN; i++) {
      // shift the result
      if (i > 0) result <<= 1;
      // get the shift index and mask
      long   sidx = 32 - DES_FPBOX[i];
      t_octa mask = (x >> sidx) & 0x00000001UL;
      // set the result
      result |= mask;
    }
    return result;
  }

  // this procedure computes a feistel value
  static t_quad des_feistel (const t_quad x, const t_octa skey) {
    // expand the input quad
    t_octa ex = des_feistel_expand (x);
    // add the key value
    t_octa ek = ex ^ skey;
    // process sbox substitution
    t_quad rs = des_feistel_sbox (ek);
    // process the pbox substitution
    t_quad rp = des_feistel_pbox (rs);
    // here it is
    return rp;
  }

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a cipher by key

  Des::Des (const Key& key) : ModeCipher (DES_ALGO_NAME, DES_BLOK_SIZE) {
    // initialize the cipher
    p_rkey = new t_octa[DES_ROUND_NUM];
    // set the key
    setkey (key);
  }

  // create a cipher by key and flag

  Des::Des (const Key& key, 
	    const bool rflg) : ModeCipher (DES_ALGO_NAME, DES_BLOK_SIZE) {
    // initialize the cipher
    p_rkey = new t_octa[DES_ROUND_NUM];
    // set the key
    setkey (key);
    // set the reverse flag
    setrflg (rflg);
  }

  // destroy this cipher

  Des::~Des (void) {
    delete [] p_rkey;
  }

  // return the class name

  String Des::repr (void) const {
    return "Des";
  }

  // reset this cipher
  
  void Des::reset (void) {
    wrlock ();
    try {
      // reset the mode cipher
      ModeCipher::reset ();
      // round key and state buffer reset
      for (long i = 0; i < DES_ROUND_NUM; i++) p_rkey[i] = nilo;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // clear this cipher
  
  void Des::clear (void) {
    wrlock ();
    try {
      // clear the mode cipher
      ModeCipher::clear ();
      // expand the key
      des_key_expand (p_rkey, d_ckey);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set the cipher key

  void Des::setkey (const Key& key) {
    wrlock ();
    try {
      // set the base key
      ModeCipher::setkey (key);
      // expand the key
      des_key_expand (p_rkey, d_ckey);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    } 
  }
  
  // encode a block buffer into another one

  void Des::encode (t_byte* bo, const t_byte* bi) {
    wrlock ();
    try {
      // bind the input buffer to an octa
      t_octa obi = des_bi_to_octa (bi);
      // perform the initial permutation
      t_octa oip = des_ip (obi);
      // extract the left and right halves (32 bits)
      t_quad lh = (t_quad) ((oip >> 32) & 0x00000000FFFFFFFFULL);
      t_quad rh = (t_quad) (oip & 0x00000000FFFFFFFFULL);
      // loop for all rounds
      for (long i = 0; i < DES_ROUND_NUM; i++) {
	// compute the feistel value
	t_quad fh = lh ^ des_feistel (rh, p_rkey[i]);
	// update the left and right halves
	lh = rh;
	rh = fh;
      }
      // combine the final halves (reverse rh-lh)
      t_octa ofp = (((t_octa) rh) << 32) | ((t_octa) lh);
      // perform the reverse permutation
      t_octa orp = des_rp (ofp);
      // bind the octa to an output buffer
      des_octa_to_bo (bo, orp);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // decode a block buffer into another one

  void Des::decode (t_byte* bo, const t_byte* bi) {
    wrlock ();
    try {
      // bind the input buffer to an octa
      t_octa obi = des_bi_to_octa (bi);
      // perform the initial permutation
      t_octa oip = des_ip (obi);
      // extract the left and right halves (32 bits)
      t_quad lh = (t_quad) ((oip >> 32) & 0x00000000FFFFFFFFULL);
      t_quad rh = (t_quad) (oip & 0x00000000FFFFFFFFULL);
      // loop for all rounds
      for (long i = 0; i < DES_ROUND_NUM; i++) {
	// compute the feistel value
	t_quad fh = lh ^ des_feistel (rh, p_rkey[DES_ROUND_NUM-i-1]);
	// update the left and right halves
	lh = rh;
	rh = fh;
      }
      // combine the final halves (reverse rh-lh)
      t_octa ofp = (((t_octa) rh) << 32) | ((t_octa) lh);
      // perform the reverse permutation
      t_octa orp = des_rp (ofp);
      // bind the octa to an output buffer
      des_octa_to_bo (bo, orp);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // -------------------------------------------------------------------------
  // - object setion                                                         -
  // -------------------------------------------------------------------------

  // create a new object in a generic way
  
  Object* Des::mknew (Vector* argv) {
    long argc = (argv == nullptr) ? 0 : argv->length ();
    // check for 1 argument
    if (argc == 1) {
      // check for a key
      Object* obj = argv->get (0);
      Key*    key = dynamic_cast <Key*> (obj);
      if (key != nullptr) return new Des (*key);
      throw Exception ("argument-error", "invalid arguments with des");
    }
    // check for 2 arguments
    if (argc == 2) {
      // check for a key
      Object* obj = argv->get (0);
      Key*    key  = dynamic_cast <Key*> (obj);
      if (key == nullptr) {
	throw Exception ("argument-error", "invalid arguments with des");
      }
      // get the reverse flag and object
      bool rflg = argv->getbool (1);
      return new Des (*key, rflg);
    }
    throw Exception ("argument-error", "too many arguments with des");
  }
}
