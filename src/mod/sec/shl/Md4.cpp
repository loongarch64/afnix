// ---------------------------------------------------------------------------
// - Md4.cpp                                                                 -
// - afnix:sec module - MD4 message digest class implementation              -
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

#include "Md4.hpp"
#include "Ascii.hpp"
#include "Vector.hpp"
#include "Cryptics.hxx"
#include "Exception.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - private section                                                       -
  // -------------------------------------------------------------------------

  // MD4 algorithm constants
  static const char* MD4_ALGO_NAME   = "MD-4";
  static const long  MD4_BMSG_LENGTH = 64;
  static const long  MD4_BPAD_LENGTH = 64;
  static const long  MD4_HASH_LENGTH = 16;

  // MD4 message padding
  const t_byte PAD[MD4_BPAD_LENGTH] = {
    0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
  };

  // MD4 constants
  const long S11 = 3;
  const long S12 = 7;
  const long S13 = 11;
  const long S14 = 19;
  const long S21 = 3;
  const long S22 = 5;
  const long S23 = 9;
  const long S24 = 13;
  const long S31 = 3;
  const long S32 = 9;
  const long S33 = 11;
  const long S34 = 15;

  // MD4 basic functions
  static inline t_quad F (t_quad x, t_quad y, t_quad z) {
    return ((x & y) | ((~x) & z));
  }
  static inline t_quad G (t_quad x, t_quad y, t_quad z) {
    return ((x & y) | (x & z) | (y & z));
  }
  static inline t_quad H (t_quad x, t_quad y, t_quad z) {
    return (x ^ y ^ z);
  }

  // transformations for rounds 1, 2, 3
  static inline void FF (t_quad& a, t_quad b, t_quad c, t_quad d, t_quad x, 
			 long s) {
    a += F (b, c, d) + x;
    a  = qrotl (a, s);
  }
  static inline void GG (t_quad& a, t_quad b, t_quad c, t_quad d, t_quad x, 
			 long s) {
    a += G (b, c, d) + x + 0x5a827999UL;
    a  = qrotl (a, s);
  }
  static inline void HH (t_quad& a, t_quad b, t_quad c, t_quad d, t_quad x, 
			 long s) {
    a += H (b, c, d) + x + 0x6ed9eba1UL;
    a  = qrotl (a, s);
  }

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a default hasher (MD-4)

  Md4::Md4 (void) : Hasher (MD4_ALGO_NAME, MD4_BMSG_LENGTH, MD4_HASH_LENGTH) {
    reset ();
  }

  // create a hasher with a result length (MD-4)

  Md4::Md4 (const long rlen) : Hasher (MD4_ALGO_NAME, MD4_BMSG_LENGTH, 
				       MD4_HASH_LENGTH, rlen) {
    reset ();
  }

  // return the class name

  String Md4::repr (void) const {
    return "Md4";
  }

  // reset this MD4 object

  void Md4::reset (void) {
    wrlock ();
    Hasher::reset ();
    d_state[0] = 0x67452301UL;
    d_state[1] = 0xefcdab89UL;
    d_state[2] = 0x98badcfeUL;
    d_state[3] = 0x10325476UL;
    unlock ();
  }

  // update the hasher state with the buffer data
  
  bool Md4::update (void) {
    wrlock ();
    try {
      // make sure the buffer is full
      if (length () != MD4_BMSG_LENGTH) {
	unlock ();
	return false;
      }
      // initialize state values
      t_quad a = d_state[0];
      t_quad b = d_state[1];
      t_quad c = d_state[2];
      t_quad d = d_state[3];
      // decode a block in 16 quads
      t_quad x[16]; lebtoq (x, (t_byte*) p_data, MD4_BMSG_LENGTH);
      // round 1
      FF (a, b, c, d, x[ 0], S11);
      FF (d, a, b, c, x[ 1], S12);
      FF (c, d, a, b, x[ 2], S13);
      FF (b, c, d, a, x[ 3], S14);
      FF (a, b, c, d, x[ 4], S11);
      FF (d, a, b, c, x[ 5], S12);
      FF (c, d, a, b, x[ 6], S13);
      FF (b, c, d, a, x[ 7], S14);
      FF (a, b, c, d, x[ 8], S11);
      FF (d, a, b, c, x[ 9], S12);
      FF (c, d, a, b, x[10], S13);
      FF (b, c, d, a, x[11], S14);
      FF (a, b, c, d, x[12], S11);
      FF (d, a, b, c, x[13], S12);
      FF (c, d, a, b, x[14], S13);
      FF (b, c, d, a, x[15], S14);
      
      // round 2
      GG (a, b, c, d, x[ 0], S21);
      GG (d, a, b, c, x[ 4], S22);
      GG (c, d, a, b, x[ 8], S23);
      GG (b, c, d, a, x[12], S24);
      GG (a, b, c, d, x[ 1], S21);
      GG (d, a, b, c, x[ 5], S22);
      GG (c, d, a, b, x[ 9], S23);
      GG (b, c, d, a, x[13], S24);
      GG (a, b, c, d, x[ 2], S21);
      GG (d, a, b, c, x[ 6], S22);
      GG (c, d, a, b, x[10], S23);
      GG (b, c, d, a, x[14], S24);
      GG (a, b, c, d, x[ 3], S21);
      GG (d, a, b, c, x[ 7], S22);
      GG (c, d, a, b, x[11], S23);
      GG (b, c, d, a, x[15], S24);
      
      // round 3
      HH (a, b, c, d, x[ 0], S31);
      HH (d, a, b, c, x[ 8], S32);
      HH (c, d, a, b, x[ 4], S33);
      HH (b, c, d, a, x[12], S34);
      HH (a, b, c, d, x[ 2], S31);
      HH (d, a, b, c, x[10], S32);
      HH (c, d, a, b, x[ 6], S33);
      HH (b, c, d, a, x[14], S34);
      HH (a, b, c, d, x[ 1], S31);
      HH (d, a, b, c, x[ 9], S32);
      HH (c, d, a, b, x[ 5], S33);
      HH (b, c, d, a, x[13], S34);
      HH (a, b, c, d, x[ 3], S31);
      HH (d, a, b, c, x[11], S32);
      HH (c, d, a, b, x[ 7], S33);
      HH (b, c, d, a, x[15], S34);
      
      // state update
      d_state[0] += a;
      d_state[1] += b;
      d_state[2] += c;
      d_state[3] += d;
      
      // clear the buffer count to indicate processing
      unlock ();
      return true;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // finish processing by padding the message
  
  void Md4::finish (void) {
    wrlock ();
    try {
      // get message length
      t_octa len = getwcnt ();
      // how much padding do we have to do - the message must be congruent
      // to 56 modulo 64 (aka 448 bits modulo 512)
      long res = len % MD4_BPAD_LENGTH;
      long pad = (res < 56) ? 56 - res : 120 - res;
      // process with the padding
      process (PAD, pad);
      // now add the length - this is in bits !!!
      t_byte blen[8];
      t_octa bits = len << 3;
      leotob (blen, &bits, 1);
      process (blen, 8);
      // update the result array
      leqtob (p_hash, d_state, 4);
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
  
  Object* Md4::mknew (Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();
    // check for 0 argument
    if (argc == 0) return new Md4;
    // check for 1 argument
    if (argc == 1) {
      long rlen = argv->getlong (0);
      return new Md4 (rlen);
    }
    // invalid arguments
    throw Exception ("argument-error", "too many arguments for MD4");
  }
}
