// ---------------------------------------------------------------------------
// - TlsState.cpp                                                            -
// - afnix:tls service - tls state class implementation                      -
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
#include "Boolean.hpp"
#include "Integer.hpp"
#include "TlsUtils.hpp"
#include "TlsState.hpp"
#include "TlsTypes.hxx"
#include "TlsSuite.hpp"
#include "Evaluable.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"
#include "TlsExtension.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - private section                                                       -
  // -------------------------------------------------------------------------

  // the default flags
  static const bool   STA_SFLG_DEF = false;
  // the default cipher
  static const t_word STA_CIFR_DEF = 0x0000U;
  // the default size
  static const long   STA_KSIZ_DEF = 0L;
  static const long   STA_BSIZ_DEF = 0L;
  static const long   STA_VSIZ_DEF = 0L;
  static const long   STA_HSIZ_DEF = 0L;
  static const t_octa STA_CNUM_DEF = nilo;
  static const t_octa STA_SNUM_DEF = nilo;
  
  // the state name and info
  static const String PN_STA_SFLG  = "TLS-STA-SFLG";
  static const String PI_STA_SFLG  = "TLS STATE SERVER FLAG";
  static const String PN_STA_RMAJ  = "TLS-STA-RMAJ";
  static const String PI_STA_RMAJ  = "TLS STATE MAJOR REQUESTED";
  static const String PN_STA_RMIN  = "TLS-STA-RMIN";
  static const String PI_STA_RMIN  = "TLS STATE MINOR REQUESTED";
  static const String PN_STA_VMAJ  = "TLS-STA-VMAJ";
  static const String PI_STA_VMAJ  = "TLS STATE MAJOR VERSION";
  static const String PN_STA_VMIN  = "TLS-STA-VMIN";
  static const String PI_STA_VMIN  = "TLS STATE MINOR VERSION";
  static const String PN_STA_CRND  = "TLS-STA-CRND";
  static const String PI_STA_CRND  = "TLS STATE CLIENT RANDOM";
  static const String PN_STA_SRND  = "TLS-STA-SRND";
  static const String PI_STA_SRND  = "TLS STATE SERVER RANDOM";
  static const String PN_STA_MKEY  = "TLS-STA-MKEY";
  static const String PI_STA_MKEY  = "TLS STATE PREMASTER KEY";
  static const String PN_STA_MBUF  = "TLS-STA-MBUF";
  static const String PI_STA_MBUF  = "TLS STATE MASTER SECRET";
  static const String PN_STA_EBUF  = "TLS-STA-EBUF";
  static const String PI_STA_EBUF  = "TLS STATE KEY EXPANSION";
  static const String PN_STA_CMAC  = "TLS-STA-CMAC";
  static const String PI_STA_CMAC  = "TLS STATE CLIENT MAC KEY";
  static const String PN_STA_SMAC  = "TLS-STA-SMAC";
  static const String PI_STA_SMAC  = "TLS STATE SERVER MAC KEY";
  static const String PN_STA_CKEY  = "TLS-STA-CKEY";
  static const String PI_STA_CKEY  = "TLS STATE CLIENT CIPHER KEY";
  static const String PN_STA_SKEY  = "TLS-STA-SKEY";
  static const String PI_STA_SKEY  = "TLS STATE SERVER CIPHER KEY";
  static const String PN_STA_CIVB  = "TLS-STA-CIVB";
  static const String PI_STA_CIVB  = "TLS STATE CLIENT IV";
  static const String PN_STA_SIVB  = "TLS-STA-SIVB";
  static const String PI_STA_SIVB  = "TLS STATE SERVER IV";
  static const String PN_STA_CIFR  = "TLS-STA-CIFR";
  static const String PI_STA_CIFR  = "TLS STATE CIPHER CODE";
  static const String PN_STA_STEL  = "TLS-STA-STEL";
  static const String PI_STA_STEL  = "TLS STATE SUITE LENGTH";
  static const String PN_STA_EXTL  = "TLS-STA-EXTL";
  static const String PI_STA_EXTL  = "TLS STATE EXTENSION LENGTH";

  // this procedure create a plist with extension info
  static Plist tls_exts_plst (const Vector& exts) {
    Plist result;
    // loop in the vector
    long xlen = exts.length ();
    for (long k = 0L; k < xlen; k++) {
      // get the extension
      auto extn = dynamic_cast<TlsExtension*>(exts.get (k));
      if (extn == nullptr) continue;
      // get the information plist
      result = result.merge(extn->getinfo(), k);
    }
    return result;
  }
  
  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a default tls state

  TlsState::TlsState (void) {
    p_cbcf = nullptr;
    p_sbcf = nullptr;
    p_chmc = nullptr;
    p_shmc = nullptr;
    p_suit = nullptr;
    p_cert = nullptr;
    p_exts = nullptr;
    reset ();
  }

  // create a tls state by flag

  TlsState::TlsState (const bool sflg) {
    p_cbcf = nullptr;
    p_sbcf = nullptr;
    p_chmc = nullptr;
    p_shmc = nullptr;
    p_suit = nullptr;
    p_cert = nullptr;
    p_exts = nullptr;
    reset ();
    d_sflg = sflg;
  }

  // create a tls state by flag and parameters

  TlsState::TlsState (const bool sflg, const TlsParams& prms) {
    p_cbcf = nullptr;
    p_sbcf = nullptr;
    p_chmc = nullptr;
    p_shmc = nullptr;
    p_suit = nullptr;
    p_cert = nullptr;
    p_exts = nullptr;
    reset ();
    d_sflg = sflg;
    d_prms = prms;
    d_rmaj = prms.getvmaj ();
    d_rmin = prms.getvmin ();
    d_vmaj = d_rmaj;
    d_vmin = d_rmin;
    setcert (prms.getcert (), prms.getckey ());
  }

  // copy construct this tls state

  TlsState::TlsState (const TlsState& that) {
    that.rdlock ();
    try {
      d_sflg = that.d_sflg;
      d_rmaj = that.d_rmaj;
      d_rmin = that.d_rmin;
      d_vmaj = that.d_vmaj;
      d_vmin = that.d_vmin;
      d_cifr = that.d_cifr;
      d_ksiz = that.d_ksiz;
      d_bsiz = that.d_bsiz;
      d_vsiz = that.d_vsiz;
      d_hsiz = that.d_hsiz;
      d_cnum = that.d_cnum;
      d_snum = that.d_snum;
      d_crnd = that.d_crnd;
      d_srnd = that.d_srnd;
      d_mbuf = that.d_mbuf;
      d_ebuf = that.d_ebuf;
      d_cmac = that.d_cmac;
      d_smac = that.d_smac;
      d_ckey = that.d_ckey;
      d_skey = that.d_skey;
      d_civb = that.d_civb;
      d_sivb = that.d_sivb;
      d_info = that.d_info;
      d_hvmb = that.d_hvmb;
      Object::iref (p_cbcf = that.p_cbcf);
      Object::iref (p_sbcf = that.p_sbcf);
      Object::iref (p_chmc = that.p_chmc);
      Object::iref (p_shmc = that.p_shmc);
      Object::iref (p_suit = that.p_suit);
      Object::iref (p_cert = that.p_cert);
      Object::iref (p_exts = that.p_exts);
      that.unlock ();
    } catch (...) {
      that.unlock ();
      throw;
    }
  }

  // destroy this state

  TlsState::~TlsState (void) {
    Object::dref (p_cbcf);
    Object::dref (p_sbcf);
    Object::dref (p_chmc);
    Object::dref (p_shmc);
    Object::dref (p_suit);
    Object::dref (p_cert);
    Object::dref (p_exts);
  }
  
  // assign a tls parameters to this one

  TlsState& TlsState::operator = (const TlsState& that) {
    // check for self assignation
    if (this == &that) return *this;
    // lock and assign
    wrlock ();
    that.rdlock ();
    try {
      d_sflg = that.d_sflg;
      d_rmaj = that.d_rmaj;
      d_rmin = that.d_rmin;
      d_vmaj = that.d_vmaj;
      d_vmin = that.d_vmin;
      d_cifr = that.d_cifr;
      d_ksiz = that.d_ksiz;
      d_bsiz = that.d_bsiz;
      d_vsiz = that.d_vsiz;
      d_hsiz = that.d_hsiz;
      d_cnum = that.d_cnum;
      d_snum = that.d_snum;
      d_crnd = that.d_crnd;
      d_srnd = that.d_srnd;
      d_mbuf = that.d_mbuf;
      d_ebuf = that.d_ebuf;
      d_cmac = that.d_cmac;
      d_smac = that.d_smac;
      d_ckey = that.d_ckey;
      d_skey = that.d_skey;
      d_civb = that.d_civb;
      d_sivb = that.d_sivb;
      d_info = that.d_info;
      d_hvmb = that.d_hvmb;
      Object::iref (that.p_cbcf);
      Object::iref (that.p_sbcf);
      Object::iref (that.p_chmc);
      Object::iref (that.p_shmc);
      Object::iref (that.p_suit);
      Object::iref (that.p_cert);
      Object::iref (that.p_exts);
      Object::dref (p_cbcf);
      Object::dref (p_sbcf);
      Object::dref (p_chmc);
      Object::dref (p_shmc);
      Object::dref (p_suit);
      Object::dref (p_cert);
      Object::dref (p_exts);
      p_cbcf = that.p_cbcf;
      p_sbcf = that.p_sbcf;
      p_chmc = that.p_chmc;
      p_shmc = that.p_shmc;
      p_suit = that.p_suit;
      p_cert = that.p_cert;
      p_exts = that.p_exts;
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
  
  String TlsState::repr (void) const {
    return "TlsState";
  }

  // reset the tls state object

  void TlsState::reset (void) {
    wrlock ();
    try {
      d_sflg = STA_SFLG_DEF;
      d_rmaj = TLS_VMAJ_DEF;
      d_rmin = TLS_VMIN_DEF;
      d_vmaj = d_rmaj;
      d_vmin = d_rmin;
      d_ksiz = STA_KSIZ_DEF;
      d_bsiz = STA_BSIZ_DEF;
      d_vsiz = STA_VSIZ_DEF;
      d_hsiz = STA_HSIZ_DEF;
      d_cnum = STA_CNUM_DEF;
      d_snum = STA_SNUM_DEF;
      d_crnd.reset ();
      d_srnd.reset ();
      d_mbuf.reset ();
      d_ebuf.reset ();
      d_cmac.reset ();
      d_smac.reset ();
      d_ckey.reset ();
      d_skey.reset ();
      d_civb.reset ();
      d_sivb.reset ();
      d_info.reset ();
      d_hvmb.reset ();
      setcifr(STA_CIFR_DEF);
      Object::dref (p_cbcf); p_cbcf = nullptr;
      Object::dref (p_sbcf); p_sbcf = nullptr;
      Object::dref (p_chmc); p_chmc = nullptr;
      Object::dref (p_shmc); p_shmc = nullptr;
      Object::dref (p_suit); p_suit = nullptr;
      Object::dref (p_cert); p_cert = nullptr;
      Object::dref (p_exts); p_exts = nullptr;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the connection info list

  Plist TlsState::getinfo (void) const {
    rdlock ();
    try {
      // create a result plist
      Plist plst = d_info;
      // add the state info
      plst.add (PN_STA_SFLG, PI_STA_SFLG, d_sflg);
      plst.add (PN_STA_RMAJ, PI_STA_RMAJ, (t_long) d_rmaj);
      plst.add (PN_STA_RMIN, PI_STA_RMIN, (t_long) d_rmin);
      plst.add (PN_STA_VMAJ, PI_STA_VMAJ, (t_long) d_vmaj);
      plst.add (PN_STA_VMIN, PI_STA_VMIN, (t_long) d_vmin);
      plst.add (PN_STA_CRND, PI_STA_CRND, d_crnd.format ());
      plst.add (PN_STA_SRND, PI_STA_SRND, d_srnd.format ());
      plst.add (PN_STA_MKEY, PI_STA_MKEY, d_mkey.format ());
      plst.add (PN_STA_MBUF, PI_STA_MBUF, d_mbuf.format ());
      plst.add (PN_STA_EBUF, PI_STA_EBUF, d_ebuf.format ());
      plst.add (PN_STA_CMAC, PI_STA_CMAC, d_cmac.format ());
      plst.add (PN_STA_SMAC, PI_STA_SMAC, d_smac.format ());
      plst.add (PN_STA_CKEY, PI_STA_CKEY, d_ckey.format ());
      plst.add (PN_STA_SKEY, PI_STA_SKEY, d_skey.format ());
      plst.add (PN_STA_CIVB, PI_STA_CIVB, d_civb.format ());
      plst.add (PN_STA_SIVB, PI_STA_SIVB, d_sivb.format ());
      plst.add (PN_STA_CIFR, PI_STA_CIFR, TlsUtils::toscod (d_cifr));
      plst.add (PN_STA_STEL, PI_STA_STEL,
		(p_suit == nullptr) ? 0LL : (t_long) p_suit->length ());
      plst.add (PN_STA_EXTL, PI_STA_EXTL,
		(p_exts == nullptr) ? 0LL : (t_long) p_exts->length ());
      // check for extension debug
      if ((d_prms.getdext () == true) && (p_exts != nullptr)) {
	plst+= tls_exts_plst (*p_exts);
      }
      unlock ();
      return plst;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // add some information to the state info

  void TlsState::addinfo (const Plist& plst) {
    wrlock ();
    try {
      d_info += plst;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }
  // add some information to the state info

  void TlsState::addinfo (const String&  name, const String& info,
			  const bool     bval) {
    wrlock ();
    try {
      d_info.add (name, info, bval);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // add some information to the state info

  void TlsState::addinfo (const String&  name, const String& info,
			  const t_long   ival) {
    wrlock ();
    try {
      d_info.add (name, info, ival);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // add some information to the state info

  void TlsState::addinfo (const String&  name, const String& info,
			  const t_real   rval) {
    wrlock ();
    try {
      d_info.add (name, info, rval);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // add some information to the state info

  void TlsState::addinfo (const String&  name, const String& info,
			  const Literal& lval) {
    wrlock ();
    try {
      d_info.add (name, info, lval);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // get the server flag

  bool TlsState::getsflg (void) const {
    rdlock ();
    try {
      bool result = d_sflg;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the tls major requested

  t_byte TlsState::getrmaj (void) const {
    rdlock ();
    try {
      t_byte result = d_rmaj;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the tls minor requested

  t_byte TlsState::getrmin (void) const {
    rdlock ();
    try {
      t_byte result = d_rmin;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // get the tls major version

  t_byte TlsState::getvmaj (void) const {
    rdlock ();
    try {
      t_byte result = d_vmaj;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the tls minor version

  t_byte TlsState::getvmin (void) const {
    rdlock ();
    try {
      t_byte result = d_vmin;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set the tls version at once

  void TlsState::setvers (const t_byte vmaj, const t_byte vmin) {
    wrlock ();
    try {
      // check version
      if (tls_vers_valid (vmaj, vmin) == false) {
	String vers = tls_vers_tostring (vmaj, vmin);
	throw Exception ("tls-error", "invalid tls version", vers);
      }
      // update major version
      if (vmaj < d_vmaj) {
	d_vmaj = vmaj;
	d_vmin = vmin;
      } else if (vmaj == d_vmaj) {
	// update minor version
	if (vmin < d_vmin) d_vmin = vmin;
      }
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set the selected cipher code

  void TlsState::setcifr (const t_word cifr) {
    wrlock ();
    try {
      // save the cipher code and reset
      d_cifr = cifr;
      d_ksiz = 0L;
      d_bsiz = 0L;
      d_vsiz = 0L;
      d_hsiz = 0L;
      // compute cipher size
      if (p_suit != nullptr) {
	TlsSuite::s_cnfo cnfo = p_suit->getcnfo (cifr);
	d_ksiz = cnfo.d_ksiz;
	d_bsiz = cnfo.d_bsiz;
	d_vsiz = cnfo.d_vsiz;
	d_hsiz = cnfo.d_hmac ? cnfo.d_hsiz : 0L;
      }
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the selected cipher

  t_word TlsState::getcifr (void) const {
    rdlock ();
    try {
      t_word result = d_cifr;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the cipher key size

  long TlsState::getksiz (void) const {
    rdlock ();
    try {
      long result = d_ksiz;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // get the cipher block size

  long TlsState::getbsiz (void) const {
    rdlock ();
    try {
      long result = d_bsiz;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the cipher iv size

  long TlsState::getvsiz (void) const {
    rdlock ();
    try {
      long result = d_vsiz;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // get the hash size

  long TlsState::gethsiz (void) const {
    rdlock ();
    try {
      long result = d_hsiz;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set the cipher suite by cipher code

  void TlsState::setsuit (const t_word cifr) {
    wrlock ();
    try {
      // check for a cipher suit
      if (p_suit == nullptr) {
	Object::iref (p_suit = new TlsSuite(d_vmaj, d_vmin));
      }
      setcifr (cifr);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    } 
  }
  
  // set the cipher suite and selected cipher

  void TlsState::setsuit (TlsSuite* suit) {
    // check for nil
    if (suit == nullptr) return;
    // lock and set
    wrlock ();
    try {
      // check clean state
      if (p_suit != nullptr) {
	throw Exception ("tls-error", "state cipher suite already set");
      }
      // update the suite by version
      suit->update (d_vmaj, d_vmin);
      // set the cipher suite
      Object::iref (p_suit = suit);
      // set the selected cipher
      setcifr (p_suit->locate ());
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the state cipher suite

  TlsSuite* TlsState::getsuit (void) const {
    rdlock ();
    try {
      TlsSuite* result = p_suit;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set the extensions vector

  void TlsState::setexts (Vector* exts) {
    // check for nil
    if (exts == nullptr) return;
    // lock and set
    wrlock ();
    try {
      // check clean state
      if (p_exts != nullptr) {
	throw Exception ("tls-error", "state cipher extensions already set");
      }
      // set the extensions vector
      Object::iref (p_exts = exts);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set the state certificate by list

  void TlsState::setcert (const String& cert, const String& ckey) {
    // check for nil
    if ((cert.isnil () == true) || (ckey.isnil () == true)) return;
    // lock and set
    wrlock ();
    try {
      // check clean state
      if (p_cert != nullptr) {
	throw Exception ("tls-error", "state certificate already set");
      }
      // create a new certificate
      Object::iref (p_cert = new TlsCerts (cert, ckey));
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }	       
  }

  // set the state certificate

  void TlsState::setcert (TlsCerts* cert) {
    wrlock ();
    try {
      Object::iref (cert); Object::dref (p_cert);
      p_cert = cert;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // get the state certificate

  TlsCerts* TlsState::getcert (void) const {
    rdlock ();
    try {
      TlsCerts* result = p_cert;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set the client random
  
  void TlsState::setcrnd (const Buffer& crnd) {
    wrlock ();
    try {
      d_crnd = crnd;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the client random buffer

  Buffer TlsState::getcrnd (void) const {
    rdlock ();
    try {
      Buffer result = d_crnd;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // set the server random

  void TlsState::setsrnd (const Buffer& srnd) {
    wrlock ();
    try {
      d_srnd = srnd;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the server random buffer

  Buffer TlsState::getsrnd (void) const {
    rdlock ();
    try {
      Buffer result = d_srnd;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set the premaster key

  void TlsState::setmkey (const Buffer& mkey) {
    wrlock ();
    try {
      d_mkey = mkey;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the premaster key

  Buffer TlsState::getmkey (void) const {
    rdlock ();
    try {
      Buffer result = d_mkey;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // get the exchange private key

  Key* TlsState::getprvk (void) const {
    rdlock ();
    try {
      Key* result = (p_cert == nullptr) ? nullptr : p_cert->getprvk ();
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the exchange public key key

  Key* TlsState::getpubk (void) const {
    rdlock ();
    try {
      Key* result = (p_cert == nullptr) ? nullptr : p_cert->getpubk ();
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set the state master secret

  void TlsState::setmbuf (const Buffer& mbuf) {
    wrlock ();
    try {
      d_mbuf = mbuf;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the master secret buffer

  Buffer TlsState::getmbuf (void) const {
    rdlock ();
    try {
      Buffer result = d_mbuf;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set the key expansion buffer and other buffers

  void TlsState::setebuf (const Buffer& ebuf) {
    wrlock ();
    try {
      // set the expansion buffer
      d_ebuf = ebuf;
      // extract the mac secret
      long spos = 0L;
      if (d_hsiz > 0L) {
	d_cmac = ebuf.extract (spos, d_hsiz); spos += d_hsiz;
	d_smac = ebuf.extract (spos, d_hsiz); spos += d_hsiz;
      } else {
	d_cmac.reset ();
	d_smac.reset ();
      }
      // extract the key
      if (d_ksiz > 0L) {
	d_ckey = ebuf.extract (spos, d_ksiz); spos += d_ksiz;
	d_skey = ebuf.extract (spos, d_ksiz); spos += d_ksiz;
      } else {
	d_ckey.reset ();
	d_skey.reset ();
      }
      // extract the initial vector
      if (d_vsiz > 0L) {
	d_civb = ebuf.extract (spos, d_vsiz); spos += d_vsiz;
	d_sivb = ebuf.extract (spos, d_vsiz);
      } else {
	d_civb.reset ();
	d_sivb.reset ();
      }
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // get the key expansion buffer

  Buffer TlsState::getebuf (void) const {
    rdlock ();
    try {
      Buffer result = d_ebuf;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the client block cipher

  Cipher* TlsState::getcbcf (void) const {
    rdlock ();
    try {
      Cipher* result = p_cbcf;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // get the server block cipher

  Cipher* TlsState::getsbcf (void) const {
    rdlock ();
    try {
      Cipher* result = p_sbcf;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the client hmac

  Hmac* TlsState::getchmc (void) const {
    rdlock ();
    try {
      Hmac* result = p_chmc;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // get the server hmac

  Hmac* TlsState::getshmc (void) const {
    rdlock ();
    try {
      Hmac* result = p_shmc;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // get the handshake verify message buffer

  Buffer& TlsState::gethvmb (void) {
    rdlock ();
    try {
      Buffer& result = d_hvmb;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the handshake verify message buffer

  const Buffer& TlsState::gethvmb (void) const {
    rdlock ();
    try {
      const Buffer& result = d_hvmb;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // geta new state hasher

  Hasher* TlsState::newhash (void) const {
    rdlock ();
    try {
      Hasher* result =
	(p_suit == nullptr) ? nullptr : p_suit->gethasher (d_cifr);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // get the client sequence number

  t_octa TlsState::getcnum (void) const {
    rdlock ();
    try {
      t_octa result = d_cnum;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the server sequence number

  t_octa TlsState::getsnum (void) const {
    rdlock ();
    try {
      t_octa result = d_snum;      
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // get a new client sequence number

  t_octa TlsState::newcnum (const bool fflg) {
    wrlock ();
    try {
      // get the client sequence number
      t_octa result = d_cnum;
      // update only with non null hmac
      if ((fflg == true) || (p_chmc != nullptr)) d_cnum++;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get a new server sequence number

  t_octa TlsState::newsnum (const bool fflg) {
    wrlock ();
    try {
      // get the server sequence number
      t_octa result = d_snum;
      // update only with non null hmac
      if ((fflg == true) || (p_shmc != nullptr)) d_snum++;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // change the state client cipher spec

  bool TlsState::chgccs (void) {
    wrlock ();
    try {
      // remove old cipher and hmac
      Object::dref (p_cbcf); p_cbcf = nullptr;
      Object::dref (p_chmc); p_chmc = nullptr;
      // rebuild new cipher and hmac
      if (p_suit != nullptr) {
	p_cbcf = p_suit->getcipher (d_cifr, d_sflg, d_ckey, d_civb);
	p_chmc = p_suit->gethmac   (d_cifr, d_cmac);
	Object::iref (p_cbcf); Object::iref (p_chmc); 
      }
      unlock ();
      return true;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // change the state server cipher spec

  bool TlsState::chgscs (void) {
    wrlock ();
    try {
      // remove old cipher and hmac
      Object::dref (p_sbcf); p_sbcf = nullptr;
      Object::dref (p_shmc); p_shmc = nullptr;
      // rebuild new cipher and hmac
      if (p_suit != nullptr) {
	p_sbcf = p_suit->getcipher (d_cifr, !d_sflg, d_skey, d_sivb);
	p_shmc = p_suit->gethmac   (d_cifr, d_smac);
	Object::iref (p_sbcf); Object::iref (p_shmc);
      }
      unlock ();
      return true;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // -------------------------------------------------------------------------
  // - object section                                                        -
  // -------------------------------------------------------------------------

  // the quark zone
  static const long QUARK_ZONE_LENGTH = 16;
  static QuarkZone  zone (QUARK_ZONE_LENGTH);

  // the object supported quarks
  static const long QUARK_SERVP   = zone.intern ("server-p");
  static const long QUARK_GETKSIZ = zone.intern ("get-key-size");
  static const long QUARK_GETBSIZ = zone.intern ("get-block-size");
  static const long QUARK_GETVSIZ = zone.intern ("get-iv-size");
  static const long QUARK_GETHSIZ = zone.intern ("get-hash-size");
  static const long QUARK_GETCERT = zone.intern ("get-certificate");
  static const long QUARK_GETSUIT = zone.intern ("get-cipher-suite");
  static const long QUARK_GETVMAJ = zone.intern ("get-major-version");
  static const long QUARK_GETVMIN = zone.intern ("get-minor-version");
  static const long QUARK_GETCRND = zone.intern ("get-client-random-buffer");
  static const long QUARK_GETSRND = zone.intern ("get-server-random-buffer");
  static const long QUARK_GETMKEY = zone.intern ("get-premaster-key-buffer");
  static const long QUARK_GETMBUF = zone.intern ("get-master-secret-buffer");
  static const long QUARK_GETEBUF = zone.intern ("get-key-expansion-buffer");
  static const long QUARK_GETCNUM = zone.intern ("get-client-sequence-number");
  static const long QUARK_GETSNUM = zone.intern ("get-server-sequence-number");

  // create a new object in a generic way

  Object* TlsState::mknew (Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();

    // check for 0 argument
    if (argc == 0) return new TlsState;
    // check for 1 argument
    if (argc == 1) {
      bool sflg = argv->getbool (0);
      return new TlsState (sflg);
    }
    // too many arguments
    throw Exception ("argument-error", 
                     "too many argument with tls state constructor");
  }

  // return true if the given quark is defined

  bool TlsState::isquark (const long quark, const bool hflg) const {
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
  
  Object* TlsState::apply (Evaluable* zobj, Nameset* nset, const long quark,
			   Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();
    
    // dispatch 0 arguments
    if (argc == 0) {
      if (quark == QUARK_SERVP)   return new Boolean (getsflg ());
      if (quark == QUARK_GETVMAJ) return new Byte (getvmaj ());
      if (quark == QUARK_GETVMIN) return new Byte (getvmin ());
      if (quark == QUARK_GETKSIZ) return new Integer (getksiz ());
      if (quark == QUARK_GETBSIZ) return new Integer (getbsiz ());
      if (quark == QUARK_GETVSIZ) return new Integer (getvsiz ());
      if (quark == QUARK_GETHSIZ) return new Integer (gethsiz ());
      if (quark == QUARK_GETCRND) return new Buffer  (getcrnd ());
      if (quark == QUARK_GETSRND) return new Buffer  (getsrnd ());
      if (quark == QUARK_GETMKEY) return new Buffer  (getmkey ());
      if (quark == QUARK_GETMBUF) return new Buffer  (getmbuf ());
      if (quark == QUARK_GETEBUF) return new Buffer  (getebuf ());
      if (quark == QUARK_GETCNUM) return new Integer (getcnum ());
      if (quark == QUARK_GETSNUM) return new Integer (getsnum ());
      if (quark == QUARK_GETSUIT) {
	rdlock ();
	try {
	  Object* result = getsuit ();
	  zobj->post (result);
	  unlock ();
	  return result;
	} catch (...) {
	  unlock ();
	  throw;
	}
      }
      if (quark == QUARK_GETCERT) {
	rdlock ();
	try {
	  Object* result = getcert ();
	  zobj->post (result);
	  unlock ();
	  return result;
	} catch (...) {
	  unlock ();
	  throw;
	}
      }
    }
    // call the info method
    return TlsInfos::apply (zobj, nset, quark, argv);
  }
}
