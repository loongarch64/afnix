// ---------------------------------------------------------------------------
// - Signer.cpp                                                              -
// - afnix:sec module - base message signature class implementation          -
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

#include "Ascii.hpp"
#include "Vector.hpp"
#include "Signer.hpp"
#include "Boolean.hpp"
#include "Unicode.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"
#include "InputOutput.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a signer by name

  Signer::Signer (const String& name) {
    d_name = name;
  }

  // create a signer by name and key

  Signer::Signer (const String& name, const Key& key) {
    // initialize signature
    d_name = name;
    // set the key
    setkey (key);
  }

  // return the class name

  String Signer::repr (void) const {
    return "Signer";
  }

  // return the signer name

  String Signer::getname (void) const {
    rdlock ();
    try {
      String result = d_name;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set the signer key

  void Signer::setkey (const Key& key) {
    wrlock ();
    try {
      // check for a valid key
      if (key.issk () == false) {
	throw Exception ("signer-error", "invalid signer key");
      }
      // save the key
      d_skey = key;
      // reset everything
      reset  ();
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the signer key

  Key Signer::getkey (void) const {
    rdlock ();
    try {
      Key result = d_skey;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // derive a signature from an octet string

  Signature Signer::derive (const String& s) {
    long    size = 0;
    t_byte* sbuf = Unicode::stob (size, s);
    wrlock ();
    try {
      reset ();
      process (sbuf, size);
      Signature result = finish ();
      delete [] sbuf;
      unlock ();
      return result;
    } catch (...) {
      delete [] sbuf;
      unlock ();
      throw;
    }
  }

  // compute a signature from a string

  Signature Signer::compute (const String& msg) {
    char* cbuf = Unicode::encode (Encoding::EMOD_UTF8, msg);
    long  size = Ascii::strlen (cbuf);
    wrlock ();
    try {
      reset   ();
      process ((t_byte*) cbuf, size);
      Signature result = finish ();
      delete [] cbuf;
      unlock ();
      return result;
    } catch (...) {
      delete [] cbuf;
      unlock ();
      throw;
    }
  }

  // compute a signature from a buffer

  Signature Signer::compute (Buffer& buf) {
    wrlock ();
    try {
      reset   ();
      process (buf);
      Signature result = finish ();
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // compute a signature from an input stream

  Signature Signer::compute (InputStream& is) {
    wrlock ();
    try {
      reset   ();
      process (is);
      Signature result = finish ();
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // compute the signature of a signable object

  Signature Signer::compute (Signable* sobj) {
    wrlock ();
    try {
      // check for serial object
      if (sobj == nullptr) {
	throw Exception ("signer-error", "nil signable object to sign");
      }
      // serialze in a buffer
      InputOutput io; sobj->wrsign (io);
      // compute the signature
      Signature result = compute (io);
      unlock ();
      return result;      
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  
  // verify a signature from a string

  bool Signer::verify (const Signature& sgn, const String& msg) {
    char* cbuf = Unicode::encode (Encoding::EMOD_UTF8, msg);
    long  size = Ascii::strlen (cbuf);
    wrlock ();
    try {
      reset   ();
      process ((t_byte*) cbuf, size);
      bool result = validate (sgn);
      delete [] cbuf;
      unlock ();
      return result;
    } catch (...) {
      delete [] cbuf;
      unlock ();
      throw;
    }
  }

  // verify a signature from a buffer

  bool Signer::verify (const Signature& sgn, Buffer& buf) {
    wrlock ();
    try {
      reset   ();
      process (buf);
      bool result = validate (sgn);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // verify a signature from an input stream

  bool Signer::verify (const Signature& sgn, InputStream& is) {
    wrlock ();
    try {
      reset   ();
      process (is);
      bool result = validate (sgn);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // verify a signature from a signable object

  bool Signer::verify (const Signature& sgn, const Signable* sobj) {
    wrlock ();
    try {
      // check for serial object
      if (sobj == nullptr) {
	throw Exception ("signer-error", "nil signable object to verify");
      }
      // serialze in a buffer
      InputOutput io; sobj->wrsign (io);
      // verify the signature
      bool result = verify (sgn, io);
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

  // the signer supported quarks
  static const long QUARK_RESET   = zone.intern ("reset");
  static const long QUARK_DERIVE  = zone.intern ("derive");
  static const long QUARK_VERIFY  = zone.intern ("verify");
  static const long QUARK_COMPUTE = zone.intern ("compute");

  // return true if the given quark is defined

  bool Signer::isquark (const long quark, const bool hflg) const {
    rdlock ();
    try {
      if (zone.exists (quark) == true) {
	unlock ();
	return true;
      }
      bool result = hflg ? Nameable::isquark (quark, hflg) : false;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // apply this object with a set of arguments and a quark
  
  Object* Signer::apply (Evaluable* zobj, Nameset* nset, const long quark,
			 Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();
    
    // check for 0 argument
    if (argc == 0) {
      if (quark == QUARK_RESET) {
	reset ();
	return nullptr;
      }
    }
    // check for 1 argument
    if (argc == 1) {
      if (quark == QUARK_DERIVE) {
        String s = argv->getstring (0);
        return new Signature (derive (s));
      }
      if (quark == QUARK_COMPUTE) {
        Object* obj = argv->get (0);
        // check for a literal
        auto lval = dynamic_cast <Literal*> (obj);
        if (lval != nullptr) {
          String msg = lval->tostring ();
	  return new Signature (compute (msg));
        }
        // check for a buffer
        auto bval = dynamic_cast <Buffer*> (obj);
        if (bval != nullptr) {
	  return new Signature (compute (*bval));
	}
        // check for an input stream
        auto is = dynamic_cast <InputStream*> (obj);
        if (is != nullptr) {
	  return new Signature (compute (*is));
	}
        // check for a serial object
        auto sobj = dynamic_cast <Signable*> (obj);
        if (sobj != nullptr) {
	  return new Signature (compute (sobj));
	}
        // invalid object
        throw Exception ("type-error", "invalid object for signature compute",
                         Object::repr (obj));
      }
    }
    // check for 2 arguments
    if (argc == 2) {
      if (quark == QUARK_VERIFY) {
        Object* obj = argv->get (0);
        // check for a signature
        auto sval = dynamic_cast <Signature*> (obj);
        if (sval == nullptr) {
	  throw Exception ("type-error", "invalid object for signature verify",
			   Object::repr (obj));
	}
	// check for a literal
	obj = argv->get (1);
	auto lval = dynamic_cast <Literal*> (obj);
	if (lval != nullptr) {
	  String msg = lval->tostring ();
	  return new Boolean (verify (*sval, msg));
        }
        // check for a buffer
        auto bval = dynamic_cast <Buffer*> (obj);
        if (bval != nullptr) {
	  return new Boolean (verify (*sval, *bval));
	}
        // check for an input stream
        auto is = dynamic_cast <InputStream*> (obj);
        if (is != nullptr) {
	  return new Boolean (verify (*sval, *is));
	}
        // check for a serial object
        auto sobj = dynamic_cast <Signable*> (obj);
        if (sobj != nullptr) {
	  return new Boolean (verify (*sval, sobj));
	}
        // invalid object
        throw Exception ("type-error", "invalid object for signature verify",
                         Object::repr (obj));
      }
    }
    // call the nameable method
    return Nameable::apply (zobj, nset, quark, argv);
  }
}
