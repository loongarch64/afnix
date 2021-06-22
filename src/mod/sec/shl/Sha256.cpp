// ---------------------------------------------------------------------------
// - Sha256.cpp                                                              -
// - afnix:sec module - SHA256 class implementation                          -
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

#include "Ascii.hpp"
#include "Sha256.hpp"
#include "Vector.hpp"
#include "Cryptics.hxx"
#include "Exception.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - private section                                                       -
  // -------------------------------------------------------------------------

  // SHA-256 algorithm constants
  static const char* SHA256_ALGO_NAME   = "SHA-256";
  static const long  SHA256_BMSG_LENGTH = 64;
  static const long  SHA256_BPAD_LENGTH = 64;
  static const long  SHA256_HASH_LENGTH = 32;

  // SHA-256 message padding
  const t_byte PAD[SHA256_BPAD_LENGTH] = {
    0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
  };

  // SHA-256 constants
  const t_quad K[64] = {
    0x428A2F98, 0x71374491, 0xB5C0FBCF, 0xE9B5DBA5,
    0x3956C25B, 0x59F111F1, 0x923F82A4, 0xAB1C5ED5,
    0xD807AA98, 0x12835B01, 0x243185BE, 0x550C7DC3,
    0x72BE5D74, 0x80DEB1FE, 0x9BDC06A7, 0xC19BF174,
    0xE49B69C1, 0xEFBE4786, 0x0FC19DC6, 0x240CA1CC,
    0x2DE92C6F, 0x4A7484AA, 0x5CB0A9DC, 0x76F988DA,
    0x983E5152, 0xA831C66D, 0xB00327C8, 0xBF597FC7,
    0xC6E00BF3, 0xD5A79147, 0x06CA6351, 0x14292967,
    0x27B70A85, 0x2E1B2138, 0x4D2C6DFC, 0x53380D13,
    0x650A7354, 0x766A0ABB, 0x81C2C92E, 0x92722C85,
    0xA2BFE8A1, 0xA81A664B, 0xC24B8B70, 0xC76C51A3,
    0xD192E819, 0xD6990624, 0xF40E3585, 0x106AA070,
    0x19A4C116, 0x1E376C08, 0x2748774C, 0x34B0BCB5,
    0x391C0CB3, 0x4ED8AA4A, 0x5B9CCA4F, 0x682E6FF3,
    0x748F82EE, 0x78A5636F, 0x84C87814, 0x8CC70208,
    0x90BEFFFA, 0xA4506CEB, 0xBEF9A3F7, 0xC67178F2
  };

  // SHA-256 initial states
  const t_quad I0 = 0x6a09e667;
  const t_quad I1 = 0xbb67ae85;
  const t_quad I2 = 0x3c6ef372;
  const t_quad I3 = 0xa54ff53a;
  const t_quad I4 = 0x510e527f;
  const t_quad I5 = 0x9b05688c;
  const t_quad I6 = 0x1f83d9ab;
  const t_quad I7 = 0x5be0cd19;

  // SHA-256 CH function
  static inline t_quad CH (t_quad x, t_quad y, t_quad z) {
    return (x & y) ^ (~x & z);
  }
  // SHA-256 MJ function
  static inline t_quad MJ (t_quad x, t_quad y, t_quad z) {
    return (x & y) ^ (x & z) ^ (y & z);
  }
  // SHA-256 E0 function
  static inline t_quad E0 (t_quad x) {
    return qrotr (x, 2) ^ qrotr (x, 13) ^ qrotr (x, 22);
  }
  // SHA-256 E1 function
  static inline t_quad E1 (t_quad x) {
    return qrotr (x, 6) ^ qrotr (x, 11) ^ qrotr (x, 25);
  }
  // SHA-256 S0 function
  static inline t_quad S0 (t_quad x) {
    return qrotr (x, 7) ^ qrotr (x, 18) ^ (x >> 3);
  }
  // SHA-256 S1 function
  static inline t_quad S1 (t_quad x) {
    return qrotr (x, 17) ^ qrotr (x, 19) ^ (x >> 10);
  }

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a default hasher (SHA-256)

  Sha256::Sha256 (void) : Hasher (SHA256_ALGO_NAME, SHA256_BMSG_LENGTH,
				  SHA256_HASH_LENGTH) {
    reset ();
  }

  // create a hasher with a result length (SHA-256)

  Sha256::Sha256 (const long rlen) : Hasher (SHA256_ALGO_NAME, 
					     SHA256_BMSG_LENGTH, 
					     SHA256_HASH_LENGTH, rlen) {
    reset ();
  }

  // return the class name

  String Sha256::repr (void) const {
    return "Sha256";
  }

  // reset this SHA-256 object

  void Sha256::reset (void) {
    wrlock ();
    Hasher::reset ();
    d_state[0] = I0;
    d_state[1] = I1;
    d_state[2] = I2;
    d_state[3] = I3;
    d_state[4] = I4;
    d_state[5] = I5;
    d_state[6] = I6;
    d_state[7] = I7;
    unlock ();
  }

  // update the hasher state with the buffer data
  
  bool Sha256::update (void) {
    wrlock ();
    try {
      // make sure the buffer is full
      if (length () != SHA256_BMSG_LENGTH) {
	unlock ();
	return false;
      }
      // decode a block in 16 quads
      t_quad M[16]; bebtoq (M, (t_byte*) p_data, SHA256_BMSG_LENGTH);
      // prepare a message schedule
      t_quad W[64];
      for (long i = 0; i < 16; i++) W[i] = M[i];
      for (long i = 16; i < 64; i++) {
	W[i] = S1 (W[i-2]) + W[i-7] + S0 (W[i-15]) + W[i-16];
      }				    
      // initialize state values
      t_quad a = d_state[0];
      t_quad b = d_state[1];
      t_quad c = d_state[2];
      t_quad d = d_state[3];
      t_quad e = d_state[4];
      t_quad f = d_state[5];
      t_quad g = d_state[6];
      t_quad h = d_state[7];
      // compute working values
      for (long i = 0; i < 64; i++) {
	// compute temporary values
	t_quad T1 = h + E1 (e) + CH (e,f,g) + K[i] + W[i];
	t_quad T2 = E0 (a) + MJ (a,b,c);
	// update working values
	h = g;      g = f; f = e;
	e = d + T1; d = c; c = b;
	b = a;      a = T1 + T2;
      }
      // state update
      d_state[0] += a;
      d_state[1] += b;
      d_state[2] += c;
      d_state[3] += d;
      d_state[4] += e;
      d_state[5] += f;
      d_state[6] += g;
      d_state[7] += h;
      // clear the buffer count to indicate processing
      unlock ();
      return true;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // finish processing by padding the message
  
  void Sha256::finish (void) {
    wrlock ();
    try {
      // get message length
      t_octa len = getwcnt ();
      // how much padding do we have to do - the message must be congruent
      // to 56 modulo 64 (aka 448 bits modulo 512)
      long res = len % SHA256_BPAD_LENGTH;
      long pad = (res < 56) ? 56 - res : 120 - res;
      // process with the padding
      process (PAD, pad);
      // now add the length - this is in bits !!!
      t_byte blen[8];
      t_octa bits = len << 3;
      beotob (blen, &bits, 1);
      process (blen, 8);
      // update the result array
      beqtob (p_hash, d_state, 8);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // -------------------------------------------------------------------------
  // - object section                                                        -
  // -------------------------------------------------------------------------

  // create a new object in a generic way

  Object* Sha256::mknew (Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();
    // check for 0 argument
    if (argc == 0) return new Sha256;
    // check for 1 argument
    if (argc == 1) {
      long rlen = argv->getlong (0);
      return new Sha256 (rlen);
    }
    // invalid arguments
    throw Exception ("argument-error", "too many arguments for SHA-256");
  }
}
