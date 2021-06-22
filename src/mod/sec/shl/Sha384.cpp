// ---------------------------------------------------------------------------
// - Sha384.cpp                                                              -
// - afnix:sec module - SHA-384 class implementation                         -
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
#include "Sha384.hpp"
#include "Vector.hpp"
#include "Cryptics.hxx"
#include "Exception.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - private section                                                       -
  // -------------------------------------------------------------------------

  // SHA-384 algorithm constants
  static const char* SHA384_ALGO_NAME   = "SHA-384";
  static const long  SHA384_BMSG_LENGTH = 128;
  static const long  SHA384_BPAD_LENGTH = 128;
  static const long  SHA384_HASH_LENGTH = 48;

  // SHA-384 message padding
  const t_byte PAD[SHA384_BPAD_LENGTH] = {
    0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
  };

  // SHA-384 constants
  const t_octa K[80] = {
    0x428A2F98D728AE22ULL, 0x7137449123EF65CDULL, 
    0xB5C0FBCFEC4D3B2FULL, 0xE9B5DBA58189DBBCULL,
    0x3956C25BF348B538ULL, 0x59F111F1B605D019ULL,
    0x923F82A4AF194F9BULL, 0xAB1C5ED5DA6D8118ULL,
    0xD807AA98A3030242ULL, 0x12835B0145706FBEULL,
    0x243185BE4EE4B28CULL, 0x550C7DC3D5FFB4E2ULL,
    0x72BE5D74F27B896FULL, 0x80DEB1FE3B1696B1ULL,
    0x9BDC06A725C71235ULL, 0xC19BF174CF692694ULL,
    0xE49B69C19EF14AD2ULL, 0xEFBE4786384F25E3ULL, 
    0x0FC19DC68B8CD5B5ULL, 0x240CA1CC77AC9C65ULL,
    0x2DE92C6F592B0275ULL, 0x4A7484AA6EA6E483ULL,
    0x5CB0A9DCBD41FBD4ULL, 0x76F988DA831153B5ULL,
    0x983E5152EE66DFABULL, 0xA831C66D2DB43210ULL,
    0xB00327C898FB213FULL, 0xBF597FC7BEEF0EE4ULL,
    0xC6E00BF33DA88FC2ULL, 0xD5A79147930AA725ULL,
    0x06CA6351E003826FULL, 0x142929670A0E6E70ULL,
    0x27B70A8546D22FFCULL, 0x2E1B21385C26C926ULL,
    0x4D2C6DFC5AC42AEDULL, 0x53380D139D95B3DFULL,
    0x650A73548BAF63DEULL, 0x766A0ABB3C77B2A8ULL,
    0x81C2C92E47EDAEE6ULL, 0x92722C851482353BULL,
    0xA2BFE8A14CF10364ULL, 0xA81A664BBC423001ULL,
    0xC24B8B70D0F89791ULL, 0xC76C51A30654BE30ULL,
    0xD192E819D6EF5218ULL, 0xD69906245565A910ULL,
    0xF40E35855771202AULL, 0x106AA07032BBD1B8ULL,
    0x19A4C116B8D2D0C8ULL, 0x1E376C085141AB53ULL,
    0x2748774CDF8EEB99ULL, 0x34B0BCB5E19B48A8ULL,
    0x391C0CB3C5C95A63ULL, 0x4ED8AA4AE3418ACBULL,
    0x5B9CCA4F7763E373ULL, 0x682E6FF3D6B2B8A3ULL,
    0x748F82EE5DEFB2FCULL, 0x78A5636F43172F60ULL, 
    0x84C87814A1F0AB72ULL, 0x8CC702081A6439ECULL,
    0x90BEFFFA23631E28ULL, 0xA4506CEBDE82BDE9ULL, 
    0xBEF9A3F7B2C67915ULL, 0xC67178F2E372532BULL,
    0xCA273ECEEA26619CULL, 0xD186B8C721C0C207ULL, 
    0xEADA7DD6CDE0EB1EULL, 0xF57D4F7FEE6ED178ULL,
    0x06F067AA72176FBAULL, 0x0A637DC5A2C898A6ULL, 
    0x113F9804BEF90DAEULL, 0x1B710B35131C471BULL,
    0x28DB77F523047D84ULL, 0x32CAAB7B40C72493ULL, 
    0x3C9EBE0A15C9BEBCULL, 0x431D67C49C100D4CULL,
    0x4CC5D4BECB3E42B6ULL, 0x597F299CFC657E2AULL, 
    0x5FCB6FAB3AD6FAECULL, 0x6C44198C4A475817ULL
  };

  // SHA-384 initial states
  const t_octa I0 = 0xCBBB9D5DC1059ED8ULL;
  const t_octa I1 = 0x629A292A367CD507ULL;
  const t_octa I2 = 0x9159015A3070DD17ULL;
  const t_octa I3 = 0x152FECD8F70E5939ULL;
  const t_octa I4 = 0x67332667FFC00B31ULL;
  const t_octa I5 = 0x8EB44A8768581511ULL;
  const t_octa I6 = 0xDB0C2E0D64F98FA7ULL;
  const t_octa I7 = 0x47B5481DBEFA4FA4ULL;

  // SHA-384 CH function
  static inline t_octa CH (t_octa x, t_octa y, t_octa z) {
    return (x & y) ^ (~x & z);
  }
  // SHA-384 MJ function
  static inline t_octa MJ (t_octa x, t_octa y, t_octa z) {
    return (x & y) ^ (x & z) ^ (y & z);
  }
  // SHA-384 E0 function
  static inline t_octa E0 (t_octa x) {
    return orotr (x, 28) ^ orotr (x, 34) ^ orotr (x, 39);
  }
  // SHA-384 E1 function
  static inline t_octa E1 (t_octa x) {
    return orotr (x, 14) ^ orotr (x, 18) ^ orotr (x, 41);
  }
  // SHA-384 S0 function
  static inline t_octa S0 (t_octa x) {
    return orotr (x, 1) ^ orotr (x, 8) ^ (x >> 7);
  }
  // SHA-384 S1 function
  static inline t_octa S1 (t_octa x) {
    return orotr (x, 19) ^ orotr (x, 61) ^ (x >> 6);
  }

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a default hasher (SHA-384)

  Sha384::Sha384 (void) : Hasher (SHA384_ALGO_NAME, SHA384_BMSG_LENGTH,
				  SHA384_HASH_LENGTH) {
    reset ();
  }

  // create a hasher with a result length (SHA-384)

  Sha384::Sha384 (const long rlen) : Hasher (SHA384_ALGO_NAME, 
					     SHA384_BMSG_LENGTH, 
					     SHA384_HASH_LENGTH, rlen) {
    reset ();
  }

  // return the class name

  String Sha384::repr (void) const {
    return "Sha384";
  }

  // reset this SHA-384 object

  void Sha384::reset (void) {
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

  // update the SHA-384 state with the buffer data
  
  bool Sha384::update (void) {
    wrlock ();
    try {
      // make sure the buffer is full
      if (length () != SHA384_BMSG_LENGTH) {
	unlock ();
	return false;
      }
      // decode a block in 16 quads
      t_octa M[16]; bebtoo (M, (t_byte*) p_data, SHA384_BMSG_LENGTH);
      // prepare a message schedule
      t_octa W[80];
      for (long i = 0; i < 16; i++) W[i] = M[i];
      for (long i = 16; i < 80; i++) {
	W[i] = S1 (W[i-2]) + W[i-7] + S0 (W[i-15]) + W[i-16];
      }				    
      // initialize state values
      t_octa a = d_state[0];
      t_octa b = d_state[1];
      t_octa c = d_state[2];
      t_octa d = d_state[3];
      t_octa e = d_state[4];
      t_octa f = d_state[5];
      t_octa g = d_state[6];
      t_octa h = d_state[7];
      // compute working values
      for (long i = 0; i < 80; i++) {
	// compute temporary values
	t_octa T1 = h + E1 (e) + CH (e,f,g) + K[i] + W[i];
	t_octa T2 = E0 (a) + MJ (a,b,c);
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
  
  void Sha384::finish (void) {
    wrlock ();
    try {
      // get message length
      t_octa ulen = 0;
      t_octa llen = getwcnt ();
      // how much padding do we have to do - the message must be congruent
      // to 112 modulo 128 (aka 896 bits modulo 1024)
      long res = llen % SHA384_BPAD_LENGTH;
      long pad = (res < 112) ? 112 - res : 240 - res;
      // process with the padding
      process (PAD, pad);
      // now add the length - this is in bits computed from 128 bits!!!
      t_byte blen[8];
      t_octa ubit = (ulen << 3) | (llen >> 61);
      beotob  (blen, &ubit, 1);
      process (blen, 8);
      t_octa lbit = llen << 3;
      beotob  (blen, &lbit, 1);
      process (blen, 8);
      // update the result array
      beotob (p_hash, d_state, 6);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // -------------------------------------------------------------------------
  // - object section                                                         -
  // -------------------------------------------------------------------------

  // create a new object in a generic way

  Object* Sha384::mknew (Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();
    // check for 0 argument
    if (argc == 0) return new Sha384;
    // check for 1 argument
    if (argc == 1) {
      long rlen = argv->getlong (0);
      return new Sha384 (rlen);
    }
    // invalid arguments
    throw Exception ("argument-error", "too many arguments for SHA-384");
  }
}
