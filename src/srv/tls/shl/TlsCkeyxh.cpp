// ---------------------------------------------------------------------------
// - TlsCkeyxh.cpp                                                           -
// - afnix:tls service - tls client key exchange class implementation        -
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
#include "Crypto.hpp"
#include "TlsTypes.hxx"
#include "TlsUtils.hpp"
#include "TlsCkeyxh.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - private section                                                       -
  // -------------------------------------------------------------------------

  // map a key type to a cipher type
  static Crypto::t_cifr tls_tocifr (const Key& key) {
    switch (key.gettype ()) {
    case Key::CKEY_KRSA:
      return Crypto::CIFR_RSA;
      break;
    default:
      break;
    }
    throw Exception ("tls-error", "cannot map exchange key to cipher");
  }
  
  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a default key exchange

  TlsCkeyxh::TlsCkeyxh (void) {
    d_mlen = 0L;
    p_mbuf = nullptr;
  }

  // create a key exchange by state

  TlsCkeyxh::TlsCkeyxh (TlsState* ssta) {
    // create a premaster secret
    d_mlen = TLS_SIZE_MSX;
    p_mbuf = TlsUtils::random (d_mlen, false);
    // set the state version
    if (ssta != nullptr) {
      // collect version
      p_mbuf[0] = ssta->getrmaj ();
      p_mbuf[1] = ssta->getrmin ();
      // set the premaster key
      ssta->setmkey(getmbuf());
    }
  }

  // create a key exchange by message block

  TlsCkeyxh::TlsCkeyxh (TlsHblock* hblk) {
    d_mlen = 0L;
    p_mbuf = nullptr;
    decode (hblk, nullptr);
  }
  
  // create a key exchange by message block and key

  TlsCkeyxh::TlsCkeyxh (TlsHblock* hblk, TlsState* ssta) {
    d_mlen = 0L;
    p_mbuf = nullptr;
    decode (hblk, ssta);
  }

  // destroy this  block
  
  TlsCkeyxh::~TlsCkeyxh (void) {
    delete [] p_mbuf; p_mbuf = nullptr;
    reset ();
  }

  // return the class name
  
  String TlsCkeyxh::repr (void) const {
    return "TlsCkeyxh";
  }

  // reset the key exchange

  void TlsCkeyxh::reset (void) {
    wrlock ();
    try {
      d_mlen = 0L;
      delete [] p_mbuf; p_mbuf = nullptr;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the premaster buffer

  Buffer TlsCkeyxh::getmbuf (void) const {
    rdlock ();
    try {
      // check for valid buffer
      if (d_mlen == 0L) {
	throw Exception ("tls-error", "invalid null premaster buffer");
      }
      Buffer result;
      if (result.add ((char*) p_mbuf, d_mlen) != d_mlen) {
	throw Exception ("tls-error", "invalid premaster buffer");
      }
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // get the key exchange info as a plist

  Plist TlsCkeyxh::getinfo (void) const {
    rdlock ();
    try {
      // create a result plist
      Plist plst;
      // here it is
      unlock ();
      return plst;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // decode the message block

  void TlsCkeyxh::decode (TlsHblock* hblk, TlsState* ssta) {
    wrlock ();
    try {
      // reset eveything
      reset ();
      if (hblk == nullptr) {
	unlock ();
	return;
      }
      // the pre-master buffer
      Buffer mbuf;
      // get the cipher private key
      Key* prvk = (ssta == nullptr) ? nullptr : ssta->getprvk ();
      if (prvk != nullptr) {
	// create a decoding cipher by key
	Cipher* cifr = Crypto::mkcipher (tls_tocifr (*prvk), *prvk, true);
	if (cifr == nullptr) {
	  throw Exception ("tls-error", "cannot create key exchange cipher");
	}
	// map the handshake to a buffer
	Buffer hbuf = hblk->tobuffer();
	// the first 2 bytes should be the block size
	t_byte hbyt = (t_byte) hbuf.read ();
	t_byte lbyt = (t_byte) hbuf.read ();
	long   hlen = (((t_word) hbyt) << 8) + ((t_word) lbyt);
	// check the current buffer size
	if (hbuf.length () != hlen) {
	  delete cifr;
	  throw Exception ("tls-error", "inconsistent encrypted exchange size");
	}
	long blen = cifr->stream (mbuf, hbuf);
	delete cifr;
	if (blen != mbuf.length ()) {
	  throw Exception ("tls-error",
			   "internal error while decoding client key exchange");
	}
      } else {
	mbuf = hblk->tobuffer ();
      }
      // copy the pre-master secret
      d_mlen = mbuf.length ();
      delete [] p_mbuf; p_mbuf = (d_mlen > 0L) ? new t_byte[d_mlen] : nullptr;
      if (mbuf.copy ((char*) p_mbuf, d_mlen) != d_mlen) {
	throw Exception ("tls-error", "cannot extract pre-master secret");
      }
      mbuf.reset ();
      // check against malicious version
      if (tls_vers_valid (p_mbuf[0], p_mbuf[1]) == false) {
	throw Exception ("tls-error", "malicious version detected");
      }
      // update the state premaster key
      if (ssta != nullptr) ssta->setmkey(getmbuf());
      // done
      unlock ();
    } catch (...) {
      reset ();
      unlock ();
      throw;
    }
  }
  
  // map a client key exchange to a chunk block

  TlsChunk TlsCkeyxh::tochunk (TlsState* sta) const {
    rdlock ();
    try {
      // get the public encoding key
      TlsCerts* cert = (sta == nullptr) ? nullptr : sta->getcert ();
      Key* pubk = (cert == nullptr) ? nullptr : cert->getpubk ();
      // create the pre-master buffer
      Buffer mbuf(d_mlen);
      if (mbuf.add ((char*) p_mbuf, d_mlen) != d_mlen) {
	throw Exception ("tls-error",
			 "cannot create client key exchange buffer");
      }
      TlsChunk result;
      if (pubk != nullptr) {
	// create an encoding cipher by key
	Cipher* cifr = Crypto::mkcipher (tls_tocifr (*pubk), *pubk, false);
	if (cifr == nullptr) {
	  throw Exception ("tls-error", "cannot create key exchange cipher");
	}
	// stream into a buffer
	Buffer cbuf;
	cifr->stream (cbuf, mbuf);
	delete cifr;
	// add buffer size in the chunk
	long clen = cbuf.length ();
	if (clen  > 65536L) {
	  throw Exception ("tls-error", "inconsistent key exchange cipher");
	}
	t_byte hl = ((t_word) clen) >> 8;
	t_byte bl = ((t_word) clen) & 0x00FF;
	result.add (hl); result.add (bl);
	// fill the chunk
	result.add (cbuf);
      } else {
	if (d_mlen > 65536L) {
	  throw Exception ("tls-error", "inconsistent key exchange cipher");
	}
	// get the buffer length
	t_byte hl = ((t_word) d_mlen) >> 8;
	t_byte bl = ((t_word) d_mlen) & 0x00FF;
	result.add (hl); result.add (bl);
	result.add (mbuf);
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

  // create a new object in a generic way

  Object* TlsCkeyxh::mknew (Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();

    // check for 0 argument
    if (argc == 0) return new TlsCkeyxh;
    // check for 1 argument
    if (argc == 1) {
      Object*     obj = argv->get (0);
      TlsHblock* hblk = dynamic_cast<TlsHblock*> (obj);
      if ((hblk == nullptr) && (obj != nullptr)) {
	throw Exception ("type-error", 
			 "invalid object as tls client key exchange block",
			 Object::repr (obj));
      }
      return new TlsCkeyxh (hblk);
    }
    // too many arguments
    throw Exception ("argument-error", 
                     "too many argument with tls client key exchange");
  }
}
