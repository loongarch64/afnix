// ---------------------------------------------------------------------------
// - TlsProto.hxx                                                            -
// - afnix:tls service - tls protocol internal                               -
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

#ifndef  AFNIX_TLSPROTO_HXX
#define  AFNIX_TLSPROTO_HXX

#ifndef  AFNIX_TLSPROTO_HPP
#include "TlsProto.hpp"
#endif

#ifndef  AFNIX_TLSSUITE_HXX
#include "TlsSuite.hxx"
#endif

namespace afnix {
  // the master secret prf label
  static const String TLS_LABL_MS = "master secret";
  // the key expansion prf label
  static const String TLS_LABL_KE = "key expansion";
  // the client finished prf label
  static const String TLS_LABL_CF = "client finished";
  // the server finished prf label
  static const String TLS_LABL_SF = "server finished";
  
  // the server handshake server sequence
  using t_shss = void (TlsProto::*) (OutputStream*, TlsState*) const;
  // the server handshake server change cipher spec
  using t_shsc = bool (TlsProto::*) (OutputStream*, TlsState*) const;
  // the server handshake client sequence
  using t_shcs = TlsHblock* (TlsProto::*) (InputStream*, TlsState*) const;
  // the server handshake client change cipher spec
  using t_shcc = bool (TlsProto::*) (InputStream*, TlsState*) const;

  // the handshake server sequence
  struct s_xhss {
    // the send server hello
    t_shss p_shlo;
    // the send server certificate
    t_shss p_scrt;
    // the send server key exchange
    t_shss p_skxh;
    // the send server certificate request
    t_shss p_sreq;
    // the send server done
    t_shss p_sdon;
    // the send change cipher spec
    t_shsc p_sccs;
    // the send server finish
    t_shss p_sfin;
    // the client key exchange
    t_shcs p_ckxh;
    // the client change cipher spec
    t_shcc p_cccs;
    // the client finished
    t_shcs p_cfin;
    // the key exchange message
    TlsInfos* p_mkxh;
    // the finished message
    TlsInfos* p_mfin;
    // the change cipher spec flag
    bool d_cccs;
    // the client verify message buffer
    Buffer d_cvmb;
    // create a default handshake sequence
    s_xhss (void) {
      p_mkxh = nullptr;
      p_mfin = nullptr;
      reset ();
    }
    // create a handshake sequence by cipher code
    s_xhss (const t_word cifr) {
      p_mkxh = nullptr;
      p_mfin = nullptr;
      reset ();
      setcifr (cifr);
    }
    // reset the handshake sequence
    void reset (void) {
      // server send protocol
      p_shlo = nullptr;
      p_scrt = nullptr;
      p_skxh = nullptr;
      p_sreq = nullptr;
      p_sdon = nullptr;
      p_sccs = nullptr;
      p_sfin = nullptr;
      // server receive protocol
      p_ckxh = nullptr;
      p_cccs = nullptr;
      p_cfin = nullptr;
      // received client messages and flag
      delete p_mkxh; p_mkxh = nullptr;
      delete p_mfin; p_mfin = nullptr;
      d_cccs = false;
    }
    
    // set a handshake sequence by cipher code
    void setcifr (const t_word cifr) {
      // set server send protocol
      p_shlo = &TlsProto::sndshlo;
      p_sdon = &TlsProto::sndsdon;
      p_sccs = &TlsProto::sndsccs;
      p_sfin = &TlsProto::sndxfin;
      // set server receive protocol
      p_ckxh = &TlsProto::rcvchsk;
      p_cccs = &TlsProto::rcvcccs;
      p_cfin = &TlsProto::rcvchsk;
      // set cipher specific protocol
      switch (cifr) {
      case TLS_NULL_WITH_NULL_NULL:
	break;
      case TLS_RSA_WITH_NULL_MD5:
      case TLS_RSA_WITH_NULL_SHA:
      case TLS_RSA_WITH_RC4_128_MD5:
      case TLS_RSA_WITH_RC4_128_SHA:
      case TLS_RSA_WITH_AES_128_CBC_SHA:
      case TLS_RSA_WITH_AES_256_CBC_SHA:
      case TLS_RSA_WITH_AES_128_CBC_SHA256:
      case TLS_RSA_WITH_AES_256_CBC_SHA256:
      case TLS_RSA_WITH_AES_128_GCM_SHA256:
      case TLS_RSA_WITH_AES_256_GCM_SHA384:
	p_scrt = &TlsProto::sndscrt;	
	break;
      default:
	throw Exception ("tls-error", "invalid cipher code in sequence");
	break;
      }
    }
    // send a server handshake sequence
    void sndss (OutputStream* os, TlsProto* tlsp, TlsState* ssta) {
      // check for nil
      if (tlsp == nullptr) return;
      // execute in sequence
      if (p_shlo != nullptr) (tlsp->*p_shlo) (os, ssta);
      if (p_scrt != nullptr) (tlsp->*p_scrt) (os, ssta);
      if (p_skxh != nullptr) (tlsp->*p_skxh) (os, ssta);
      if (p_sreq != nullptr) (tlsp->*p_sreq) (os, ssta);
      if (p_sdon != nullptr) (tlsp->*p_sdon) (os, ssta);
    }
    // receive a client handshake sequence
    void rcvcs (InputStream* is, TlsProto* tlsp, TlsState* ssta) {
      // check for nil
      if (tlsp == nullptr) return;
      // execute in sequence
      if (p_ckxh != nullptr) p_mkxh = (tlsp->*p_ckxh) (is, ssta);
    }
    // finish a server handshake sequence
    void finss (InputStream* is, TlsProto* tlsp, TlsState* ssta) {
      // check for nil
      if (tlsp == nullptr) return;
      // save the client verify message buffer
      if (ssta != nullptr) d_cvmb = ssta->gethvmb();
      // execute in sequence
      if (p_cccs != nullptr) d_cccs = (tlsp->*p_cccs) (is, ssta);
      if (p_cfin != nullptr) p_mfin = (tlsp->*p_cfin) (is, ssta);
    }
    // send a server finish sequence
    void sndfs (OutputStream* os, TlsProto* tlsp, TlsState* ssta) {
      // check for nil
      if (tlsp == nullptr) return;
      // execute in sequence
      if (p_sccs != nullptr) (tlsp->*p_sccs) (os, ssta);
      if (p_sfin != nullptr) (tlsp->*p_sfin) (os, ssta);
    }
  };

  // the client handshake client sequence
  using t_chcs = void (TlsProto::*) (OutputStream*, TlsState*) const;
  // the client handshake server sequence
  using t_chss = TlsHblock* (TlsProto::*) (InputStream*, TlsState*) const;
  // the client change cipher spec
  using t_chcc = bool (TlsProto::*) (OutputStream*, TlsState*) const;
  // the server change cipher spec
  using t_chsc = bool (TlsProto::*) (InputStream*, TlsState*) const;
  
  // the handshake client sequence
  struct s_xhcs {
    // the receive certificate
    t_chss p_rcrt;
    // the generic receive server handshake
    t_chss p_rhsk;
    // the client key exchange
    t_chcs p_ckxh;
    // the client change cipher spec
    t_chcc p_cccs;
    // the client finish message
    t_chcs p_cfin;
    // the server change cipher spec
    t_chsc p_sccs;
    // the server finish message
    t_chss p_sfin;
    // the server certificate message
    TlsInfos* p_mcrt;
    // the certificate request message
    TlsInfos* p_mcrr;
    // the finished message
    TlsInfos* p_mfin;
    // the change cipher spec flag
    bool d_sccs;
    // the server verify message buffer
    Buffer d_svmb;
    // create a default handshake sequence
    s_xhcs (void) {
      p_mcrt = nullptr;
      p_mcrr = nullptr;
      p_mfin = nullptr;
      reset ();
    }
    // create a handshake sequence by cipher code
    s_xhcs (const t_word cifr) {
      p_mcrt = nullptr;
      p_mcrr = nullptr;
      p_mfin = nullptr;
      reset ();
      setcifr (cifr);
    }
    // destroy this handshake sequence
    ~s_xhcs (void) {
      reset ();
    }
    // reset the handshake sequence
    void reset (void) {
      // client receive protocol
      p_rcrt = nullptr;
      p_rhsk = nullptr;
      p_ckxh = nullptr;
      p_cccs = nullptr;
      p_cfin = nullptr;
      p_sccs = nullptr;
      p_sfin = nullptr;
      // received server messages
      delete p_mcrt; p_mcrt = nullptr;
      delete p_mcrr; p_mcrr = nullptr;
      delete p_mfin; p_mfin = nullptr;
      d_sccs = false;
    }
    // set a handshake sequence by cipher code
    void setcifr (const t_word cifr) {
      // set generic protocol
      p_rhsk = &TlsProto::rcvshsk;
      // set client send protocol
      p_ckxh = &TlsProto::sndckxh;
      // set client change cipher spec
      p_cccs = &TlsProto::sndcccs;
      // set client send finish
      p_cfin = &TlsProto::sndxfin;
      // set server change cipher spec
      p_sccs = &TlsProto::rcvsccs;
      // set server finished
      p_sfin = &TlsProto::rcvshsk;
      // set cipher specific protocol
      switch (cifr) {
      case TLS_NULL_WITH_NULL_NULL:
	break;
      case TLS_RSA_WITH_NULL_MD5:
      case TLS_RSA_WITH_NULL_SHA:
      case TLS_RSA_WITH_RC4_128_MD5:
      case TLS_RSA_WITH_RC4_128_SHA:
      case TLS_RSA_WITH_AES_128_CBC_SHA:
      case TLS_RSA_WITH_AES_256_CBC_SHA:
      case TLS_RSA_WITH_AES_128_CBC_SHA256:
      case TLS_RSA_WITH_AES_256_CBC_SHA256:
      case TLS_RSA_WITH_AES_128_GCM_SHA256:
      case TLS_RSA_WITH_AES_256_GCM_SHA384:
	p_rcrt = &TlsProto::rcvshsk;	
	break;
      default:
	throw Exception ("tls-error", "invalid cipher code in sequence");
	break;
      }
    }
    // receive a server handshake sequence
    void rcvss (InputStream* is, TlsProto* tlsp, TlsState* ssta) {
      // check for nil
      if (tlsp == nullptr) return;
      // execute in sequence
      if (p_rcrt != nullptr) {
	p_mcrt = (tlsp->*p_rcrt) (is, ssta);
	if (TlsProto::ishblk(p_mcrt, TLS_HSHK_CRT) == false) {
	  throw Exception ("tls-error",
			   "invalid certificate handshake message");
	}
      }
      // loop until server done
      while (true) {
	// get the tls message
	TlsInfos* iblk = (tlsp->*p_rhsk) (is, ssta);
	// check for valid handshake
	if (TlsProto::ishblk(iblk, TLS_HSHK_CRR) == true) {
	  p_mcrr = iblk;
	} else if (TlsProto::ishblk(iblk, TLS_HSHK_SHD) == true) {
	  delete iblk;
	  break;
	} else {
	  throw Exception ("tls-error", "invalid handshake message");
	}
      }
    }
    // send a finish handshake sequence
    void fincs (OutputStream* os, TlsProto* tlsp, TlsState* ssta) {
      // check for nil
      if (tlsp == nullptr) return;
      // execute in sequence
      if (p_ckxh != nullptr) (tlsp->*p_ckxh) (os, ssta);
    }
    // send a client finish sequence
    void sndfs (OutputStream* os, TlsProto* tlsp, TlsState* ssta) {
      // check for nil
      if (tlsp == nullptr) return;
      // execute in sequence
      if (p_cccs != nullptr) (tlsp->*p_cccs) (os, ssta);
      if (p_cfin != nullptr) (tlsp->*p_cfin) (os, ssta);
    }
    // receive a server finish sequence
    void rcvfs (InputStream* is, TlsProto* tlsp, TlsState* ssta) {
      // check for nil
      if (tlsp == nullptr) return;
      // save the server verify message buffer
      if (ssta != nullptr) d_svmb = ssta->gethvmb();
      // execute in sequence
      if (p_sccs != nullptr) d_sccs = (tlsp->*p_sccs) (is, ssta);
      if (p_sfin != nullptr) p_mfin = (tlsp->*p_sfin) (is, ssta);
    }
  };
}

#endif
