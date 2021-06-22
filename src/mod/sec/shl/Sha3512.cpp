// ---------------------------------------------------------------------------
// - Sha3512.cpp                                                             -
// - afnix:sec module - SHA-3-512 class implementation                       -
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
#include "Vector.hpp"
#include "System.hpp"
#include "Keccak.hxx"
#include "Sha3512.hpp"
#include "Exception.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - private section                                                       -
  // -------------------------------------------------------------------------

  // SHA-3-512 algorithm constants
  static const char* SHA3512_ALGO_NAME = "SHA-3-512";
  static const long  SHA3512_BMSG_BITS = 512;
  static const long  SHA3512_BMSG_BSIZ = kcak_sch_bsiz (SHA3512_BMSG_BITS);
  static const long  SHA3512_BMSG_OSIZ = kcak_sch_osiz (SHA3512_BMSG_BITS);
  static const long  SHA3512_HASH_SIZE = SHA3512_BMSG_BITS / 8;

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a default hasher (SHA-3-512)

  Sha3512::Sha3512 (void) : Hasher (SHA3512_ALGO_NAME,
				    SHA3512_BMSG_BSIZ, SHA3512_HASH_SIZE) {
    reset ();
  }

  // create a hasher with a result length (SHA-3-512)

  Sha3512::Sha3512 (const long rlen) : Hasher (SHA3512_ALGO_NAME, 
					       SHA3512_BMSG_BSIZ, 
					       SHA3512_HASH_SIZE, rlen) {
    reset ();
  }
  
  // return the class name

  String Sha3512::repr (void) const {
    return "Sha3512";
  }
  
  // reset this SHA-3-512 object

  void Sha3512::reset (void) {
    wrlock ();
    try {
      Hasher::reset ();
      for (long k = 0L; k < 25L; k++) d_hsts[k] = nilo;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // update the hasher state with the buffer data
  
  bool Sha3512::update (void) {
    wrlock ();
    try {
      // make sure the buffer is full
      if (length () != SHA3512_BMSG_BSIZ) {
	unlock ();
	return false;
      }
      // prepare a message in the buffer
      t_octa M[SHA3512_BMSG_OSIZ];
      if (System::isbe () == true) {
	bebtoo (M, (t_byte*) p_data, SHA3512_BMSG_BSIZ);
      } else {
	lebtoo (M, (t_byte*) p_data, SHA3512_BMSG_BSIZ);
      }
      // initialize the keccak loop
      kcak_ini_loop (d_hsts, M, SHA3512_BMSG_OSIZ);
      // run the keccak loop
      kcak_run_loop (d_hsts);
      unlock ();
      return true;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // finish processing by padding the message
  
  void Sha3512::finish (void) {
    wrlock ();
    try {
      // compute the amount of padding
      long wcnt = getwcnt ();
      if (wcnt >= SHA3512_BMSG_BSIZ) {
	throw Exception ("sha3512-error", "invalid buffer size in finish");
      }
      long plen = SHA3512_BMSG_BSIZ - wcnt;
      // add padding data as needed
      if (plen == 1L) add (0x86U);
      else if (plen == 2L) {
	add (0X06U);
	add (0x80U);
      } else {
	add (0X06U);
	for (long k = 0L; k < plen - 2L; k++) add (0x00U);
	add (0x80U);	
      }
      // now update the state
      if (update () == false) {
	throw Exception ("sha3512-error", "invalid finish state");
      }
      // update the result array
      if (System::isbe () == true) {
	beotob (p_hash, d_hsts, SHA3512_HASH_SIZE / 8);
      } else {
	leotob (p_hash, d_hsts, SHA3512_HASH_SIZE / 8);
      }
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

  Object* Sha3512::mknew (Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();
    // check for 0 argument
    if (argc == 0) return new Sha3512;
    // check for 1 argument
    if (argc == 1) {
      long rlen = argv->getlong (0);
      return new Sha3512 (rlen);
    }
    // invalid arguments
    throw Exception ("argument-error", "too many arguments for SHA-3-512");
  }
}
