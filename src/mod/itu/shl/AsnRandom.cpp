// ---------------------------------------------------------------------------
// - AsnRandom.cpp                                                           -
// - afnix:itu module - asn random object generator implementation           -
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

#include "Cons.hpp"
#include "Vector.hpp"
#include "Bitset.hpp"
#include "AsnBits.hpp"
#include "AsnOctets.hpp"
#include "Exception.hpp"
 
namespace afnix {

  // return an asn random bit string

  Object* itu_asnbrnd (Evaluable* zobj, Nameset* nset, Cons* args) {
    // get the arguments
    Vector* argv = Vector::eval (zobj, nset, args);
    long    argc = (argv == nullptr) ? 0 : argv->length ();
    try {
      // check for 1 argument
      if (argc == 1) {
	long bits = argv->getlong (0);
	delete argv; argv = nullptr;
	return new AsnBits (Bitset::random (bits));
      }
      throw Exception ("argument-error", 
		       "too many argument with get-random-asn-bits");
    } catch (...) {
      delete argv;
      throw;
    }
  }

  // return an asn random octet string

  Object* itu_asnornd (Evaluable* zobj, Nameset* nset, Cons* args) {
    // get the arguments
    Vector* argv = Vector::eval (zobj, nset, args);
    long    argc = (argv == nullptr) ? 0 : argv->length ();
    try {
      // check for 1 argument
      if (argc == 1) {
	// get the buffer size
	long size = argv->getlong (0);
	delete argv; argv = nullptr;
	// create a random buffer
	Buffer* rbuf = Buffer::random (size);
	// create the the asn buffer
	AsnOctets* result = new AsnOctets (*rbuf);
	// clean and return
	delete rbuf;
	return result;	 
      }
      throw Exception ("argument-error", 
		       "too many argument with get-random-asn-octets");
    } catch (...) {
      delete argv;
      throw;
    }
  }
}
