// ---------------------------------------------------------------------------
// - Md5.cpp                                                                 -
// - afnix:sec module - MD5 message digest class implementation              -
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

#include "Md5.hpp"
#include "Ascii.hpp"
#include "Vector.hpp"
#include "Cryptics.hxx"
#include "Exception.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - private section                                                       -
  // -------------------------------------------------------------------------

  // MD5 algorithm constants
  static const char* MD5_ALGO_NAME   = "MD-5";
  static const long  MD5_BMSG_LENGTH = 64;
  static const long  MD5_BPAD_LENGTH = 64;
  static const long  MD5_HASH_LENGTH = 16;

  // MD5 message padding
  const t_byte PAD[MD5_BPAD_LENGTH] = {
    0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
  };
  
  // MD5 constants
  const long S11 =  7;
  const long S12 = 12;
  const long S13 = 17;
  const long S14 = 22;
  const long S21 =  5;
  const long S22 =  9;
  const long S23 = 14;
  const long S24 = 20;
  const long S31 =  4;
  const long S32 = 11;
  const long S33 = 16;
  const long S34 = 23;
  const long S41 =  6;
  const long S42 = 10;
  const long S43 = 15;
  const long S44 = 21;

  // MD5 basic functions
  static inline t_quad F (t_quad x, t_quad y, t_quad z) {
    return ((x & y) | ((~x) & z));
  }
  static inline t_quad G (t_quad x, t_quad y, t_quad z) {
    return ((x & z) | (y & (~z)));
  }
  static inline t_quad H (t_quad x, t_quad y, t_quad z) {
    return (x ^ y ^ z);
  }
  static inline t_quad I (t_quad x, t_quad y, t_quad z) {
    return (y ^ (x | (~z)));
  }
  // transformations for rounds 1, 2, 3, 4
  static inline void FF (t_quad& a, t_quad b, t_quad c, t_quad d, t_quad x, 
			 long s, t_quad ac) {
    a += F (b, c, d) + x + ac;
    a  = qrotl (a, s);
    a += b;
  }
  static inline void GG (t_quad& a, t_quad b, t_quad c, t_quad d, t_quad x, 
			 long s, t_quad ac) {
    a += G (b, c, d) + x + ac;
    a  = qrotl (a, s);
    a += b;
  }
  static inline void HH (t_quad& a, t_quad b, t_quad c, t_quad d, t_quad x, 
			 long s, t_quad ac) {
    a += H (b, c, d) + x + ac;
    a  = qrotl (a, s);
    a += b;
  }
  static inline void II (t_quad& a, t_quad b, t_quad c, t_quad d, t_quad x, 
			 long s, t_quad ac) {
    a += I (b, c, d) + x + ac;
    a  = qrotl (a, s);
    a += b;
  }

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a default hasher (MD-5)

  Md5::Md5 (void) : Hasher (MD5_ALGO_NAME, MD5_BMSG_LENGTH, MD5_HASH_LENGTH) {
    reset ();
  }

  // create a hasher with a result length (MD-5)

  Md5::Md5 (const long rlen) : Hasher (MD5_ALGO_NAME, MD5_BMSG_LENGTH, 
				       MD5_HASH_LENGTH, rlen) {
    reset ();
  }

  // return the class name

  String Md5::repr (void) const {
    return "Md5";
  }

  // reset this MD5 object

  void Md5::reset (void) {
    wrlock ();
    Hasher::reset ();
    d_state[0] = 0x67452301;
    d_state[1] = 0xefcdab89;
    d_state[2] = 0x98badcfe;
    d_state[3] = 0x10325476;
    unlock ();
  }

  // update the hasher state with the buffer data
  
  bool Md5::update (void) {
    wrlock ();
    try {
      // make sure the buffer is full
      if (length () != MD5_BMSG_LENGTH) {
	unlock ();
	return false;
      }
      // initialize state values
      t_quad a = d_state[0];
      t_quad b = d_state[1];
      t_quad c = d_state[2];
      t_quad d = d_state[3];
      // decode a block in 16 quads
      t_quad x[16]; lebtoq (x, (t_byte*) p_data, MD5_BMSG_LENGTH);
      // round 1
      FF (a, b, c, d, x[ 0], S11, 0xd76aa478);
      FF (d, a, b, c, x[ 1], S12, 0xe8c7b756);
      FF (c, d, a, b, x[ 2], S13, 0x242070db);
      FF (b, c, d, a, x[ 3], S14, 0xc1bdceee);
      FF (a, b, c, d, x[ 4], S11, 0xf57c0faf);
      FF (d, a, b, c, x[ 5], S12, 0x4787c62a);
      FF (c, d, a, b, x[ 6], S13, 0xa8304613);
      FF (b, c, d, a, x[ 7], S14, 0xfd469501);
      FF (a, b, c, d, x[ 8], S11, 0x698098d8);
      FF (d, a, b, c, x[ 9], S12, 0x8b44f7af);
      FF (c, d, a, b, x[10], S13, 0xffff5bb1);
      FF (b, c, d, a, x[11], S14, 0x895cd7be);
      FF (a, b, c, d, x[12], S11, 0x6b901122);
      FF (d, a, b, c, x[13], S12, 0xfd987193);
      FF (c, d, a, b, x[14], S13, 0xa679438e);
      FF (b, c, d, a, x[15], S14, 0x49b40821);
      // round 2
      GG (a, b, c, d, x[ 1], S21, 0xf61e2562);
      GG (d, a, b, c, x[ 6], S22, 0xc040b340);
      GG (c, d, a, b, x[11], S23, 0x265e5a51);
      GG (b, c, d, a, x[ 0], S24, 0xe9b6c7aa);
      GG (a, b, c, d, x[ 5], S21, 0xd62f105d);
      GG (d, a, b, c, x[10], S22,  0x2441453);
      GG (c, d, a, b, x[15], S23, 0xd8a1e681);
      GG (b, c, d, a, x[ 4], S24, 0xe7d3fbc8);
      GG (a, b, c, d, x[ 9], S21, 0x21e1cde6);
      GG (d, a, b, c, x[14], S22, 0xc33707d6);
      GG (c, d, a, b, x[ 3], S23, 0xf4d50d87);
      GG (b, c, d, a, x[ 8], S24, 0x455a14ed);
      GG (a, b, c, d, x[13], S21, 0xa9e3e905);
      GG (d, a, b, c, x[ 2], S22, 0xfcefa3f8);
      GG (c, d, a, b, x[ 7], S23, 0x676f02d9);
      GG (b, c, d, a, x[12], S24, 0x8d2a4c8a);
      // round 3
      HH (a, b, c, d, x[ 5], S31, 0xfffa3942);
      HH (d, a, b, c, x[ 8], S32, 0x8771f681);
      HH (c, d, a, b, x[11], S33, 0x6d9d6122);
      HH (b, c, d, a, x[14], S34, 0xfde5380c);
      HH (a, b, c, d, x[ 1], S31, 0xa4beea44);
      HH (d, a, b, c, x[ 4], S32, 0x4bdecfa9);
      HH (c, d, a, b, x[ 7], S33, 0xf6bb4b60);
      HH (b, c, d, a, x[10], S34, 0xbebfbc70);
      HH (a, b, c, d, x[13], S31, 0x289b7ec6);
      HH (d, a, b, c, x[ 0], S32, 0xeaa127fa);
      HH (c, d, a, b, x[ 3], S33, 0xd4ef3085);
      HH (b, c, d, a, x[ 6], S34,  0x4881d05);
      HH (a, b, c, d, x[ 9], S31, 0xd9d4d039);
      HH (d, a, b, c, x[12], S32, 0xe6db99e5);
      HH (c, d, a, b, x[15], S33, 0x1fa27cf8);
      HH (b, c, d, a, x[ 2], S34, 0xc4ac5665);
      // round 4
      II (a, b, c, d, x[ 0], S41, 0xf4292244);
      II (d, a, b, c, x[ 7], S42, 0x432aff97);
      II (c, d, a, b, x[14], S43, 0xab9423a7);
      II (b, c, d, a, x[ 5], S44, 0xfc93a039);
      II (a, b, c, d, x[12], S41, 0x655b59c3);
      II (d, a, b, c, x[ 3], S42, 0x8f0ccc92);
      II (c, d, a, b, x[10], S43, 0xffeff47d);
      II (b, c, d, a, x[ 1], S44, 0x85845dd1);
      II (a, b, c, d, x[ 8], S41, 0x6fa87e4f);
      II (d, a, b, c, x[15], S42, 0xfe2ce6e0);
      II (c, d, a, b, x[ 6], S43, 0xa3014314);
      II (b, c, d, a, x[13], S44, 0x4e0811a1);
      II (a, b, c, d, x[ 4], S41, 0xf7537e82);
      II (d, a, b, c, x[11], S42, 0xbd3af235);
      II (c, d, a, b, x[ 2], S43, 0x2ad7d2bb);
      II (b, c, d, a, x[ 9], S44, 0xeb86d391);
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
  
  void Md5::finish (void) {
    wrlock ();
    try {
      // get message length
      t_octa len = getwcnt ();
      // how much padding do we have to do - the message must be congruent
      // to 56 modulo 64 (aka 448 bits modulo 512)
      long res = len % MD5_BPAD_LENGTH;
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
  
  Object* Md5::mknew (Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();
    // check for 0 argument
    if (argc == 0) return new Md5;
    // check for 1 argument
    if (argc == 1) {
      long rlen = argv->getlong (0);
      return new Md5 (rlen);
    }
    // invalid arguments
    throw Exception ("argument-error", "too many arguments for MD5");
  }
}
