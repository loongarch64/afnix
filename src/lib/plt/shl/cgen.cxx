// ---------------------------------------------------------------------------
// - cgen.cxx                                                                -
// - standard platform library - C generator functions implementation        -
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

#include "cthr.hpp"
#include "csys.hpp"
#include "cclk.hpp"
#include "cgen.hpp"
#include "cgen.hxx"

namespace afnix {

  // -------------------------------------------------------------------------
  // - private section                                                       -
  // -------------------------------------------------------------------------

  // static mutex creation function
  static void* cgen_rmtx_create (void);
  // mutex of generator service
  static void* cgen_rmtx = cgen_rmtx_create ();
  // this function destroy the mutex at exit
  static void cgen_rmtx_destroy (void) {
    c_mtxdestroy (cgen_rmtx);
  }
  // this function initialize a mutex statically and register its
  static void* cgen_rmtx_create (void) {
    void* result = c_mtxcreate ();
    c_atexit (cgen_rmtx_destroy);
    return result;
  }

  // the mersenne-twister [MT19937] pseudo-random number generator
  #if (AFNIX_PLATFORM_BITS == 32)
  // the twister internal constants
  static const long   MT_NN = 624L;
  static const long   MT_NM = MT_NN - 1L;
  static const long   MT_MM = 227L;
  static const t_quad MT_MA = 0x9908B0DFUL;
  static const t_quad MT_UM = 0x80000000UL;
  static const t_quad MT_LM = 0x7FFFFFFFUL;
  static const t_quad MT_MG[2] = {0UL, MT_MA};
  // the maximum values for MT19937
  static const t_real CGEN_PRNG_RMAX = 1.0 / 4294967295.0;
  static const t_real CGEN_PRNG_NMAX = 1.0 / 4294967296.0;
  
  // the twister structure
  struct s_mtprng {
    // the mt states
    t_quad d_state[MT_NN];
    // the mt index
    long   d_index;
    // create a default mt with seed 19650218
    s_mtprng (void) {
      ldseed (19650218UL);
    }
    // create a mt with a seed
    s_mtprng (const t_quad seed) {
      ldseed (seed);
    }
    // load the generator with a seed
    void ldseed (const t_quad seed) {
      // set the states
      d_state[0] = seed;
      for (long k = 1L; k < MT_NN; k++) {
	t_quad  x = d_state[k-1] ^ (d_state[k-1] >> 30);
	d_state[k] = 1812433253UL * x + (t_quad) k;
      }
      // reset the index
      d_index = 0;
    }
    // generate the random state
    void setrnd (void) {
      // do nothing if the index is not null
      if (d_index != 0) return;
      // make a round
      for (long k = 0L; k < MT_MM; k++) {
	t_quad  x = (d_state[k] & MT_UM) | (d_state[k+1] & MT_LM);
	d_state[k] = d_state[k + MT_MM]^(x >> 1)^ MT_MG[(long)(x & 1ULL)];
      }
      for (long k = MT_MM; k < MT_NM; k++) {
	t_quad  x = (d_state[k] & MT_UM) | (d_state[k+1] & MT_LM);
	d_state[k] = d_state[k+MT_MM-MT_NN]^(x >> 1)^MT_MG[(long)(x & 1ULL)];
      }
      t_quad x = (d_state[MT_NM] & MT_UM) | (d_state[0] & MT_LM);
      d_state[MT_NM] = d_state[MT_MM-1] ^ (x >> 1)^MT_MG[(long)(x & 1ULL)];
    }
    // get a random real value
    t_real getreal (const bool iflg) {
      t_quad rval = getquad ();
      t_real result = 0.0;
      if (iflg == true) {
	result = rval * CGEN_PRNG_RMAX;
      } else {
	result = rval * CGEN_PRNG_NMAX;
      }
      return result;
    }
     // get a random byte
    t_byte getbyte (void) {
      return (t_byte) (0xFFU * getreal (true));
    }
    // get a random word
    t_word getword (void) {
      return (t_word) (0xFFFFU * getreal (true));
    }
    // get a random quad value
    t_quad getquad (void) {
      // generate a state if the index is null
      if (d_index == 0) setrnd ();
      // extract the number
      t_quad y = d_state[d_index];
      y ^= (y >> 11);
      y ^= (y << 7)  & 0x9D2C5680UL;
      y ^= (y << 15) & 0xEFC60000UL;
      y ^= (y >> 18);
      // adjust the index
      d_index = (d_index + 1L) % MT_NN;
      // here it is
      return y;
    }
    // get an octa random number
    t_octa getocta (void) {
      return (t_octa) (((t_octa) getquad ()) << 32) | ((t_octa) getquad ());
    }
  };
  #endif

  // the mersenne-twister [MT19937-64] pseudo-random number generator
  #if (AFNIX_PLATFORM_BITS == 64)
  // the twister internal constants
  static const long   MT_NN = 312L;
  static const long   MT_NM = MT_NN - 1L;
  static const long   MT_MM = 156L;
  static const t_octa MT_MA = 0xB5026F5AA96619E9ULL;
  static const t_octa MT_UM = 0xFFFFFFFF80000000ULL;
  static const t_octa MT_LM = 0x000000007FFFFFFFULL;
  static const t_octa MT_MG[2] = {0ULL, MT_MA};
  // the maximum values for MT19937-64
  static const t_real CGEN_PRNG_RMAX = 1.0 / 9007199254740991.0;
  static const t_real CGEN_PRNG_NMAX = 1.0 / 9007199254740992.0;
  
  // the twister structure
  struct s_mtprng {
    // the mt states
    t_octa d_state[MT_NN];
    // the mt index
    long   d_index;
    // create a default mt with seed 19650218
    s_mtprng (void) {
      ldseed (19650218ULL);
    }
    // create a mt with a seed
    s_mtprng (const t_octa seed) {
      ldseed (seed);
    }
    // load the generator with a seed
    void ldseed (const t_octa seed) {
      // set the states
      d_state[0] = seed;
      for (long k = 1L; k < MT_NN; k++) {
	t_octa   x = d_state[k-1] ^ (d_state[k-1] >> 62);
	d_state[k] = 0x5851F42D4C957F2DULL * x + (t_octa) k;
      }
      // reset the index
      d_index = 0;
    }
    // generate the random state
    void setrnd (void) {
      // do nothing if the index is not null
      if (d_index != 0) return;
      // make a round
      for (long k = 0L; k < MT_MM; k++) {
	t_octa x = (d_state[k] & MT_UM) | (d_state[k+1] & MT_LM);
	d_state[k] = d_state[k + MT_MM]^(x >> 1)^ MT_MG[(long)(x & 1ULL)];
      }
      for (long k = MT_MM; k < MT_NM; k++) {
	t_octa   x = (d_state[k] & MT_UM) | (d_state[k+1] & MT_LM);
	d_state[k] = d_state[k+MT_MM-MT_NN]^(x >> 1)^MT_MG[(long)(x & 1ULL)];
      }
      t_octa x = (d_state[MT_NM] & MT_UM) | (d_state[0] & MT_LM);
      d_state[MT_NM] = d_state[MT_MM-1] ^ (x >> 1)^MT_MG[(long)(x & 1ULL)];
    }
    // get a random real value
    t_real getreal (const bool iflg) {
      t_octa rval = getocta ();
      t_real result = 0.0;
      if (iflg == true) {
	result = (rval >> 11) * CGEN_PRNG_RMAX;
      } else {
	result = (rval >> 11) * CGEN_PRNG_NMAX;
      }
      return result;
    }
    // get a random byte
    t_byte getbyte (void) {
      return (t_byte) (0xFFU * getreal (true));
    }
    // get a random word
    t_word getword (void) {
      return (t_word) (0xFFFFU * getreal (true));
    }
    // get a quad random number
    t_quad getquad (void) {
      return (t_quad) (0xFFFFFFFFU * getreal (true));
    }
    // get a random octa value
    t_octa getocta (void) {
      // generate a state if the index is null
      if (d_index == 0) setrnd ();
      // extract the number
      t_octa y = d_state[d_index];
      y ^= (y >> 29) & 0x5555555555555555ULL;
      y ^= (y << 17) & 0x71D67FFFEDA60000ULL;
      y ^= (y << 37) & 0xFFF7EEE000000000ULL;
      y ^= (y >> 43);
      // adjust the index
      d_index = (d_index + 1L) % MT_NN;
      // here it is
      return y;
    }
  };
  #endif
  
  // the random engine seeding flag
  static bool cgen_sflg = false;
  // the mersenne-twister prng
  static s_mtprng cgen_prng;

  // -------------------------------------------------------------------------
  // - public section                                                        -
  // -------------------------------------------------------------------------

  // initialize the random generator

  void c_initrnd (void) {
    // compute a seed
    t_octa seed = (t_octa) (c_time () * c_getpid ());
    // seed the random generator
    c_mtxlock (cgen_rmtx);
    cgen_prng.ldseed (seed);
    cgen_sflg = true;
    c_mtxunlock (cgen_rmtx);
  }

  // return a random byte

  t_byte c_byternd (void) {
    c_mtxlock (cgen_rmtx);
    t_byte result = cgen_prng.getbyte ();
    c_mtxunlock (cgen_rmtx);
    return result;
  }

  // return a random word

  t_word c_wordrnd (void) {
    c_mtxlock (cgen_rmtx);
    t_word result = cgen_prng.getword ();
    c_mtxunlock (cgen_rmtx);
    return result;
  }

  // return a quad random number

  t_quad c_quadrnd (void) {
    c_mtxlock (cgen_rmtx);
    t_quad result = cgen_prng.getquad ();
    c_mtxunlock (cgen_rmtx);
    return result;
  }

  // return an octa random number

  t_octa c_octarnd (void) {
    c_mtxlock (cgen_rmtx);
    t_octa result = cgen_prng.getocta ();
    c_mtxunlock (cgen_rmtx);
    return result;
  }
  
  // return a real random number between 0.0 and 1.0
  
  t_real c_realrnd (const bool iflg) {
    c_mtxlock (cgen_rmtx);
    t_real result = cgen_prng.getreal (iflg);
    c_mtxunlock (cgen_rmtx);
    return result;
  }
  
  // create a new random engine

  void* c_newprng (void) {
    return new s_mtprng;
  }

  // create a new random engine by seed

  void* c_newprng (const long seed) {
    return new s_mtprng(seed);
  }

  // delete a random engine

  void c_delprng (void* pp) {
    auto prng = reinterpret_cast<s_mtprng*>(pp);
    delete prng;
  }
  
  // return a random byte

  t_byte c_byternd (void* pp) {
    auto prng = reinterpret_cast<s_mtprng*>(pp);
    t_byte result = (prng == nullptr) ? c_byternd () : prng->getbyte ();
    return result;
  }

  // return a random word

  t_word c_wordrnd (void* pp) {
    auto prng = reinterpret_cast<s_mtprng*>(pp);
    t_word result = (prng == nullptr) ? c_wordrnd () : prng->getword ();
    return result;
  }

  // return a quad random number

  t_quad c_quadrnd (void* pp) {
    auto prng = reinterpret_cast<s_mtprng*>(pp);
    t_quad result = (prng == nullptr) ? c_quadrnd () : prng->getquad ();
    return result;
  }

  // return an octa random number

  t_octa c_octarnd (void* pp) {
    auto prng = reinterpret_cast<s_mtprng*>(pp);
    t_octa result = (prng == nullptr) ? c_octarnd () : prng->getocta ();
    return result;
  }
  
  // return a real random number between 0.0 and 1.0
  
  t_real c_realrnd (void* pp, const bool iflg) {
    auto prng = reinterpret_cast<s_mtprng*>(pp);
    t_real result = (prng == nullptr) ? c_realrnd (iflg) : prng->getreal (iflg);
    return result;
  }
}
