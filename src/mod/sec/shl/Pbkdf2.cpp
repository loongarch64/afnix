// ---------------------------------------------------------------------------
// - Pbkdf2.cpp                                                              -
// - afnix:sec module - pkcs key derivation (KDF2) class implementation      -
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

#include "Hmac.hpp"
#include "Kdf2.hpp"
#include "Vector.hpp"
#include "Pbkdf2.hpp"
#include "Crypto.hpp"
#include "Unicode.hpp"
#include "Utility.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - private section                                                       -
  // -------------------------------------------------------------------------

  // the default hasher
  static const String KDF_HASH_NAME = "SHA2-256";

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a default key derivation function

  Pbkdf2::Pbkdf2 (void) {
    d_slen = 0L;
    p_salt = nullptr;
    d_kbsz = 0L;
    d_inum = 1L;
    Object::iref (p_hobj = Crypto::mkhasher (KDF_HASH_NAME));
  }
  
  // create a kdf by key size

  Pbkdf2::Pbkdf2 (const long kbsz) {
    d_slen = 0L;
    p_salt = nullptr;
    d_kbsz = (kbsz <= 0L) ? 0L : kbsz;
    d_inum = 1L;
    Object::iref (p_hobj = Crypto::mkhasher (KDF_HASH_NAME));
  }

  // create a kdf by key size and iteration number

  Pbkdf2::Pbkdf2 (const long kbsz, const long inum) {
    d_slen = 0L;
    p_salt = nullptr;
    d_kbsz = (kbsz <= 0L) ? 0L : kbsz;
    d_inum = (inum <= 0L) ? 1L : inum;
    Object::iref (p_hobj = Crypto::mkhasher (KDF_HASH_NAME));
  }

  // destroy this key derivation function

  Pbkdf2::~Pbkdf2 (void) {
    delete [] p_salt;
    Object::dref (p_hobj);
  }
  
  // return the class name

  String Pbkdf2::repr (void) const {
    return "Pbkdf2";
  }

  // bind a hasher object

  void Pbkdf2::bind (Hasher* hobj) {
    wrlock ();
    try {
      Object::iref (hobj); Object::dref (p_hobj); p_hobj = hobj;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // generate a random salt by size

  void Pbkdf2::newsalt (const long size) {
    wrlock ();
    try {
      // clean old salt
      delete [] p_salt; p_salt = 0L; d_slen = 0L;
      // reset size and allocate
      d_slen = (size <= 0L) ? 0L : size;
      if (d_slen > 0L) {
	p_salt = new t_byte[d_slen];
	for (long k = 0; k < d_slen; k++) p_salt[k] = Utility::byternd ();
      }
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set the salt by string

  void Pbkdf2::setsalt (const String& salt) {
    wrlock ();
    try {
      // clean old salt
      delete [] p_salt; p_salt = 0L; d_slen = 0L;
      // convert the salt
      p_salt = Unicode::stob (d_slen, salt);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // set the salt by octet string and size

  void Pbkdf2::setsalt (const t_byte* ostr, const long size) {
    wrlock ();
    try {
      // clean old salt
      delete [] p_salt; p_salt = 0L; d_slen = 0L;
      // reset size and allocate
      d_slen = (size <= 0L) ? 0L : size;
      if (d_slen > 0L) {
	p_salt = new t_byte[d_slen];
	for (long k = 0; k < d_slen; k++) p_salt[k] = ostr[k];
      }
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the salt as a buffer

  Buffer Pbkdf2::getsalt (void) const {
    rdlock ();
    try {
      Buffer result; result.add (reinterpret_cast<char*>(p_salt), d_slen);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // set the key derivateion size

  void Pbkdf2::setkbsz (const long kbsz) {
    wrlock ();
    try {
      d_kbsz = (kbsz <= 0L) ? 0L : kbsz;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // set the iteration number

  void Pbkdf2::setinum (const long inum) {
    wrlock ();
    try {
      d_kbsz = (inum <= 0L) ? 1L : inum;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // derive a key from a string

  Buffer Pbkdf2::derive (const String& ostr) const {
    rdlock ();
    try {
      // create a hmac key
      Key key (Key::CKEY_KMAC, ostr);
      // create a hmac by key and hash
      Hmac* mac = new Hmac (key, p_hobj);
      // create a key derivation function
      Kdf2 kdf (mac, d_kbsz, d_inum);
      // process with the salt
      kdf.derive (p_salt, d_slen);
      // get the buffer result
      Buffer result = kdf.getkbuf ();
      if (result.length () != d_kbsz) {
	throw Exception ("pbkdf2-error", "inconsistent key derivation size");
      }
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // derive a key from an octet string

  Buffer Pbkdf2::derive (const t_byte* ostr, const long size) const {
    rdlock ();
    try {
      // create a hmac key
      Key (Key::CKEY_KMAC, size, ostr);
      // create a key derivation function
      Kdf2 kdf (p_hobj, d_kbsz, d_inum);
      // process with the salt
      kdf.derive (p_salt, d_slen);
      // get the buffer result
      Buffer result = kdf.getkbuf ();
      if (result.length () != d_kbsz) {
	throw Exception ("pbkdf2-error", "inconsistent key derivation size");
      }
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
  static const long QUARK_ZONE_LENGTH = 7;
  static QuarkZone  zone (QUARK_ZONE_LENGTH);

  // the supported quarks
  static const long QUARK_BIND    = zone.intern ("bind");
  static const long QUARK_DERIVE  = zone.intern ("derive");
  static const long QUARK_NEWSALT = zone.intern ("new-salt");
  static const long QUARK_GETSALT = zone.intern ("get-salt");
  static const long QUARK_SETSALT = zone.intern ("set-salt");
  static const long QUARK_SETKBSZ = zone.intern ("set-key-size");
  static const long QUARK_SETINUM = zone.intern ("set-iteration-number");
  
  // create a new object in a generic way
  
  Object* Pbkdf2::mknew (Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();
    // check for 0 argument
    if (argc == 0) return new Pbkdf2;
    // check for 1 argument
    if (argc == 1) {
      // get the key size
      long kbsz = argv->getlong (0);
      return new Pbkdf2 (kbsz);
    }
    // check for 2 arguments
    if (argc == 2) {
      // get the key size
      long kbsz = argv->getlong (0);
      // get the iteration number
      long inum = argv->getlong (1);
      return new Pbkdf2 (kbsz, inum);
    }
    // invalid arguments
    throw Exception ("argument-error", "too many arguments for PBKDF2");
  }

  // return true if the given quark is defined

  bool Pbkdf2::isquark (const long quark, const bool hflg) const {
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
  
  Object* Pbkdf2::apply (Evaluable* zobj, Nameset* nset, const long quark,
			 Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();

    // check for 0 argument
    if (argc == 0) {
      if (quark == QUARK_GETSALT) return new Buffer (getsalt ());
    } 
    // check for 1 argument
    if (argc == 1) {
      if (quark == QUARK_BIND) {
	Object* obj = argv->get (0);
	auto hobj = dynamic_cast<Hasher*>(obj);
	if (hobj == nullptr) {
	  throw Exception ("type-eroor", "invalid object as hasher in bind",
			   Object::repr(obj));
	}
	bind (hobj);
	return nullptr;
      }
      if (quark == QUARK_NEWSALT) {
	long size= argv->getlong (0);
	newsalt (size);
	return nullptr;
      }
      if (quark == QUARK_SETSALT) {
	String salt = argv->getstring (0);
	setsalt (salt);
	return nullptr;
      }
      if (quark == QUARK_SETKBSZ) {
	long kbsz= argv->getlong (0);
	setkbsz (kbsz);
	return nullptr;
      }
      if (quark == QUARK_SETINUM) {
	long inum= argv->getlong (0);
	setinum (inum);
	return nullptr;
      }
      if (quark == QUARK_DERIVE) {
	String ostr = argv->getstring (0);
	Buffer result = derive (ostr);
	return new Buffer (result);
      }
    }
    // call the object method
    return Object::apply (zobj, nset, quark, argv);
  }  
}
