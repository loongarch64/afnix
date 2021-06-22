// ---------------------------------------------------------------------------
// - TlsCerts.cpp                                                            -
// - afnix:tls service - tls certificate list class implementation           -
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

#include "X509.hpp"
#include "Pkcs1.hpp"
#include "Vector.hpp"
#include "Strvec.hpp"
#include "Integer.hpp"
#include "TlsCerts.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"
#include "InputMapped.hpp"

namespace afnix {
  
  // -------------------------------------------------------------------------
  // - private section                                                       -
  // -------------------------------------------------------------------------

  // this procedure create a chunk from a buffer
  static TlsChunk tls_buf_chk (const Buffer& buf) {
    TlsChunk result;
    // add the buffer length
    long blen = buf.length ();
    result.add ((t_byte) ((blen & 0x00FF0000) >> 16));
    result.add ((t_byte) ((blen & 0x0000FF00) >> 8));
    result.add ((t_byte)  (blen & 0x000000FF));
    // add the buffer
    for (long k = 0L; k < blen; k++) result.add (buf.get (k));
    return result;
  }
  
  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create an empty certificate list 

  TlsCerts::TlsCerts (void) {
    reset ();
  }

  // create a certificate list by names

  TlsCerts::TlsCerts (const String& cert) {
    reset ();
    addcert (cert);
  }

  // create a certificate list by names and key

  TlsCerts::TlsCerts (const String& cert, const String& prvk) {
    reset ();
    addcert (cert);
    setprvk (prvk);
  }

  // create a certificate list by handshake block

  TlsCerts::TlsCerts (TlsHblock* hblk) {
    decode (hblk);
  }
  
  // return the class name
  
  String TlsCerts::repr (void) const {
    return "TlsCerts";
  }

  // reset this class

  void TlsCerts::reset (void) {
    wrlock ();
    try {
      d_cert = "";
      d_prvk = "";
      d_vbuf.reset ();
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the certificate list info as a plist

  Plist TlsCerts::getinfo (void) const {
    rdlock ();
    try {
      // create a result plist
      Plist plst;
      plst.add ("TLS-HSK-CERT", "TLS SERVER CERTIFICATE LIST", d_cert);
      // here it is
      unlock ();
      return plst;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // get the number of certificates

  long TlsCerts::length (void) const {
    rdlock ();
    try {
      long result = d_vbuf.length ();
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // add certificates

  void TlsCerts::addcert (const String& cert) {
    wrlock ();
    try {
      // split the list name
      Strvec clst = Strvec::split (cert, ":");
      long   clen = clst.length ();
      // check for null or multiple add
      if (clen == 0) {
	unlock ();
	return;
      }
      if (clen > 1L) {
	for (long k = 0L; k < clen; k++) addcert (clst.get (k));
	unlock ();
	return;
      }
      // single add starts here
      Buffer* buf = Pki::tobuffer (cert, Pem::PEMC_CERT);
      if (buf != nullptr) {
	d_vbuf.add (buf);
	if (d_cert.isnil () == true) {
	  d_cert = cert;
	} else {
	  d_cert = d_cert + ':' + cert;
	}
      }
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set the certificate key

  void TlsCerts::setprvk (const String& prvk) {
    wrlock ();
    try {
      d_prvk = prvk;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the certificate private key
  
  Key* TlsCerts::getprvk (void) const {
    rdlock ();
    Key* result = nullptr;
    try {
      // assume pkcs 1 key format
      Pkcs1 pkcs (d_prvk);
      // extract key
      result = new Key (*pkcs.getkey ());
      unlock ();
      return result;
    } catch (...) {
      delete result;
      unlock ();
      throw;
    }
  }

  // get the certificate public key

  Key* TlsCerts::getpubk (void) const {
    rdlock ();
    try {
      // check for a buffer
      if (d_vbuf.length () == 0L) {
	unlock ();
	return nullptr;
      }
      // create a certificate by buffer
      auto cbuf = dynamic_cast<Buffer*> (d_vbuf.get (0));
      if (cbuf == nullptr) {
	throw Exception ("tls-error", "cannot extract certificate buffer");
      }
      X509 cert (*cbuf);
      Xkey xkey = cert.getskey ();
      // extract the key from the certificate key
      Key* result = xkey.tokey ();
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // decode the hblock into a certificate list

  void TlsCerts::decode (TlsHblock* hblk) {
    wrlock ();
    try {
      // reset eveything
      reset ();
      if (hblk == nullptr) {
        unlock ();
        return;
      }
      // the block offset
      long boff = 0L;
      // get the chunk size
      t_byte bh = hblk->getbyte (boff++);
      t_byte bm = hblk->getbyte (boff++);
      t_byte bl = hblk->getbyte (boff++);
      t_quad cs = (((t_quad) bh) << 16) + (((t_quad) bm) << 8) + ((t_quad) bl);
      // loop for block buffer
      t_quad bc = nilq;
      while (bc < cs) {
	// get the buffer size
	bh = hblk->getbyte (boff++);
	bm = hblk->getbyte (boff++);
	bl = hblk->getbyte (boff++);
	t_quad bs =
	  (((t_quad) bh) << 16) + (((t_quad) bm) << 8) + ((t_quad) bl);
	// create a new buffer and load it
	Buffer* buf = new Buffer((long) bs);
	for (long k = 0L; k < (long) bs; k++) {
	  buf->add((char) hblk->getbyte (boff++));
	}
	// add the buffer to the buffer vector
	d_vbuf.add (buf);
	// update the count
	bc += 3 + bs;
      }
      // check size for consistency
      if (bc != cs) {
	throw Exception ("tls-error", "inconsistent certificate list size");
      }
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // map a certificate list to a chunk block

  TlsChunk TlsCerts::tochunk (void) const {
    rdlock ();
    try {
      // the certificate list chunk
      TlsChunk clc;
      // loop in the certificate list
      long vlen = d_vbuf.length ();
      for (long k = 0; k < vlen; k++) {
	// get the buffer by index
	Buffer* buf = dynamic_cast <Buffer*> (d_vbuf.get (k));
	if (buf == nullptr) {
	  throw Exception ("tls-error", "cannot map certificate buffer");
	}
	// map the certificate chunk
	TlsChunk cc = tls_buf_chk (*buf);
	// merge it in the list chunk
	clc.add (cc);
      }
      // create the result chunk
      TlsChunk result;
      t_quad clen = clc.length ();
      result.add ((t_byte) ((clen & 0x00FF0000) >> 16));
      result.add ((t_byte) ((clen & 0x0000FF00) >> 8));
      result.add ((t_byte)  (clen & 0x000000FF));
      result.add (clc);
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
  static const long QUARK_ZONE_LENGTH = 5;
  static QuarkZone  zone (QUARK_ZONE_LENGTH);

  // the object supported quarks
  static const long QUARK_ADD     = zone.intern ("add");
  static const long QUARK_LENGTH  = zone.intern ("length");
  static const long QUARK_GETPUBK = zone.intern ("get-public-key");
  static const long QUARK_SETPRVK = zone.intern ("set-private-key");
  static const long QUARK_GETPRVK = zone.intern ("get-private-key");

  // create a new object in a generic way

  Object* TlsCerts::mknew (Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();
    
    // check for 0 argument
    if (argc == 0) return new TlsCerts;
    // check for 1 argument
    if (argc == 1) {
      String cert = argv->getstring (0);
      return new TlsCerts (cert);
    }
    // check for 2 arguments
    if (argc == 2) {
      String cert = argv->getstring (0);
      String prvk = argv->getstring (1);
      return new TlsCerts (cert, prvk);
    }
    // too many arguments
    throw Exception ("argument-error", 
                     "too many argument with tls certificate list");
  }

  // return true if the given quark is defined

  bool TlsCerts::isquark (const long quark, const bool hflg) const {
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

  Object* TlsCerts::apply (Evaluable* zobj, Nameset* nset, const long quark,
			   Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();
    
    // check for 0 argument
    if (argc == 0) {
      if (quark == QUARK_LENGTH)   return new Integer (length ());
      if (quark == QUARK_GETPUBK)  return getpubk ();
      if (quark == QUARK_GETPRVK)  return getprvk ();
    }
    // check for 1 argument
    if (argc == 1) {
      if (quark == QUARK_ADD) {
	String cert = argv->getstring (0);
	addcert (cert);
	return nullptr;
      }
      if (quark == QUARK_SETPRVK) {
	String prvk = argv->getstring (0);
	setprvk (prvk);
	return nullptr;
      }
    }
    // call the tls infos method
    return TlsInfos::apply (zobj, nset, quark, argv);
  }
}
