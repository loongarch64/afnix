// ---------------------------------------------------------------------------
// - Keyring.cpp                                                             -
// - afnix:sec module - keyring class implementation                         -
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
#include "Kdf2.hpp"
#include "Secsid.hxx"
#include "Keyring.hpp"
#include "Boolean.hpp"
#include "Evaluable.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"
#include "InputOutput.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - private section                                                       -
  // -------------------------------------------------------------------------

  // default keyring cipher
  static const Crypto::t_cifr KRG_CIFR_DEF = Crypto::CIFR_AES;
  static const Crypto::t_hash KRG_HASH_DEF = Crypto::HASH_SHA256;
  // default keyring size
  static const long           KRG_CKSZ_DEF = 256L;

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a default keyring

  Keyring::Keyring (void) {
    d_cifr = KRG_CIFR_DEF;
    d_hash = KRG_HASH_DEF;
    d_cksz = KRG_CKSZ_DEF;
    p_keys = nullptr;
    p_lbuf = nullptr;
  }

  // create a keyring by name

  Keyring::Keyring (const String& name) : Taggable (name) {
    d_cifr = KRG_CIFR_DEF;
    d_hash = KRG_HASH_DEF;
    d_cksz = KRG_CKSZ_DEF;
    p_keys = nullptr;
    p_lbuf = nullptr;
  }

  // create a keyring by name and info

  Keyring::Keyring (const String& name,
		    const String& info): Taggable (name, info) {
    d_cifr = KRG_CIFR_DEF;
    d_hash = KRG_HASH_DEF;
    d_cksz = KRG_CKSZ_DEF;
    p_keys = nullptr;
    p_lbuf = nullptr;
  }

  // copy construct this keyring

  Keyring::Keyring (const Keyring& that) {
    that.rdlock ();
    try {
      // copy base taggable
      Taggable::operator = (that);
      // copy locally
      d_cifr = that.d_cifr;
      d_hash = that.d_hash;
      d_cksz = that.d_cksz;
      Object::iref(p_keys = that.p_keys);
      Object::iref(p_lbuf = that.p_lbuf);
      that.unlock ();
    } catch (...) {
      that.unlock ();
      throw;
    }
  }

  Keyring::~Keyring(void) {
    Object::dref (p_keys);
    Object::dref (p_lbuf);
  }
  
  // assign a keyring to this one

  Keyring& Keyring::operator = (const Keyring& that) {
    // check for self assignation
    if (this == &that) return *this;
    // lock and assign
    wrlock ();
    that.rdlock ();
    try {
      // assign base taggable
      Taggable::operator = (that);
      // assign locally
      d_cifr = that.d_cifr;
      d_hash = that.d_hash;
      d_cksz = that.d_cksz;
      Object::iref(that.p_keys); Object::dref(p_keys); p_keys = that.p_keys;
      Object::iref(that.p_lbuf); Object::dref(p_lbuf); p_lbuf = that.p_lbuf;
      // here it is
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

  String Keyring::repr (void) const {
    return "Keyring";
  }

  // return a clone of this object

  Object* Keyring::clone (void) const {
    return new Keyring (*this);
  }

  // return the serial did

  t_word Keyring::getdid (void) const {
    return SRL_DEOD_SEC;
  }

  // return the serial sid

  t_word Keyring::getsid (void) const {
    return SRL_KRNG_SID;
  }
  
  // serialize this keyring

  void Keyring::wrstream (OutputStream& os) const {
    rdlock ();
    try {
      // serialize base object
      Taggable::wrstream (os);
      // serialize locally
      os.write ((char) d_cifr);
      os.write ((char) d_hash);
      Serial::wrlong (d_cksz, os);
      if (p_keys == nullptr) {
	Serial::wrnilid (os);
      } else {
        p_keys->serialize (os);
      }
      if (p_lbuf == nullptr) {
	Serial::wrnilid (os);
      } else {
        p_lbuf->serialize (os);
      }
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // deserialize this key

  void Keyring::rdstream (InputStream& is) {
    wrlock ();
    try {
      // deserialize base object
      Taggable::rdstream (is);
      // deserialize locally
      d_cifr = static_cast<Crypto::t_cifr> (is.read ());
      d_hash = static_cast<Crypto::t_hash> (is.read ());
      d_cksz = Serial::rdlong (is);
      Object::dref (p_keys); Object::dref (p_lbuf);
      p_keys = dynamic_cast <Vector*> (Serial::deserialize (is));
      p_lbuf = dynamic_cast <Buffer*> (Serial::deserialize (is));
      Object::iref (p_keys); Object::iref (p_lbuf);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // create a keyring key by credential

  Key Keyring::tockey (const String& cred) const {
    rdlock ();
    Hasher* hash = nullptr;
    try {
      // check for an aes key
      if (d_cifr == Crypto::CIFR_AES) {
	// get a hasher by type
	hash = Crypto::mkhasher (d_hash);
	if (hash == nullptr) {
	  throw Exception ("keyring-error", "cannot create key hasher");
	}
	// create the key derivation function
	long cksz = d_cksz / 8;
	Kdf2 kdf (hash, cksz);
	// derive a key by credential
	kdf.compute (cred);
	// get the key as a buffer
	Buffer kbuf = kdf.getkbuf ();
	// create a symetric key
	Key ckey (kbuf);
	unlock ();
	return ckey;
      }
      // invalid cipher
      throw Exception ("keyring-error", "invalid cipher in keyring");
    } catch (...) {
      delete hash;
      unlock ();
      throw;
    }
  }

  // check if a keyring is open

  bool Keyring::isopen (void) const {
    rdlock ();
    try {
      // check for open consistency
      if ((p_keys != nullptr) && (p_lbuf == nullptr)) {
	unlock ();
	return true;
      }
      // check for close consistency
      if ((p_keys == nullptr) && (p_lbuf != nullptr)) {
	unlock ();
	return false;
      }
      // check for new consistency
      if ((p_keys == nullptr) && (p_lbuf == nullptr)) {
	unlock ();
	return false;
      }
      // something is screwed here - clean everything for safety
      Object::dref (p_keys); p_keys = nullptr;
      Object::dref (p_lbuf); p_lbuf = nullptr;
      throw Exception ("keyring-error", " inconsistent keyring state");
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // open the keyring by credential

  void Keyring::open (const String& cred) {
    wrlock ();
    try {
      // create a credential key
      Key ckey = tockey (cred);
      // open by key
      open (ckey);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // open the keyring by credential key

  void Keyring::open (const Key& ckey) {
    wrlock ();
    try {
      // check for valid buffer
      if (p_lbuf == nullptr) {
	unlock ();
	return;
      }
      // check for valid key
      if (d_cifr == Crypto::CIFR_AES) {
	if (ckey.gettype () != Key::CKEY_KSYM) {
	  throw Exception ("keyring-error", "invalid credential key type");
	}
	if (ckey.getbits () != d_cksz) {
	  throw Exception ("keyring-error", "invalid credential key size");
	}
	// create an aes cipher
	Aes cifr (ckey, true);
	// unstream the lock buffer
	Buffer ob;
	cifr.stream (ob, *p_lbuf); Object::dref (p_lbuf); p_lbuf = nullptr;
	// deserialize the buffer
	InputOutput io(ob);
	p_keys = dynamic_cast <Vector*> (Serial::deserialize (io));
	Object::iref (p_keys);
	// done
	unlock (); return;
      }
      throw Exception ("keyring-error", "invalid key for open");
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // close the keyring by credential

  void Keyring::close (const String& cred) {
    wrlock ();
    try {
      // create a credential key
      Key ckey = tockey (cred);
      // close by key
      close (ckey);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // close the keyring by credential key

  void Keyring::close (const Key& ckey) {
    wrlock ();
    try {
      // create an input output buffer
      InputOutput io;
      if (p_keys == nullptr) {
	Serial::wrnilid (io);
      } else {
	p_keys->serialize (io);
      }
      Object::dref (p_keys); p_keys = nullptr;
      // check for valid key
      if (d_cifr == Crypto::CIFR_AES) {
	if (ckey.gettype () != Key::CKEY_KSYM) {
	  throw Exception ("keyring-error", "invalid credential key type");
	}
	if (ckey.getbits () != d_cksz) {
	  throw Exception ("keyring-error", "invalid credential key size");
	}
	// create an aes cipher
	Aes cifr (ckey);
	// encrypt in the keyring buffer
	if (p_lbuf == nullptr) Object::iref (p_lbuf = new Buffer);
	cifr.stream(*p_lbuf, io);
	// done
	unlock (); return;
      }
      throw Exception ("keyring-error", "invalid key for close");
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // check if a key exists by name

  bool Keyring::exists (const String& name) const {
    rdlock ();
    try {
      long klen = (p_keys == nullptr) ? 0L : p_keys->length ();
      for (long k = 0; k < klen; k++) {
	auto key = dynamic_cast <Key*> (p_keys->get(k));
	if (key == nullptr) continue;
	if (key->getname () == name) {
	  unlock ();
	  return true;
	}
      }
      unlock ();
      return false;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // add a key to the keyring

  void Keyring::add (const Key& key) {
    wrlock ();
    try {
      if (p_keys == nullptr) Object::iref (p_keys = new Vector);
      p_keys->add (new Key(key));
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get a key by name

  Key Keyring::get (const String& name) const {
    rdlock ();
    try {
      long klen = (p_keys == 0L) ? 0L : p_keys->length ();
      for (long k = 0; k < klen; k++) {
	auto key = dynamic_cast <Key*> (p_keys->get(k));
	if (key == nullptr) continue;
	if (key->getname () == name) {
	  Key result = *key;
	  unlock ();
	  return result;
	}
      }
      throw Exception ("keyring-error" "cannot fin key ", name);
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // get the vector of keys

  Vector* Keyring::tokeys (void) {
    rdlock ();
    try {
      Vector* result = p_keys;
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

  // the object supported quarks
  static const long QUARK_ADD     = zone.intern ("add");
  static const long QUARK_GET     = zone.intern ("get");
  static const long QUARK_OPEN    = zone.intern ("open");
  static const long QUARK_CLOSE   = zone.intern ("close");
  static const long QUARK_TOKEYS  = zone.intern ("to-keys");
  static const long QUARK_EXISTSP = zone.intern ("exists-p");
  static const long QUARK_ISOPENP = zone.intern ("isopen-p");
  
  // create a new object in a generic way
  
  Object* Keyring::mknew (Vector* argv) {
    long argc = (argv == nullptr) ? 0 : argv->length ();
    // check for 0 argument
    if (argc == 0) return new Keyring;
    // check for 1 argument
    if (argc == 1) {
      String name = argv->getstring (0);
      return new Keyring (name);
    }
    // check for 2 arguments
    if (argc == 2) {
      String name = argv->getstring (0);
      String info = argv->getstring (1);
      return new Keyring (name, info);
    }
    // invalid arguments
    throw Exception ("argument-error", "too many arguments with keyring");
  }

  // return true if the given quark is defined

  bool Keyring::isquark (const long quark, const bool hflg) const {
    rdlock ();
    try {
      if (zone.exists (quark) == true) {
	unlock ();
	return true;
      }
      bool result = hflg ? Taggable::isquark (quark, hflg) : false;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // apply this object with a set of arguments and a quark
  
  Object* Keyring::apply (Evaluable* zobj, Nameset* nset, const long quark,
			  Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();
    
    // dispatch 0 argument
    if (argc == 0) {
      if (quark == QUARK_ISOPENP) return new Boolean (isopen ());
      if (quark == QUARK_TOKEYS) {
	rdlock ();
	try {
	  Object* result = tokeys ();
	  zobj->post (result);
	  unlock ();
	  return result;
	} catch (...) {
	  unlock ();
	  throw;
	}
      }
    }
    // dispatch 1 argument
    if (argc == 1) {
      if (quark == QUARK_GET) {
	String name = argv->getstring (0);
	return new Key (get(name));
      }
      if (quark == QUARK_ADD) {
	Object* obj = argv->get (0);
	auto    key = dynamic_cast <Key*> (obj);
	if (key == nullptr) {
	  throw Exception ("type-error", "invalid object with add",
			   Object::repr (obj));
	}
	add (*key);
	return nullptr;
      }
      if (quark == QUARK_EXISTSP) {
	String name = argv->getstring (0);
	return new Boolean (exists(name));
      }
      // open the keyring
      if (quark == QUARK_OPEN) {
	Object* obj = argv->get (0);
	// check for a string
	auto sobj = dynamic_cast <String*> (obj);
	if (sobj != nullptr) {
	  open (*sobj);
	  return nullptr;
	}
	// check for a key
	auto ckey = dynamic_cast <Key*> (obj);
	if (ckey != nullptr) {
	  open (*ckey);
	  return nullptr;
	}
	throw Exception ("type-error", "invalid object with open",
			 Object::repr (obj));
      }
      // close the keyring
      if (quark == QUARK_CLOSE) {
	Object* obj = argv->get (0);
	// check for a string
	auto sobj = dynamic_cast <String*> (obj);
	if (sobj != nullptr) {
	  close (*sobj);
	  return nullptr;
	}
	// check for a key
	auto ckey = dynamic_cast <Key*> (obj);
	if (ckey != nullptr) {
	  close (*ckey);
	  return nullptr;
	}
	throw Exception ("type-error", "invalid object with close",
			 Object::repr (obj));
      }
    }
    // call the taggable method
    return Taggable::apply (zobj, nset, quark, argv);
  }
}
