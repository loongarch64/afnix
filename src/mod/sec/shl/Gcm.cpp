// ---------------------------------------------------------------------------
// - Gcm.cpp                                                                 -
// - afnix:sec module - galois counter mode class implementation             -
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

#include "Gcm.hpp"
#include "Galois.hpp"
#include "Vector.hpp"
#include "System.hpp"
#include "Unicode.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"
#include "BlockCipher.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - private section                                                       -
  // -------------------------------------------------------------------------

  // the gcm algo name
  static const char*  GCM_ALGO_NAME  = "GCM";
  // the gcm block size
  static const long    GCM_BLOK_SIZ  = 16L;
  // the gcm polynomial x^128 + x^7 + x^2 + x + 1
  static const String  GCM_POLY_STR  = "0x100000000000000000000000000000087";
  static const Relatif GCM_POLY_RVL  = Relatif(GCM_POLY_STR);
  // the gdm nil block
  static const t_byte  GCM_BLOK_NIL[GCM_BLOK_SIZ] =
    {
     0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 
     0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U
    };
  
  // increement the counter buffer
  static inline void gcm_nist_incr (Buffer& cb) {
    union {
      t_quad ival;
      t_byte bval[4];
    };
    for (long k = 0L; k < 4; k++) bval[k] = cb.get(12+k);
    ival = System::qswap (ival);
    ival++;
    ival = System::qswap (ival);
    for (long k = 0L; k < 4; k++) cb.set (12+k, bval[k]);
  }
  
  // the nist ghash function
  static inline Buffer gcm_nist_bhsh (const Buffer& bi, const Relatif& hnil,
				      const Galois& gfld) {
    // make sure we have the right size
    if (bi.length () != GCM_BLOK_SIZ) {
      throw Exception ("gcm-error", "invalid ghash buffer size");
    }
    // compute bx * h
    Relatif xval (bi.tobyte (), bi.tosize ());
    Relatif yval = gfld.gmm (xval, hnil);
    // create a target buffer
    t_byte ybuf[GCM_BLOK_SIZ]; yval.tobyte (ybuf, GCM_BLOK_SIZ);
    Buffer result; result.add ((char*) ybuf, GCM_BLOK_SIZ);
    return result;
  }
  
  // the nist ghash function
  static Buffer gcm_nist_bhsh (const Buffer&    bi, const Buffer& bl,
			       const Relatif& hnil, const Galois& gfld) {
    // special case with nil buffer
    if (bl.length () == 0L) return gcm_nist_bhsh (bi, hnil, gfld);
    // make sure we have the right size
    if ((bi.length () != GCM_BLOK_SIZ) || (bl.length () != GCM_BLOK_SIZ)) {
      throw Exception ("gcm-error", "invalid ghash buffer size");
    }
    // build the xor buffer
    Buffer bx = bi; bx ^= bl;
    // compute the block hash
    return gcm_nist_bhsh (bx, hnil, gfld);
  }

  // compute the hash of the complete block
  static Buffer gcm_nist_ghsh (const Buffer& ba, const Relatif& hnil,
			       const Galois& gfld) {
    // get the buffer size
    long alen = ba.length ();
    // we need an empty buffer
    Buffer bl (GCM_BLOK_SIZ);
    // the number of block to process
    long blen = alen / GCM_BLOK_SIZ;
    long rlen = alen % GCM_BLOK_SIZ;
    // loop in the authority
    for (long k = 0L; k < blen; k++) {
      // extract buffer at postion
      Buffer bi = ba.extract (k * GCM_BLOK_SIZ, GCM_BLOK_SIZ);
      // compute the running buffer
      bl = gcm_nist_bhsh (bi, bl, hnil, gfld);
    }
    // process last block if any
    if (rlen != 0) {
      // extract buffer at postion
      Buffer bi = ba.extract (blen * GCM_BLOK_SIZ, GCM_BLOK_SIZ);
      // compute the running buffer
      bl = gcm_nist_bhsh (bi, bl, hnil, gfld);
    }
    return bl;
  }
  
  // the nist gctr function
  static Buffer gcm_nist_gctr (Buffer& cb, const Buffer& bi, Cipher* cifr) {
    // check for valid cipher
    if (cifr == nullptr) {
      throw Exception ("gcm-erro", "invalid nil cipher in gctr");
    }
    // make sure we have the right size
    if ((cb.length () != GCM_BLOK_SIZ) || (bi.length () != GCM_BLOK_SIZ)) {
      throw Exception ("gcm-error", "invalid gctr buffer size");
    }
    // encode the counter
    Buffer result; Buffer ccnt = cb;
    if (cifr->stream (result, ccnt) != GCM_BLOK_SIZ) {
      throw Exception ("gcm-error", "inconsistent gctr encoding");
    }
    // xor the result buffer
    result ^= bi;
    // increment the counter
    gcm_nist_incr (cb);
    // here it is
    return result;
  }

  // nist j0 block generation
  static Buffer gcm_nist_j0 (const Buffer& iv, const Relatif& hnil,
			     const Galois& gfld) {
    // collect iv size
    long ivsz = iv.length ();
    // famous 96 bits special case - don't ask me why - it's in the standard
    Buffer j0;
    if (ivsz == 12) {
      // preset result
      j0 = iv;
      // add 0x00000001
      j0.add ((char) 0x00); j0.add ((char) 0x00); j0.add ((char) 0x00);
      j0.add ((char) 0x01);
    } else {
      // preset buffer
      Buffer bi = iv;
      // add padding 0 upto to 128 bits blocks
      long pads = 16 - (ivsz % 16);
      for (long k = 0L; k < pads; k++) bi.add ((char) 0x00);
      // add another 64 bits 0 padding
      for (long k = 0L; k < 8; k++) bi.add ((char) 0x00);
      // add the iv size
      union {
	t_byte bval[8];
	t_octa oval;
      };
      oval = System::oswap ((t_octa) ivsz);
      for (long k = 0L; k < 8; k++) bi.add ((char) bval[k]);
      // compute the hash buffer
      j0 = gcm_nist_ghsh (bi, hnil, gfld);
    }
    return j0;
  }

  // the private gcm structure
  struct s_gcm {
    /// the authentication length
    long d_alen;
    /// the cipher text length
    long d_tlen;
    /// the galois field
    Galois d_gfld;
    /// the coded null block
    Relatif d_hnil;
    /// the current hash
    Buffer d_hbuf;
    /// the cipher counter
    Buffer d_ccnt;
    /// the hash counter
    Buffer d_hcnt;
    
    // create a default gcm
    s_gcm (void) {
      d_gfld = { GCM_POLY_RVL };
      reset();
    }
    // reset the gcm
    void reset (void) {
      d_alen = 0L;
      d_tlen = 0L;
      d_hnil = 0L;
      d_hbuf.reset ();
      d_ccnt.reset ();
      d_hcnt.reset ();
      d_hbuf.add ((const char*)  GCM_BLOK_NIL, GCM_BLOK_SIZ);
    }
  
    // clear the gcm
    void clear (void) {
      d_alen = 0L;
      d_tlen = 0L;
      d_hbuf.reset ();
      d_ccnt.reset ();
      d_hcnt.reset ();
      d_hbuf.add ((const char*)  GCM_BLOK_NIL, GCM_BLOK_SIZ);
    }

    // preset the gcm
    void preset (const Buffer& iv, const Buffer& auth) {
      // check for iv
      if ((d_ccnt.empty () == true) || (d_hcnt.empty () == true)) setiv (iv);
      // check for auth
      if ((d_hbuf.empty () == true) || (d_alen == 0L)) setauth (auth);
    }
    
    // set the gcm authentication data
    void setauth (const Buffer& auth) {
      // do nothing with null authority
      if (auth.length () == 0L) return;
      // set the hash buffer and authentication length
      d_hbuf = gcm_nist_ghsh (auth, d_hnil, d_gfld);
      d_alen = auth.length ();
    }

    // set the initial vector
    void setiv (const Buffer& iv) {
      // do nothing with null vector
      if (iv.length () == 0L) return;
      // compute initial j0
      Buffer j0 = gcm_nist_j0 (iv, d_hnil, d_gfld);
      // preset the cipher counter
      d_ccnt = j0; gcm_nist_incr (d_ccnt);
      d_hcnt = j0;
    }

    // process a buffer
    Buffer encode (Buffer& ib, Cipher* cifr) {
      // check for null
      if (cifr == nullptr) return 0L;
      // fill the temporary buffer
      Buffer tb (GCM_BLOK_SIZ);
      while (ib.empty () == false) {
	tb.add (ib.read());
	if (tb.length () == GCM_BLOK_SIZ) break;
      }
      long ilen = tb.length ();
      d_tlen += ilen;
      while (tb.length() != GCM_BLOK_SIZ) tb.add (nilc);
      // compute output buffer
      Buffer ob = gcm_nist_gctr (d_ccnt, tb, cifr);
      // update the hash buffer
      tb = ob; tb.trim (ilen, true);
      while (tb.length() != GCM_BLOK_SIZ) tb.add (nilc);
      d_hbuf = gcm_nist_bhsh (tb, d_hbuf, d_hnil, d_gfld);
      // here it is
      return ob;
    }

    // process a buffer
    Buffer decode (Buffer& ib, Cipher* cifr) {
      // check for null
      if (cifr == nullptr) return 0L;
      // fill the temporary buffer
      Buffer tb (GCM_BLOK_SIZ);
      while (ib.empty () == false) {
	tb.add (ib.read());
	if (tb.length () == GCM_BLOK_SIZ) break;
      }
      d_tlen += tb.length ();
      while (tb.length() != GCM_BLOK_SIZ) tb.add (nilc);
      // compute output buffer
      Buffer ob = gcm_nist_gctr (d_ccnt, tb, cifr);
      // update the hash buffer
      d_hbuf = gcm_nist_bhsh (tb, d_hbuf, d_hnil, d_gfld);
      // here it is
      return ob;
    }

    // finish the gcm processing
    long finish (Cipher* cifr) {
      // check for null
      if (cifr == nullptr) return 0L;
      // process authentication length
      union {
	t_byte bval[8];
	t_octa oval;
      };
      oval = System::oswap (((t_octa) d_alen) << 3);
      // prepare the final buffer
      Buffer ib;
      for (long k = 0L; k < 8; k++) ib.add ((char) bval[k]);
      // process cipher text length
      oval = System::oswap (((t_octa) d_tlen) << 3);
      for (long k = 0L; k < 8; k++) ib.add ((char) bval[k]);
      // update the hash
      d_hbuf = gcm_nist_bhsh (ib, d_hbuf, d_hnil, d_gfld);
      // run the final gctr
      d_hbuf = gcm_nist_gctr (d_hcnt, d_hbuf, cifr);
      // nothing to report
      return 0L;
    }
  };
  
  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a default gcm

  Gcm::Gcm (void) : AeadCipher (GCM_ALGO_NAME, GCM_BLOK_SIZ) {
    p_xgcm = new s_gcm;
    p_cifr = nullptr;
    reset ();
  }

  // create a gcm by cipher

  Gcm::Gcm (Cipher* cifr) : AeadCipher (GCM_ALGO_NAME, GCM_BLOK_SIZ) {
    p_xgcm = new s_gcm;
    p_cifr = nullptr;
    setcifr (cifr);
  }
  
  // create a gcm by cipher

  Gcm::Gcm (Cipher* cifr, const bool rflg)
    : AeadCipher (GCM_ALGO_NAME, GCM_BLOK_SIZ) {
    p_xgcm = new s_gcm;
    p_cifr = nullptr;
    setcifr (cifr);
    setrflg (rflg);
  }
  // destroy this gcm

  Gcm::~Gcm (void) {
    delete p_xgcm;
  }
  
  // return the class name

  String Gcm::repr (void) const {
    return "Gcm";
  }
  
  // reset the gcm

  void Gcm::reset (void) {
    wrlock ();
    try {
      // reset base class
      AeadCipher::reset ();
      // reset locally
      d_eiv.reset ();
      if (p_xgcm != nullptr) p_xgcm->reset ();
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // clear the gcm

  void Gcm::clear (void) {
    wrlock ();
    try {
      // clear base class
      AeadCipher::clear ();
      // clear locally
      d_eiv = d_iv;
      if (p_xgcm != nullptr) p_xgcm->clear ();
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set the cipher key

  void Gcm::setkey (const Key& key) {
    wrlock ();
    try {
      // set the base key
      AeadCipher::setkey (key);
      // set the gcm cipher key if any
      if (p_cifr != nullptr) p_cifr->setkey (d_ckey);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    } 
  }

  // normalize a data size
  
  t_long Gcm::waist (const t_long size) const {
    rdlock ();
    try {
      t_long result = 0L;
      if (p_cifr != nullptr) {
	result = p_cifr->waist (size);
      }
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // set the initial vector

  void Gcm::setiv (const String& ivs) {
    wrlock ();
    try {
      // set base vector
      AeadCipher::setiv (ivs);
      // set the extended iv
      d_eiv = d_iv;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set the initial vector

  void Gcm::setiv (const Buffer& kiv) {
    wrlock ();
    try {
      // set base vector
      AeadCipher::setiv (kiv);
      // set the extended iv
      d_eiv = d_iv;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // add an explicit iv
  
  void Gcm::addiv (const String& evs) {
    // convert the octet string
    long evsz = 0;
    t_byte* sbuf = Unicode::stob (evsz, evs);
    // lock and set
    wrlock ();
    try {
      // check for valid argument
      if (evsz <= 0L) {
	throw Exception ("cipher-error", "invalid explicit iv string size");
      }
      // set the initial vector
      d_eiv.add((char*)sbuf, evsz);
      delete [] sbuf;
      unlock ();
    } catch (...) {
      delete [] sbuf;
      unlock ();
      throw;
    }
  }
  
  // add an explicit iv

  void Gcm::addiv (const Buffer& eiv) {
    wrlock ();
    try {
      // update the extended iv
      d_eiv.add (eiv);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // set the authentication data by string
  
  void Gcm::setauth (const String& ads) {
    wrlock ();
    try {
      // set base authentication
      AeadCipher::setauth (ads);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // set the gcm authentication data

  void Gcm::setauth (const Buffer& auth) {
    wrlock ();
    try {
      // set base authentication
      AeadCipher::setauth (auth);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set the gcm cipher

  void Gcm::setcifr (Cipher* cifr) {
    wrlock ();
    try {
      // look for a block cipher
      auto bcfr = dynamic_cast<BlockCipher*> (cifr);
      if (bcfr == nullptr) {
	throw Exception ("gcm-error", "invalid cipher to set",
			 Object::repr (cifr));
      }
      // check or valid block size
      if (bcfr->getcbsz () != GCM_BLOK_SIZ) {
	throw Exception ("gcm-error", "invalid cipher block size");
      }
      // check for valid mode
      if (bcfr->getrflg () == true) {
	throw Exception ("gcm-error", "invalid cipher block reverse mode");
      }
      // set the gcm cipher
      Object::iref (cifr); Object::dref (p_cifr); p_cifr = cifr;
      // reset the gcm
      reset ();
      // create a nil buffer
      Buffer ib (GCM_BLOK_SIZ); 
      Buffer ob (GCM_BLOK_SIZ);
      if (ib.add ((const char*) GCM_BLOK_NIL, GCM_BLOK_SIZ) != GCM_BLOK_SIZ) { 
	throw Exception ("gcm-error", "cannot compute gcm nil block");
      }
      if (p_cifr->stream (ob, ib) != GCM_BLOK_SIZ) {
	throw Exception ("gcm-error", "cannot compute gcm nil block");
      }
      if (ob.tosize () != GCM_BLOK_SIZ) {
	throw Exception ("gcm-error", "invalid gcm hnil block size");
      }
      // update target nil value
      p_xgcm->d_hnil = Relatif (ob.tobyte (), ob.tosize ());
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // -------------------------------------------------------------------------
  // - protected section                                                     -
  // -------------------------------------------------------------------------

  // encode an input buffer into an output buffer

  long Gcm::encode (Buffer& ob, Buffer& ib) {
    wrlock ();
    try {
      // check valid mode
      if (d_rflg == true) {
	throw Exception ("gcm-error", "calling encode in reverse mode");
      }
      // check the block size
      if ((d_cbsz == 0) || (ib.empty () == true)) {
	unlock ();
	return 0L;
      }
      // encode with gcm structure
      long ilen = ib.length();
      Buffer rb = p_xgcm->encode (ib, p_cifr);
      ilen -= ib.length();
      if (ilen <= 0L) {
	throw Exception ("gcm-error", "really something bad in encode");
      }
      rb.trim  (ilen, true); ob.add (rb);
      // here it is
      d_encs[0] += ilen; d_encs[1] += ilen;
      unlock ();
      return ilen;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // encode an input stream into an output buffer

  long Gcm::encode (Buffer& ob, InputStream& is) {
    wrlock ();
    try {
      // check valid mode
      if (d_rflg == true) {
	throw Exception ("cipher-error", "calling encode in reverse mode");
      }      
      // check the block size
      if ((d_cbsz == 0) || (is.valid () == false)) {
	unlock ();
	return 0L;
      }
      // create a buffer for filling
      Buffer ib (d_cbsz);
      for (long k = 0L; k < d_cbsz; k++) {
	if (is.valid () == false) break;
	ib.add(is.read ());
      }
      // encode with gcm structure
      long ilen = ib.length ();
      Buffer rb = p_xgcm->encode (ib, p_cifr);
      rb.trim  (ilen, true); ob.add (rb);
      // here it is
      d_encs[0] += ilen; d_encs[1] += ilen;
      unlock ();
      return ilen;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // encode an input stream into an output stream
  
  long Gcm::encode (OutputStream& os, InputStream& is) {
    wrlock ();
    try {
      // check valid mode
      if (d_rflg == true) {
	throw Exception ("cipher-error", "calling encode in reverse mode");
      }      
      // check the block size
      if ((d_cbsz == 0) || (is.valid () == false)) {
	unlock ();
	return 0L;
      }
      // create a buffer for filling
      Buffer ib (d_cbsz);
      for (long k = 0L; k < d_cbsz; k++) {
	if (is.valid () == false) break;
	ib.add(is.read ());
      }
      // encode with gcm structure
      long ilen = ib.length();
      Buffer rb = p_xgcm->encode (ib, p_cifr);
      rb.trim (ilen, true);
      // write the buffer
      if (os.write (rb) != ilen) {
	throw Exception ("gcm-error", "error in encode write");
      }
      // here it is
      d_encs[0] += ilen; d_encs[1] += ilen;
      unlock ();
      return ilen;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // decode an input buffer into an output buffer

  long Gcm::decode (Buffer& ob, Buffer& ib) {
    wrlock ();
    try {
      // check valid mode
      if (d_rflg == false) {
	throw Exception ("gcm-error", "calling decode in normal mode");
      }      
      // check the block size
      if ((d_cbsz == 0) || (ib.empty () == true)) {
	unlock ();
	return 0L;
      }
      // decode with gcm structure
      long ilen = ib.length ();
      Buffer rb = p_xgcm->decode (ib, p_cifr);
      ilen -= ib.length();
      if (ilen <= 0L) {
	throw Exception ("gcm-error", "really something bad in encode");
      }
      rb.trim (ilen, true); ob.add (rb);
      // here it is
      d_decs[0] += ilen; d_decs[1] += ilen;
      unlock ();
      return ilen;
    } catch (...) {
      unlock ();
      throw;
    }
    return 0L;
  }

  // decode an input stream into an output buffer

  long Gcm::decode (Buffer& ob, InputStream& is) {
    wrlock ();
    try {
      // check valid mode
      if (d_rflg == false) {
	throw Exception ("gcm-error", "calling decode in normal mode");
      }      
      // check the block size
      if ((d_cbsz == 0) || (is.valid () == false)) {
	unlock ();
	return 0;
      }
      // create a buffer for filling
      Buffer ib (d_cbsz);
      for (long k = 0L; k < d_cbsz; k++) {
	if (is.valid () == false) break;
	ib.add(is.read ());
      }
      // decode with gcm structure
      long ilen = ib.length ();
      Buffer rb = p_xgcm->decode (ib, p_cifr);
      rb.trim (ilen, true); ob.add (rb);
      // here it is
      d_decs[0] += ilen; d_decs[1] += ilen;
      unlock ();
      return ilen;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // decode an input stream into an output stream

  long Gcm::decode (OutputStream& os, InputStream& is) {
    wrlock ();
    try {
      // check valid mode
      if (d_rflg == false) {
	throw Exception ("gcm-error", "calling decode in normal mode");
      }      
      // check the block size
      if ((d_cbsz == 0) || (is.valid () == false)) {
	unlock ();
	return 0;
      }
      // create a buffer for filling
      Buffer ib (d_cbsz);
      for (long k = 0L; k < d_cbsz; k++) {
	if (is.valid () == false) break;
	ib.add(is.read ());
      }
      // decode with gcm structure
      long ilen = ib.length ();
      Buffer rb = p_xgcm->decode (ib, p_cifr);
      rb.trim (ilen, true);
      // write the buffer
      if (os.write (rb) != ilen) {
	throw Exception ("gcm-error", "error in decode write");
      }
      // here it is
      d_decs[0] += ilen; d_decs[1] += ilen;
      unlock ();
      return ilen;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // preset the gcm processing

  long Gcm::preset (void) {
    wrlock ();
    try {
      // preset the gcm
      p_xgcm->preset (d_eiv, d_auth);
      // preset result
      long result = 0L;
      // done here
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // finish the gcm processing

  long Gcm::finish (void) {
    wrlock ();
    try {
      // finish with gcm structure
      long result = p_xgcm->finish (p_cifr);
      // update authentication tag
      d_atag = p_xgcm->d_hbuf;
      // validate the authentication tag size
      if (d_atag.length () != getatsz ()) {
	throw Exception ("gcm-error", "inconsistent authentication tag size");
      }
      // done here
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

  // the signature supported quarks
  static const long QUARK_SETCIFR = zone.intern ("set-cipher");

  // create a new object in a generic way
  
  Object* Gcm::mknew (Vector* argv) {
    long argc = (argv == nullptr) ? 0 : argv->length ();
    // check for 0 argument
    if (argc == 0) return new Gcm;
    // check for 1 argument
    if (argc == 1) {
      Object* obj = argv->get (0);
      auto cifr = dynamic_cast<Cipher*>(obj);
      if (cifr != nullptr) return new Gcm (cifr);
      // invalid object
      throw Exception ("type-error", "invalid object with gcm constructor",
		       Object::repr (obj));
    }
    // invalid arguments
    throw Exception ("argument-error", "too many arguments with gcm");
  }

  // return true if the given quark is defined

  bool Gcm::isquark (const long quark, const bool hflg) const {
    rdlock ();
    try {
      if (zone.exists (quark) == true) {
	unlock ();
	return true;
      }
      bool result = hflg ? AeadCipher::isquark (quark, hflg) : false;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // apply this object with a set of arguments and a quark
  
  Object* Gcm::apply (Evaluable* zobj, Nameset* nset, const long quark,
			 Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();
    
    // check for 1 argument
    if (argc == 1) {
      if (quark == QUARK_SETCIFR) {
	Object* obj = argv->get (0);
	// check for a cipher
	auto cifr = dynamic_cast<Cipher*>(obj);
	if (cifr != nullptr) {
	  setcifr (cifr);
	  return nullptr;
	}
	// invalid object
	throw Exception ("type-error", "invalid object with set-cipher",
			 Object::repr (obj));
      }
    }
    // call the aead cipher method
    return AeadCipher::apply (zobj, nset, quark, argv);
  }
}
