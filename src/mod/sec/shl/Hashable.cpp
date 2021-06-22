// ---------------------------------------------------------------------------
// - Hashable.cpp                                                            -
// - afnix:sec module - hashable base class implementation                   -
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

#include "Byte.hpp"
#include "Ascii.hpp"
#include "Vector.hpp"
#include "Unicode.hpp"
#include "Integer.hpp"
#include "Hashable.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a hashable by name

  Hashable::Hashable (const String& name) {
    d_name = name;
  }
  
  // return the hashable name

  String Hashable::getname (void) const {
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
  
  // derive a message from an octet string

  String Hashable::derive (const String& s) {
    long    size = 0;
    t_byte* sbuf = Unicode::stob (size, s);
    wrlock ();
    try {
      reset ();
      process (sbuf, size);
      finish  ();
      String result = format ();
      reset ();
      delete [] sbuf;
      unlock ();
      return result;
    } catch (...) {
      delete [] sbuf;
      unlock ();
      throw;
    }
  }
  
  // compute a message from a string

  String Hashable::compute (const String& msg) {
    char* cbuf = Unicode::encode (Encoding::EMOD_UTF8, msg);
    long  size = Ascii::strlen (cbuf);
    wrlock ();
    try {
      reset   ();
      process ((t_byte*) cbuf, size);
      finish  ();
      String result = format ();
      delete [] cbuf;
      unlock ();
      return result;
    } catch (...) {
      delete [] cbuf;
      unlock ();
      throw;
    }
  }

  // compute a message from a buffer

  String Hashable::compute (Buffer& buf) {
    wrlock ();
    try {
      reset   ();
      process (buf);
      finish  ();
      String result = format ();
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // compute a message from an input stream

  String Hashable::compute (InputStream& is) {
    wrlock ();
    try {
      reset   ();
      process (is);
      finish  ();
      String result = format ();
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // push the hasher result into a buffer

  long Hashable::pushb (Buffer& buf) {
    rdlock ();
    try {
      long rlen = getrlen ();
      for (long k = 0L; k < rlen; k++) buf.add ((char) getbyte(k));
      unlock ();
      return rlen;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // hash and push a buffer in a buffer

  long Hashable::pushb (Buffer& obuf, Buffer& ibuf)
  {
    wrlock();
    try {
      reset ();
      process (ibuf);
      finish ();
      long result = pushb (obuf);
      unlock();
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
  static const long QUARK_ZONE_LENGTH = 9;
  static QuarkZone  zone (QUARK_ZONE_LENGTH);

  // the supported quarks
  static const long QUARK_RESET   = zone.intern ("reset");
  static const long QUARK_FORMAT  = zone.intern ("format");
  static const long QUARK_DERIVE  = zone.intern ("derive");
  static const long QUARK_COMPUTE = zone.intern ("compute");
  static const long QUARK_GETBYTE = zone.intern ("get-byte");
  static const long QUARK_GETHLEN = zone.intern ("get-hash-length");
  static const long QUARK_GETHVAL = zone.intern ("get-hash-value");
  static const long QUARK_GETRLEN = zone.intern ("get-result-length");
  static const long QUARK_GETRVAL = zone.intern ("get-result-value");

  // return true if the given quark is defined

  bool Hashable::isquark (const long quark, const bool hflg) const {
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
  
  Object* Hashable::apply (Evaluable* zobj, Nameset* nset, const long quark,
			   Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();
    
    // check for 0 argument
    if (argc == 0) {
      if (quark == QUARK_FORMAT)  return new String (format  ());
      if (quark == QUARK_GETHLEN) return new Integer (gethlen ());
      if (quark == QUARK_GETHVAL) return new Relatif (gethval ());
      if (quark == QUARK_GETRLEN) return new Integer (getrlen ());
      if (quark == QUARK_GETRVAL) return new Relatif (getrval ());
      if (quark == QUARK_RESET) {
	reset ();
	return nullptr;
      }
    }
    // check for 1 argument
    if (argc == 1) {
      if (quark == QUARK_GETBYTE) {
	long index = argv->getlong (0);
	return new Byte (getbyte (index));
      }
      if (quark == QUARK_DERIVE) {
	String s = argv->getstring (0);
	return new String (derive (s));
      }
      if (quark == QUARK_COMPUTE) {
	Object* obj = argv->get (0);
	// check for a literal
	Literal* lval = dynamic_cast <Literal*> (obj);
	if (lval != nullptr) {
	  String msg = lval->tostring ();
	  return new String (compute (msg));
	}
	// check for a buffer
	Buffer* bval = dynamic_cast <Buffer*> (obj);
	if (bval != nullptr) return new String (compute (*bval));
	// check for an input stream
	InputStream* is = dynamic_cast <InputStream*> (obj);
	if (is != nullptr) return new String (compute (*is));
	// invalid object
	throw Exception ("type-error", "invalid object for hashable compute",
			 Object::repr (obj));
      }
    }
    // call the nameable method
    return Nameable::apply (zobj, nset, quark, argv);
  }
}
