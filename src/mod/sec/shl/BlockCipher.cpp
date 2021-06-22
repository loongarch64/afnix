// ---------------------------------------------------------------------------
// - BlockCipher.cpp                                                         -
// - afnix:sec module - block cipher class implementation                    -
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

#include "Vector.hpp"
#include "Integer.hpp"
#include "QuarkZone.hpp"
#include "BlockCipher.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a block cipher by key

  BlockCipher::BlockCipher (const String& name,
			    const long cbsz) : Cipher (name) {
    // check the cipher size
    if (cbsz <= 0) {
      throw Exception ("cipher-error", "invalid cipher block size");
    }
    // set the cipher info
    d_cbsz = cbsz;
  }

  // reset the block cipher

  void BlockCipher::reset (void) {
    wrlock ();
    try {
      // reset the cipher
      Cipher::reset ();
      // done
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // clear this block cipher

  void BlockCipher::clear (void) {
    wrlock ();
    try {
      // clear the cipher
      Cipher::clear ();
      // done
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // set the block cipher key

  void BlockCipher::setkey (const Key& key) {
    wrlock ();
    try {
      // check the key for validity
      if (key.isbk () == false) {
	throw Exception ("cipher-error", "invalid key for block cipher");
      } 
      // set the key and return
      Cipher::setkey (key);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return the cipher block size
  
  long BlockCipher::getcbsz (void) const {
    rdlock ();
    try {
      long result = d_cbsz;
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
  static const long QUARK_SETIV   = zone.intern ("set-iv");
  static const long QUARK_GETIV   = zone.intern ("get-iv");
  static const long QUARK_GETCBSZ = zone.intern ("get-block-size");

  // return true if the given quark is defined

  bool BlockCipher::isquark (const long quark, const bool hflg) const {
    rdlock ();
    try {
      if (zone.exists (quark) == true) {
	unlock ();
	return true;
      }
      bool result = hflg ? Cipher::isquark (quark, hflg) : false;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // apply this object with a set of arguments and a quark
  
  Object* BlockCipher::apply (Evaluable* zobj, Nameset* nset, const long quark,
			      Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();
    
    // check for 0 argument
    if (argc == 0) {
      if (quark == QUARK_GETIV)   return new String  (getiv ());
      if (quark == QUARK_GETCBSZ) return new Integer (getcbsz ());
    }
    // check for 1 argument
    if (argc == 1) {
      if (quark == QUARK_SETIV) {
	String iv = argv->getstring (0);
	setiv (iv);
	return nullptr;
      }
    }
    // call the cipher method
    return Cipher::apply (zobj, nset, quark, argv);
  }
}
