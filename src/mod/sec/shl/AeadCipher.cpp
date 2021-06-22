// ---------------------------------------------------------------------------
// - AeadCipher.cpp                                                          -
// - afnix:sec module - aead cipher class implementation                     -
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
#include "Unicode.hpp"
#include "AeadCipher.hpp"
#include "QuarkZone.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create an aead cipher by key

  AeadCipher::AeadCipher (const String& name, 
			  const long    cbsz) : BlockCipher (name, cbsz) {
    d_iv.reset ();
    d_auth.reset ();
    d_atag.reset ();
  }

  // reset the aead cipher

  void AeadCipher::reset (void) {
    wrlock ();
    try {
      // reset the cipher
      BlockCipher::reset ();
      // reset locally
      d_iv.reset ();
      d_auth.reset ();
      d_atag.reset ();
      // done
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // clear this aead cipher

  void AeadCipher::clear (void) {
    wrlock ();
    try {
      // clear the cipher
      BlockCipher::clear ();
      // clear locally
      d_atag.reset ();
      // done
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set the aead cipher key

  void AeadCipher::setkey (const Key& key) {
    wrlock ();
    try {
      // set the base key
      BlockCipher::setkey (key);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // set the initial vector
  
  void AeadCipher::setiv (const String& ivs) {
    // convert the octet string
    long ivsz = 0;
    t_byte* sbuf = Unicode::stob (ivsz, ivs);
    // lock and set
    wrlock ();
    try {
      // check for valid argument
      if (ivsz <= 0L) {
	throw Exception ("cipher-error", "invalid iv string size");
      }
      // set the initial vector
      d_iv.reset ();
      d_iv.add((char*)sbuf, ivsz);
      delete [] sbuf;
      unlock ();
    } catch (...) {
      delete [] sbuf;
      unlock ();
      throw;
    }
  }

  // set the initial vector by buffer
  
  void AeadCipher::setiv (const Buffer& kiv) {
    wrlock ();
    try {
      d_iv = kiv;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // get the initial vector buffer

  String AeadCipher::getiv (void) const {
    rdlock ();
    try {
      String result = d_iv.format();
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set the authentication data by string
  
  void AeadCipher::setauth (const String& ads) {
    // convert the octet string
    long adsz = 0;
    t_byte* sbuf = Unicode::stob (adsz, ads);
    // lock and set
    wrlock ();
    try {
      // check for valid argument
      if (adsz <= 0L) {
	throw Exception ("cipher-error", "invalid authentication string size");
      }
      // set the authentication data
      d_auth.reset ();
      d_auth.add((char*)sbuf, adsz);
      delete [] sbuf;
      unlock ();
      return;
    } catch (...) {
      delete [] sbuf;
      unlock ();
      throw;
    }
  }

  // set the authentication data by buffer
  
  void AeadCipher::setauth (const Buffer& auth) {
    wrlock ();
    try {
      d_auth = auth;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // get the authentication data

  String AeadCipher::getauth (void) const {
    rdlock ();
    try {
      String result = d_auth.format ();
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the authentication block size

  long AeadCipher::getatsz  (void) const {
    rdlock ();
    try {
      long result = getcbsz ();
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // get the authentication tag

  Buffer AeadCipher::getatag (void) const {
    rdlock ();
    try {
      Buffer result = d_atag;
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
  static const long QUARK_ZONE_LENGTH = 4;
  static QuarkZone  zone (QUARK_ZONE_LENGTH);

  // the object supported quarks
  static const long QUARK_ADDIV   = zone.intern ("add-iv");
  static const long QUARK_SETAUTH = zone.intern ("set-authentication-data");
  static const long QUARK_GETAUTH = zone.intern ("get-authentication-data");
  static const long QUARK_GETATAG = zone.intern ("get-authentication-tag");

  // return true if the given quark is defined

  bool AeadCipher::isquark (const long quark, const bool hflg) const {
    rdlock ();
    try {
      if (zone.exists (quark) == true) {
	unlock ();
	return true;
      }
      bool result = hflg ? BlockCipher::isquark (quark, hflg) : false;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // apply this object with a set of arguments and a quark
  
  Object* AeadCipher::apply (Evaluable* zobj, Nameset* nset, const long quark,
			     Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();
    
    // check for 0 argument
    if (argc == 0) {
      if (quark == QUARK_GETATAG) return new Buffer (getatag ());
      if (quark == QUARK_GETAUTH) return new String (getauth ());
    }
    // check for 1 argument
    if (argc == 1) {
      if (quark == QUARK_SETAUTH) {
	String auth = argv->getstring (0);
	setauth (auth);
	return nullptr;
      }
      if (quark == QUARK_ADDIV) {
	String eiv = argv->getstring (0);
	addiv (eiv);
	return nullptr;
      }
    }
    // call the block cipher method
    return BlockCipher::apply (zobj, nset, quark, argv);
  }
}
