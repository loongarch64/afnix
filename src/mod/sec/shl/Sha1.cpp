// ---------------------------------------------------------------------------
// - Sha1.cpp                                                                -
// - afnix:sec module - SHA1 class implementation                            -
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

#include "Sha1.hpp"
#include "Ascii.hpp"
#include "Vector.hpp"
#include "Cryptics.hxx"
#include "Exception.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - private section                                                       -
  // -------------------------------------------------------------------------

  // SHA-1 algorithm constants
  static const char* SHA1_ALGO_NAME   = "SHA-1";
  static const long  SHA1_BMSG_LENGTH = 64;
  static const long  SHA1_BPAD_LENGTH = 64;
  static const long  SHA1_HASH_LENGTH = 20;

  // SHA-1 message padding
  const t_byte PAD[SHA1_BPAD_LENGTH] = {
    0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
  };

  // SHA-1 K0 constant ~  0 <= t <= 19
  const t_quad K0 = 0x5a827999;
  // SHA-1 K1 constant ~ 20 <= t <= 39
  const t_quad K1 = 0x6ed9eba1;
  // SHA-1 K2 constant ~ 40 <= t <= 59
  const t_quad K2 = 0x8f1bbcdc;
  // SHA-1 K3 constant ~ 60 <= t <= 79
  const t_quad K3 = 0xca62c1d6;

  // SHA-1 initial states
  const t_quad S0 = 0x67452301;
  const t_quad S1 = 0xefcdab89;
  const t_quad S2 = 0x98badcfe;
  const t_quad S3 = 0x10325476;
  const t_quad S4 = 0xc3d2e1f0;
  
  // SHA-1 F0 function ~ 0 <= t <= 19
  static inline t_quad F0 (t_quad x, t_quad y, t_quad z) {
    return (x & y) ^ (~x & z);
  }
  // SHA-1 F1 function ~ 20 <= t <= 39
  static inline t_quad F1 (t_quad x, t_quad y, t_quad z) {
    return x ^ y ^ z;
  }
  // SHA-1 F2 function ~ 40 <= t <= 59
  static inline t_quad F2 (t_quad x, t_quad y, t_quad z) {
    return (x & y) ^ (x & z) ^ (y & z);
  }
  // SHA-1 F3 function ~ 60 <= t <= 79
  static inline t_quad F3 (t_quad x, t_quad y, t_quad z) {
    return x ^ y ^ z;
  }

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a default hasher (SHA-1)

  Sha1::Sha1 (void) : Hasher (SHA1_ALGO_NAME, SHA1_BMSG_LENGTH, 
			      SHA1_HASH_LENGTH) {
    reset ();
  }

  // create a hasher with a result length (SHA-1)

  Sha1::Sha1 (const long rlen) : Hasher (SHA1_ALGO_NAME, SHA1_BMSG_LENGTH, 
					 SHA1_HASH_LENGTH, rlen) {
    reset ();
  }

  // return the class name

  String Sha1::repr (void) const {
    return "Sha1";
  }

  // reset this SHA-1 object

  void Sha1::reset (void) {
    wrlock ();
    Hasher::reset ();
    d_state[0] = S0;
    d_state[1] = S1;
    d_state[2] = S2;
    d_state[3] = S3;
    d_state[4] = S4;
    unlock ();
  }

  // update the hasher state with the buffer data
  
  bool Sha1::update (void) {
    wrlock ();
    try {
      // make sure the buffer is full
      if (length () != SHA1_BMSG_LENGTH) {
	unlock ();
	return false;
      }
      // decode a block in 16 quads
      t_quad x[16]; bebtoq (x, (t_byte*) p_data, SHA1_BMSG_LENGTH);
      // prepare a message schedule
      t_quad W[80];
      for (long i = 0; i < 16; i++) W[i] = x[i];
      for (long i = 16; i < 80; i++) {
	W[i] = qrotl (W[i-3]^W[i-8]^W[i-14]^W[i-16],1);
      }				    
      // initialize state values
      t_quad a = d_state[0];
      t_quad b = d_state[1];
      t_quad c = d_state[2];
      t_quad d = d_state[3];
      t_quad e = d_state[4];
      // update ~ 0 <= t <= 19
      for (long i = 0; i < 20; i++) {
	t_quad t = qrotl (a,5) + F0 (b,c,d) + e + K0 + W[i];
	e = d;
	d = c;
	c = qrotl (b, 30);
	b = a;
	a = t;
      }
      // update ~ 20 <= t <= 39
      for (long i = 20; i < 40; i++) {
	t_quad t = qrotl (a,5) + F1 (b,c,d) + e + K1 + W[i];
	e = d;
	d = c;
	c = qrotl (b, 30);
	b = a;
	a = t;
      }
      // update ~ 40 <= t <= 59
      for (long i = 40; i < 60; i++) {
	t_quad t = qrotl (a,5) + F2 (b,c,d) + e + K2 + W[i];
	e = d;
	d = c;
	c = qrotl (b, 30);
	b = a;
	a = t;
      }
      // update ~ 60 <= t <= 79
      for (long i = 60; i < 80; i++) {
	t_quad t = qrotl (a,5) + F3 (b,c,d) + e + K3 + W[i];
	e = d;
	d = c;
	c = qrotl (b, 30);
	b = a;
	a = t;
      }
      // state update
      d_state[0] += a;
      d_state[1] += b;
      d_state[2] += c;
      d_state[3] += d;
      d_state[4] += e;
      // clear the buffer count to indicate processing
      unlock ();
      return true;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // finish processing by padding the message
  
  void Sha1::finish (void) {
    wrlock ();
    try {
      // get message length
      t_octa len = getwcnt ();
      // how much padding do we have to do - the message must be congruent
      // to 56 modulo 64 (aka 448 bits modulo 512)
      long res = len % SHA1_BPAD_LENGTH;
      long pad = (res < 56) ? 56 - res : 120 - res;
      // process with the padding
      process (PAD, pad);
      // now add the length - this is in bits !!!
      t_byte blen[8];
      t_octa bits = len << 3;
      beotob (blen, &bits, 1);
      process (blen, 8);
      // update the result array
      beqtob (p_hash, d_state, 5);
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

  Object* Sha1::mknew (Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();
    // check for 0 argument
    if (argc == 0) return new Sha1;
    // check for 1 argument
    if (argc == 1) {
      long rlen = argv->getlong (0);
      return new Sha1 (rlen);
    }
    // invalid arguments
    throw Exception ("argument-error", "too many arguments for SHA-1");
  }
}
