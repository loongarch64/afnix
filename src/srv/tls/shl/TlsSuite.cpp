// ---------------------------------------------------------------------------
// - TlsSuite.cpp                                                            -
// - afnix:tls service - tls cipher suite class implementation               -
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

#include "Aes.hpp"
#include "Rc4.hpp"
#include "Md5.hpp"
#include "Gcm.hpp"
#include "Sha1.hpp"
#include "Sha256.hpp"
#include "Sha384.hpp"
#include "Vector.hpp"
#include "Boolean.hpp"
#include "Integer.hpp"
#include "Utility.hpp"
#include "TlsUtils.hpp"
#include "TlsSuite.hxx"
#include "QuarkZone.hpp"
#include "Exception.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - private section                                                       -
  // -------------------------------------------------------------------------

  // find a valid cipher info by code
  static long tls_find_cnfo (const long clen, const TlsSuite::s_cnfo* cnfo,
			     const t_word code) {
    if ((clen == 0L) || (cnfo == nullptr)) return -1L;
    for (long k = 0L; k < clen; k++) {
      if (cnfo[k].d_code == code) return k;
    }
    return -1L;
  }

  // create a cipher by info, flag, buffer and initial vector
  static Cipher* tls_make_cifr (const TlsSuite::s_cnfo& cnfo, const bool rflg,
				const Buffer& kbuf, const Buffer& kiv) {
    // the cipher result
    Cipher* result = nullptr;
    // select base cipher
    switch (cnfo.d_cifr) {
    case TlsSuite::TLS_CIFR_NIL:
      break;
    case TlsSuite::TLS_CIFR_AES:
      if (cnfo.d_cmod == TlsSuite::TLS_CMOD_GCM) {
	Aes* cifr = new Aes (kbuf); cifr->setpmod (ModeCipher::PMOD_NONE);
	result = new Gcm (cifr, rflg);
      } else {
	result = new Aes (kbuf, rflg);
      }
      break;
    case TlsSuite::TLS_CIFR_RC4:
      result = new Rc4 (kbuf, rflg);
      break;
    default:
      break;
    };
    // set the block cipher mode / iv
    auto mc = dynamic_cast <ModeCipher*> (result);
    if (mc != nullptr) {
      switch (cnfo.d_cmod) {
      case TlsSuite::TLS_CMOD_NIL:
        mc->setcmod (ModeCipher::CMOD_ECBM);
        break;
      case TlsSuite::TLS_CMOD_CBC:
        mc->setcmod (ModeCipher::CMOD_CBCM);
	mc->setiv (kiv);
        break;
      case TlsSuite::TLS_CMOD_EBC:
        mc->setcmod (ModeCipher::CMOD_EBCM);
        break;
      default:
	break;
      };
      // force the nil padding mode
      mc->setpmod (ModeCipher::PMOD_NONE);
    }
    // set the aead cipher mode / iv
    auto ac = dynamic_cast <AeadCipher*> (result);
    if (ac != nullptr) {
      switch (cnfo.d_cmod) {
      case TlsSuite::TLS_CMOD_GCM:
	ac->setiv (kiv);
        break;
      default:
	break;
      };
    }
    // here it is
    return result;
  }

  // create a hasher by cipher info

  static Hasher* tls_make_hash (const TlsSuite::s_cnfo& cnfo) {
    // the hasher result
    Hasher* result = nullptr;
    // select hmac by hasher
    switch (cnfo.d_hash) {
    case TlsSuite::TLS_HASH_NIL:
      break;
    case TlsSuite::TLS_HASH_MD5:
      result = new Md5;
      break;
    case TlsSuite::TLS_HASH_SHA:
      result = new Sha1;
      break;
    case TlsSuite::TLS_HASH_SHA256:
      result = new Sha256;
      break;
    case TlsSuite::TLS_HASH_SHA384:
      result = new Sha384;
      break;
    };
    return result;
  }
  
  // create a mac by key buffer

  static Hmac* tls_make_hmac (const TlsSuite::s_cnfo& cnfo,
			      const Buffer& kbuf) {
    // create a hasher by info
    Hasher* hash = tls_make_hash (cnfo);
    if (hash == nullptr) return nullptr;
    // create the hmac
    Hmac* result = new Hmac (Key (Key::CKEY_KMAC, kbuf), hash);
    return result;
  }

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a default suite

  TlsSuite::TlsSuite (void) {
    // preset list
    d_clen = 0L;
    p_clst = nullptr;
    d_slen = 0L;
    p_slst = nullptr;
    // update to default version
    update (TLS_VMAJ_DEF, TLS_VMIN_DEF);
  }

  // create a suite by version code

  TlsSuite::TlsSuite (const t_word vcod) {
    // preset lists
    d_clen = 0L;
    p_clst = nullptr;
    d_slen = 0L;
    p_slst = nullptr;
    // update by version
    update (vcod);
  }
  
  // create a suite by version

  TlsSuite::TlsSuite (const t_byte vmaj, const t_byte vmin) {
    // preset lists
    d_clen = 0L;
    p_clst = nullptr;
    d_slen = 0L;
    p_slst = nullptr;
    // update by version
    update (vmaj, vmin);
  }
  
  // destroy this suite

  TlsSuite::~TlsSuite (void) {
    delete [] p_clst;
    delete [] p_slst;
  }

  // return the class name
  
  String TlsSuite::repr (void) const {
    return "TlsSuite";
  }

  // reset the 

  void TlsSuite::reset (void) {
    wrlock ();
    try {
      d_slen = 0L;
      delete [] p_slst;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // get the cipher list info as a plist
  
  Plist TlsSuite::getinfo (void) const {
    rdlock ();
    try {
      // create a result plist
      Plist plst;
      // loop in the list
      for (long k = 0L; k < d_slen; k++) {
	// get the code and validate
	t_word code = p_slst[k];
        if (valid (code) == true) {
	  // get the cipher info by code
	  s_cnfo cnfo = getcnfo (code);
	  // map name, info and value
	  String name = cnfo.p_name;
	  String info = "VALIDITY ";
	  info += Utility::tohexa (cnfo.d_vmin, true, true);
	  info += ":";
	  info += Utility::tohexa (cnfo.d_vmax, true, true);
	  String pval = TlsUtils::toscod (cnfo.d_code);
	  plst.add (name, info, pval);
        }
      }
      // here it is
      unlock ();
      return plst;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // add a new cipher in the list by code

  void TlsSuite::add (const t_word code) {
    wrlock ();
    try {
      // resize the array
      if (d_slen > 0L) {
	t_word* slst = new t_word[d_slen+1L];
	for (long k = 0; k < d_slen; k++) slst[k] = p_slst[k];
	delete [] p_slst; p_slst = slst;
      } else {
	p_slst = new t_word[1];
      }
      // here it is
      p_slst[d_slen++] = code;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // add a new cipher in the list by tls code
  
  void TlsSuite::add (const t_byte ucod, const t_byte lcod) {
    wrlock ();
    try {
      // convert into a word code
      t_word code = TlsUtils::toword (ucod, lcod);
      // and now push it
      add (code);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the suite length

  long TlsSuite::length (void) const {
    rdlock ();
    try {
      long result = d_slen;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get a cifr code by suite index

  t_word TlsSuite::getcifr (const long sidx) const {
    rdlock ();
    try {
      // check for valid index
      if ((sidx < 0L) || (sidx >= d_slen)) {
	throw Exception ("tls-error", "invalid tls suite index");
      }
      t_word result = p_slst[sidx];
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // check if a cipher is valid by code

  bool TlsSuite::valid (const t_word code) const {
    rdlock ();
    try {
      // find a cipher info index by code
      long cidx = tls_find_cnfo (d_clen, p_clst, code);
      // format result
      bool result = (cidx == -1L) ? false : true;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // check if a cipher is valid by code
  
  bool TlsSuite::valid (const t_byte ucod, const t_byte lcod) const {
    rdlock ();
    try {
      // convert into a word code
      t_word code = TlsUtils::toword (ucod, lcod);
      // and now test for it
      bool result = valid (code);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // locate the first valid cipher code

  t_word TlsSuite::locate (void) const {
    rdlock ();
    try {
      for (long k = 0L; k < d_slen; k++) {
	// get the code to check
	t_word code = p_slst[k];
	// check for validity
	if (valid (code) == true) {
	  unlock ();
	  return code;
	}
      }
      throw Exception ("tls-error", "cannot find a valid cipher code");
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // update the cipher info list by version code

  void TlsSuite::update (const t_word vcod) {
    wrlock ();
    try {
      // delete the old list
      delete [] p_clst; d_clen = 0L;
      // loop into the full list
      for (long k = 0L; k < TLS_CNFO_SIZ; k++) {
	// get the cipher info
	s_cnfo cnfo = TLS_CNFO_LST[k];
	// check for validity
	if ((vcod < cnfo.d_vmin) || (vcod > cnfo.d_vmax)) continue;
	// add in the list
	if (d_clen > 0L) {
	  s_cnfo* clst = new s_cnfo[d_clen+1L];
	  for (long k = 0L; k < d_clen; k++) clst[k] = p_clst[k];
	  delete [] p_clst; p_clst = clst;
	} else {
	  p_clst = new s_cnfo[1];
	}
	p_clst[d_clen++] = cnfo;
      }
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // update the cipher info list by version

  void TlsSuite::update (const t_byte vmaj, const t_byte vmin) {
    wrlock ();
    try {
      // convert into a word code
      t_word vcod = TlsUtils::toword (vmaj, vmin);
      // update by version code
      update (vcod);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // create the suite from the cipher info

  void TlsSuite::mksuit (void) {
    wrlock ();
    try {
      // clean old list
      delete [] p_slst; p_slst = nullptr; d_slen = 0L;
      // loop and add
      for (long k = d_clen - 1L; k >= 0; k--) add (p_clst[k].d_code);
      unlock ();;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // get a cipher info structure by code

  TlsSuite::s_cnfo TlsSuite::getcnfo (const t_word code) const {
    rdlock ();
    try {
      // find a standard structure info index by code
      long cidx = tls_find_cnfo (d_clen, p_clst, code);
      if (cidx == -1) {
	throw Exception ("tls-error", "cannot find cipher info with code",
			 Utility::tohexa (code, true, true));
      }
      TlsSuite::s_cnfo result = p_clst[cidx];
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // find a cipher info structure by tls code

  TlsSuite::s_cnfo TlsSuite::getcnfo (const t_byte ucod,
				      const t_byte lcod) const {
    rdlock ();
    try {
      // convert into a word code
      t_word code = TlsUtils::toword (ucod, lcod);
      // and get the cipher info
      TlsSuite::s_cnfo result = getcnfo (code);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get cipher suite as a print table

  PrintTable* TlsSuite::getctbl (void) const {
    rdlock ();
    try {
      // create the table and header
      PrintTable* ptbl = new PrintTable(4);
      ptbl->sethead (0, "NAME");
      ptbl->sethead (1, "CODE");
      ptbl->sethead (2, "MINV");
      ptbl->sethead (3, "MAXV");
      // fill the table
      for (long k = 0L; k < d_clen; k++) {
	long row = ptbl->add ();
	ptbl->set (row, 0, p_clst[k].p_name);
	ptbl->set (row, 1, TlsUtils::toscod (p_clst[k].d_code));
	ptbl->set (row, 2, Utility::tohexa (p_clst[k].d_vmin, true, true));
	ptbl->set (row, 3, Utility::tohexa (p_clst[k].d_vmax, true, true));
      }
      unlock ();
      return ptbl;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get a hasher by code

  Hasher* TlsSuite::gethasher (const t_word  code) const {
    rdlock ();
    try {
      // locate the cipher info
      s_cnfo cnfo = getcnfo (code);
      // create the hasher result
      Hasher* result = tls_make_hash (cnfo);
      // here it is
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // get a cipher by code and key buffer

  Cipher* TlsSuite::getcipher (const t_word  code, const bool rflg,
			       const Buffer& kbuf, const Buffer& kiv) const {
    rdlock ();
    try {
      // locate the cipher info
      s_cnfo cnfo = getcnfo (code);
      // create the cipher result
      Cipher* result = tls_make_cifr (cnfo, rflg, kbuf, kiv);
      // here it is
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get a hmac by code and key buffer

  Hmac* TlsSuite::gethmac (const t_word code, const Buffer& kbuf) const {
    rdlock ();
    try {
      // locate the cipher info
      s_cnfo cnfo = getcnfo (code);
      // create the hmac result
      Hmac* result = tls_make_hmac (cnfo, kbuf);
      // here it is
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
  static const long QUARK_UPDATE  = zone.intern ("update");
  static const long QUARK_GETCTBL = zone.intern ("get-cipher-table");

  // create a new object in a generic way

  Object* TlsSuite::mknew (Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();
    
    // check for 0 argument
    if (argc == 0) return new TlsSuite;
    // check for 1 argument
    if (argc == 1) {
      t_word vcod = argv->getword (0);
      return new TlsSuite (vcod);
    }
    // check for 2 arguments
    if (argc == 2) {
      t_word vmaj = argv->getbyte (0);
      t_word vmin = argv->getbyte (1);
      return new TlsSuite (vmaj, vmin);
    }
    // too many arguments
    throw Exception ("argument-error", 
                     "too many argument with tls cipher suite");
  }

  // return true if the given quark is defined

  bool TlsSuite::isquark (const long quark, const bool hflg) const {
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

  Object* TlsSuite::apply (Evaluable* zobj, Nameset* nset, const long quark,
			   Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();
    
    // check for 0 argument
    if (argc == 0) {
      if (quark == QUARK_GETCTBL) return getctbl ();
    }
    // check for 1 argument
    if (argc == 1) {
      if (quark == QUARK_UPDATE){
	t_word vcod = argv->getword (0);
	update (vcod);
	return nullptr;
      }
    }
    // check for 2 arguments
    if (argc == 2) {
      if (quark == QUARK_UPDATE){
	t_word vmaj = argv->getbyte (0);
	t_word vmin = argv->getbyte (1);
	update (vmaj, vmin);
	return nullptr;
      }
    }
    // call the tls infos method
    return TlsInfos::apply (zobj, nset, quark, argv);
  }
}
