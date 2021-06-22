// ---------------------------------------------------------------------------
// - Prng.cpp                                                                -
// - afnix:mth module - Pseudo-random number generator implementation        -
// ---------------------------------------------------------------------------
// - This program is free software;  you can redistribute it  and/or  modify -
// - it provided that this copyright notice is kept intact.                  -
// -                                                                         -
// - This program  is  distributed in  the hope  that it will be useful, but -
// - without  any  warranty;  without  even   the   implied    warranty   of -
// - merchantability or prngness for a particular purpose.  In no event shall -
// - the copyright holder be liable for any  direct, indirect, incidental or -
// - special damages arising in any way out of the use of this software.     -
// ---------------------------------------------------------------------------
// - copyright (c) 1999-2021 amaury darsch                                   -
// ---------------------------------------------------------------------------

#include "Prng.hpp"
#include "Real.hpp"
#include "Byte.hpp"
#include "Vector.hpp"
#include "Boolean.hpp"
#include "Integer.hpp"
#include "Evaluable.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"
#include "cgen.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - class section                                                        -
  // -------------------------------------------------------------------------

  // create a default prng

  Prng::Prng (void) {
    p_prng = c_newprng ();
  }
  
  // create a prng by seed

  Prng::Prng (const long seed) {
    p_prng = c_newprng (seed);
  }

  // destroy this prng

  Prng::~Prng (void) {
    c_delprng (p_prng);
  }
  
  // return the class name
  
  String Prng::repr (void) const {
    return "Prng";
  }

  // get a random byte

  t_byte Prng::getbyte (void) {
    wrlock ();
    try {
      t_byte result = c_byternd (p_prng);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // get a random word

  t_word Prng::getword (void) {
    wrlock ();
    try {
      t_word result = c_wordrnd (p_prng);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // get a random quad

  t_quad Prng::getquad (void) {
    wrlock ();
    try {
      t_quad result = c_quadrnd (p_prng);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get a random octa

  t_octa Prng::getocta (void) {
    wrlock ();
    try {
      t_octa result = c_octarnd (p_prng);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get a random real

  t_real Prng::getreal (const bool iflg) {
    wrlock ();
    try {
      t_real result = c_realrnd (p_prng, iflg);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // -------------------------------------------------------------------------
  // - object section                                                        -
  // -------------------------------------------------------------------------

  // the quark zone
  static const long QUARK_ZONE_LENGTH = 3;
  static QuarkZone  zone (QUARK_ZONE_LENGTH);

  // the object supported quarks
  static const long QUARK_GETBYTE = zone.intern ("get-byte");
  static const long QUARK_GETREAL = zone.intern ("get-real");
  static const long QUARK_GETINTG = zone.intern ("get-integer");

  // create a new object in a generic way

  Object* Prng::mknew (Vector* argv) {
    long argc = (argv == nullptr) ? 0 : argv->length ();
    
    // check for 0 argument
    if (argc == 0) return new Prng;
    // check for 1 argument
    if (argc == 1) {
      long seed = argv->getlong (0);
      return new Prng (seed);
    }
    // invalid arguments
    throw Exception ("argument-error", "invalid arguments with prng object");
  }

  // return true if the given quark is defined

  bool Prng::isquark (const long quark, const bool hflg) const {
    rdlock ();
    try {
      if (zone.exists (quark) == true) {
	unlock ();
	return true;
      }
      bool result = hflg ? Object::isquark (quark, hflg) : false;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // apply this object with a set of arguments and a quark
  
  Object* Prng::apply (Evaluable* zobj, Nameset* nset, const long quark,
		      Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();

    // dispatch 0 argument
    if (argc == 0) {
      if (quark == QUARK_GETBYTE) return new Byte (getbyte ());
      if (quark == QUARK_GETREAL) return new Real (getreal (true));
      if (quark == QUARK_GETINTG) return new Integer (getocta ());
    }
    // dispatch 1 argument
    if (argc == 1) {
      if (quark == QUARK_GETREAL) {
	bool iflg = argv->getbool (0);
	return new Real(getreal (iflg));
      }
    }
    // call the object method
    return Object::apply (zobj, nset, quark, argv);
  }
}

