// ---------------------------------------------------------------------------
// - Kdf.cpp                                                                 -
// - afnix:sec module - base key derivation function class implementation    -
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

#include "Kdf.hpp"
#include "Byte.hpp"
#include "Ascii.hpp"
#include "Vector.hpp"
#include "Unicode.hpp"
#include "Integer.hpp"
#include "Evaluable.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a kdf object by size

  Kdf::Kdf (const String& name, const long kbsz) {
    if (kbsz <= 0) {
      throw Exception ("size-error", "invalid kdf buffer size");
    }
    d_name = name;
    d_kbsz = kbsz;
    p_kbuf = new t_byte[d_kbsz];
    reset ();
  }

  // destroy this kdf object
  
  Kdf::~Kdf (void) {
    delete [] p_kbuf;    
  }

  // return the class name

  String Kdf::repr (void) const {
    return "Kdf";
  }

  // return the kdf name

  String Kdf::getname (void) const {
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

  // reset this kdf object

  void Kdf::reset (void) {
    wrlock ();
    try {
      for (long i = 0; i < d_kbsz; i++) p_kbuf[i] = nilc;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the key buffer size

  long Kdf::getkbsz (void) const {
    rdlock ();
    try {
      long result = d_kbsz;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the key buffer byte by index

  t_byte Kdf::getbyte (const long index) const {
    rdlock ();
    try {
      if (index >= d_kbsz) {
	throw Exception ("index-error", "key buffer index is out of bound");
      }
      t_byte result = p_kbuf[index];
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the key as a buffer

  Buffer Kdf::getkbuf (void) const {
    rdlock ();
    try {
      Buffer result;
      result.add (reinterpret_cast<char*>(p_kbuf), d_kbsz);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // format the key buffer as a string
  
  String Kdf::format (void) const {
    rdlock ();
    try {
      String result = Ascii::btos (p_kbuf, d_kbsz);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // compute a key from a message

  String Kdf::compute (const String& msg) {
    char* cbuf = Unicode::encode (Encoding::EMOD_UTF8, msg);
    long  size = Ascii::strlen (cbuf);
    // derive the key
    wrlock ();
    try {
      // process the octet string
      derive ((t_byte*) cbuf, size);
      // format result
      String result = format ();
      // clean old buffer
      delete [] cbuf;
      // unlock and return
      unlock ();
      return result;
    } catch (...) {
      delete [] cbuf;
      unlock ();
      throw;
    }
  }

  // compute a key from a string

  String Kdf::derive (const String& s) {
    // extract the octed string
    long size = 0;
    t_byte* sbuf = Unicode::stob (size, s);
    // derive the key
    wrlock ();
    try {
      // process the octet string
      derive (sbuf, size);
      // format result
      String result = format ();
      // clean old buffer
      delete [] sbuf;
      // unlock and return
      unlock ();
      return result;
    } catch (...) {
      delete [] sbuf;
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

  // the hasher supported quarks
  static const long QUARK_RESET   = zone.intern ("reset");
  static const long QUARK_FORMAT  = zone.intern ("format");
  static const long QUARK_DERIVE  = zone.intern ("derive");
  static const long QUARK_COMPUTE = zone.intern ("compute");
  static const long QUARK_GETKBSZ = zone.intern ("get-size");
  static const long QUARK_GETBYTE = zone.intern ("get-byte");
  static const long QUARK_GETKBUF = zone.intern ("get-key-buffer");

  // return true if the given quark is defined

  bool Kdf::isquark (const long quark, const bool hflg) const {
    rdlock ();
    if (zone.exists (quark) == true) {
      unlock ();
      return true;
    }
    bool result = hflg ? Nameable::isquark (quark, hflg) : false;
    unlock ();
    return result;
  }
  
  // apply this object with a set of arguments and a quark
  
  Object* Kdf::apply (Evaluable* zobj, Nameset* nset, const long quark,
		      Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();
    
    // check for 0 argument
    if (argc == 0) {
      if (quark == QUARK_FORMAT)  return new String  (format  ());
      if (quark == QUARK_GETKBSZ) return new Integer (getkbsz ());
      if (quark == QUARK_GETKBUF) return new Buffer  (getkbuf ());
      if (quark == QUARK_RESET) {
	reset ();
	return nullptr;
      }
    }
    // check for 1 argument
    if (argc == 1) {
      if (quark == QUARK_DERIVE) {
	String s = argv->getstring (0);
	return new String (derive (s));
      }
      if (quark == QUARK_COMPUTE) {
	String msg = argv->getstring (0);
	return new String (compute (msg));
      }
      if (quark == QUARK_GETBYTE) {
	long index = argv->getlong (0);
	return new Byte (getbyte (index));
      }
    }
    // call the nameable method
    return Nameable::apply (zobj, nset, quark, argv);
  }
}
