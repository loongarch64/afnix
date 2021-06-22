// ---------------------------------------------------------------------------
// - Rc5.cpp                                                                 -
// - afnix:sec module - RC5 block cipher class implementation                -
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

#include "Rc5.hpp"
#include "Vector.hpp"
#include "Integer.hpp"
#include "Cryptics.hxx"
#include "QuarkZone.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - private section                                                       -
  // -------------------------------------------------------------------------

  // RC5 block cipher constants
  static const char*  RC5_ALGO_NAME = "RC5";
  static const long   RC5_BLOK_SIZE = 8;

  // RC5 cipher parameters
  static const long   RC5_NRND_DEFV = 12;                          
  static const long   RC5_NRND_MAXV = 255;                          
  static const long   RC5_SKSZ_DEFV = 16;
  static const long   RC5_SKSZ_MAXV = 255;

  static const long   RC5_WORD_SIZE = 2;
  static const t_quad RC5_EKEY_PCST = 0xb7e15163UL;
  static const t_quad RC5_EKEY_QCST = 0x9e3779b9UL;

  // this procedure expands the key into an expanded key
  static t_quad* rc5_key_expand (const Key& key, 
				 const long sksz, const long eksz) {
    // check the key size
    if (key.getsize () != sksz) {
      throw Exception ("key-error", "invalid secret key size");
    }
    // compute the number of key quads
    long qksz = ((sksz % 4) == 0) ? sksz / 4 : (sksz / 4) + 1;
    // initialize the expanded key
    t_quad* ekey = new t_quad[eksz];
    for (long i = 0; i < eksz; i++) ekey[i] = nilq;
    // fill the quad based key
    t_quad qkey[qksz];
    for (long i = sksz - 1; i >= 0; i--) {
      t_quad qval = (t_quad) key.getbyte (i);
      qkey[i/4] = (qkey[i/4] << 8) | qval;
    }
    // initialize the expanded key
    ekey[0] = RC5_EKEY_PCST;
    for (long i = 1; i < eksz; i++) ekey[i] = ekey[i-1] + RC5_EKEY_QCST;
    // mix the expanded key with the secret key
    long   i = 0;    long   j = 0; long kmax = 3 * eksz;
    t_quad a = nilq; t_quad b = nilq;
    for (long k = 0; k < kmax; k++) {
      a = ekey[i] = qrotl (ekey[i] + (a + b), 3);
      b = qkey[j] = qrotl (qkey[j] + (a + b), (long) ((a + b) % 32));
      i = (i + 1) % eksz;
      j = (j + 1) % qksz;
    }
    // here is the expanded key
    return ekey;
  }

  // this procedure bind an input buffer to a round quad
  static inline void rc5_bi_to_rw (t_quad rw[2], const t_byte* bi) {
    // check input buffer
    if (bi == nullptr) return;
    // bind the round quad
    rw[0]  = ((t_quad) bi[0]) << 24;
    rw[0] |= ((t_quad) bi[1]) << 16;
    rw[0] |= ((t_quad) bi[2]) << 8;
    rw[0] |= ((t_quad) bi[3]);
    rw[1]  = ((t_quad) bi[4]) << 24;
    rw[1] |= ((t_quad) bi[5]) << 16;
    rw[1] |= ((t_quad) bi[6]) << 8;
    rw[1] |= ((t_quad) bi[7]);
  }

  // this procedure bind a round quad to an output buffer
  static inline void rc5_rw_to_bo (t_byte* bo, const t_quad rw[2]) {
    // check output buffer
    if (bo == nullptr) return;
    // bind the round quad
    bo[3] = (t_byte) rw[0];
    bo[2] = (t_byte) (rw[0] >> 8);
    bo[1] = (t_byte) (rw[0] >> 16);
    bo[0] = (t_byte) (rw[0] >> 24);
    bo[7] = (t_byte) rw[1];
    bo[6] = (t_byte) (rw[1] >> 8);
    bo[5] = (t_byte) (rw[1] >> 16);
    bo[4] = (t_byte) (rw[1] >> 24);
  }

  // this procedure performs an encryption round
  static void rc5_enc (t_quad rw[2], const t_quad* ekey, const long nrnd) {
    rw[0] = rw[0] + ekey[0];
    rw[1] = rw[1] + ekey[1];
    for (long i = 1; i <= nrnd; i++) {
      rw[0] = qrotl (rw[0] ^ rw[1], (long) (rw[1] % 32)) + ekey[2*i];
      rw[1] = qrotl (rw[1] ^ rw[0], (long) (rw[0] % 32)) + ekey[2*i+1];
    }
  }

  // this procedure performs a decryption round
  static void rc5_dec (t_quad rw[2], const t_quad* ekey, const long nrnd) {
    for (long i = nrnd; i > 0; i--) {
      rw[1] = qrotr (rw[1] - ekey[2*i+1], (long) (rw[0] % 32)) ^ rw[0];
      rw[0] = qrotr (rw[0] - ekey[2*i],   (long) (rw[1] % 32)) ^ rw[1];
    }
    rw[1] = rw[1] - ekey[1];
    rw[0] = rw[0] - ekey[0];
  }

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a cipher by key

  Rc5::Rc5 (const Key& key) : ModeCipher (RC5_ALGO_NAME, RC5_BLOK_SIZE) {
    // initialize the default parameters
    d_nrnd = RC5_NRND_DEFV;
    d_sksz = RC5_SKSZ_DEFV;
    d_eksz = 0;
    p_ekey = nullptr;
    // set the key
    setkey (key);
  }

  // create a cipher by key and flag

  Rc5::Rc5 (const Key& key, 
	    const bool rflg) : ModeCipher (RC5_ALGO_NAME, RC5_BLOK_SIZE) {
    // initialize the default parameters
    d_nrnd = RC5_NRND_DEFV;
    d_sksz = RC5_SKSZ_DEFV;
    d_eksz = 0;
    p_ekey = nullptr;
    // set the key
    setkey (key);
    // set the reverse flag
    setrflg (rflg);
  }

  // destroy this cipher

  Rc5::~Rc5 (void) {
    delete [] p_ekey;
  }
  
  // return the class name

  String Rc5::repr (void) const {
    return "Rc5";
  }

  // reset this cipher
  
  void Rc5::reset (void) {
    wrlock ();
    try {
      // reset the mode cipher
      ModeCipher::reset ();
      // reset the expanded key
      d_nrnd = RC5_NRND_DEFV;
      d_sksz = RC5_SKSZ_DEFV;
      d_eksz = 0;
      delete [] p_ekey; p_ekey = nullptr;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // clear this cipher
  
  void Rc5::clear (void) {
    wrlock ();
    try {
      // cleat the mode cipher
      ModeCipher::clear ();
      // reset the expanded key
      delete [] p_ekey; p_ekey = nullptr;
      // recompute the expanded key size
      d_eksz = 2 * (d_nrnd + 1);
      // expand the key
      p_ekey = rc5_key_expand (d_ckey, d_sksz, d_eksz);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set the cipher key

  void Rc5::setkey (const Key& key) {
    wrlock ();
    try {
      // set the base key
      ModeCipher::setkey (key);
      // clear the cipher
      clear ();
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    } 
  }
  
  // set the number of rounds

  void Rc5::setnrnd (const long nrnd) {
    wrlock ();
    try {
      if ((nrnd <= 0) || (nrnd > RC5_NRND_MAXV)) {
	throw Exception ("cipher-error", "invalid number of rounds");
      }
      // set the number and clear
      d_nrnd = nrnd;
      clear ();
      // unlock and return
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the number of rounds

  long Rc5::getnrnd (void) const {
    rdlock ();
    try {
      long result = d_nrnd;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set the secret key size

  void Rc5::setsksz (const long sksz) {
    wrlock ();
    try {
      if ((sksz <= 0) || (sksz > RC5_SKSZ_MAXV)) {
	throw Exception ("cipher-error", "invalid secret key size");
      }
      // set the size and reset
      d_sksz = sksz;
      clear ();
      // unlock and return
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the secret key size

  long Rc5::getsksz (void) const {
    rdlock ();
    try {
      long result = d_sksz;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // encode a block buffer into another one

  void Rc5::encode (t_byte* bo, const t_byte* bi) {
    wrlock ();
    try {
      // bind the round quad
      t_quad rw[RC5_WORD_SIZE] = {nilq, nilq};
      rc5_bi_to_rw (rw, bi);
      // encode the round quad
      rc5_enc (rw, p_ekey, d_nrnd);
      // bind the output buffer
      rc5_rw_to_bo (bo, rw);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // decode a block buffer into another one

  void Rc5::decode (t_byte* bo, const t_byte* bi) {
    wrlock ();
    try {
      // bind the round quad state
      t_quad rw[RC5_WORD_SIZE] = {nilq, nilq};
      rc5_bi_to_rw (rw, bi);
      // decode the round quad
      rc5_dec (rw, p_ekey, d_nrnd);
      // bind the output buffer
      rc5_rw_to_bo (bo, rw);
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
  static const long QUARK_ZONE_LENGTH = 4;
  static QuarkZone  zone (QUARK_ZONE_LENGTH);

  // the object supported quarks
  static const long QUARK_SETNRND = zone.intern ("set-rounds");
  static const long QUARK_GETNRND = zone.intern ("get-rounds");
  static const long QUARK_SETSKSZ = zone.intern ("set-secret-size");
  static const long QUARK_GETSKSZ = zone.intern ("get-secret-size");

  // create a new object in a generic way
  
  Object* Rc5::mknew (Vector* argv) {
    long argc = (argv == nullptr) ? 0 : argv->length ();
    // check for 1 argument
    if (argc == 1) {
      // check for a key
      Object* obj = argv->get (0);
      Key*    key = dynamic_cast <Key*> (obj);
      if (key != nullptr) return new Rc5 (*key);
      throw Exception ("argument-error", "invalid arguments with RC5");
    }
    // check for 2 arguments
    if (argc == 2) {
      // check for a key
      Object* obj = argv->get (0);
      Key*    key  = dynamic_cast <Key*> (obj);
      if (key == nullptr) {
	throw Exception ("argument-error", "invalid arguments with RC5");
      }
      // get the reverse flag and object
      bool rflg = argv->getbool (1);
      return new Rc5 (*key, rflg);
    }
    throw Exception ("argument-error", "too many arguments with RC5");
  }

  // return true if the given quark is defined

  bool Rc5::isquark (const long quark, const bool hflg) const {
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
  
  Object* Rc5::apply (Evaluable* zobj, Nameset* nset, const long quark,
		      Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();
    
    // check for 0 argument
    if (argc == 0) {
      if (quark == QUARK_GETNRND) return new Integer (getnrnd ());
      if (quark == QUARK_GETSKSZ) return new Integer (getsksz ());
    }
    // check for 1 argument
    if (argc == 1) {
      if (quark == QUARK_SETNRND) {
	long nrnd = argv->getlong (0);
	setnrnd (nrnd);
	return nullptr;
      }
      if (quark == QUARK_SETSKSZ) {
	long sksz = argv->getlong (0);
	setsksz (sksz);
	return nullptr;
      }
    }
    // call the block cipher method
    return ModeCipher::apply (zobj, nset, quark, argv);
  }
}
