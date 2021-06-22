// ---------------------------------------------------------------------------
// - TlsChunk.cpp                                                            -
// - afnix:tls service - tls data chunk class implementation                 -
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
#include "Vector.hpp"
#include "Integer.hpp"
#include "TlsTypes.hxx"
#include "TlsChunk.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create an empty chunk

  TlsChunk::TlsChunk (void) {
    d_clen = 0L;
    p_data = new t_byte[TLS_RLEN_MAX];
    reset ();
  }

  // create a chunk by buffer

  TlsChunk::TlsChunk (const Buffer& buf) {
    d_clen = 0L;
    p_data = new t_byte[TLS_RLEN_MAX];
    reset ();
    add (buf);
  }
  
  // copy construct this chunk

  TlsChunk::TlsChunk (const TlsChunk& that) {
    that.rdlock ();
    try {
      p_data = new t_byte[TLS_RLEN_MAX];
      reset ();
      d_clen = that.d_clen;
      for (long k = 0L; k < d_clen; k++) p_data[k] = that.p_data[k];
      that.unlock ();
    } catch (...) {
      that.unlock ();
      throw;
    }
  }
  
  // destroy this chunk

  TlsChunk::~TlsChunk (void) {
    reset ();
    delete [] p_data; p_data = nullptr;
  }
  
  // assign a chunk to this one

  TlsChunk& TlsChunk::operator = (const TlsChunk& that) {
    // check for self-assignation
    if (this == &that) return *this;
    // lock and assign
    wrlock ();
    that.rdlock ();
    try {
      reset ();
      d_clen = that.d_clen;
      for (long k = 0L; k < d_clen; k++) p_data[k] = that.p_data[k];
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
  
  String TlsChunk::repr (void) const {
    return "TlsChunk";
  }

  // clone this object
  
  Object* TlsChunk::clone (void) const {
    return new TlsChunk (*this);
  }

  // reset this chunk

  void TlsChunk::reset (void) {
    wrlock ();
    try {
      d_clen = 0L;
      for (long k = 0L; k < TLS_RLEN_MAX; k++) p_data[k] = nilc;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the chunk info as a plist

  Plist TlsChunk::getinfo (void) const {
    rdlock ();
    try {
      // create a result plist
      Plist plst;
      // set the chunk info
      plst.add ("TLS-CHK-CLEN", "TLS CHUNK LENGTH", (t_long) length ());
      // here it is
      unlock ();
      return plst;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the chunk length

  long TlsChunk::length (void) const {
    rdlock ();
    try {
      long result = d_clen;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // add a byte to the chunk

  void TlsChunk::add (const t_byte val) {
    wrlock ();
    try {
      // check valid length
      if (d_clen >= TLS_RLEN_MAX) {
	throw Exception ("tls-error", "out-of-band data chunk");
      }
      p_data[d_clen++] = val;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // add a buffer to a chunk

  void TlsChunk::add (const Buffer& buf) {
    wrlock ();
    try {
      // check valid length
      long blen = buf.length ();
      if (d_clen + blen >= TLS_RLEN_MAX) {
	throw Exception ("tls-error", "out-of-band data chunk");
      }
      for (long k = 0; k < blen; k++) p_data[d_clen++] = buf.get(k);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // add a chunk to a chunk
  
  void TlsChunk::add (const TlsChunk& chnk) {
    wrlock ();
    try {
      // check valid length
      long clen = chnk.length ();
      if (d_clen + clen >= TLS_RLEN_MAX) {
	throw Exception ("tls-error", "out-of-band data chunk");
      }
      for (long k = 0; k < clen; k++) p_data[d_clen++] = chnk.p_data[k];
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // set a byte at a certain position

  void TlsChunk::set (const long pos, const t_byte val) {
    wrlock ();
    try {
      // check valid index
      if ((pos < 0L) || (pos >= TLS_RLEN_MAX)) {
	throw Exception ("tls-error", "invalid chunk index in set");
      }
      p_data[pos] = val;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get a byte at a certain position

  t_byte TlsChunk::get (const long pos) const {
    wrlock ();
    try {
      // check valid index
      if ((pos < 0L) || (pos >= TLS_RLEN_MAX)) {
	throw Exception ("tls-error", "invalid chunk index in get");
      }
      t_byte result = p_data[pos];
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
  static const long QUARK_ADD    = zone.intern ("add");
  static const long QUARK_SET    = zone.intern ("set");
  static const long QUARK_GET    = zone.intern ("get");
  static const long QUARK_LENGTH = zone.intern ("length");

  // create a new object in a generic way

  Object* TlsChunk::mknew (Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();

    // check for 0 argument
    if (argc == 0) return new TlsChunk;
    // too many arguments
    throw Exception ("argument-error", 
                     "too many argument with tls chunk constructor");
  }

  // return true if the given quark is defined

  bool TlsChunk::isquark (const long quark, const bool hflg) const {
    rdlock ();
    try {
      if (zone.exists (quark) == true) {
	unlock ();
	return true;
      }
      bool result = hflg ? TlsInfos::isquark (quark, hflg) : false;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // apply this object with a set of arguments and a quark

  Object* TlsChunk::apply (Evaluable* zobj, Nameset* nset, const long quark,
			   Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();
    
    // check for 0 argument
    if (argc == 0) {
      if (quark == QUARK_LENGTH)  return new Integer (length  ());
    }
    // check for 1 argument
    if (argc == 1) {
      if (quark == QUARK_ADD) {
	t_byte val = argv->getbyte (0);
	add (val);
	return nullptr;
      }
      if (quark == QUARK_GET) {
	long pos = argv->getlong (0);
	return new Byte (get (pos));
      }
    }
    // check for 2 arguments
    if (argc == 1) {
      if (quark == QUARK_SET) {
	long   pos = argv->getlong (0);
	t_byte val = argv->getbyte (1);
	set (pos, val);
	return nullptr;
      }
    }
    // call the tls infos method
    return TlsInfos::apply (zobj, nset, quark, argv);
  }
}
