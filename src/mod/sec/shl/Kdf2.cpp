// ---------------------------------------------------------------------------
// - Kdf2.cpp                                                                -
// - afnix:sec module - key derivation function (KDF2) class implementation  -
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

#include "Kdf2.hpp"
#include "Kdf2.hxx"
#include "Vector.hpp"
#include "Exception.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - private section                                                       -
  // -------------------------------------------------------------------------

  // KDF2 algorithm constants
  static const String KDF_ALGO_NAME = "KDF-2";

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a kdf by hashable and size

  Kdf2::Kdf2 (Hashable*  hobj, const long kbsz) :
    Hkdf (hobj, KDF_ALGO_NAME, kbsz) {
    d_inum = 1L;
    reset ();
  }
  
  // create a kdf by hashable, size and iteration number

  Kdf2::Kdf2 (Hashable* hobj, const long kbsz, const long inum) :
    Hkdf (hobj,KDF_ALGO_NAME,kbsz) {
    d_inum = (inum <= 0L) ? 1L : inum;
    reset ();
  }
  
  // return the class name

  String Kdf2::repr (void) const {
    return "Kdf2";
  }

  // derive the key from an octet string

  void Kdf2::derive (const t_byte* ostr, const long size) {
    wrlock ();
    try {
      // do nothing but reset without hash
      if (p_hobj == nullptr) {
	reset  ();
	unlock ();
	return;
      }
      // get the hashable result length
      long rlen = p_hobj->getrlen ();
      // get the max counter and index
      long cmax = d_kbsz / rlen;
      if ((d_kbsz % rlen) != 0) cmax++;
      long kidx = 0;
      // the kdf2 hashable
      s_kdf2 kdf2 (p_hobj, d_inum);
      // key generation loop
      for (long i = 1; i <= cmax; i++) {
	// derive the kdf2 hash buffer
	kdf2.derive (ostr, size, i);
	// copy the hashable result
	for (long j = 0; j < rlen; j++) {
	  if (kidx >= d_kbsz) break;
	  p_kbuf[kidx++] = kdf2.p_hash[j];
	}
      }
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
  
  Object* Kdf2::mknew (Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();
    // check for 2 argument
    if (argc == 2) {
      // get the hashable object
      Object*  obj = argv->get (0);
      Hashable* hobj = dynamic_cast <Hashable*> (obj);
      if (hobj == nullptr) {
	throw Exception ("type-error", "invalid object with KDF2 constructor",
			 Object::repr (obj));
      }
      // get the key size
      long kbsz = argv->getlong (1);
      return new Kdf2 (hobj, kbsz);
    }
    // invalid arguments
    throw Exception ("argument-error", "too many arguments for KDF2");
  }
}
