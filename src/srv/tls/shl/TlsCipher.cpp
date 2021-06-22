// ---------------------------------------------------------------------------
// - TlsCipher.cpp                                                          -
// - afnix:tls service - tls connect class implementation                    -
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
#include "Runnable.hpp"
#include "TlsUtils.hpp"
#include "TlsCipher.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"
#include "ModeCipher.hpp"
#include "AeadCipher.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - private section                                                       -
  // -------------------------------------------------------------------------

  // the explicit aead iv size
  static const long CV_EIV_SIZE = 8L;

  // generate an explicit initial vector
  static Buffer tls_keiv_encd (void) {
    return TlsUtils::random (CV_EIV_SIZE);
  }
  
  // collect an explicit initial vector
  static Buffer tls_keiv_decd (TlsBuffer& ib) {
    Buffer keiv = ib.move (CV_EIV_SIZE);
    return keiv;
  }
  
  // collect an encode authentication data
  static Buffer tls_auth_encd (const TlsBuffer& tb, const long atsz,
			       TlsState* ssta) {
    // create the result buffer
    Buffer auth;
    if (ssta == nullptr) return auth;
    // extract the sequence number
    t_octa snum = ssta->getsflg () ? ssta->newsnum(true) : ssta->newcnum(true);
    // fill the buffer
    auth.add (snum, true);
    auth.add ((char) tb.gettype ());
    auth.add ((char) tb.getvmaj ());
    auth.add ((char) tb.getvmin ());
    auth.add ((t_word) tb.length (), true);
    // here it is
    return auth;
  }
  
  // collect a decode authentication data
  static Buffer tls_auth_decd (const TlsBuffer& tb, const long atsz,
			       TlsState* ssta) {
    // create the result buffer
    Buffer auth;
    if (ssta == nullptr) return auth;
    // extract the sequence number
    t_octa snum = ssta->getsflg () ? ssta->newcnum(true) : ssta->newsnum(true);
    // fill the buffer
    auth.add (snum, true);
    auth.add ((char) tb.gettype ());
    auth.add ((char) tb.getvmaj ());
    auth.add ((char) tb.getvmin ());
    auth.add ((t_word) (tb.length () - atsz), true);
    // here it is
    return auth;
  }
  
  // encode a hmac block in a buffer
  static TlsBuffer tls_hmac_encd (const TlsBuffer& hb, TlsState* ssta) {
    // check the block
    if (hb.empty () == true) {
      throw Exception ("tls-error", "invalid nil block in hmac encode");
    }
    // get the hmac
    Hmac*  hmac = nullptr;
    t_octa snum = nilo;
    if (ssta != nullptr) {
      hmac = ssta->getsflg () ? ssta->getshmc() : ssta->getchmc();
      snum = ssta->getsflg () ? ssta->newsnum(false) : ssta->newcnum(false);
    }
    // check for nil hmac
    if (hmac == nullptr) return hb;
    // reset the hmac
    hmac->reset();
    // create the hmac buffers
    Buffer hbuf;
    Buffer dbuf = hb;
    hbuf.add ((t_octa) snum, true);
    hbuf.add ((char) hb.gettype());
    hbuf.add ((char) hb.getvmaj());
    hbuf.add ((char) hb.getvmin());
    hbuf.add ((t_word) hb.length(), true);
    // compute the mac
    hmac->process (hbuf);
    hmac->process (dbuf);
    hmac->finish ();
    TlsBuffer result = hb.rebind (0L, 0L); hmac->pushb (result);
    // paranoid check
    long hlen = hmac->getrlen();
    if (result.length() != hlen) {
      throw Exception ("tls-error", "inconsistent hmac buffer");
    }
    return result;
  }
  
  // decode a hmac block in a buffer
  static long tls_hmac_decd (const TlsBuffer& hb, TlsState* ssta) {
    // check the block
    if (hb.empty () == true) {
      throw Exception ("tls-error", "invalid nil block in hmac decode");
    }	
    // get the hmac
    Hmac*  hmac = nullptr;
    t_octa snum = nilo;
    if (ssta != nullptr) {
      hmac = ssta->getsflg () ? ssta->getchmc() : ssta->getshmc();
      snum = ssta->getsflg () ? ssta->newcnum(false) : ssta->newsnum(false);
    }
    // check for nil hmac
    if (hmac == nullptr) return 0L;
    // reset the hmac
    hmac->reset ();
    // get the hmac length and record length
    long hlen = hmac->getrlen();
    long blen = hb.length () - hlen;
    if (blen < 0) {
      throw Exception ("tls-error", "inconsistent hmac block length");
    }
    // collect data and hmac buffer
    Buffer dbuf = hb.extract (0L, blen);
    Buffer vbuf = hb.extract (blen, hlen);
    // create the hmac buffer
    Buffer hbuf;
    hbuf.add (snum, true);
    hbuf.add ((char) hb.gettype ());
    hbuf.add ((char) hb.getvmaj ());
    hbuf.add ((char) hb.getvmin ());
    hbuf.add ((t_word) blen, true);
    // compute the mac
    Buffer mbuf;
    hmac->process (hbuf);
    hmac->process (dbuf);
    hmac->finish ();
    hmac->pushb (mbuf);
    // check the computed result
    return (mbuf == vbuf) ? hlen : -1L;
  }
  
  // encode an input buffer to an output buffer with a mode cipher
  static long tls_cifr_encd (TlsBuffer& ob, TlsBuffer& ib, ModeCipher& mc) {
    // check for padding
    long ilen = ib.length();
    long cbsz = mc.getcbsz ();
    long psiz = cbsz - ((ilen + 1) % cbsz);
    if (psiz > 255) {
      throw Exception ("tls-error", "invalid record padding size");
    }
    for (long k = 0; k <= psiz; k++) ib.add ((char) psiz);
    if (ib.length() != ilen + psiz + 1) {
      throw Exception ("tls-error", "inconsistent padded record size");
    }
    // rebind output buffer
    ob = ib.rebind (0L, 0L);
    // encode the buffer
    return mc.stream (ob, ib);
  }
  
  // decode an input buffer to an output buffer with a mode cipher
  static long tls_cifr_decd (TlsBuffer& ob, TlsBuffer& ib, ModeCipher& mc) {
    // create a temporary buffer with meta data
    TlsBuffer tb = ib.rebind (0L, 0L);
    // decode the stream into a buffer
    long result = mc.stream (tb, ib);
    if (result != tb.length ()) {
      throw Exception ("record-error", "inconsistent decode size");
    }
    // remove the padding if any with block cipher
    if (result > 0) {
      long plen = (long) tb.get(result - 1);
      if ((plen + 1) >= result) {
	throw Exception ("record-error", "inconsistent record padding");
      }
      result -= (plen + 1);
    }
    // rebind with result data length
    ob = tb.rebind (0L, result);
    return result;
  }
  
  // encode an input buffer to an output buffer with an aead cipher
  static long tls_cifr_encd (Buffer& ob, Buffer& ib, AeadCipher& ac) {
    // encode the input buffer
    Buffer tb;
    long result = ac.stream (tb, ib);
    // collect the authentication tag
    Buffer atag = ac.getatag ();
    // build final buffer
    ob.add (tb); ob.add (atag); result += CV_EIV_SIZE; result += ac.getatsz();
    if (result != ob.length ()) {
      throw Exception ("tls-errors", "inconsistent encode aead buffer size");
    }
    return result;
  }
  
  // decode an input buffer to an output buffer with an aead cipher
  static long tls_cifr_decd (TlsBuffer& ob, TlsBuffer& ib, AeadCipher& ac) {
    // extract the authentication tag
    Buffer atag = ib.collect (ac.getatsz ());
    // decode the stream into a buffer
    long result = ac.stream (ob, ib);
    if (result != ob.length ()) {
      throw Exception ("record-error", "inconsistent decode size");
    }
    // check authentication tag
    if (ac.getatag () != atag) {
      throw Exception ("record-error", "inconsistent authentication tag");
    }
    return result;
  }
  
  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a nil tls cipher
  

  TlsCipher::TlsCipher (void) {
    p_ssta = nullptr;
  }

  // create a tls cipher by state

  TlsCipher::TlsCipher (TlsState* ssta) {
    Object::iref (p_ssta = ssta);
  }

  // destroy this tls cipher

  TlsCipher::~TlsCipher (void) {
    Object::dref (p_ssta);
  }
  
  // return the class name
  
  String TlsCipher::repr (void) const {
    return "TlsCipher";
  }

  // encode an input buffer into an output buffer

  long TlsCipher::encode (TlsBuffer& ob, TlsBuffer& ib) {
    wrlock ();
    try {
      // collect the encoding cipher
      Cipher* cifr = nullptr;
      if (p_ssta != nullptr) {
	cifr = p_ssta->getsflg () ? p_ssta->getsbcf () : p_ssta->getcbcf ();
      }
      // check for a nil cipher
      if (cifr == nullptr) {
	ob = ib; long result = ob.length ();
	unlock ();
	return result;
      }
      // check for a mode cipher
      auto mc = dynamic_cast<ModeCipher*>(cifr);
      if (mc != nullptr) {
	// create a hmac buffer
	TlsBuffer hb = tls_hmac_encd (ib, p_ssta);
	// add the mac prior encoding
	ib.add (hb);
	// encode the buffer
	long result = tls_cifr_encd (ob, ib, *mc);
	if (result != ob.length ()) {
	  throw Exception ("tls-error", "inconsistent encode buffer size");
	}
	unlock ();
	return result;
      }
      // check for an aead cipher
      auto ac = dynamic_cast<AeadCipher*>(cifr);
      if (ac != nullptr) {
	// clear the cipher from explicit data
	ac->clear ();
	// collect the explicit nonce and authentication data
	Buffer keiv = tls_keiv_encd (); ob.add (keiv);
	Buffer auth = tls_auth_encd (ib, ac->getatsz (), p_ssta);
	// update the cipher kiv and authentication data
	ac->addiv (keiv); ac->setauth (auth);
	// encode the buffer
	long result = tls_cifr_encd (ob, ib, *ac);
	unlock ();
	return result;
      }
      // simply encode with the cipher
      long result = cifr->stream (ob, ib);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // decode an input buffer into an output buffer

  long TlsCipher::decode (TlsBuffer& ob, TlsBuffer& ib) {
    wrlock ();
    try {
      // collect the decoding cipher
      Cipher* cifr = nullptr;
      if (p_ssta != nullptr) {
	cifr = p_ssta->getsflg () ? p_ssta->getcbcf () : p_ssta->getsbcf ();
      }
      // check for a nil cipher
      if (cifr == nullptr) {
	ob = ib; long result = ob.length ();
	unlock ();
	return result;
      }
      // check for a mode cipher
      auto mc = dynamic_cast<ModeCipher*>(cifr);
      if (mc != nullptr) {
	// decode the block
	long rlen = tls_cifr_decd (ob, ib, *mc);
	// verify the hmac
	long hlen = tls_hmac_decd (ob, p_ssta);
	if (hlen == -1L) {
	  throw Exception ("tls-error", "hmac verification failure");
	}
	// recompute trimmed buffer
	long result = rlen - hlen;
	if (ob.trim (result, true) == false) {
	  throw Exception ("tls-error", "decode buffer trim failure");
	}
	unlock ();
	return result;
      }
      // check for an aead cipher
      auto ac = dynamic_cast<AeadCipher*>(cifr);
      if (ac != nullptr) {
	// clear the buffer of explicit data
	ac->clear();
	// collect the explicit nonce and authentication data
	Buffer keiv = tls_keiv_decd (ib);
	Buffer auth = tls_auth_decd (ib, ac->getatsz (), p_ssta);
	// update the cipher kiv and authentication data
	ac->addiv (keiv); ac->setauth (auth);
	// decode the buffer
	long result = tls_cifr_decd (ob, ib, *ac);
	unlock ();
	return result;
      }
      // simply decode with the cipher
      long result = cifr->stream (ob, ib);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // get the tls state

  TlsState* TlsCipher::getssta (void) const {
    rdlock ();
    try {
      TlsState* result = p_ssta;
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

  Object* TlsCipher::mknew (Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();

    // check for 0 argument
    if (argc == 0) return new TlsCipher;
    // check for 1 argument
    if (argc == 1) {
      Object* obj = argv->get (0);
      auto ssta = dynamic_cast <TlsState*> (obj);
      if ((ssta == nullptr) && (obj != nullptr)) {
	throw Exception ("type-error", "invalid object as tls state",
			 Object::repr (obj));
      }
      return new TlsCipher (ssta);
    }
    // too many arguments
    throw Exception ("argument-error", 
                     "too many argument with tls cipher constructor");
  }
}
