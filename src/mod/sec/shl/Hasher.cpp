// ---------------------------------------------------------------------------
// - Hasher.cpp                                                              -
// - afnix:sec module - base message hasher class implementation             -
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
#include "Hasher.hpp"
#include "Boolean.hpp"
#include "Integer.hpp"
#include "Cryptics.hxx"
#include "QuarkZone.hpp"
#include "Exception.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a hasher by name and size

  Hasher::Hasher (const String& name, const long size, 
		  const long hlen) : BlockBuffer (size), Hashable (name) {
    // check for size
    if (hlen <= 0) {
      throw Exception ("size-error", "invalid hasher size in constructor");
    }
    // initialize hasher
    d_hlen = hlen;
    d_rlen = d_hlen;
    p_hash = new t_byte[d_hlen];
    reset ();
  }

  // create a hasher by name, size and result length

  Hasher::Hasher (const String& name, const long size, const long hlen,
		  const long rlen) : BlockBuffer (size), Hashable (name) {
    // check for size
    if ((hlen <= 0) || (rlen <= 0) || (rlen > hlen)) {
      throw Exception ("size-error", "invalid hasher size in constructor");
    }
    // initialize hasher
    d_hlen = hlen;
    d_rlen = rlen;
    p_hash = new t_byte[d_hlen];
    reset ();
  }

  // destroy this hasher
  
  Hasher::~Hasher (void) {
    delete [] p_hash;
  }

  // reset this hasher buffer

  void Hasher::reset (void) {
    wrlock ();
    try {
      BlockBuffer::reset ();
      for (long i = 0; i < d_hlen; i++) p_hash[i] = nilc;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return the hash value length

  long Hasher::gethlen (void) const {
    rdlock ();
    try {
      long result = d_hlen;
      unlock ();
      return result;    
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return the hash result length

  long Hasher::getrlen (void) const {
    rdlock ();
    try {
      long result = d_rlen;
      unlock ();
      return result;    
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the hash value by index

  t_byte Hasher::getbyte (const long index) const {
    rdlock ();
    try {
      if (index >= d_rlen) {
	throw Exception ("index-error", "hash index is out of bound");
      }
      t_byte result = p_hash[index];
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // get the hash value as a relatif

  Relatif Hasher::gethval (void) const {
    rdlock ();
    try {
      Relatif result (p_hash, d_hlen);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the hash result as a relatif

  Relatif Hasher::getrval (void) const {
    rdlock ();
    try {
      Relatif result (p_hash, d_rlen);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // format the message hash
  
  String Hasher::format (void) const {
    rdlock ();
    try {
      String result = Ascii::btos (p_hash, d_rlen);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // process a message by data

  void Hasher::process (const t_byte* msg, const long size) {
    wrlock ();
    try {
      long blen = size;
      while (blen != 0) {
	long step = copy ((char*) msg, blen);
	if (full () == true) update ();
	msg  += step;
	blen -= step;
      }
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // process a message with a buffer

  void Hasher::process (Buffer& buf) {
    wrlock ();
    try {
      while (buf.empty () == false) {
	copy (buf);
	if (full () == true) update ();
      }
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // process a message with an input stream

  void Hasher::process (InputStream& is) {
    wrlock ();
    try {
      while (is.valid () == true) {
	copy (is);
	if (full () == true) update ();
      }
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // check if the string is a valid hash value

  bool Hasher::ishash (const String& s) const {
    rdlock ();
    try {
      bool result = ishhs (s, d_rlen * 2);
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
  static const long QUARK_HASHP = zone.intern ("hash-p");

  // return true if the given quark is defined

  bool Hasher::isquark (const long quark, const bool hflg) const {
    rdlock ();
    if (zone.exists (quark) == true) {
      unlock ();
      return true;
    }
    // check the hashable class
    bool result = hflg ? Hashable::isquark (quark, hflg) : false;
    // check the block buffer class
    if (result == false) {
      result = hflg ? BlockBuffer::isquark (quark, hflg) : false;
    }
    unlock ();
    return result;
  }
  
  // apply this object with a set of arguments and a quark
  
  Object* Hasher::apply (Evaluable* zobj, Nameset* nset, const long quark,
			 Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();
    
    // check for 1 argument
    if (argc == 1) {
      if (quark == QUARK_HASHP) {
	String s = argv->getstring (0);
	return new Boolean (ishash (s));
      }
    }
    // check the hashable class
    if (Hashable::isquark (quark, true) == true) {
      return Hashable::apply (zobj, nset, quark, argv);
    }
    // fallback with the block buffer method
    return BlockBuffer::apply (zobj, nset, quark, argv);
  }
}
