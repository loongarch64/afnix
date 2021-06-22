// ---------------------------------------------------------------------------
// - TlsConnect.cpp                                                          -
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

#include "Md5.hpp"
#include "Sha1.hpp"
#include "Vector.hpp"
#include "TlsAlert.hpp"
#include "TlsShake.hpp"
#include "TlsTypes.hxx"
#include "TlsProto.hxx"
#include "Evaluable.hpp"
#include "TlsChello.hpp"
#include "TlsShello.hpp"
#include "TlsCkeyxh.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"
#include "TlsConnect.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - private section                                                       -
  // -------------------------------------------------------------------------

  // this procedure compute the server master secret buffer
  static Buffer tls_get_smsb (const s_xhss& xhss, TlsProto* tlsp,
			      TlsState* ssta) {
    // get the key exchange message
    TlsHblock* hblk = dynamic_cast <TlsHblock*> (xhss.p_mkxh);
    if ((hblk == nullptr) || (ssta == nullptr)) {
      throw Exception ("tls-error", "nil object for master secret compute");
    }
    // decode the key exchange
    TlsCkeyxh ckxh (hblk, ssta);
    // get the premaster key
    Buffer mkey = ssta->getmkey ();
    // combine the client and server random
    Buffer seed; seed.add (ssta->getcrnd ()); seed.add (ssta->getsrnd ());
    // call the protocol prf
    Buffer result = tlsp->xprf (mkey, TLS_LABL_MS, seed, TLS_SIZE_MSX);
    // here it is
    return result;
  }

  // this procedure compute the client master secret buffer
  static Buffer tls_get_cmsb (TlsProto* tlsp, TlsState* ssta) {
    // check for nil
    if (ssta == nullptr) {
      throw Exception ("tls-error", "nil state for premaster secret");
    }
    // get the premaster key
    Buffer mkey = ssta->getmkey ();
    // combine the client and server random
    Buffer seed; seed.add (ssta->getcrnd ()); seed.add (ssta->getsrnd ());
    // call the protocol prf
    Buffer result = tlsp->xprf (mkey, TLS_LABL_MS, seed, TLS_SIZE_MSX);
    // here it is
    return result;
  }

  // this procedure compute the key expansion block buffer
  static Buffer tls_get_ebuf (TlsProto* tlsp, TlsState* ssta) {
    if ((tlsp == nullptr) || (ssta == nullptr)) {
      throw Exception ("tls-error", "nil object for key expansion compute");
    }
    // compute the block length
    long blen = 2 * (ssta->getksiz () + ssta->getvsiz () + ssta->gethsiz ());
    // get the master secret buffer
    Buffer mbuf = ssta->getmbuf ();
    // combine the server and client random [opposite to master secret ah! ah!]
    Buffer seed; seed.add (ssta->getsrnd ()); seed.add (ssta->getcrnd ());
    // compute the prf
    return tlsp->xprf (mbuf, TLS_LABL_KE, seed, blen);
  }

  // this procedure verify the client finished message block
  static void tls_chk_cfin (const s_xhss& xhss,TlsProto* tlsp,TlsState* ssta) {
    // get the handshake finished message
    TlsHblock* hblk = dynamic_cast <TlsHblock*> (xhss.p_mfin);
    if ((hblk == nullptr) || (tlsp == nullptr) || (ssta == nullptr)) {
      throw Exception ("tls-error", "nil object for finished verification");
    }
    // check the block finished size
    if (hblk->length() != TLS_SIZE_FIN) {
      throw Exception ("tls-error", "invalid finished message size");
    }    
    // compute the finished signature and verify
    Buffer finb = tlsp->cfin (ssta->getmbuf(), xhss.d_cvmb);
    if (finb.length() != TLS_SIZE_FIN){
      throw Exception ("tls-error", "invalid finished buffer size");
    }
    // compare the block buffer with the computed buffer
    if (hblk->tobuffer() != finb) {
      throw Exception ("tls-error", "handshake verify failure");
    }
  }
  
  // this procedure verify the server finished message block
  static void tls_chk_sfin (const s_xhcs& xhcs,TlsProto* tlsp,TlsState* ssta) {
    // get the handshake finished message
    TlsHblock* hblk = dynamic_cast <TlsHblock*> (xhcs.p_mfin);
    if ((hblk == nullptr) || (tlsp == nullptr) || (ssta == nullptr)) {
      throw Exception ("tls-error", "nil object for finished verification");
    }
    // check the block finished size
    if (hblk->length() != TLS_SIZE_FIN) {
      throw Exception ("tls-error", "invalid finished message size");
    }    
    // compute the finished signature and verify
    Buffer finb = tlsp->sfin (ssta->getmbuf(), xhcs.d_svmb);
    if (finb.length() != TLS_SIZE_FIN){
      throw Exception ("tls-error", "invalid finished buffer size");
    }
    // compare the block buffer with the computed buffer
    if (hblk->tobuffer() != finb) {
      throw Exception ("tls-error", "handshake verify failure");
    }
  }

  // this procedure extracts the certificate list from a message
  static TlsCerts* tls_get_cert (const s_xhcs& xhcs) {
    // get the key exchange message
    TlsHblock* hblk = dynamic_cast <TlsHblock*> (xhcs.p_mcrt);
    if (hblk == nullptr) return nullptr;
    // check for a valid certificate block
    if (hblk->gettype () != TLS_HSHK_CRT) {
      throw Exception ("tls-error", "invalid handshake block as certificate");
    }
    // map the block as a certificate list
    return new TlsCerts (hblk);
  }
  
  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a tls connect by flag

  TlsConnect::TlsConnect (const bool sflg) {
    d_sflg = sflg;
  }

  // create a tls connect by flag and parameters

  TlsConnect::TlsConnect (const bool sflg, const TlsParams& prms) {
    d_sflg = sflg;
    setprms (prms);
  }

  // return the class name
  
  String TlsConnect::repr (void) const {
    return "TlsConnect";
  }

  // set the tls parameters

  void TlsConnect::setprms (const TlsParams& prms) {
    wrlock ();
    try {
      d_prms = prms;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // perform a handshake operation

  TlsState* TlsConnect::connect (InputStream* is, OutputStream* os)  {
    // check for nil
    if ((is == nullptr) || (os == nullptr)) return nullptr;
    // connect and lock
    wrlock ();
    try {
      // connect the tls socket
      TlsState* result = d_sflg ? getss (is, os) : getcs (is, os);
      unlock ();
      return result;
    } catch (TlsAlert& msg) {
      msg.write (os);
      unlock ();
      return nullptr;
    } catch (...) {
      TlsAlert(TlsAlert::TLS_DESC_HFL).write (os);
      unlock ();
      throw;
    }
  }

  // -------------------------------------------------------------------------
  // - protected server section                                              -
  // -------------------------------------------------------------------------

  // initialize the server state

  TlsState* TlsConnect::iniss (InputStream* is) {
    // check for nil
    if (is == nullptr) return nullptr;
    // lock and connect
    wrlock ();
    TlsState*  ssta = nullptr;
    TlsChello* chlo = nullptr;
    try {
      // check for consistency
      if (d_sflg == false) {
	throw Exception ("tls-error", "invalid server state initialize");
      }
      // create a server state by flag and protect it
      ssta = new TlsState (d_sflg, d_prms); Object::iref (ssta);
      // get the client hello message
      chlo = dynamic_cast <TlsChello*> (TlsProto().rcvchlo (is, ssta));
      if (chlo == nullptr) {
	throw Exception ("tls-error", "cannot get client hello message");
      }
      ssta->addinfo (chlo->getinfo ());
      // update the state version
      ssta->setvers (chlo->getvmaj (), chlo->getvmin ());
      // update the client random
      ssta->setcrnd (chlo->getrand ());
      // update the cipher suite
      ssta->setsuit (chlo->getsuit ());
      // update the extensions vector
      ssta->setexts (chlo->getexts ());
      // clean and unlock
      delete chlo;
      unlock ();
      return ssta;
    } catch (...) {
      Object::dref (ssta);
      delete chlo;
      unlock ();
      throw;
    }
  }

  // get a server state

  TlsState* TlsConnect::getss (InputStream* is, OutputStream* os) {
    // check for nil
    if ((is == nullptr) || (os == nullptr)) return nullptr;
    // lock and connect
    wrlock ();
    TlsState* ssta = nullptr;
    TlsProto* tlsp = nullptr;
    try {
      // initialize the server state
      ssta = iniss (is);
      // create a tls protocol by state
      tlsp = TlsProto::create (ssta);
      // create a tls handshake sequence
      s_xhss xhss (ssta->getcifr ());
      // send server handshake sequence
      xhss.sndss (os, tlsp, ssta);
      // receive a client handshake sequence
      xhss.rcvcs (is, tlsp, ssta);
      // get/set the master key
      ssta->setmbuf (tls_get_smsb (xhss, tlsp, ssta));
      // get/set the expansion block buffer
      ssta->setebuf (tls_get_ebuf (tlsp, ssta));      
      // finish a server handshake sequence
      xhss.finss (is, tlsp, ssta);
      // verify the client finished message
      tls_chk_cfin (xhss, tlsp, ssta);
      // send the server finish sequence
      xhss.sndfs (os, tlsp, ssta);
      // clean and unlock
      delete tlsp; Object::tref (ssta);
      unlock ();
      return ssta;
    } catch (...) {
      Object::dref (ssta);
      delete tlsp;
      unlock ();
      throw;
    }
  }

  // -------------------------------------------------------------------------
  // - protected client section                                              -
  // -------------------------------------------------------------------------

  // initialize the client state

  TlsState* TlsConnect::inics (InputStream* is, OutputStream* os) {
    // check for nil
    if ((is == nullptr) || (os == nullptr)) return nullptr;
    // lock and connect
    wrlock ();
    TlsState*  ssta = nullptr;
    TlsShello* shlo = nullptr;
    try {
      // check for consistency
      if (d_sflg == true) {
	throw Exception ("tls-error", "invalid client state initialize");
      }
      // create a client state by flag and protect it
      ssta = new TlsState (d_sflg, d_prms); Object::iref (ssta);
      // send a client hello message
      TlsProto().sndchlo (os, ssta);
      // get the server hello message
      shlo = dynamic_cast <TlsShello*> (TlsProto().rcvshlo (is, ssta));
      if (shlo == nullptr) {
	throw Exception ("tls-error", "cannot get server hello message");
      }
      ssta->addinfo (shlo->getinfo ());
      // update the state version
      ssta->setvers (shlo->getvmaj (), shlo->getvmin ());
      // update the server random
      ssta->setsrnd (shlo->getrand ());
      // update the cipher suit/code
      ssta->setsuit (shlo->getcifr ());
      // clean and unlock
      delete shlo;
      // clean and unlock
      unlock ();
      return ssta;
    } catch (...) {
      Object::dref (ssta);
      delete shlo;
      unlock ();
      throw;
    }
  }

  // get a client state

  TlsState* TlsConnect::getcs (InputStream* is, OutputStream* os) {
    // check for nil
    if ((is == nullptr) || (os == nullptr)) return nullptr;
    // lock and connect
    wrlock ();
    TlsState* ssta = nullptr;
    TlsProto* tlsp = nullptr;
    try {
      // initialize the client state
      ssta = inics (is, os);
      // create a tls protocol by state
      tlsp = TlsProto::create (ssta);
      // create a tls handshake sequence
      s_xhcs xhcs (ssta->getcifr ());
      // receive a server handshake sequence
      xhcs.rcvss (is, tlsp, ssta);
      // set the certificate list
      ssta->setcert (tls_get_cert (xhcs));
      // send the finish client handshake
      xhcs.fincs (os, tlsp, ssta);
      // get/set the master key
      ssta->setmbuf (tls_get_cmsb (tlsp, ssta));
      // get/set the expansion block buffer
      ssta->setebuf (tls_get_ebuf (tlsp, ssta));
      // send the server finish sequence
      xhcs.sndfs (os, tlsp, ssta);
      // receive a server finish sequence
      xhcs.rcvfs (is, tlsp, ssta);
      // verify the server finished message
      tls_chk_sfin (xhcs, tlsp, ssta);
      // clean and unlock
      delete tlsp; Object::tref (ssta);
      unlock ();
      return ssta;
    } catch (...) {
      Object::dref (ssta);
      delete tlsp;
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
  static const long QUARK_CONNECT = zone.intern ("connect");
  static const long QUARK_SETPRMS = zone.intern ("set-parameters");
 
  // create a new object in a generic way

  Object* TlsConnect::mknew (Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();

    // check for 1 argument
    if (argc == 1) {
      bool sflg = argv->getbool (0);
      return new TlsConnect (sflg);
    }
    // check for 2 arguments
    if (argc == 2) {
      bool sflg = argv->getbool (0);
      Object* obj = argv->get (1);
      TlsParams* prms = dynamic_cast <TlsParams*> (obj);
      if (prms == nullptr) {
	throw Exception ("type-error", "invalid object as parameters",
			 Object::repr (obj));
      }
      return new TlsConnect (sflg, *prms);
    }
    // too many arguments
    throw Exception ("argument-error", 
                     "too many argument with tls connect constructor");
  }
  
  // return true if the given quark is defined

  bool TlsConnect::isquark (const long quark, const bool hflg) const {
    rdlock ();
    try {
      if (zone.exists (quark) == true) {
	unlock ();
	return true;
      }
      bool result = hflg ? Object::isquark (quark, hflg) : false;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // apply this object with a set of arguments and a quark
  
  Object* TlsConnect::apply (Evaluable* zobj, Nameset* nset, const long quark,
			     Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();
    // dispatch 1 argument
    if (argc == 1) {
      if (quark == QUARK_SETPRMS) {
	Object* obj = argv->get (0);
	auto prms = dynamic_cast<TlsParams*> (obj);
	if (prms == nullptr) {
	  throw Exception ("type-error", "invalid object as parameters",
			   Object::repr (obj));
	}
	setprms (*prms);
	return  nullptr;
      }
    }
    // dispatch 2 arguments
    if (argc == 2) {
      if (quark == QUARK_CONNECT) {
	Object* obj = argv->get (0);
	auto is = dynamic_cast<InputStream*> (obj);
	if (is == nullptr) {
	  throw Exception ("type-error", "invalid object as input stream",
			   Object::repr (obj));
	}
	obj = argv->get (1);
	auto os = dynamic_cast<OutputStream*> (obj);
	if (os == nullptr) {
	  throw Exception ("type-error", "invalid object as output stream",
			   Object::repr (obj));
	}
	return connect (is, os);
      }
    }
    // call the object method
    return Object::apply (zobj, nset, quark, argv);
  }
}
