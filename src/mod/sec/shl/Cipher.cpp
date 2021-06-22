// ---------------------------------------------------------------------------
// - Cipher.cpp                                                              -
// - afnix:sec module - base cipher class implementation                     -
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

#include "Cipher.hpp"
#include "Vector.hpp"
#include "Boolean.hpp"
#include "Integer.hpp"
#include "QuarkZone.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a cipher by name

  Cipher::Cipher (const String& name) : Codec (name) {
    reset ();
  }

  // create a cipher by name and key

  Cipher::Cipher (const String& name, const Key& key) : Codec (name) {
    d_ckey = key;
  }

  // reset this cipher

  void Cipher::reset (void) {
    wrlock ();
    try {
      // reset base codec
      Codec::reset ();
      // reset locally
      d_ckey.reset ();
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // clear this cipher

  void Cipher::clear (void) {
    wrlock ();
    try {
      // clear base codec
      Codec::clear ();
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // set the cipher key

  void Cipher::setkey (const Key& key) {
    wrlock ();
    try {
      d_ckey = key;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the cipher key

  Key Cipher::getkey (void) const {
    rdlock ();
    try {
      Key result = d_ckey;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // normalize a data size

  t_long Cipher::waist (const t_long size) const {
    rdlock ();
    try {
      t_long result = (size < 0) ? 0 : size;
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
  static const long QUARK_WAIST  = zone.intern ("waist");
  static const long QUARK_SETKEY = zone.intern ("set-key");
  static const long QUARK_GETKEY = zone.intern ("get-key");

  // return true if the given quark is defined

  bool Cipher::isquark (const long quark, const bool hflg) const {
    rdlock ();
    try {
      if (zone.exists (quark) == true) {
	unlock ();
	return true;
      }
      bool result = hflg ? Codec::isquark (quark, hflg) : false;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // apply this object with a set of arguments and a quark
  
  Object* Cipher::apply (Evaluable* zobj, Nameset* nset, const long quark,
			 Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();
    
    // check for 0 argument
    if (argc == 0) {
      if (quark == QUARK_GETKEY)  return new Key (getkey ());
    }
    // check for 1 argument
    if (argc == 1) {
      if (quark == QUARK_SETKEY) {
	Object* obj = argv->get (0);
	Key*    key = dynamic_cast <Key*> (obj);
	if (key != nullptr) {
	  setkey (*key);
	  return nullptr;
	}
      }
      if (quark == QUARK_WAIST) {
	t_long size = argv->getlong (0);
	return new Integer (waist (size));
      }
    }
    // call the codec method
    return Codec::apply (zobj, nset, quark, argv);
  }
}
