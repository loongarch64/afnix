// ---------------------------------------------------------------------------
// - Crypto.cpp                                                              -
// - afnix:sec module - cryptographic function implementation                -
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

#include "Aes.hpp"
#include "Rsa.hpp"
#include "Des.hpp"
#include "Rc2.hpp"
#include "Rc4.hpp"
#include "Rc5.hpp"
#include "Md2.hpp"
#include "Md4.hpp"
#include "Md5.hpp"
#include "Dsa.hpp"
#include "Sha1.hpp"
#include "Vector.hpp"
#include "Sha224.hpp"
#include "Sha256.hpp"
#include "Sha384.hpp"
#include "Sha512.hpp"
#include "Sha3224.hpp"
#include "Sha3256.hpp"
#include "Sha3384.hpp"
#include "Sha3512.hpp"
#include "Crypto.hpp"
#include "Utility.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"

namespace afnix {
  
  // -------------------------------------------------------------------------
  // - private section                                                       -
  // -------------------------------------------------------------------------

  // the default rsa key size
  static const long CPTO_KRSA_SIZE = 2048L;
  // the default dsa key size
  static const long CPTO_KDSA_SIZE = 1024;
  
  // the default hasher
  static const Crypto::t_hash CPTO_HASH_DEF = Crypto::HASH_SHA256;
  
  // the base crypto object name
  static const char* CPTO_NAME_ASM       = "ASYMMETRIC";
  static const char* CPTO_NAME_SGN       = "SIGNING";
  static const char* CPTO_NAME_AES       = "AES";
  static const char* CPTO_NAME_RSA       = "RSA";
  static const char* CPTO_NAME_DES       = "DES";
  static const char* CPTO_NAME_DSA       = "DSA";
  static const char* CPTO_NAME_RC2       = "RC2";
  static const char* CPTO_NAME_RC4       = "RC4";
  static const char* CPTO_NAME_RC5       = "RC5";
  static const char* CPTO_NAME_MD2       = "MD2";
  static const char* CPTO_NAME_MD4       = "MD4";
  static const char* CPTO_NAME_MD5       = "MD5";
  static const char* CPTO_NAME_SHA1      = "SHA1";
  static const char* CPTO_NAME_SHA224    = "SHA2-224";
  static const char* CPTO_NAME_SHA256    = "SHA2-256";
  static const char* CPTO_NAME_SHA384    = "SHA2-384";
  static const char* CPTO_NAME_SHA512    = "SHA2-512";
  static const char* CPTO_NAME_SHA512224 = "SHA2-512/224";
  static const char* CPTO_NAME_SHA512256 = "SHA2-512/256";
  static const char* CPTO_NAME_SHA3224   = "SHA2-3-224";
  static const char* CPTO_NAME_SHA3256   = "SHA2-3-256";
  static const char* CPTO_NAME_SHA3384   = "SHA2-3-384";
  static const char* CPTO_NAME_SHA3512   = "SHA2-3-512";

  // -------------------------------------------------------------------------
  // - key section                                                        -
  // -------------------------------------------------------------------------

  // create a new key by type

  Key* Crypto::mkkey (const t_xkey type) {
    Key* result = nullptr;
    switch (type) {
    case XKEY_RSA:
    case XKEY_ASM:
      result = new Key(Key::CKEY_KRSA, CPTO_KRSA_SIZE);
      break;
    case XKEY_DSA:
    case XKEY_SGN:
      result = new Key(Key::CKEY_KDSA, CPTO_KDSA_SIZE);
      break;
    default:
      result = nullptr;
      break;
    }
    return result;
  }
  
  // create a new key by name

  Key* Crypto::mkkey (const String& name) {
    if (name == CPTO_NAME_RSA) return new Key(Key::CKEY_KRSA, CPTO_KRSA_SIZE);
    if (name == CPTO_NAME_DSA) return new Key(Key::CKEY_KDSA, CPTO_KDSA_SIZE);
    if (name == CPTO_NAME_ASM) return Crypto::mkkey (XKEY_RSA);
    if (name == CPTO_NAME_SGN) return Crypto::mkkey (XKEY_DSA);
    throw Exception ("crypto-error", "invalid key object name", name);
  }

  // -------------------------------------------------------------------------
  // - hasher section                                                        -
  // -------------------------------------------------------------------------

  // create a default hasher

  Hasher* Crypto::mkhasher (void) {
    return mkhasher (CPTO_HASH_DEF);
  }
  
  // create a new hasher by type

  Hasher* Crypto::mkhasher (const t_hash type) {
    Hasher* result = nullptr;
    switch (type) {
    case HASH_MD2:
      result = new Md2;
      break;
    case HASH_MD4:
      result = new Md4;
      break;
    case HASH_MD5:
      result = new Md5;
      break;
    case HASH_SHA1:
      result = new Sha1;
      break;
    case HASH_SHA224:
      result = new Sha224;
      break;
    case HASH_SHA256:
      result = new Sha256;
      break;
    case HASH_SHA384:
      result = new Sha384;
      break;
    case HASH_SHA512:
      result = new Sha512;
      break;
    case HASH_SHA512224:
      result = new Sha512(224, true);
      break;
    case HASH_SHA512256:
      result = new Sha512(256, true);
      break;
    case HASH_SHA3224:
      result = new Sha3224;
      break;
    case HASH_SHA3256:
      result = new Sha3256;
      break;
    case HASH_SHA3384:
      result = new Sha3384;
      break;
    case HASH_SHA3512:
      result = new Sha3512;
      break;
    default:
      result = nullptr;
      break;
    }
    return result;
  }

  // create a new hasher by name

  Hasher* Crypto::mkhasher (const String& name) {
    if (name == CPTO_NAME_MD2)       return new Md2;
    if (name == CPTO_NAME_MD4)       return new Md4;
    if (name == CPTO_NAME_MD5)       return new Md5;
    if (name == CPTO_NAME_SHA1)      return new Sha1;
    if (name == CPTO_NAME_SHA224)    return new Sha224;
    if (name == CPTO_NAME_SHA256)    return new Sha256;
    if (name == CPTO_NAME_SHA384)    return new Sha384;
    if (name == CPTO_NAME_SHA512)    return new Sha512;
    if (name == CPTO_NAME_SHA512224) return new Sha512 (224, true);
    if (name == CPTO_NAME_SHA512256) return new Sha512 (256, true);
    if (name == CPTO_NAME_SHA3224)   return new Sha3224;
    if (name == CPTO_NAME_SHA3256)   return new Sha3256;
    if (name == CPTO_NAME_SHA3384)   return new Sha3384;
    if (name == CPTO_NAME_SHA3512)   return new Sha3512;
    throw Exception ("crypto-error", "invalid hasher object name", name);
  }

  // create a new hasher by size

  Hasher* Crypto::mkhasher (const long bits) {
    if (bits == 128) return new Md5;
    if (bits == 160) return new Sha1;
    if (bits == 224) return new Sha224;
    if (bits == 256) return new Sha256;
    if (bits == 384) return new Sha384;
    if (bits == 512) return new Sha512;
    throw Exception ("crypto-error", "invalid hasher size", (t_quad) bits);
  }

  // validate a hasher by name
  
  bool Crypto::ishasher (const String& name) {
    if (name == CPTO_NAME_MD2)       return true;
    if (name == CPTO_NAME_MD4)       return true;
    if (name == CPTO_NAME_MD5)       return true;
    if (name == CPTO_NAME_SHA1)      return true;
    if (name == CPTO_NAME_SHA224)    return true;
    if (name == CPTO_NAME_SHA256)    return true;
    if (name == CPTO_NAME_SHA384)    return true;
    if (name == CPTO_NAME_SHA512)    return true;
    if (name == CPTO_NAME_SHA512224) return true;
    if (name == CPTO_NAME_SHA512256) return true;
    if (name == CPTO_NAME_SHA3224)   return true;
    if (name == CPTO_NAME_SHA3256)   return true;
    if (name == CPTO_NAME_SHA3384)   return true;
    if (name == CPTO_NAME_SHA3512)   return true;
    return false;
  }
  
  // -------------------------------------------------------------------------
  // - cipher section                                                        -
  // -------------------------------------------------------------------------

  // create a new block cipher by type and key

  Cipher* Crypto::mkcipher (const t_cifr type, const Key& ckey) {
    if (type == CIFR_NIL) return nullptr;
    if (type == CIFR_AES) return new Aes (ckey);
    if (type == CIFR_RSA) return new Rsa (ckey);
    if (type == CIFR_DES) return new Des (ckey);
    if (type == CIFR_RC2) return new Rc2 (ckey);
    if (type == CIFR_RC4) return new Rc4 (ckey);
    if (type == CIFR_RC5) return new Rc5 (ckey);
    throw Exception ("crypto-error", "invalid cipher object type");
  }
  
  // create a new block cipher by name

  Cipher* Crypto::mkcipher (const String& name, const Key& ckey) { 
    if (name == CPTO_NAME_AES) return new Aes (ckey);
    if (name == CPTO_NAME_RSA) return new Rsa (ckey);
    if (name == CPTO_NAME_DES) return new Des (ckey);
    if (name == CPTO_NAME_RC2) return new Rc2 (ckey);
    if (name == CPTO_NAME_RC4) return new Rc4 (ckey);
    if (name == CPTO_NAME_RC5) return new Rc5 (ckey);
    throw Exception ("crypto-error", "invalid cipher object name", name);
  }
  
  // create a new block cipher by type and key

  Cipher* Crypto::mkcipher (const t_cifr type, const Key& ckey, 
			    const bool rflg) {
    if (type == CIFR_NIL) return nullptr;
    if (type == CIFR_AES) return new Aes (ckey, rflg);
    if (type == CIFR_RSA) return new Rsa (ckey, rflg);
    if (type == CIFR_DES) return new Des (ckey, rflg);
    if (type == CIFR_RC2) return new Rc2 (ckey, rflg);
    if (type == CIFR_RC4) return new Rc4 (ckey, rflg);
    if (type == CIFR_RC5) return new Rc5 (ckey, rflg);
    throw Exception ("crypto-error", "invalid cipher object type");
  }

  // create a new block cipher by name

  Cipher* Crypto::mkcipher (const String& name, const Key& ckey, 
			    const bool rflg) {
    if (name == CPTO_NAME_AES) return new Aes (ckey, rflg);
    if (name == CPTO_NAME_RSA) return new Rsa (ckey, rflg);
    if (name == CPTO_NAME_DES) return new Des (ckey, rflg);
    if (name == CPTO_NAME_RC2) return new Rc2 (ckey, rflg);
    if (name == CPTO_NAME_RC4) return new Rc4 (ckey, rflg);
    if (name == CPTO_NAME_RC5) return new Rc5 (ckey, rflg);
    throw Exception ("crypto-error", "invalid cipher object name", name);
  }

  // validate a cipher by name

  bool Crypto::iscipher (const String& name) {
    if (name == CPTO_NAME_AES) return true;
    if (name == CPTO_NAME_RSA) return true;
    if (name == CPTO_NAME_DES) return true;
    if (name == CPTO_NAME_RC2) return true;
    if (name == CPTO_NAME_RC4) return true;
    if (name == CPTO_NAME_RC5) return true;
    return false;
  }

  // -------------------------------------------------------------------------
  // - signer section                                                        -
  // -------------------------------------------------------------------------

  // create a new signer by type

  Signer* Crypto::mksigner (const t_sign type) {
    Signer* result = nullptr;
    switch (type) {
    case SIGN_DSA:
      result = new Dsa;
      break;
    default:
      result = nullptr;
      break;
    }
    return result;
  }

  // create a signer by key

  Signer* Crypto::mksigner (const Key& skey) {
    Key::t_ckey type = skey.gettype ();
    if (type == Key::CKEY_KNIL) return nullptr;
    if (type == Key::CKEY_KDSA) return new Dsa (skey);
    throw Exception ("crypto-error", "invalid signer key type");
  }
  
  // create a signer by signature

  Signer* Crypto::mksigner (const Signature& sign) {
    return Crypto::mksigner (sign.getpubk ());
  }
  
  // create a new signer by name

  Signer* Crypto::mksigner (const String& name) {
    if (name == CPTO_NAME_DSA) return new Dsa;
    throw Exception ("crypto-error", "invalid signer object name", name);
  }

  // validate a signer by name

  bool Crypto::issigner (const String& name) {
    if (name == CPTO_NAME_DSA) return true;
    return false;
  }
  
  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a default crypto object
  
  Crypto::Crypto (void) {
    Object::iref (p_hash = Crypto::mkhasher ());
  }
  
  // copy constuct a crypto object

  Crypto::Crypto (const Crypto& that) {
    that.rdlock ();
    try {
      Object::iref (p_hash = that.p_hash);
      that.unlock ();
    } catch (...) {
      that.unlock ();
      throw;
    }
  }

  // move constuct a crypto object

  Crypto::Crypto (Crypto&& that) {
    that.wrlock ();
    try {
      p_hash = that.p_hash; that.p_hash = nullptr;
      that.unlock ();
    } catch (...) {
      that.unlock ();
      throw;
    }
  }

  // destroy this crypto object

  Crypto::~Crypto (void) {
    Object::dref (p_hash);
  }
  
  // assign a crypto object to this one

  Crypto& Crypto::operator = (const Crypto& that) {
    // check for self-assignation
    if (this == &that) return *this;
    // lock and assign
    wrlock ();
    that.rdlock ();
    try {
      Object::iref (that.p_hash); Object::dref (p_hash);
      p_hash = that.p_hash;
      unlock ();
      that.unlock ();
      return *this;
    } catch (...) {
      unlock ();
      that.unlock ();
      throw;
    }
  }
  
  // move a crypto object to this one

  Crypto& Crypto::operator = (Crypto&& that) {
    // check for self-assignation
    if (this == &that) return *this;
    // lock and assign
    wrlock ();
    that.wrlock ();
    try {
      p_hash = that.p_hash; that.p_hash = nullptr;
      unlock ();
      that.unlock ();
      return *this;
    } catch (...) {
      unlock ();
      that.unlock ();
      throw;
    }
  }

  // return the class name

  String Crypto::repr (void) const {
    return "Crypto";
  }

  // hash a message into a string

  String Crypto::hash (const String& msg) {
    wrlock ();
    try {
      // check for a null hasher
      if (p_hash == nullptr) Object::iref (p_hash = Crypto::mkhasher ());
      // compute the hash message
      String result = p_hash->compute (msg);
      // here it is
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

  // the hasher supported quarks
  static const long QUARK_HASH = zone.intern ("hash");

  // create a new object in a generic way

  Object* Crypto::mknew (Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();
    // check for 0 argument
    if (argc == 0) return new Crypto;
    // invalid arguments
    throw Exception ("argument-error", "too many arguments for crypto object");
  }
  
  // return true if the given quark is defined
  
  bool Crypto::isquark (const long quark, const bool hflg) const {
    rdlock ();
    try {
      if (zone.exists (quark) == true) {
	unlock ();
	return true;
      }
      // check the object class
      bool result = hflg ? Object::isquark (quark, hflg) : false;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // apply this object with a set of arguments and a quark
  
  Object* Crypto::apply (Evaluable* zobj, Nameset* nset, const long quark,
                         Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();
    
    // check for 1 argument
    if (argc == 1) {
      if (quark == QUARK_HASH) {
        String msg = argv->getstring (0);
        return new String (hash (msg));
      }
    }
    // call the object method
    return Object::apply (zobj, nset, quark, argv);
  }
}
