// ---------------------------------------------------------------------------
// - ModeCipher.cpp                                                         -
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

#include "Item.hpp"
#include "Ascii.hpp"
#include "Vector.hpp"
#include "Unicode.hpp"
#include "Utility.hpp"
#include "Integer.hpp"
#include "QuarkZone.hpp"
#include "ModeCipher.hpp"
#include "ModeCipher.hxx"
#include "InputString.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a block cipher by key

  ModeCipher::ModeCipher (const String& name, 
			  const long    cbsz) : BlockCipher (name, cbsz) {
    // set the cipher info
    d_ivsz = d_cbsz;
    d_pmod = PMOD_N800;
    d_cmod = CMOD_ECBM;
    // create the byte blocks
    p_bl = new t_byte[d_cbsz];
    // preset initial vector
    p_iv = bc_piv (d_ivsz);
    // clear eveything
    clear ();
  }

  // destroy this block cipher

  ModeCipher::~ModeCipher (void) {
    delete [] p_bl;
    delete [] p_iv;
  }

  // reset the block cipher

  void ModeCipher::reset (void) {
    wrlock ();
    try {
      // reset the cipher
      BlockCipher::reset ();
      // reset the last block
      for (long i = 0; i < d_cbsz; i++) p_bl[i] = nilc;
      // reset the initial vector
      for (long i = 0; i < d_ivsz; i++) p_iv[i] = nilc;
      // reset modes
      d_pmod = PMOD_N800;
      d_cmod = CMOD_ECBM;
      // done
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // clear this block cipher

  void ModeCipher::clear (void) {
    wrlock ();
    try {
      // clear the cipher
      BlockCipher::clear ();
      // set the last block with the initial vector
      bc_pbl (d_cmod, p_bl, p_iv, d_cbsz, d_ivsz);
      d_iblk = false;
      // done
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set the mode cipher key

  void ModeCipher::setkey (const Key& key) {
    wrlock ();
    try {
      // set the base key
      BlockCipher::setkey (key);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // normalize a file size

  t_long ModeCipher::waist (const t_long size) const {
    rdlock ();
    try {
      // compute the initial size
      t_long result = (size / d_cbsz) * d_cbsz;
      if ((size == 0) || ((size % d_cbsz) != 0)) result+= d_cbsz;
      // eventually add a padding block
      switch (d_pmod) {
      case PMOD_BITM:
      case PMOD_X923:
      case PMOD_N800:
	if ((size > 0) && ((size % d_cbsz) == 0)) result += d_cbsz;
	break;
      default:
	break;
      }
      // eventually add a cipher mode block
      switch (d_cmod) {
      case CMOD_CBCM:
      case CMOD_EBCM:
      case CMOD_CFBM:
      case CMOD_OFBM:
	if (result > 0) result += d_cbsz;
	break;
      default:
	break;
      }
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  void ModeCipher::setiv (const String& ivs) {
    // convert the octet string
    long ivsz = 0;
    t_byte* sbuf = Unicode::stob (ivsz, ivs);
    // lock and set
    wrlock ();
    try {
      // clear the cipher
      clear ();
      // check for valid argument
      if (ivsz <= 0L) {
	throw Exception ("cipher-error", "invalid iv string size");
      }
      if (ivsz != d_ivsz) {
	delete [] p_iv; p_iv = new t_byte[ivsz]; d_ivsz = ivsz;
      }
      // set and clean
      for (long i = 0L; i < d_ivsz; i++) p_iv[i] = sbuf[i];
      delete [] sbuf;
      // clear the cipher
      clear ();
      unlock ();
    } catch (...) {
      delete [] sbuf;
      unlock ();
      throw;
    }
  }

  // set the initial vector by buffer
  
  void ModeCipher::setiv (const Buffer& kiv) {
    t_byte* sbuf = nullptr;
    wrlock ();
    try {
      // check for valid arguments
      long ivsz = kiv.length ();
      if (ivsz <= 0L) {
	throw Exception ("cipher-error", "invalid iv buffer size");
      }	
      if (ivsz != d_ivsz) {
	delete [] p_iv; p_iv = new t_byte[ivsz]; d_ivsz = ivsz;
      }
      // convert and set
      sbuf = (t_byte*) kiv.tochar ();
      for (long i = 0; i < d_ivsz; i++) p_iv[i] = sbuf[i];
      delete [] sbuf;
      // clear the cipher
      clear ();
      unlock ();
    } catch (...) {
      delete [] sbuf;
      unlock ();
      throw;
    }
  }
  
  // get the initial vector buffer

  String ModeCipher::getiv (void) const {
    rdlock ();
    try {
      String result = Ascii::btos (p_iv, d_ivsz);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set the block cipher padding mode

  void ModeCipher::setpmod (const t_pmod pmod) {
    wrlock ();
    try {
      d_pmod = pmod;
      clear  ();
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the block cipher padding mode

  ModeCipher::t_pmod ModeCipher::getpmod (void) const {
    rdlock ();
    try {
      t_pmod result = d_pmod;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set the block cipher mode

  void ModeCipher::setcmod (const t_cmod cmod) {
    wrlock ();
    try {
      d_cmod = cmod;
      clear ();
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the block cipher mode

  ModeCipher::t_cmod ModeCipher::getcmod (void) const {
    rdlock ();
    try {
      t_cmod result = d_cmod;
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

  long ModeCipher::encode (Buffer& ob, Buffer& ib) {
    wrlock ();
    try {
      // check valid mode
      if (d_rflg == true) {
	throw Exception ("cipher-error", "calling encode in reverse mode");
      }
      // check the block size
      long result = 0L;
      if ((d_cbsz == 0) || (ib.empty () == true)) {
	unlock ();
	return result;
      }
      // check for initial explicit mode
      if ((d_iblk == false) && (d_cmod == CMOD_EBCM)) {
	// create a buffer with a random value
	Buffer rb;
	for (long k = 0L; k < d_cbsz; k++) rb.add((char) Utility::byternd());
	// mark the initial block
	d_iblk = true;
	// encode the buffer
	result += encode (ob, rb);
      }
      // initialize the local buffers
      t_byte bi[d_cbsz];
      t_byte bo[d_cbsz];
      t_byte bt[d_cbsz];
      // initialize the input buffer
      long cc = 0;
      for (long i = 0; i < d_cbsz; i++) {
	if (ib.empty () == true) break;
	bi[cc++] = ib.read ();
      }
      // fill in the last byte
      for (long i = cc; i < d_cbsz; i++) bi[i] = nilc;
      // check for padding
      bool padf = (cc < d_cbsz) ? bc_pad (d_pmod, bi, d_cbsz, cc) : false;
      if (padf == true) cc = d_cbsz;
      // preprocess the block mode
      bc_pbi (d_cmod, true, bt, bi, p_bl, d_cbsz);
      // encode the block
      bc_bme (d_cmod) ? encode (bo, bt) : decode (bo, bt);
      // postprocess the block mode
      bc_pbo (d_cmod, true, bo, bi, p_bl, d_cbsz);
      // update the output buffer
      ob.add ((char*) bo, d_cbsz);
      // check for extra padded block
      if ((padf == false) && (ib.empty () == true)) {
	// fill next pad block
	bool pnxf =  bc_nxp (d_pmod, bi, d_cbsz);
	// add it in the buffer
	if (pnxf == true) {
	  // preprocess the block mode
	  bc_pbi (d_cmod, true, bt, bi, p_bl, d_cbsz);
	  // encode the block
	  bc_bme (d_cmod) ? encode (bo, bt) : decode (bo, bt);
	  // postprocess the block mode
	  bc_pbo (d_cmod, true, bo, bi, p_bl, d_cbsz);
	  // update the output buffer
	  ob.add ((char*) bo, d_cbsz);
	}
      }
      result += cc;
      d_encs[0] += result; d_encs[1] += result;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // encode an input stream into an output buffer

  long ModeCipher::encode (Buffer& ob, InputStream& is) {
    wrlock ();
    try {
      // check valid mode
      if (d_rflg == true) {
	throw Exception ("cipher-error", "calling encode in reverse mode");
      }      
      // check the block size
      long result = 0L;
      if ((d_cbsz == 0) || (is.valid () == false)) {
	unlock ();
	return result;
      }
      // check for initial explicit mode
      if ((d_iblk == false) && (d_cmod == CMOD_EBCM)) {
	// create a buffer with a random value
	Buffer rb;
	for (long k = 0L; k < d_cbsz; k++) rb.add((char) Utility::byternd());
	// mark the initial block
	d_iblk = true;
	// encode the buffer
	result += encode (ob, rb);
      }
      // initialize the local buffers
      t_byte bi[d_cbsz];
      t_byte bo[d_cbsz];
      t_byte bt[d_cbsz];
      // initialize the input buffer
      long cc = 0;
      for (long i = 0; i < d_cbsz; i++) {
	if (is.valid () == false) break;
	bi[cc++] = is.read ();
      }
      // fill in the last byte
      for (long i = cc; i < d_cbsz; i++) bi[i] = nilc;
      // check for padding
      bool padf = (cc < d_cbsz) ? bc_pad (d_pmod, bi, d_cbsz, cc) : false;
      // preprocess the block mode
      bc_pbi (d_cmod, true, bt, bi, p_bl, d_cbsz);
      // encode the block
      bc_bme (d_cmod) ? encode (bo, bt) : decode (bo, bt);
      // postprocess the block mode
      bc_pbo (d_cmod, true, bo, bi, p_bl, d_cbsz);
      // update the output stream
      ob.add ((char*) bo, d_cbsz);
      // check for extra padded block
      if ((padf == false) && (is.valid () == false)) {
	// fill next pad block
	bool pnxf =  bc_nxp (d_pmod, bi, d_cbsz);
	// add it in the buffer
	if (pnxf == true) {
	  // preprocess the block mode
	  bc_pbi (d_cmod, true, bt, bi, p_bl, d_cbsz);
	  // encode the block
	  bc_bme (d_cmod) ? encode (bo, bt) : decode (bo, bt);
	  // postprocess the block mode
	  bc_pbo (d_cmod, true, bo, bi, p_bl, d_cbsz);
	  // update the output stream
	  ob.add ((char*) bo, d_cbsz);
	}
      }
      result += cc;
      d_encs[0] += result; d_encs[1] += result;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // encode an input stream into an output stream

  long ModeCipher::encode (OutputStream& os, InputStream& is) {
    wrlock ();
    try {
      // check valid mode
      if (d_rflg == true) {
	throw Exception ("cipher-error", "calling encode in reverse mode");
      }      
      // check the block size
      long result = 0l;
      if ((d_cbsz == 0) || (is.valid () == false)) {
	unlock ();
	return result;
      }
      // check for initial explicit mode
      if ((d_iblk == false) && (d_cmod == CMOD_EBCM)) {
	// create an input string with a random value
	InputString rs (Utility::strnrnd (d_cbsz));
	// mark the initial block
	d_iblk = true;
	// encode the buffer
	result += encode (os, rs);
      }
      // initialize the local buffers
      t_byte bi[d_cbsz];
      t_byte bo[d_cbsz];
      t_byte bt[d_cbsz];
      // initialize the input buffer
      long cc = 0;
      for (long i = 0; i < d_cbsz; i++) {
	if (is.valid () == false) break;
	bi[cc++] = is.read ();
      }
      // fill in the last byte
      for (long i = cc; i < d_cbsz; i++) bi[i] = nilc;
      // check for padding
      bool padf = (cc < d_cbsz) ? bc_pad (d_pmod, bi, d_cbsz, cc) : false;
      // preprocess the block mode
      bc_pbi (d_cmod, true, bt, bi, p_bl, d_cbsz);
      // encode the block
      bc_bme (d_cmod) ? encode (bo, bt) : decode (bo, bt);
      // postprocess the block mode
      bc_pbo (d_cmod, true, bo, bi, p_bl, d_cbsz);
      // update the output stream
      os.write ((char*) bo, d_cbsz);
      // check for extra padded block
      if ((padf == false) && (is.valid () == false)) {
	// fill next pad block
	bool pnxf =  bc_nxp (d_pmod, bi, d_cbsz);
	// add it in the buffer
	if (pnxf == true) {
	  // preprocess the block mode
	  bc_pbi (d_cmod, true, bt, bi, p_bl, d_cbsz);
	  // encode the block
	  bc_bme (d_cmod) ? encode (bo, bt) : decode (bo, bt);
	  // postprocess the block mode
	  bc_pbo (d_cmod, true, bo, bi, p_bl, d_cbsz);
	  // update the output stream
	  os.write ((char*) bo, d_cbsz);
	}
      }
      result += cc;
      d_encs[0] += result; d_encs[1] += result;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // decode an input buffer into an output buffer

  long ModeCipher::decode (Buffer& ob, Buffer& ib) {
    wrlock ();
    try {
      // check valid mode
      if (d_rflg == false) {
	throw Exception ("cipher-error", "calling decode in normal mode");
      }      
      // check the block size
      if ((d_cbsz == 0) || (ib.empty () == true)) {
	unlock ();
	return 0L;
      }
      // initialize the local buffer
      t_byte bi[d_cbsz];
      t_byte bo[d_cbsz];
      t_byte bt[d_cbsz];
      // initialize the input buffer
      long cc = 0L;
      for (long i = 0; i < d_cbsz; i++) {
	if (ib.empty () == true) break;
	bi[cc++] = ib.read ();
      }
      // check the block size
      if (cc != d_cbsz) {
	throw Exception ("cipher-error", "invalid block size to decode");
      }
      // preprocess the block mode
      bc_pbi (d_cmod, false, bt, bi, p_bl, d_cbsz);
      // decode the block
      bc_bmd (d_cmod) ? decode (bo, bt) : encode (bo, bt);
      // postprocess the block mode
      bc_pbo (d_cmod, false, bo, bi, p_bl, d_cbsz);
      // check for last block unpadding
      if (ib.empty () == true) {
	cc = bc_unp (d_pmod, bo, d_cbsz);
      }
      // update the output buffer - except the first block
      if (cc > 0L) {
	if ((d_iblk == false) && (d_cmod == CMOD_EBCM)) {
	  cc = 0L;
	  d_iblk = true;
	} else {
	  ob.add ((char*) bo, cc);
	}
      }
      d_decs[0] += cc; d_decs[1] += cc;
      unlock ();
      return cc;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // decode an input stream into an output buffer

  long ModeCipher::decode (Buffer& ob, InputStream& is) {
    wrlock ();
    try {
      // check valid mode
      if (d_rflg == false) {
	throw Exception ("cipher-error", "calling decode in normal mode");
      }      
      // check the block size
      if ((d_cbsz == 0) || (is.valid () == false)) {
	unlock ();
	return 0;
      }
      // initialize the local buffer
      t_byte bi[d_cbsz];
      t_byte bo[d_cbsz];
      t_byte bt[d_cbsz];
      // initialize the input buffer
      long cc = 0L;
      for (long i = 0; i < d_cbsz; i++) {
	if (is.valid () == false) break;
	bi[cc++] = is.read ();
      }
      // check the block size
      if (cc != d_cbsz) {
	throw Exception ("cipher-error", "invalid block size to decode");
      }
      // preprocess the block mode
      bc_pbi (d_cmod, false, bt, bi, p_bl, d_cbsz);
      // decode the block
      bc_bmd (d_cmod) ? decode (bo, bt) : encode (bo, bt);
      // postprocess the block mode
      bc_pbo (d_cmod, false, bo, bi, p_bl, d_cbsz);
      // check for last block unpadding
      if (is.valid () == false) {
	cc = bc_unp (d_pmod, bo, d_cbsz);
      }
      // update the output buffer
      if (cc > 0L) {
	if ((d_iblk == false) && (d_cmod == CMOD_EBCM)) {
	  cc = 0L;
	  d_iblk = true;
	} else {
	  ob.add ((char*) bo, cc);
	}
      }
      d_decs[0] += cc; d_decs[1] += cc;
      unlock ();
      return cc;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // decode an input stream into an output stream

  long ModeCipher::decode (OutputStream& os, InputStream& is) {
    wrlock ();
    try {
      // check valid mode
      if (d_rflg == false) {
	throw Exception ("cipher-error", "calling decode in normal mode");
      }      
      // check the block size
      if ((d_cbsz == 0) || (is.valid () == false)) {
	unlock ();
	return 0;
      }
      // initialize the local buffer
      t_byte bi[d_cbsz];
      t_byte bo[d_cbsz];
      t_byte bt[d_cbsz];
      // initialize the input buffer
      long cc = 0L;
      for (long i = 0; i < d_cbsz; i++) {
	if (is.valid () == false) break;
	bi[cc++] = is.read ();
      }
      // check the block size
      if (cc != d_cbsz) {
	throw Exception ("cipher-error", "invalid block size to decode");
      }
      // preprocess the block mode
      bc_pbi (d_cmod, false, bt, bi, p_bl, d_cbsz);
      // decode the block
      bc_bmd (d_cmod) ? decode (bo, bt) : encode (bo, bt);
      // postprocess the block mode
      bc_pbo (d_cmod, false, bo, bi, p_bl, d_cbsz);
      // check for last block unpadding
      if (is.valid () == false) {
	cc = bc_unp (d_pmod, bo, d_cbsz);
      }
      // update the output buffer
      if (cc > 0L) {
	if ((d_iblk == false) && (d_cmod == CMOD_EBCM)) {
	  cc = 0L;
	  d_iblk = true;
	} else {
	  os.write ((char*) bo, cc);
	}
      }
      d_decs[0] += cc; d_decs[1] += cc;
      unlock ();
      return cc;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // preset the stream processing

  long ModeCipher::preset (void) {
    wrlock ();
    try {
      // preset result
      long result = 0L;
      // preset iv in explicit mode
      if (d_cmod == CMOD_EBCM) {
	// clear the last block
	for (long i = 0; i < d_cbsz; i++) p_bl[i] = nilc;
	// reset the initial vector
	for (long i = 0; i < d_ivsz; i++) p_iv[i] = nilc;
	// set the last block with the initial vector
	bc_pbl (d_cmod, p_bl, p_iv, d_cbsz, d_ivsz);
	d_iblk = false;
      }
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // finish the stream processing

  long ModeCipher::finish (void) {
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

  // the object eval quarks
  static const long QUARK_NONE    = String::intern ("PAD-NONE");
  static const long QUARK_BITM    = String::intern ("PAD-BIT-MODE");
  static const long QUARK_X923    = String::intern ("PAD-ANSI-X923");
  static const long QUARK_N800    = String::intern ("PAD-NIST-800");
  static const long QUARK_ECBM    = String::intern ("MODE-ECB");
  static const long QUARK_CBCM    = String::intern ("MODE-CBC");
  static const long QUARK_EBCM    = String::intern ("MODE-EBC");
  static const long QUARK_CFBM    = String::intern ("MODE-CFB");
  static const long QUARK_OFBM    = String::intern ("MODE-OFB");
  static const long QUARK_BCIPHER = String::intern ("ModeCipher");

  // the quark zone
  static const long QUARK_ZONE_LENGTH = 4;
  static QuarkZone  zone (QUARK_ZONE_LENGTH);

  // the object supported quarks
  static const long QUARK_SETCMOD = zone.intern ("set-block-mode");
  static const long QUARK_GETCMOD = zone.intern ("get-block-mode");
  static const long QUARK_SETPMOD = zone.intern ("set-padding-mode");
  static const long QUARK_GETPMOD = zone.intern ("get-padding-mode");

  // map an enumeration item to a block cipher padding mode
  static inline ModeCipher::t_pmod item_to_pmod (const Item& item) {
    // check for an input cipher item
    if (item.gettid () != QUARK_BCIPHER)
      throw Exception ("item-error", "item is not a block cipher item");
    // map the item to the enumeration
    long quark = item.getquark ();
    if (quark == QUARK_NONE) return ModeCipher::PMOD_NONE;
    if (quark == QUARK_BITM) return ModeCipher::PMOD_BITM;
    if (quark == QUARK_X923) return ModeCipher::PMOD_X923;
    if (quark == QUARK_N800) return ModeCipher::PMOD_N800;
    throw Exception ("item-error",
                     "cannot map item to block cipher padding mode");
  }

  // map an enumeration item to a block cipher mode
  static inline ModeCipher::t_cmod item_to_cmod (const Item& item) {
    // check for an input cipher item
    if (item.gettid () != QUARK_BCIPHER)
      throw Exception ("item-error", "item is not a block cipher item");
    // map the item to the enumeration
    long quark = item.getquark ();
    if (quark == QUARK_ECBM) return ModeCipher::CMOD_ECBM;
    if (quark == QUARK_CBCM) return ModeCipher::CMOD_CBCM;
    if (quark == QUARK_EBCM) return ModeCipher::CMOD_EBCM;
    if (quark == QUARK_CFBM) return ModeCipher::CMOD_CFBM;
    if (quark == QUARK_OFBM) return ModeCipher::CMOD_OFBM;
    throw Exception ("item-error",
                     "cannot map item to block cipher mode");
  }

  // map a block cipher padding mode to an item
  static inline Item* pmod_to_item (const ModeCipher::t_pmod pmod) {
    switch (pmod) {
    case ModeCipher::PMOD_NONE:
      return new Item (QUARK_BCIPHER, QUARK_NONE);
      break;
    case ModeCipher::PMOD_BITM:
      return new Item (QUARK_BCIPHER, QUARK_BITM);
      break;
    case ModeCipher::PMOD_X923:
      return new Item (QUARK_BCIPHER, QUARK_X923);
      break;
    case ModeCipher::PMOD_N800:
      return new Item (QUARK_BCIPHER, QUARK_N800);
      break;
    }
    return nullptr;
  }

  // map a block cipher mode to an item
  static inline Item* cmod_to_item (const ModeCipher::t_cmod cmod) {
    switch (cmod) {
    case ModeCipher::CMOD_ECBM:
      return new Item (QUARK_BCIPHER, QUARK_ECBM);
      break;
    case ModeCipher::CMOD_CBCM:
      return new Item (QUARK_BCIPHER, QUARK_CBCM);
      break;
    case ModeCipher::CMOD_EBCM:
      return new Item (QUARK_BCIPHER, QUARK_EBCM);
      break;
    case ModeCipher::CMOD_CFBM:
      return new Item (QUARK_BCIPHER, QUARK_CFBM);
      break;
    case ModeCipher::CMOD_OFBM:
      return new Item (QUARK_BCIPHER, QUARK_OFBM);
      break;
    }
    return nullptr;
  }

  // evaluate an object data member

  Object* ModeCipher::meval (Evaluable* zobj, Nameset* nset,
			     const long quark) {
    if (quark == QUARK_NONE)
      return new Item (QUARK_BCIPHER, QUARK_NONE);
    if (quark == QUARK_BITM)
      return new Item (QUARK_BCIPHER, QUARK_BITM);
    if (quark == QUARK_X923)
      return new Item (QUARK_BCIPHER, QUARK_X923);
    if (quark == QUARK_N800)
      return new Item (QUARK_BCIPHER, QUARK_N800);
    if (quark == QUARK_ECBM)
      return new Item (QUARK_BCIPHER, QUARK_ECBM);
    if (quark == QUARK_CBCM)
      return new Item (QUARK_BCIPHER, QUARK_CBCM);
    if (quark == QUARK_CFBM)
      return new Item (QUARK_BCIPHER, QUARK_CFBM);
    if (quark == QUARK_OFBM)
      return new Item (QUARK_BCIPHER, QUARK_OFBM);
    throw Exception ("eval-error", "cannot evaluate member",
                     String::qmap (quark));
  }

  // return true if the given quark is defined

  bool ModeCipher::isquark (const long quark, const bool hflg) const {
    rdlock ();
    try {
      if (zone.exists (quark) == true) {
	unlock ();
	return true;
      }
      bool result = hflg ? BlockCipher::isquark (quark, hflg) : false;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // apply this object with a set of arguments and a quark
  
  Object* ModeCipher::apply (Evaluable* zobj, Nameset* nset, const long quark,
			     Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();
    
    // check for 0 argument
    if (argc == 0) {
      if (quark == QUARK_GETPMOD) return pmod_to_item (getpmod ());
      if (quark == QUARK_GETCMOD) return cmod_to_item (getcmod ());
    }
    // check for 1 argument
    if (argc == 1) {
      if (quark == QUARK_SETPMOD) {
	Object* obj = argv->get (0);
	Item*  iobj = dynamic_cast <Item*> (obj);
	if (iobj == nullptr) {
	  throw Exception ("type-error", "invalid object with set padding mode",
			   Object::repr (obj));
	}
	t_pmod pmod = item_to_pmod (*iobj);
	setpmod (pmod);
	return nullptr;
      }
      if (quark == QUARK_SETCMOD) {
	Object* obj = argv->get (0);
	Item*  iobj = dynamic_cast <Item*> (obj);
	if (iobj == nullptr) {
	  throw Exception ("type-error", "invalid object with set mode",
			   Object::repr (obj));
	}
	t_cmod cmod = item_to_cmod (*iobj);
	setcmod (cmod);
	return nullptr;
      }
    }
    // call the block cipher method
    return BlockCipher::apply (zobj, nset, quark, argv);
  }
}
