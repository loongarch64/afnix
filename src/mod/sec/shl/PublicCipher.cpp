// ---------------------------------------------------------------------------
// - PublicCipher.cpp                                                        -
// - afnix:sec module - block cipher class implementation                    -
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
#include "Integer.hpp"
#include "QuarkZone.hpp"
#include "PublicCipher.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------
  
  // create a cipher by name and key
  
  PublicCipher::PublicCipher (const String& name) : Cipher (name) {
    // reset all block sizes
    d_mbsz = 0;
    d_cbsz = 0;
  }

  // set the public cipher key

  void PublicCipher::setkey (const Key& key) {
    wrlock ();
    try {
      // check the key for validity
      if (key.ispk () == false) {
	throw Exception ("key-error", "invalid key for public cipher");
      } 
      // set the key and return
      Cipher::setkey (key);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // return the message block size
  
  long PublicCipher::getmbsz (void) const {
    rdlock ();
    try {
      long result = d_mbsz;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // return the crypted block size

  long PublicCipher::getcbsz (void) const {
    rdlock ();
    try {
      long result = d_cbsz;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // -------------------------------------------------------------------------
  // - protected section                                                     -
  // -------------------------------------------------------------------------

  // encode an input buffer into an output buffer
  
  long PublicCipher::encode (Buffer& ob, Buffer& ib) {
    wrlock ();
    try {
      // check valid mode
      if (d_rflg == true) {
	throw Exception ("cipher-error", "calling encode in reverse mode");
      }  
      // check the block size
      if ((d_cbsz == 0) || (ib.empty () == true)) {
	unlock ();
	return 0;
      }
      // initialize the local buffers
      t_byte bi[d_mbsz];
      t_byte bo[d_cbsz];
      // initialize the input buffer
      long cc = 0;
      for (long i = 0; i < d_mbsz; i++) {
	if (ib.empty () == true) break;
	bi[cc++] = ib.read ();
      }
      // fill in the last byte
      for (long i = cc; i < d_mbsz; i++) bi[i] = nilc;
      // encode the block
      long bosz = encode (bo, bi, cc);
      // update the output buffer
      ob.add ((char*) bo, bosz);
      d_encs[0] += bosz; d_encs[1] += bosz;
      unlock ();
      return cc;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // encode an input stream into an output buffer
  
  long PublicCipher::encode (Buffer& ob, InputStream& is) {
    wrlock ();
    try {
      // check valid mode
      if (d_rflg == true) {
	throw Exception ("cipher-error", "calling encode in reverse mode");
      }   
      // check the block size
      if ((d_cbsz == 0) || (is.iseos () == true)) {
	unlock ();
	return 0;
      }
      // initialize the local buffers
      t_byte bi[d_mbsz];
      t_byte bo[d_cbsz];
      // initialize the input buffer
      long cc = 0;
      for (long i = 0; i < d_mbsz; i++) {
	if (is.iseos () == true) break;
	bi[cc++] = is.read ();
      }
      // fill in the last byte
      for (long i = cc; i < d_mbsz; i++) bi[i] = nilc;
      // encode the block
      long bosz = encode (bo, bi, cc);
      // update the output stream
      ob.add ((char*) bo, bosz);
      d_encs[0] += bosz; d_encs[1] += bosz;
      unlock ();
      return cc;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // encode an input stream into an output stream
  
  long PublicCipher::encode (OutputStream& os, InputStream& is) {
    wrlock ();
    try {
      // check valid mode
      if (d_rflg == true) {
	throw Exception ("cipher-error", "calling encode in reverse mode");
      }  
      // check the block size
      if ((d_cbsz == 0) || (is.iseos () == true)) {
	unlock ();
	return 0;
      }
      // initialize the local buffers
      t_byte bi[d_mbsz];
      t_byte bo[d_cbsz];
      // initialize the input buffer
      long cc = 0;
      for (long i = 0; i < d_mbsz; i++) {
	if (is.iseos () == true) break;
	bi[cc++] = is.read ();
      }
      // fill in the last byte
      for (long i = cc; i < d_mbsz; i++) bi[i] = nilc;
      // encode the block
      long bosz = encode (bo, bi, cc);
      // update the output stream
      os.write ((char*) bo, bosz);
      d_encs[0] += bosz; d_encs[1] += bosz;
      unlock ();
      return cc;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // decode an input buffer into an output buffer
  
  long PublicCipher::decode (Buffer& ob, Buffer& ib) {
    wrlock ();
    try {
      // check valid mode
      if (d_rflg == false) {
	throw Exception ("cipher-error", "calling decode in normal mode");
      }  
      // check the block size
      if ((d_mbsz == 0) || (d_cbsz == 0) || (ib.empty () == true)) {
	unlock ();
	return 0;
      }
      // initialize the local buffers
      t_byte bi[d_cbsz];
      t_byte bo[d_mbsz];
      // initialize the input buffer
      long cc = 0;
      for (long i = 0; i < d_cbsz; i++) {
	if (ib.empty () == true) break;
	bi[cc++] = ib.read ();
      }
      // fill in the last byte
      for (long i = cc; i < d_cbsz; i++) bi[i] = nilc;
      // decode the block
      long bosz = decode (bo, bi, cc);
      // update the output buffer
      ob.add ((char*) bo, bosz);
      d_decs[0] += bosz; d_decs[1] += bosz;
      unlock ();
      return bosz;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // decode an input stream into an output buffer
  
  long PublicCipher::decode (Buffer& ob, InputStream& is) {
    wrlock ();
    try {
      // check valid mode
      if (d_rflg == true) {
	throw Exception ("cipher-error", "calling encode in reverse mode");
      }  
      // check the block size
      if ((d_mbsz == 0) || (d_cbsz == 0) || (is.iseos () == true)) {
	unlock ();
	return 0;
      }
      // initialize the local buffers
      t_byte bi[d_cbsz];
      t_byte bo[d_mbsz];
      // initialize the input buffer
      long cc = 0;
      for (long i = 0; i < d_cbsz; i++) {
	if (is.iseos () == true) break;
	bi[cc++] = is.read ();
      }
      // fill in the last byte
      for (long i = cc; i < d_cbsz; i++) bi[i] = nilc;
      // encode the block
      long bosz = decode (bo, bi, cc);
      // update the output stream
      ob.add ((char*) bo, bosz);
      d_decs[0] += bosz; d_decs[1] += bosz;
      unlock ();
      return bosz;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // decode an input stream into an output stream
  
  long PublicCipher::decode (OutputStream& os, InputStream& is) {
    wrlock ();
    try {
      // check valid mode
      if (d_rflg == false) {
	throw Exception ("cipher-error", "calling decode in normal mode");
      }
      // check the block size
      if ((d_mbsz == 0) || (d_cbsz == 0) || (is.iseos () == true)) {
	unlock ();
	return 0;
      }
      // initialize the local buffers
      t_byte bi[d_cbsz];
      t_byte bo[d_mbsz];
      // initialize the input buffer
      long cc = 0;
      for (long i = 0; i < d_cbsz; i++) {
	if (is.iseos () == true) break;
	bi[cc++] = is.read ();
      }
      // fill in the last byte
      for (long i = cc; i < d_cbsz; i++) bi[i] = nilc;
      // encode the block
      long bosz = decode (bo, bi, cc);
      // update the output stream
      os.write ((char*) bo, bosz);
      d_decs[0] += bosz; d_decs[1] += bosz;
      unlock ();
      return bosz;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // finish the stream processig

  long PublicCipher::preset (void) {
    wrlock ();
    try {
      long result = 0L;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // finish the stream processig

  long PublicCipher::finish (void) {
    wrlock ();
    try {
      long result = 0L;
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
  static const long QUARK_ZONE_LENGTH = 2;
  static QuarkZone  zone (QUARK_ZONE_LENGTH);

  // the object supported quarks
  static const long QUARK_GETMBSZ = zone.intern ("get-message-size");
  static const long QUARK_GETCBSZ = zone.intern ("get-crypted-size");

  // return true if the given quark is defined

  bool PublicCipher::isquark (const long quark, const bool hflg) const {
    rdlock ();
    try {
      if (zone.exists (quark) == true) {
	unlock ();
	return true;
      }
      bool result = hflg ? Cipher::isquark (quark, hflg) : false;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // apply this object with a set of arguments and a quark
  
  Object* PublicCipher::apply (Evaluable* zobj, Nameset* nset, const long quark,
			       Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();
    
    // check for 0 argument
    if (argc == 0) {
      if (quark == QUARK_GETMBSZ) return new Integer (getmbsz ());
      if (quark == QUARK_GETCBSZ) return new Integer (getcbsz ());
    }
    // call the cipher method
    return Cipher::apply (zobj, nset, quark, argv);
  }
}
