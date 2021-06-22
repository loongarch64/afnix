// ---------------------------------------------------------------------------
// - TlsProto12.cpp                                                          -
// - afnix:tls service - tls protocol 1.2 class implementation               -
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
#include "Sha256.hpp"
#include "Sha384.hpp"
#include "TlsProto.hxx"
#include "TlsUtils.hpp"
#include "Exception.hpp"
#include "TlsProto12.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - private section                                                       -
  // -------------------------------------------------------------------------

  // compute a hash buffer by prf (tls <= 1.2)
  static Buffer prf_1_2 (const long    slen, const t_byte* sbuf,
			 const String& labl, const Buffer& seed,
			 const long rlen, Hasher* hash) {
    // check for valid arguments
    if ((slen < 2L) || (sbuf == nullptr) || (hash == nullptr)) return Buffer();
    try {
      // create the hmac keys and hmac object
      Key ksh (Key::CKEY_KMAC, slen, sbuf); Hmac hsh (ksh, hash);
      // create a buffer that merges the label and seed
      Buffer hbuf; hbuf.add (labl); hbuf.add (seed);
      // get the hash buffers
      Buffer bsh = TlsUtils::phash (&hsh, hbuf, rlen);
      // paranoid check
      if (bsh.length () != rlen) {
	throw Exception ("tls-error", "inconsistent p_hash buffer size");
      }
      return bsh;
    } catch (...) {
      throw;
    }
  }
  
  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a default protocol

  TlsProto12::TlsProto12 (void) {
    p_hash = nullptr;
  }

  // destroy this protocol

  TlsProto12::~TlsProto12 (void) {
    Object::dref (p_hash);
  }
  
  // return the class name
  
  String TlsProto12::repr (void) const {
    return "TlsProto12";
  }

  // compute a hash buffer by prf
  
  Buffer TlsProto12::xprf (const Buffer& sbuf, const String& labl,
			   const Buffer& seed, const long    rlen) const {
    rdlock ();
    t_byte* cbuf = nullptr;
    try {
      // get the buffer length and content
      long clen = sbuf.length ();
      cbuf = (t_byte*) sbuf.tochar ();
      // compute the prf
      Buffer result = prf_1_2 (clen, cbuf, labl, seed, rlen, p_hash);
      delete [] cbuf;
      unlock ();
      return result;
    } catch (...) {
      delete [] cbuf;
      unlock ();
      throw;
    }
  }

  // compute the server verify buffer

  Buffer TlsProto12::sfin (const Buffer& mbuf, const Buffer& vbuf) const {
    rdlock ();
    try {
      // check for valid hasher
      if (p_hash == nullptr) {
	throw Exception ("tls-error", "nil hasher with prf 1.2");
      }
      // create a seed buffer
      Buffer seed;
      Buffer br = vbuf; p_hash->pushb (seed, br);
      // compute the finished signature
      Buffer result = xprf (mbuf, TLS_LABL_SF, seed, TLS_SIZE_FIN);
      // unlock and return
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // compute the client verify buffer

  Buffer TlsProto12::cfin (const Buffer& mbuf, const Buffer& vbuf) const {
    rdlock ();
    try {
      // check for valid hasher
      if (p_hash == nullptr) {
	throw Exception ("tls-error", "nil hasher with prf 1.2");
      }
      // create a seed buffer
      Buffer seed;
      Buffer br = vbuf; p_hash->pushb (seed, br);
      // compute the finished signature
      Buffer result = xprf (mbuf, TLS_LABL_CF, seed, TLS_SIZE_FIN);
      // unlock and return
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set the protocol hasher

  void TlsProto12::sethash (Hasher* hash) {
    wrlock ();
    try {
      Object::iref (hash); Object::dref (p_hash); p_hash = hash;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // -------------------------------------------------------------------------
  // - object section                                                        -
  // -------------------------------------------------------------------------

  // create a new object in a generic way

  Object* TlsProto12::mknew (Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();

    // check for 0 argument
    if (argc == 0) return new TlsProto12;
    // too many arguments
    throw Exception ("argument-error", 
                     "too many argument with tls prtotocol constructor");
  }
}
