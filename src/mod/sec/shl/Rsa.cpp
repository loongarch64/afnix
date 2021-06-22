// ---------------------------------------------------------------------------
// - Rsa.cpp                                                                 -
// - afnix:sec module - rsa public cipher class implementation               -
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

#include "Rsa.hpp"
#include "Item.hpp"
#include "Kdf1.hpp"
#include "Kdf2.hpp"
#include "Vector.hpp"
#include "Unicode.hpp"
#include "Integer.hpp"
#include "Utility.hpp"
#include "Evaluable.hpp"
#include "QuarkZone.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - private section                                                       -
  // -------------------------------------------------------------------------

  // rsa constant
  static const char* RSA_ALGO_NAME = "RSA";

  // this procedure generates a non-null pseudo random byte
  static inline t_byte rsa_pkcs_rand (void) {
    t_byte result = 0x00;
    while (result == 0x00) {
      result = Utility::byternd ();
    }
    return result;
  }

  // this procedure pads a message in PKCS 1.5 type 1 mode
  static void rsa_pm_pkcs_11 (t_byte* bp, const long bpsz,
			      const t_byte* bi, const long bisz) {
    // initialize pad index
    long pi = 0;
    // mark type 1 padding
    bp[pi++] = 0x00;
    bp[pi++] = 0x01;
    // add padding string
    long ps = bpsz - bisz - 3;
    if (ps < 8) {
      throw Exception ("rsa-error", "type 1 padding is too small");
    }
    for (long i = 0; i < ps; i++) bp[pi++] = 0xFF;
    // mark 0 padding
    bp[pi++] = 0x00;
    // add the message
    for (long i = 0; i < bisz; i++) bp[pi++] = bi[i];
    // verification
    if (pi != bpsz) {
      throw Exception ("internal-error", "invalid rsa padding buffer index");
    }
  }

  // this procedure unpad a message in PKCS 1.5 type 1 mode
  static long rsa_um_pkcs_11 (t_byte* bo, const t_byte* bp, const long bpsz) {
    // check base size first
    if (bpsz < 11) {
      throw Exception ("rsa-error", "invalid padded buffer size");
    }
    // check type 1 marking
    long bpdx = 0;
    if (bp[bpdx++] != 0x00) {
      throw Exception ("rsa-error", "invalid padded buffer marker");
    }
    if (bp[bpdx++] != 0x01) {
      throw Exception ("rsa-error", "invalid padded buffer marker");
    }
    // remove the padding string
    while (bp[bpdx++] == 0xFF) {
      if (bpdx >= bpsz) {
	throw Exception ("rsa-error", "invalid padded buffer marker");
      }
      if (bp[bpdx] == 0x00) break;
    }
    // check padding terminal
    if (bp[bpdx++] != 0x00) {
      throw Exception ("rsa-error", "invalid padded buffer marker");
    }    
    // copy the result buffer
    long bcnt = 0;
    for (long i = bpdx; i < bpsz; i++) bo[bcnt++] = bp[i];
    // here is the message length
    return bcnt;
  }

  // this procedure pads a message in PKCS 1.5 type 2 mode
  static void rsa_pm_pkcs_12 (t_byte* bp, const long bpsz,
			      const t_byte* bi, const long bisz) {
    // initialize pad index
    long pi = 0;
    // mark type 2 padding
    bp[pi++] = 0x00;
    bp[pi++] = 0x02;
    // add padding string
    long ps = bpsz - bisz - 3;
    if (ps < 8) {
      throw Exception ("rsa-error", "type 1 padding is too small");
    }
    for (long i = 0; i < ps; i++) bp[pi++] = rsa_pkcs_rand ();
    // mark 0 padding
    bp[pi++] = 0x00;
    // add the message
    for (long i = 0; i < bisz; i++) bp[pi++] = bi[i];
    // verification
    if (pi != bpsz) {
      throw Exception ("internal-error", "invalid rsa padding buffer index");
    }
  }

  // this procedure unpad a message in PKCS 1.5 type 2 mode
  static long rsa_um_pkcs_12 (t_byte* bo, const t_byte* bp, const long bpsz) {
    // check base size first
    if (bpsz < 11) {
      throw Exception ("rsa-error", "invalid padded buffer size");
    }
    // check type 2 marking
    long bpdx = 0;
    if (bp[bpdx++] != 0x00) {
      throw Exception ("rsa-error", "invalid padded buffer marker");
    }
    if (bp[bpdx++] != 0x02) {
      throw Exception ("rsa-error", "invalid padded buffer marker");
    }
    // remove the padding string
    while (bp[bpdx++] != 0x00) {
      if (bpdx >= bpsz) {
	throw Exception ("rsa-error", "invalid padded buffer marker");
      }
    }
    // copy the result buffer
    long bcnt = 0;
    for (long i = bpdx; i < bpsz; i++) bo[bcnt++] = bp[i];
    // here is the message length
    return bcnt;
  }

  // this procedure pads a message in OAEP mode - if kflg is true then
  // use the KDF2 object instead of the default KDF1
  static void rsa_pm_oaep_kx (t_byte* bp, const long bpsz, 
			      const t_byte* bi, const long bisz, 
			      const String& labl, const String& seds,
			      Hasher* hobj, const bool kflg) {
    // get the hasher result length
    long hlen = hobj->getrlen ();
    // allocate the data block
    long dbsz = bpsz - hlen - 1;
    long pssz = dbsz - hlen - bisz - 1;
    t_byte db[dbsz];
    if (pssz < 0) {
      throw Exception ("rsa-error", "invalid rsa-oaep data block size");
    }
    // fill the data block
    hobj->compute (labl);
    for (long i = 0; i < hlen; i++) db[i] = hobj->getbyte (i);
    for (long i = 0, j = hlen; i < pssz; i++, j++) db[j] = nilc;
    db[hlen + pssz] = 0x01;
    for (long i = 0, j = hlen+pssz+1; i < bisz; i++, j++) db[j] = bi[i];
    // set the seed - either randomly or from the octet string
    t_byte seed[hlen];
    if (seds.isnil () == true) {
      for (long i = 0; i < hlen; i++) seed[i] = Utility::byternd ();
    } else {
      long slen = 0;
      t_byte* sbuf = Unicode::stob (slen, seds);
      if (slen != hlen) {
	delete [] sbuf;
	throw Exception ("rsa-error", "invalid rsa oaep seed", seds);
      }
      for (long i = 0; i < hlen; i++) seed[i] = sbuf[i];
      delete [] sbuf;
    }
    // create the mask generation function objects
    Hkdf* dkdf = nullptr;
    Hkdf* skdf = nullptr;
    if (kflg == true) {
      dkdf = new Kdf2 (hobj, dbsz);
      skdf = new Kdf2 (hobj, hlen);
    } else {
      dkdf = new Kdf1 (hobj, dbsz);
      skdf = new Kdf1 (hobj, hlen);
    }
    // build the padded block
    try {
      // derive the seed mask
      dkdf->derive (seed, hlen);
      // compute the masked data block
      for (long i = 0; i < dbsz; i++) db[i] = db[i] ^ dkdf->getbyte (i);
      // derive the seed mask
      skdf->derive (db, dbsz);
      // update the padded block
      bp[0] = 0x00;
      for (long i = 0, j = 1; i < hlen; i++, j++) {
	bp[j] = seed[i] ^ skdf->getbyte (i);
      }
      for (long i = 0, j = hlen + 1; i < dbsz; i++, j++) bp[j] = db[i];
      // clean kdf function
      delete dkdf;
      delete skdf;
    } catch (...) {
      delete dkdf;
      delete skdf;
      throw;
    }
  }

  // this procedure unpad a message in OAEP mode - if kflg is true then
  // use the KDF2 object instead of the default KDF1
  static long rsa_um_oaep_kx (t_byte* bo, const t_byte* bp, const long bpsz, 
			      const String& labl, Hasher* hobj, 
			      const bool kflg) {
    // get the hasher result length
    long hlen = hobj->getrlen ();
    // check base size first
    if (bpsz < (2*hlen + 2)) {
      throw Exception ("rsa-error", "invalid padded buffer size");
    }
    // check base marking
    if (bp[0] != 0x00) {
      throw Exception ("rsa-error", "invalid padded buffer marker");
    }
    // extract the masked seed
    t_byte seed[hlen];
    for (long i = 0, j = 1; i < hlen; i++, j++) seed[i] = bp[j];
    // extract the masked data block
    long dbsz = bpsz - hlen - 1;
    t_byte db[dbsz];
    for (long i = 0, j = hlen + 1; i < dbsz; i++, j++) db[i] = bp[j];
    // create the mask generation function objects
    Hkdf* dkdf = nullptr;
    Hkdf* skdf = nullptr;
    if (kflg == true) {
      dkdf = new Kdf2 (hobj, dbsz);
      skdf = new Kdf2 (hobj, hlen);
    } else {
      dkdf = new Kdf1 (hobj, dbsz);
      skdf = new Kdf1 (hobj, hlen);
    }
    try {
      // compute the seed mask
      skdf->derive (db, dbsz);
      // extract the seed
      for (long i = 0; i < hlen; i++) seed[i] ^= skdf->getbyte (i);
      // compute the db mask
      dkdf->derive (seed, hlen);
      // extract the data block
      for (long i = 0; i < dbsz; i++) db[i] ^= dkdf->getbyte (i);
      // compute the hashed label and verify
      hobj->compute (labl);
      for (long i = 0; i < hlen; i++) {
	if (db[i] != hobj->getbyte (i)) {
	  throw Exception ("rsa-error", "invalid padded buffer marker");
	}
      }
      // extract the padded string
      long bpdx = hlen;
      while (bpdx < dbsz) {
	if (db[bpdx] == 0x01) break;
	if (db[bpdx++] == 0x00) continue;
	throw Exception ("rsa-error", "invalid padded buffer marker");
      }
      // verify marker
      if ((bpdx >= dbsz) || (db[bpdx++] != 0x01)) {
	throw Exception ("rsa-error", "invalid padded buffer marker");
      }
      // copy the result buffer
      long bcnt = 0;
      for (long i = bpdx; i < dbsz; i++) bo[bcnt++] = db[i];
      // clean the kdf functions
      delete dkdf;
      delete skdf;
      // here is the message length
      return bcnt;
    } catch (...) {
      delete dkdf;
      delete skdf;
      throw;
    }
  }

  // this procedure pad a message from a padding type
  static void rsa_pm (const Rsa::t_pmod pmod, t_byte* bp, const long bpsz,
		      const t_byte* bi, const long bisz,
		      const String& labl, const String& seds, Hasher* hobj) {
    switch (pmod) {
    case Rsa::PAD_PKCS11:
      rsa_pm_pkcs_11 (bp, bpsz, bi, bisz);
      break;
    case Rsa::PAD_PKCS12:
      rsa_pm_pkcs_12 (bp, bpsz, bi, bisz);
      break;
    case Rsa::PAD_OAEPK1:
      rsa_pm_oaep_kx (bp, bpsz, bi, bisz, labl, seds, hobj, false);
      break;
    case Rsa::PAD_OAEPK2:
      rsa_pm_oaep_kx (bp, bpsz, bi, bisz, labl, seds, hobj, true);
      break;
    }
  }

  // this procedure unpad a message from a padding type
  static long rsa_um (const Rsa::t_pmod pmod, t_byte* bo, 
		      const t_byte* bp, const long bpsz, 
		      const String& labl, Hasher* hobj) {
    long result = 0;
    switch (pmod) {
    case Rsa::PAD_PKCS11:
      result = rsa_um_pkcs_11 (bo, bp, bpsz);
      break;
    case Rsa::PAD_PKCS12:
      result = rsa_um_pkcs_12 (bo, bp, bpsz);
      break;
    case Rsa::PAD_OAEPK1:
      result = rsa_um_oaep_kx (bo, bp, bpsz, labl, hobj, false);
      break;
    case Rsa::PAD_OAEPK2:
      result = rsa_um_oaep_kx (bo, bp, bpsz, labl, hobj, true);
      break;
    }
    return result;
  }

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a default rsa object

  Rsa::Rsa (void) : PublicCipher (RSA_ALGO_NAME) {
    // set the padding mode
    d_pmod = PAD_PKCS12;
    p_hobj = nullptr;
    // set the default key
    setkey (Key (Key::CKEY_KRSA, 1024));
  }
    
  // create a rsa cipher by key

  Rsa::Rsa (const Key& key) : PublicCipher (RSA_ALGO_NAME) {
    // set the padding mode
    d_pmod = PAD_PKCS12;
    p_hobj = nullptr;
    // set the key
    setkey (key);
  }

  // create a rsa cipher by key and reverse flag

  Rsa::Rsa (const Key& key, const bool rflg) : PublicCipher (RSA_ALGO_NAME) {
    // set the padding mode
    d_pmod = PAD_PKCS12;
    p_hobj = nullptr;
    // set the key
    setkey (key);
    // set the reverse flag
    setrflg (rflg);
  }

  // create a rsa cipher with a hasher object and a label
  Rsa::Rsa (const Key& key, Hasher* hobj, 
	    const String& labl) : PublicCipher (RSA_ALGO_NAME) {
    // force the padding mode
    d_pmod = PAD_OAEPK1;
    // set the lable and hasherR
    d_labl = labl;
    Object::iref (p_hobj = hobj);
    // set the key
    setkey (key);
  }

  // destroy this rsa object

  Rsa::~Rsa (void) {
    Object::dref (p_hobj);
  }

  // return the class name

  String Rsa::repr (void) const {
    return "Rsa";
  }

  // reset this rsa cipher
  
  void Rsa::reset (void) {
    wrlock ();
    try {
      // reset base cipher
      PublicCipher::reset ();
      // reset locally
      d_kmod = 0;
      d_kexp = 0;
      d_cbsz = 0L;
      d_mbsz = 0L;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    } 
  }

  // clear this rsa cipher
  
  void Rsa::clear (void) {
    wrlock ();
    try {
      // clear the base cipher
      PublicCipher::clear ();
      // expand the key
      if (d_rflg == false) {
	d_kmod = d_ckey.getrkey (Key::KRSA_PMOD);
	d_kexp = d_ckey.getrkey (Key::KRSA_PEXP);
      } else {
	d_kmod = d_ckey.getrkey (Key::KRSA_PMOD);
	d_kexp = d_ckey.getrkey (Key::KRSA_SEXP);
      }
      // compute message and block size
      d_cbsz = d_ckey.getsize ();
      switch (d_pmod) {
      case PAD_PKCS11:
      case PAD_PKCS12:
	d_mbsz = d_cbsz - 11;
	break;
      case PAD_OAEPK1:
      case PAD_OAEPK2:
	d_mbsz = (p_hobj == nullptr) ? 0 : d_cbsz - 2 * p_hobj->gethlen () - 2;
	break;
      }
      if (d_mbsz <= 0) {
	throw Exception ("rsa-error", "invalid message block size in clear");
      }
      // done
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    } 
  }

  // set the cipher key
  
  void Rsa::setkey (const Key& key) {
    wrlock ();
    try {
      // check the key
      if (key.gettype () != Key::CKEY_KRSA) {
	throw Exception ("rsa-error", "invalid key type for rsa cipher");
      }
      // set the cipher key
      PublicCipher::setkey (key);
      // clear everything
      clear ();
      // done
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set the cipher reverse flag

  void Rsa::setrflg (const bool rflg) {
    try {
      // set the cipher flag
      Cipher::setrflg (rflg);
      // clear and expand key
      clear ();
      // done
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set the cipher padding mode

  void Rsa::setpmod (const t_pmod pmod) {
    wrlock ();
    try {
      d_pmod = pmod;
      clear ();
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the cipher padding mode

  Rsa::t_pmod Rsa::getpmod (void) const {
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

  // set the oaep padding label

  void Rsa::setlabl (const String& labl) {
    wrlock ();
    try {
      d_labl = labl;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the oaep padding label

  String Rsa::getlabl (void) const {
    rdlock ();
    try {
      String result = d_labl;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set the oaep padding seed

  void Rsa::setseds (const String& seds) {
    wrlock ();
    try {
      d_seds = seds;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the oaep padding label

  String Rsa::getseds (void) const {
    rdlock ();
    try {
      String result = d_seds;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set the oaep hasher object

  void Rsa::sethobj (Hasher* hobj) {
    wrlock ();
    try {
      Object::iref (hobj);
      Object::dref (p_hobj);
      p_hobj = hobj;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the oaep hasher object

  Hasher* Rsa::gethobj (void) const {
    rdlock ();
    try {
      Hasher* result = p_hobj;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // pkcs encryption/decryption primitive

  Relatif Rsa::pkcsed (const Relatif& m) const {
    rdlock ();
    try {
      // check for valid message
      if ((m < 0) || (m >= d_kmod)) {
	throw Exception ("rsa-error", "out-of-range message representative");
      }
      // compute result
      Relatif result = m.pow (d_kexp, d_kmod);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    } 
  }

  // encode a block buffer into another one
  
  long Rsa::encode (t_byte* bo, const t_byte* bi, const long cc) {
    wrlock ();
    try {
      // the local padding buffer
      t_byte bp[d_cbsz];
      // the output block size
      long result = 0;
      // pad the message
      rsa_pm (d_pmod, bp, d_cbsz, bi, cc, d_labl, d_seds, p_hobj);
      // bind a relatif value
      Relatif mi (bp, d_cbsz);
      // compute the value
      Relatif mo = pkcsed (mi);
      // map the relatif to byte
      if (mo.tobyte (bo, d_cbsz) > d_cbsz) {
	throw Exception ("rsa-error", "invalid relatif to byte conversion");
      }
      result = d_cbsz;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }    
  }

  // decode a block buffer into another one

  long Rsa::decode (t_byte* bo, const t_byte* bi, const long cc) {
    wrlock ();
    try {
      // the local padding buffer
      t_byte bp[d_cbsz];
      // the output block size
      long result = 0;
      // check the size
      if (cc != d_cbsz) {
	throw Exception ("rsa-error", "invalid block size in reverse mode");
      }
      // bind a relatif value
      Relatif mi (bi, cc);
      // compute the value
      Relatif mp = pkcsed (mi);
      // map the relatif to byte
      if (mp.tobyte (bp, d_cbsz) > d_cbsz) {
	throw Exception ("rsa-error", "invalid relatif to byte conversion");
      }
      // unpad the message
      result = rsa_um (d_pmod, bo, bp, d_cbsz, d_labl, p_hobj);
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
  static const long QUARK_RSA    = String::intern ("Rsa");
  static const long QUARK_PKCS11 = String::intern ("PAD-PKCS-11");
  static const long QUARK_PKCS12 = String::intern ("PAD-PKCS-12");
  static const long QUARK_OAEPK1 = String::intern ("PAD-OAEP-K1");
  static const long QUARK_OAEPK2 = String::intern ("PAD-OAEP-K2");

  // the quark zone
  static const long QUARK_ZONE_LENGTH = 9;
  static QuarkZone  zone (QUARK_ZONE_LENGTH);

  // the object supported quarks
  static const long QUARK_SETHOBJ  = zone.intern ("set-hasher");
  static const long QUARK_GETHOBJ  = zone.intern ("get-hasher");
  static const long QUARK_SETPMOD  = zone.intern ("set-padding-mode");
  static const long QUARK_GETPMOD  = zone.intern ("get-padding-mode");
  static const long QUARK_SETPLABL = zone.intern ("set-padding-label");
  static const long QUARK_GETPLABL = zone.intern ("get-padding-label");
  static const long QUARK_SETPSEED = zone.intern ("set-padding-seed");
  static const long QUARK_GETPSEED = zone.intern ("get-padding-seed");
  static const long QUARK_PKCSPRIM = zone.intern ("pkcs-primitive");

  // map an enumeration item to a padding mode
  static inline Rsa::t_pmod item_to_pmod (const Item& item) {
    // check for an input cipher item
    if (item.gettid () != QUARK_RSA)
      throw Exception ("item-error", "item is not a rsa item");
    // map the item to the enumeration
    long quark = item.getquark ();
    if (quark == QUARK_PKCS11) return Rsa::PAD_PKCS11;
    if (quark == QUARK_PKCS12) return Rsa::PAD_PKCS12;
    if (quark == QUARK_OAEPK1) return Rsa::PAD_OAEPK1;
    if (quark == QUARK_OAEPK2) return Rsa::PAD_OAEPK2;
    throw Exception ("item-error", 
		     "cannot map item to rsa padding mode");
  }
  
  // map a rsa padding mode to an item
  static inline Item* pmod_to_item (const Rsa::t_pmod pmod) {
    switch (pmod) {
    case Rsa::PAD_PKCS11:
      return new Item (QUARK_RSA, QUARK_PKCS11);
      break;
    case Rsa::PAD_PKCS12:
      return new Item (QUARK_RSA, QUARK_PKCS12);
      break;
    case Rsa::PAD_OAEPK1:
      return new Item (QUARK_RSA, QUARK_OAEPK1);
      break;
    case Rsa::PAD_OAEPK2:
      return new Item (QUARK_RSA, QUARK_OAEPK2);
      break;
    }
    return nullptr;
  }
  
  // evaluate an object data member
  
  Object* Rsa::meval (Evaluable* zobj, Nameset* nset, const long quark) {
    if (quark == QUARK_PKCS11) 
      return new Item (QUARK_RSA, QUARK_PKCS11);
    if (quark == QUARK_PKCS12) 
      return new Item (QUARK_RSA, QUARK_PKCS12);
    if (quark == QUARK_OAEPK1) 
      return new Item (QUARK_RSA, QUARK_OAEPK1);
    if (quark == QUARK_OAEPK2) 
      return new Item (QUARK_RSA, QUARK_OAEPK2);
    throw Exception ("eval-error", "cannot evaluate member",
		     String::qmap (quark));
  }

  // create a new object in a generic way
  
  Object* Rsa::mknew (Vector* argv) {
    long argc = (argv == nullptr) ? 0 : argv->length ();
    // check for 0 argument
    if (argc == 0) return new Rsa;
    // check for 1 argument
    if (argc == 1) {
      // check for a key
      Object* obj = argv->get (0);
      Key*    key = dynamic_cast <Key*> (obj);
      if (key != nullptr) return new Rsa (*key);
      throw Exception ("argument-error", 
		       "invalid arguments with rsa", Object::repr (obj));
    }
    // check for 2 arguments
    if (argc == 2) {
      // check for a key
      Object* obj = argv->get (0);
      Key*    key  = dynamic_cast <Key*> (obj);
      if (key == nullptr) {
	throw Exception ("argument-error", 
			 "invalid arguments with rsa", Object::repr (obj));
      }
      // get the reverse flag and object
      bool rflg = argv->getbool (1);
      return new Rsa (*key, rflg);
    }
    // check for 3 arguments
    if (argc == 3) {
      // check for a key
      Object* obj = argv->get (0);
      Key*    key  = dynamic_cast <Key*> (obj);
      if (key == nullptr) {
	throw Exception ("argument-error", 
			 "invalid arguments with rsa", Object::repr (obj));
      }
      // check for a hasher
      obj = argv->get (1);
      Hasher* hobj  = dynamic_cast <Hasher*> (obj);
      if (hobj == nullptr) {
	throw Exception ("argument-error", 
			 "invalid arguments with rsa", Object::repr (obj));
      }
      // get the oaep padding label
      String labl = argv->getstring (2);
      return new Rsa (*key, hobj, labl);
    }
    throw Exception ("argument-error", "too many arguments with rsa");
  }

  // return true if the given quark is defined

  bool Rsa::isquark (const long quark, const bool hflg) const {
    rdlock ();
    try {
      if (zone.exists (quark) == true) {
	unlock ();
	return true;
      }
      bool result = hflg ? PublicCipher::isquark (quark, hflg) : false;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // apply this object with a set of arguments and a quark
  
  Object* Rsa::apply (Evaluable* zobj, Nameset* nset, const long quark,
		      Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();
    
    // check for 0 argument
    if (argc == 0) {
      if (quark == QUARK_GETPMOD)  return pmod_to_item (getpmod ());
      if (quark == QUARK_GETPLABL) return new String (getlabl ());
      if (quark == QUARK_GETPSEED) return new String (getseds ());
      if (quark == QUARK_GETHOBJ) {
	rdlock ();
	try {
	  Object* result = gethobj ();
	  zobj->post (result);
	  unlock ();
	  return result;
	} catch (...) {
	  unlock ();
	  throw;
	}
      }
    }
    // check for 1 argument
    if (argc == 1) {
      if (quark == QUARK_SETPLABL) {
	String labl = argv->getstring (0);
	setlabl (labl);
	return nullptr;
      }
      if (quark == QUARK_SETPSEED) {
	String seds = argv->getstring (0);
	setseds (seds);
	return nullptr;
      }
      if (quark == QUARK_SETHOBJ) {
	Object*  obj = argv->get (0);
	Hasher* hobj = dynamic_cast <Hasher*> (obj);
        if (hobj == nullptr) {
	  throw Exception ("type-error", "invalid object with set-hasher",
			   Object::repr (obj));
	}
	sethobj (hobj);
	return nullptr;
      }
      if (quark == QUARK_PKCSPRIM) {
	// get the relatif message
	Object* obj = argv->get (0);
	Relatif msg;
        // check for an integer
        Integer* im = dynamic_cast <Integer*> (obj);
        if (im != nullptr) msg = im->tolong ();
        // check for a relatif
        Relatif* rm = dynamic_cast <Relatif*> (obj);
        if (rm != nullptr) msg = *rm;
        // type error with base
        if ((im == nullptr) && (rm == nullptr)) {
          throw Exception ("type-error", "invalid object with pkcs primitive",
                           Object::repr (obj));
        }
	// convert the message
	return new Relatif (pkcsed (msg));
      }
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
    }
    // call the public cipher method
    return PublicCipher::apply (zobj, nset, quark, argv);
  }
}
