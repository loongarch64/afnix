// ---------------------------------------------------------------------------
// - Dsa.cpp                                                                 -
// - afnix:sec module - dss/dsa signature class implementation               -
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

#include "Dsa.hpp"
#include "Vector.hpp"
#include "Crypto.hpp"
#include "Integer.hpp"
#include "Evaluable.hpp"
#include "QuarkZone.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - private section                                                       -
  // -------------------------------------------------------------------------

  // dsa constants
  static const char* DSA_ALGO_NAME = "DSA";

  // get a hasher object by length
  static Hasher* dsa_get_hash (const Key& key) {
    // get the key q prime value
    Relatif q = key.getrkey (Key::KDSA_QPRM);
    // get the hash length
    long hlen = q.getmsb ();
    // map the hash object
    if (hlen == 160) return Crypto::mkhasher (Crypto::HASH_SHA1);
    if (hlen == 224) return Crypto::mkhasher (Crypto::HASH_SHA224);
    if (hlen == 256) return Crypto::mkhasher (Crypto::HASH_SHA256);
    throw Exception ("dsa-error", "invalid dsa hash length");
  }

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------
  
  // create a default dsa signature

  Dsa::Dsa (void) : Signer (DSA_ALGO_NAME) {
    // initialize
    p_hash = nullptr;
    d_sk   = 0;
    // create the default key
    Key key (Key::CKEY_KDSA);
    // set the key
    setkey (key);
  }

  // create a dsa signature by key

  Dsa::Dsa (const Key& key) : Signer (DSA_ALGO_NAME) {
    // initialize
    p_hash = nullptr;
    d_sk   = 0;
    // set the key
    setkey (key);
  }

  // create a dsa signature by key and secret value

  Dsa::Dsa (const Key& key, const Relatif& sk) : Signer (DSA_ALGO_NAME) {
    // initialize
    p_hash = nullptr;
    d_sk   = sk;
    // set the key
    setkey (key);
  }

  // create a dsa signature by signature

  Dsa::Dsa (const Signature& sign) : Signer (DSA_ALGO_NAME) {
    // initialize
    p_hash = nullptr;
    d_sk   = 0;
    // set the key
    setkey (sign.getpubk ());
  }
  
  // destroy this signature

  Dsa::~Dsa (void) {
    delete p_hash;
  }

  // return the class name

  String Dsa::repr (void) const {
    return "Dsa";
  }

  // reset this dsa signature
  
  void Dsa::reset (void) {
    wrlock ();
    try {
      // reset the hasher
      if (p_hash != nullptr) p_hash->reset ();
      // done
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    } 
  }

  // set the signature key
  
  void Dsa::setkey (const Key& key) {
    wrlock ();
    try {
      // check the key
      if (key.gettype () != Key::CKEY_KDSA) {
	throw Exception ("dsa-error", "invalid key type for dsa signature");
      }
      // set the signature key
      Signer::setkey (key);
      // map the hash object by key
      delete p_hash;
      p_hash = dsa_get_hash (key);
      // reset everything
      reset ();
      // done
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // process a message by data
  
  void Dsa::process (const t_byte* data, const long size) {
    wrlock ();
    try {
      if (p_hash != nullptr) p_hash->process (data, size);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }
 
  // process a message with a buffer
  
  void Dsa::process (Buffer& buf) {
    try {
      if (p_hash != nullptr) p_hash->process (buf);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // process a message with an input stream
  
  void Dsa::process (InputStream& is) {
    try {
      if (p_hash != nullptr) p_hash->process (is);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // finish the signature processing

  Signature Dsa::finish (void) {
    rdlock ();
    try {
      // finish hash processing
      if (p_hash != nullptr) p_hash->finish ();
      // extract hash result
      Relatif z = (p_hash == nullptr) ? 0 : p_hash->gethval ();
      // extract key values
      Relatif p = d_skey.getrkey (Key::KDSA_PPRM);
      Relatif q = d_skey.getrkey (Key::KDSA_QPRM);
      Relatif k = (d_sk > 0) ? d_sk : Relatif::random (q);
      Relatif g = d_skey.getrkey (Key::KDSA_PGEN);
      Relatif x = d_skey.getrkey (Key::KDSA_SKEY);
      // compute signature value
      Relatif r = Relatif::mme (g, k, p) % q;
      Relatif s = (Relatif::mmi (k, q) * (z + x*r)) % q;
      // generate the signature
      Vector ovec; ovec.add (new Relatif(r)); ovec.add (new Relatif(s));
      Signature result (Signature::SIGN_SDSA, ovec);
      // bind the public key to the signature
      result.setpubk (d_skey.topublic ());
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // validate a signature

  bool Dsa::validate (const Signature& sgn) {
    rdlock ();
    try {
      // finish hash processing
      if (p_hash != nullptr) p_hash->finish ();
      // extract hash result
      Relatif z = (p_hash == nullptr) ? 0 : p_hash->gethval ();
      // get the signature components
      Relatif r = sgn.getrcmp (Signature::SDSA_RCMP);
      Relatif s = sgn.getrcmp (Signature::SDSA_SCMP);
      // get the key components
      Relatif p = d_skey.getrkey (Key::KDSA_PPRM);
      Relatif q = d_skey.getrkey (Key::KDSA_QPRM);
      Relatif g = d_skey.getrkey (Key::KDSA_PGEN);
      Relatif y = d_skey.getrkey (Key::KDSA_PKEY);
      // check consistency
      if ((r <= 0) || (r >= q) || (s <= 0) || (s >= q)) {
	unlock ();
	return false;
      }
      // compute verification
      Relatif  w = Relatif::mmi (s, q);
      Relatif u1 = (z * w) % q;
      Relatif u2 = (r * w) % q;
      Relatif  v = ((Relatif::mme (g, u1, p) *
		     Relatif::mme (y, u2, p)) % p) % q;
      // verify
      bool result = (v == r);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // -------------------------------------------------------------------------
  // - object setion                                                         -
  // -------------------------------------------------------------------------

  // create a new object in a generic way
  
  Object* Dsa::mknew (Vector* argv) {
    long argc = (argv == nullptr) ? 0 : argv->length ();
    // check for 0 argument
    if (argc == 0) return new Dsa;
    // check for 1 argument
    if (argc == 1) {
      // check for a key
      Object* obj = argv->get (0);
      Key*    key = dynamic_cast <Key*> (obj);
      if (key != nullptr) return new Dsa (*key);
      throw Exception ("argument-error", 
		       "invalid arguments with dsa", Object::repr (obj));
    }
    // check for 2 arguments
    if (argc == 2) {
      // check for a key
      Object* obj = argv->get (0);
      Key*    key = dynamic_cast <Key*> (obj);
      if (key == nullptr) {
	throw Exception ("argument-error", 
			 "invalid arguments with dsa", Object::repr (obj));
      }
      // check for a relatif
      obj = argv->get (1);
      Relatif* rel = dynamic_cast <Relatif*> (obj);
      if (rel == nullptr) {
	throw Exception ("argument-error", 
			 "invalid arguments with dsa", Object::repr (obj));
      }
      return new Dsa (*key, *rel);
    }
    throw Exception ("argument-error", "too many arguments with dsa");
  }
}
