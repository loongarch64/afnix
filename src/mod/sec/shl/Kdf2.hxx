// ---------------------------------------------------------------------------
// - Kdf2.hxx                                                                -
// - afnix:sec module - private kdf2 definitions                             -
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

#ifndef AFNIX_KDF2_HXX
#define AFNIX_KDF2_HXX

#ifndef  AFNIX_SHA1_HPP
#include "Sha1.hpp"
#endif

namespace afnix {
  // the kdf2 structure
  struct s_kdf2 {
    // the kdf hashable
    Hashable* p_hobj;
    // the kdf salt
    t_byte* p_salt;
    // the salt size
    long d_ssiz;
    // the number of iterations
    long d_inum;
    // the hash buffer
    t_byte* p_hash;

    // create a default kdf2
    s_kdf2 (void) {
      Object::iref (p_hobj = new Sha1);
      p_salt = nullptr;
      d_ssiz = 0L;
      d_inum = 1L;
      p_hash = new t_byte[p_hobj->getrlen ()];
    }
    // create a kdf2 by hashable and iteration number
    s_kdf2 (Hashable* hobj, const long inum) {
      Object::iref (p_hobj = hobj);
      p_salt = nullptr;
      d_ssiz = 0L;
      d_inum = inum;
      p_hash = nullptr;
      if (p_hobj != nullptr) p_hash = new t_byte[p_hobj->getrlen ()];
    }
    // destroy this kdf2 hashable
    ~s_kdf2(void) {
      Object::dref (p_hobj);
      delete [] p_hash;
    }
    // derive a kdf hash
    void derive (const t_byte* ostr, const long osiz, const long i) {
      // the result length
      long rlen = p_hobj->getrlen ();
      if (rlen == 0L) return;
      t_byte* hash = new t_byte[rlen];
      // iterate with the hashable
      for (long k = 0L; k < d_inum; k++) {
	// reset the hashable
	p_hobj->reset ();
	// hash the data
	if (k == 0L) {
	  // generate counter string
	  t_byte bcnt[4];
	  bcnt[0] = (t_byte) ((i >> 24) & 0x000000FF);
	  bcnt[1] = (t_byte) ((i >> 16) & 0x000000FF);
	  bcnt[2] = (t_byte) ((i >> 8)  & 0x000000FF);
	  bcnt[3] = (t_byte) (i & 0x000000FF);
	  // process hash data
	  p_hobj->process (ostr, osiz);
	  p_hobj->process (p_salt, d_ssiz);
	  p_hobj->process (bcnt, 4);
	  p_hobj->finish  ();
	  // copy the hashable result
	  for (long j = 0; j < rlen; j++) {
	    p_hash[j] = hash[j] = p_hobj->getbyte (j);
	  }
	} else {
	  p_hobj->process (hash, rlen);
	  p_hobj->finish  ();
	  // update the hashable result
	  for (long j = 0; j < rlen; j++) {
	    p_hash[j] ^= hash[j] = p_hobj->getbyte (j);
	  }
	}
      }
      delete [] hash;
    }
  };
}
#endif
