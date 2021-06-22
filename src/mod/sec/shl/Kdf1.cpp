// ---------------------------------------------------------------------------
// - Kdf1.cpp                                                                -
// - afnix:sec module - key derivation function (KDF1) class implementation  -
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

#include "Kdf1.hpp"
#include "Vector.hpp"
#include "Exception.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - private section                                                       -
  // -------------------------------------------------------------------------

  // KDF1 algorithm constants
  static const String KDF_ALGO_NAME = "KDF-1";

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a kdf by hashable and size

  Kdf1::Kdf1 (Hashable* hobj,const long kbsz) : Hkdf (hobj,KDF_ALGO_NAME,kbsz) {
    reset ();
  }
  
  // return the class name

  String Kdf1::repr (void) const {
    return "Kdf1";
  }

  // derive the key from an octet string

  void Kdf1::derive (const t_byte* ostr, const long size) {
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
      // key generation loop
      for (long i = 0; i < cmax; i++) {
	// reset the hashable
	p_hobj->reset ();
	// generate counter string
	t_byte bcnt[4];
	bcnt[0] = (t_byte) ((i >> 24) & 0x000000FF);
	bcnt[1] = (t_byte) ((i >> 16) & 0x000000FF);
	bcnt[2] = (t_byte) ((i >> 8)  & 0x000000FF);
	bcnt[3] = (t_byte) (i & 0x000000FF);
	// hash the data
	p_hobj->process (ostr, size);
	p_hobj->process (bcnt, 4);
	p_hobj->finish  ();
	// copy the hashable result
	for (long j = 0; j < rlen; j++) {
	  if (kidx >= d_kbsz) break;
	  p_kbuf[kidx++] = p_hobj->getbyte (j);
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
  
  Object* Kdf1::mknew (Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();
    // check for 2 argument
    if (argc == 2) {
      // get the hashable object
      Object*  obj = argv->get (0);
      Hashable* hobj = dynamic_cast <Hashable*> (obj);
      if (hobj == nullptr) {
	throw Exception ("type-error", "invalid object with KDF1 constructor",
			 Object::repr (obj));
      }
      // get the key size
      long kbsz = argv->getlong (1);
      return new Kdf1 (hobj, kbsz);
    }
    // invalid arguments
    throw Exception ("argument-error", "too many arguments for KDF1");
  }
}
