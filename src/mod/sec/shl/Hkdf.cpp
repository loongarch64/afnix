// ---------------------------------------------------------------------------
// - Hkdf.cpp                                                                -
// - afnix:sec module - hashed key derivation function class implementation  -
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

#include "Hkdf.hpp"
#include "Vector.hpp"
#include "Evaluable.hpp"
#include "QuarkZone.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a hashed kdf by hashable, name and size

  Hkdf::Hkdf (Hashable* hobj, 
	      const String& name, const long kbsz) : Kdf (name, kbsz) {
    Object::iref (p_hobj = hobj);
    reset ();
  }

  // destroy this buffer
  
  Hkdf::~Hkdf (void) {
    Object::dref (p_hobj);
  }

  // return the class name

  String Hkdf::repr (void) const {
    return "Hkdf";
  }

  // reset this hkdf object

  void Hkdf::reset (void) {
    wrlock ();
    try {
      Kdf::reset ();
      if (p_hobj != nullptr) p_hobj->reset ();
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return the hashable object

  Hashable* Hkdf::gethobj (void) const {
    rdlock ();
    try {
      Hashable* result = p_hobj;
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
  static const long QUARK_ZONE_LENGTH = 1;
  static QuarkZone  zone (QUARK_ZONE_LENGTH);

  // the object supported quarks
  static const long QUARK_GETHOBJ = zone.intern ("get-hashable");

  // return true if the given quark is defined

  bool Hkdf::isquark (const long quark, const bool hflg) const {
    rdlock ();
    if (zone.exists (quark) == true) {
      unlock ();
      return true;
    }
    bool result = hflg ? Kdf::isquark (quark, hflg) : false;
    unlock ();
    return result;
  }
  
  // apply this object with a set of arguments and a quark
  
  Object* Hkdf::apply (Evaluable* zobj, Nameset* nset, const long quark,
		      Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();
    
    // check for 0 argument
    if (argc == 0) {
      if (quark == QUARK_GETHOBJ) {
	rdlock ();
	try {
	  Object* result = gethobj ();
	  zobj->post (result);
	  unlock ();
	  return result;
	} catch (...) {
	  unlock ();
	  throw;
	}
      }
    }

    // call the kdf method
    return Kdf::apply (zobj, nset, quark, argv);
  }
}
