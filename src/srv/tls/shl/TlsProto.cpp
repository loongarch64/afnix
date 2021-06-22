// ---------------------------------------------------------------------------
// - TlsProto.cpp                                                            -
// - afnix:tls service - tls protocol class implementation                   -
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
#include "Hmac.hpp"
#include "Vector.hpp"
#include "Integer.hpp"
#include "TlsData.hpp"
#include "TlsTypes.hxx"
#include "TlsProto.hxx"
#include "TlsUtils.hpp"
#include "TlsShake.hpp"
#include "TlsCspec.hpp"
#include "TlsAlert.hpp"
#include "TlsCipher.hpp"
#include "TlsChello.hpp"
#include "TlsCkeyxh.hpp"
#include "TlsFinish.hpp"
#include "TlsShello.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"
#include "TlsProto12.hpp"

namespace afnix {
  
  // -------------------------------------------------------------------------
  // - private section                                                       -
  // -------------------------------------------------------------------------

  // compute a hash buffer by prf (tls <= 1.1)
  static Buffer prf_1_0 (const long    slen, const t_byte* sbuf,
			 const String& labl, const Buffer& seed,
			 const long rlen) {
    // check for valid arguments
    if ((slen < 2L) || (sbuf == nullptr)) return Buffer();
    try {
      // compute the subkey address and adjust odd length
      long  sl = slen / 2L;
      const t_byte* s1 = &(sbuf[0]);
      const t_byte* s2 = &(sbuf[sl]);
      if ((sl % 2L) != 0L) sl++;
      // create the hmac keys and hmac object
      Key kmd (Key::CKEY_KMAC, sl, s1); Hmac hmd (kmd, new Md5);
      Key ksh (Key::CKEY_KMAC, sl, s2); Hmac hsh (ksh, new Sha1);
      // create a buffer that merges the label and seed
      Buffer hbuf; hbuf.add (labl); hbuf.add (seed);
      // get the hash buffers
      Buffer bmd = TlsUtils::phash (&hmd, hbuf, rlen);
      Buffer bsh = TlsUtils::phash (&hsh, hbuf, rlen);
      // paranoid check
      if ((bmd.length () != rlen) || (bsh.length () != rlen)) {
	throw Exception ("tls-error", "inconsistent p_hash buffer size");
      }
      // combine both hash buffer
      Buffer result (rlen); result.setrflg (false);
      for (long k = 0L; k < rlen; k++) {
	result.add ((char) (bmd.get (k) ^ bsh.get (k)));
      }
      return result;
    } catch (...) {
      throw;
    }
  }
  
  // -------------------------------------------------------------------------
  // - public section                                                        -
  // -------------------------------------------------------------------------

  // create a tls protocol by version

  TlsProto* TlsProto::create (const t_byte vmaj, const t_byte vmin) {
    // check version
    String vers = tls_vers_tostring (vmaj, vmin);
    if (tls_vers_valid (vmaj, vmin) == false) {
      throw Exception ("tls-error", "invalid tls version", vers);
    }
    // process major version
    if (vmaj == TLS_VMAJ_3XX) {
      switch (vmin) {
      case TLS_VMIN_301:
      case TLS_VMIN_302:
	return new TlsProto;
	break;
      case TLS_VMIN_303:
	return new TlsProto12;
	break;
      default:
	break;
      }
    }
    throw Exception ("tls-error", "cannot create tls protocol version", vers);
  }

  // create a tls protocol by state

  TlsProto* TlsProto::create (TlsState* sta) {
    // check for nil
    if (sta == nullptr) return nullptr;
    // get version and check
    t_byte vmaj = sta->getvmaj ();
    t_byte vmin = sta->getvmin ();
    // create the protocol
    TlsProto* proto = TlsProto::create (vmaj, vmin);
    // check for protocol 1.2
    auto p12 = dynamic_cast<TlsProto12*>(proto);
    if (p12 != nullptr) p12->sethash (sta->newhash ());
    // here it is
    return proto;
  }

  // check for a handshake message

  bool TlsProto::ishblk (TlsInfos* iblk, const t_byte type) {
    // check for handshake block
    auto hblk = dynamic_cast <TlsHblock*> (iblk);
    if (hblk == nullptr) return false;
    // check for a valid type
    if (hblk->gettype () != type) return false;
    // success
    return true;
  }
  
  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // return the class name
  
  String TlsProto::repr (void) const {
    return "TlsProto";
  }

  // compute a hash buffer by prf
  
  Buffer TlsProto::xprf (const Buffer& sbuf, const String& labl,
			 const Buffer& seed, const long    rlen) const {
    rdlock ();
    t_byte* cbuf = nullptr;
    try {
      // get the buffer length and content
      long clen = sbuf.length ();
      cbuf = (t_byte*) sbuf.tochar ();
      // compute the prf
      Buffer result = prf_1_0 (clen, cbuf, labl, seed, rlen);
      delete [] cbuf;
      unlock ();
      return result;
    } catch (...) {
      delete [] cbuf;
      unlock ();
      throw;
    }
  }

  // compute the server verify buffer

  Buffer TlsProto::sfin (const Buffer& mbuf, const Buffer& vbuf) const {
    rdlock ();
    try {
      // create a seed buffer
      Buffer seed;
      Md5  hl; Buffer bl = vbuf; hl.pushb (seed, bl);
      Sha1 hr; Buffer br = vbuf; hr.pushb (seed, br);
      // compute the finished signature
      Buffer result = xprf (mbuf, TLS_LABL_SF, seed, TLS_SIZE_FIN);
      // unlock and return
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // compute the client verify buffer

  Buffer TlsProto::cfin (const Buffer& mbuf, const Buffer& vbuf) const {
    rdlock ();
    try {
      // create a seed buffer
      Buffer seed;
      Md5  hl; Buffer bl = vbuf; hl.pushb (seed, bl);
      Sha1 hr; Buffer br = vbuf; hr.pushb (seed, br);
      // compute the finished signature
      Buffer result = xprf (mbuf, TLS_LABL_CF, seed, TLS_SIZE_FIN);
      // unlock and return
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get a record by input stream and cipher

  TlsRecord* TlsProto::getrcd (InputStream* is, TlsState* sta) const {
    rdlock ();
    try {
      if (is == nullptr) {
	unlock ();
	return nullptr;
      }
      if (sta == nullptr) {
	TlsRecord* rcd = new TlsRecord (is);
	unlock ();
	return rcd;
      }
      // get the decoding cipher
      TlsCipher dc(sta);
      // get the record
      TlsRecord* rcd = new TlsRecord (is, &dc);
      unlock ();
      return rcd;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get a message by record
  
  TlsMessage* TlsProto::getmsg (TlsRecord* rcd) const {
    // check for nil
    if (rcd == nullptr) return nullptr;
    // lock and decode
    rdlock ();
    try {
      // get the record type and check
      t_byte type = rcd->gettype ();
      // prepare result
      TlsMessage* result = nullptr;
      // map the record
      switch (type) {
      case TLS_TYPE_CCS:
	result = new TlsCspec (rcd);
	break;
      case TLS_TYPE_ALT:
	result = new TlsAlert (rcd);
	break;
      case TLS_TYPE_HSK:
	result = new TlsShake (rcd);
	break;
      case TLS_TYPE_APP:
	result = new TlsData (rcd);
	break;
      default:
	throw Exception ("tls-error", "cannot decode record into a message");
	break;
      }
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
   
  // get a message by input stream and state

  TlsMessage* TlsProto::getmsg (InputStream* is, TlsState* sta) const {
    rdlock ();
    TlsRecord* rcd = nullptr;
    try {
      // get the next available record
      rcd = getrcd (is, sta);
      // decode the record
      TlsMessage* msg = getmsg (rcd);
      unlock ();
      return msg;
    } catch (...) {
      delete rcd;
      unlock ();
      throw;
    }
  }
  
  // encode a tls message

  long TlsProto::encode (OutputStream* os, TlsMessage* msg) const {
    // check for nil
    if ((msg == nullptr) || (os == nullptr)) return 0L;
    rdlock ();
    try {
      long result = encode (os, msg, nullptr);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // encode a tls message

  long TlsProto::encode (OutputStream* os,TlsMessage* msg,TlsState* sta) const {
    // check for nil
    if ((msg == nullptr) || (os == nullptr)) return 0L;
    rdlock ();
    try {
      if (sta == nullptr) {
	long result = msg->write (os);
	unlock ();
	return result;
      }
      // get the encoding cipher
      TlsCipher ec(sta);
      // write the message
      long result = msg->write (os, &ec);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // decode a handshake block

  TlsInfos* TlsProto::decode (TlsHblock* hblk) const {
    // check for nil
    if (hblk == nullptr) return nullptr;
    // lock and decode
    rdlock ();
    try {
      // get the block type and check
      t_byte type = hblk->gettype ();
      // prepare result
      TlsInfos* result = nullptr;
      // map the record
      switch (type) {
      case TLS_HSHK_CLH:
	result = new TlsChello (hblk);
	break;
      case TLS_HSHK_SRH:
	result = new TlsShello (hblk);
	break;
      case TLS_HSHK_CKE:
	result = new TlsCkeyxh (hblk);
	break;
      case TLS_HSHK_CRT:
	result = new TlsCerts (hblk);
	break;
      case TLS_HSHK_FIN:
	result = new TlsFinish (hblk);
	break;
      default:
	throw Exception ("tls-error", "cannot decode handshake block");
	break;
      }
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // receive a client hello by input stream and state

  TlsInfos* TlsProto::rcvchlo (InputStream* is, TlsState* sta) const {
    rdlock ();
    TlsHblock*  blk = nullptr;
    try {
      // get the next available message
      TlsMessage* msg = getmsg (is, sta);
      if (msg == nullptr) {
	unlock ();
	return nullptr;
      }
      // push the message in the message buffer [for verify]
      if (sta != nullptr) msg->pushb (sta->gethvmb ());
      // map it to a handshake message
      TlsShake* shk = dynamic_cast <TlsShake*> (msg);
      if (shk == nullptr) {
	throw Exception ("tls-error", "cannot get handshake message");
      }
      // create a handshake iterator [this will destroy shk/msg]
      TlsShakeit sit (shk);
      // get the handshake block
      blk = dynamic_cast <TlsHblock*> (sit.getobj ());
      if (blk == nullptr) {
	throw Exception ("tls-error", "cannot get handshake block");
      }
      // move to the end and check
      sit.next ();
      if (sit.isend () == false) {
	throw Exception ("tls-error", "inconsistent handshake message");
      }
      // get the client block
      TlsInfos* hlo = decode (blk);
      if (hlo == nullptr) {
	throw Exception ("tls-error", "cannot decode client hello block");
      }
      delete blk;
      unlock ();
      return hlo;
    } catch (...) {
      delete blk;
      unlock ();
      throw;
    }
  }
  
  // receive a server hello by input stream and state

  TlsInfos* TlsProto::rcvshlo (InputStream* is, TlsState* sta) const {
    rdlock ();
    TlsHblock*  blk = nullptr;
    try {
      // get the next available message
      TlsMessage* msg = getmsg (is, sta);
      if (msg == nullptr) {
	unlock ();
	return nullptr;
      }
      // push the message in the message buffer [for verify]
      if (sta != nullptr) msg->pushb (sta->gethvmb ());
      // map it to a handshake message
      TlsShake* shk = dynamic_cast <TlsShake*> (msg);
      if (shk == nullptr) {
	throw Exception ("tls-error", "cannot get handshake message");
      }
      // create a handshake iterator [this will destroy shk/msg]
      TlsShakeit sit (shk);
      // get the handshake block
      blk = dynamic_cast <TlsHblock*> (sit.getobj ());
      if (blk == nullptr) {
	throw Exception ("tls-error", "cannot get handshake block");
      }
      // move to the end and check
      sit.next ();
      if (sit.isend () == false) {
	throw Exception ("tls-error", "inconsistent handshake message");
      }
      // get the server hello block
      TlsInfos* hlo = decode (blk);
      if (hlo == nullptr) {
	throw Exception ("tls-error", "cannot decode server hello block");
      }
      delete blk;
      unlock ();
      return hlo;
    } catch (...) {
      delete blk;
      unlock ();
      throw;
    }
  }

  // receive a client handshake by input stream and state

  TlsHblock* TlsProto::rcvchsk(InputStream* is, TlsState* sta) const {
    rdlock ();
    TlsHblock* blk = nullptr;
    try {
      // get the next available message
      TlsMessage* msg = getmsg (is, sta);
      if (msg == nullptr) {
	unlock ();
	return nullptr;
      }
      // push the message in the message buffer [for verify]
      if (sta != nullptr) msg->pushb (sta->gethvmb ());
      // map it to a handshake message
      TlsShake* shk = dynamic_cast <TlsShake*> (msg);
      if (shk == nullptr) {
	throw Exception ("tls-error", "cannot get handshake message");
      }
      // create a handshake iterator [this will destroy msg/shk]
      TlsShakeit sit (shk);
      // get the handshake block
      blk = dynamic_cast <TlsHblock*> (sit.getobj ());
      if (blk == nullptr) {
	throw Exception ("tls-error", "cannot get handshake block");
      }
      // move to the end and check
      sit.next ();
      if (sit.isend () == false) {
	throw Exception ("tls-error", "inconsistent handshake message");
      }
      unlock ();
      return blk;
    } catch (...) {
      delete blk;
      unlock ();
      throw;
    }
  }
  
  // receive a server handshake by input stream and stae

  TlsHblock* TlsProto::rcvshsk (InputStream* is, TlsState* sta) const {
    rdlock ();
    TlsHblock* blk = nullptr;
    try {
      // get the next available message
      TlsMessage* msg = getmsg (is, sta);
      if (msg == nullptr) {
	unlock ();
	return nullptr;
      }
      // push the message in the message buffer [for verify]
      if (sta != nullptr) msg->pushb (sta->gethvmb ());
      // map it to a handshake message
      TlsShake* shk = dynamic_cast <TlsShake*> (msg);
      if (shk == nullptr) {
	throw Exception ("tls-error", "cannot get handshake message");
      }
      // create a handshake iterator [this will destroy msg/shk]
      TlsShakeit sit (shk);
      // get the handshake block
      blk = dynamic_cast <TlsHblock*> (sit.getobj ());
      if (blk == nullptr) {
	throw Exception ("tls-error", "cannot get handshake block");
      }
      // move to the end and check
      sit.next ();
      if (sit.isend () == false) {
	throw Exception ("tls-error", "inconsistent handshake message");
      }
      unlock ();
      return blk;
    } catch (...) {
      delete blk;
      unlock ();
      throw;
    }
  }
  
  // receive a client change cipher spec

  bool TlsProto::rcvcccs (InputStream* is, TlsState* sta) const {
    rdlock ();
    TlsMessage* msg = nullptr;
    try {
      // get the next available message
      if ((msg = getmsg (is, sta)) == nullptr) {
	unlock ();
	return false;
      }
      // map it to a change cipher spec
      TlsCspec* ccs = dynamic_cast <TlsCspec*> (msg);
      if (ccs == nullptr) {
	throw Exception ("tls-error", "cannot get change cipher spec message");
      }
      // update the state with the new cipher
      bool result = (sta == nullptr) ? false : sta->chgccs ();
      delete msg;
      unlock ();
      return result;
    } catch (...) {
      delete msg;
      unlock ();
      throw;
    }
  }

  // receive a server change cipher spec

  bool TlsProto::rcvsccs (InputStream* is, TlsState* sta) const {
    rdlock ();
    TlsMessage* msg = nullptr;
    try {
      // get the next available message
      if ((msg = getmsg (is, sta)) == nullptr) {
	unlock ();
	return false;
      }
      // map it to a change cipher spec
      TlsCspec* ccs = dynamic_cast <TlsCspec*> (msg);
      if (ccs == nullptr) {
	throw Exception ("tls-error", "cannot get change cipher spec message");
      }
      // update the state with the new cipher
      bool result = (sta == nullptr) ? false : sta->chgscs ();
      delete msg;
      unlock ();
      return result;
    } catch (...) {
      delete msg;
      unlock ();
      throw;
    }
  }

  // get a server hello chunk by state

  TlsChunk TlsProto::toshlo (TlsState* sta) const {
    // lock and generate
    rdlock ();
    try {
      // check for nil first
      if (sta == nullptr) {
	throw Exception ("tls-error", "cannot generate server hello chunk");
      }
      // gather the server hello information
      t_byte vmaj = sta->getvmaj ();
      t_byte vmin = sta->getvmin ();
      t_word cifr = sta->getcifr ();
      // create a server hello
      TlsShello shlo (vmaj, vmin, cifr);
      // update the state random sequence
      sta->setsrnd (shlo.getrand ());
      // map it to a chunk
      TlsChunk result = shlo.tochunk ();
      // update the info list
      sta->addinfo (shlo.getinfo ());
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // map a server hello by state

  TlsMessage* TlsProto::getshlo (TlsState* sta) const {
    // check for nil first
    if (sta == nullptr) return nullptr;
    // lock and map
    TlsShake* hsk = nullptr;
    rdlock ();
    try {
      // create a tls handshake by state
      hsk = new TlsShake (sta->getvmaj (), sta->getvmin());
      // get the server hello chunk by state
      TlsChunk chk = toshlo (sta);
      // add the chunk block to the record
      hsk->add (TLS_HSHK_SRH, chk);
      unlock ();
      return hsk;
    } catch (...) {
      delete hsk;
      unlock ();
      throw;
    }
  }
  
  // send a server hello by state
  
  void TlsProto::sndshlo (OutputStream* os, TlsState* sta) const {
    TlsMessage* shlo = nullptr;
    rdlock ();
    try {
      // create a server hello message and encode it
      shlo = getshlo (sta);
      if (shlo == nullptr) {
	throw Exception ("tls-error", "cannot get server hello message");
      }
      // push the message in the message buffer [for verify]
      if (sta != nullptr) shlo->pushb (sta->gethvmb ());
      // encode the message
      encode (os, shlo);
      delete shlo;
      unlock ();
    } catch (...) {
      delete shlo;
      unlock ();
      throw;
    }
  }

  // get a client hello chunk by state

  TlsChunk TlsProto::tochlo (TlsState* sta) const {
    // lock and generate
    rdlock ();
    try {
      // check for nil first
      if (sta == nullptr) {
	throw Exception ("tls-error", "cannot generate client hello chunk");
      }
      // gather the server hello information
      t_byte vmaj = sta->getvmaj ();
      t_byte vmin = sta->getvmin ();
      // create a client hello
      TlsChello chlo (vmaj, vmin);
      // update the state random sequence
      sta->setcrnd (chlo.getrand ());
      // map it to a chunk
      TlsChunk result = chlo.tochunk ();
      // update the info list
      sta->addinfo (chlo.getinfo ());
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // map a client hello by state

  TlsMessage* TlsProto::getchlo (TlsState* sta) const {
    // check for nil first
    if (sta == nullptr) return nullptr;
    // lock and map
    TlsShake* hsk = nullptr;
    rdlock ();
    try {
      // create a tls handshake by state
      hsk = new TlsShake (sta->getvmaj (), sta->getvmin());
      // get the client hello chunk by state
      TlsChunk chk = tochlo (sta);
      // add the chunk block to the record
      hsk->add (TLS_HSHK_CLH, chk);
      unlock ();
      return hsk;
    } catch (...) {
      delete hsk;
      unlock ();
      throw;
    }
  }
  
  // send a client hello by state
  
  void TlsProto::sndchlo (OutputStream* os, TlsState* sta) const {
    TlsMessage* chlo = nullptr;
    rdlock ();
    try {
      // create a client hello message and encode it
      chlo = getchlo (sta);
      if (chlo == nullptr) {
	throw Exception ("tls-error", "cannot get client hello message");
      }
      // push the message in the message buffer [for verify]
      if (sta != nullptr) chlo->pushb (sta->gethvmb ());
      // encode the message
      encode (os, chlo);
      delete chlo;
      unlock ();
    } catch (...) {
      delete chlo;
      unlock ();
      throw;
    }
  }

  // get a client exchange chunk by state

  TlsChunk TlsProto::tockxh (TlsState* sta) const {
    // lock and generate
    rdlock ();
    try {
      // check for nil first
      if (sta == nullptr) {
	throw Exception ("tls-error", "nil state for client key exchange");
      }
      // create a client key exchange
      TlsCkeyxh ckxh(sta);
      // map it to a chunk
      TlsChunk result = ckxh.tochunk (sta);
      // update the info list
      sta->addinfo (ckxh.getinfo ());
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // map a client key exchange by state

  TlsMessage* TlsProto::getckxh (TlsState* sta) const {
    // check for nil first
    if (sta == nullptr) return nullptr;
    // lock and map
    TlsShake* hsk = nullptr;
    rdlock ();
    try {
      // create a tls handshake by state
      hsk = new TlsShake (sta->getvmaj (), sta->getvmin());
      // get the client key exchange chunk by state
      TlsChunk chk = tockxh (sta);
      // add the chunk block to the record
      hsk->add (TLS_HSHK_CKE, chk);
      unlock ();
      return hsk;
    } catch (...) {
      delete hsk;
      unlock ();
      throw;
    }
  }

  // send a client key exchange by state
  
  void TlsProto::sndckxh (OutputStream* os, TlsState* sta) const {
    TlsMessage* ckxh = nullptr;
    rdlock ();
    try {
      // create a client key exchange message and encode it
      ckxh = getckxh (sta);
      if (ckxh == nullptr) {
	throw Exception ("tls-error", "cannot get client key exchange message");
      }
      // push the message in the message buffer [for verify]
      if (sta != nullptr) ckxh->pushb (sta->gethvmb ());
      // encode the message
      encode (os, ckxh);
      delete ckxh;
      unlock ();
    } catch (...) {
      delete ckxh;
      unlock ();
      throw;
    }
  }
  
  // map a client change cipher spec by state

  TlsMessage* TlsProto::getcccs (TlsState* sta) const {
    // check for nil first
    if (sta == nullptr) return nullptr;
    // lock and create
    rdlock ();
    try {
      // create a change cipher spec
      TlsMessage* cccs = new TlsCspec (sta->getvmaj (), sta->getvmin());
      unlock ();
      return cccs;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // send a client change cipher spec by state
  
  bool TlsProto::sndcccs (OutputStream* os, TlsState* sta) const {
    rdlock ();
    TlsMessage* cccs = nullptr;
    try {
      // create a client change cipher spec and encode it
      TlsMessage* cccs = getcccs (sta);
      if (cccs == nullptr) {
	throw Exception ("tls-error", "cannot get client change cipher spec");
      }
      // encode the message
      encode (os, cccs);
      // update the state with the new cipher
      bool result = (sta == nullptr) ? false : sta->chgccs ();
      // done
      delete cccs;
      unlock ();
      return result;
    } catch (...) {
      delete cccs;
      unlock ();
      throw;
    }
  }

  // map a server certificate by state

  TlsMessage* TlsProto::getscrt (TlsState* sta) const {
    // check for nil first
    if (sta == nullptr) return nullptr;
    // lock and map
    TlsShake* hsk = nullptr;
    rdlock ();
    try {
      // get the state certificate
      TlsCerts* cert = sta->getcert ();
      if (cert == nullptr) {
	unlock ();
	return nullptr;
      }
      // create a tls handshake by state
      hsk = new TlsShake (sta->getvmaj (), sta->getvmin());
      // get the certificate chunk
      TlsChunk chk = cert->tochunk ();
      // add the chunk block to the record
      hsk->add (TLS_HSHK_CRT, chk);
      unlock ();
      return hsk;
    } catch (...) {
      delete hsk;
      unlock ();
      throw;
    }
  }

  // send a server certificate
  
  void TlsProto::sndscrt (OutputStream* os, TlsState* sta) const {
    TlsMessage* cert = nullptr;
    rdlock ();
    try {
      // get the certificate message
      cert = getscrt (sta);
      // check for a valid message to push and encode
      if (cert != nullptr) {
	// push the message in the message buffer [for verify]
	if (sta != nullptr) cert->pushb (sta->gethvmb ());
	// encode the message
	encode (os, cert);
      }
      delete cert;
      unlock ();
    } catch (...) {
      delete cert;
      unlock ();
      throw;
    }
  }

  // map a server done by state

  TlsMessage* TlsProto::getsdon (TlsState* sta) const {
    // check for nil first
    if (sta == nullptr) return nullptr;
    // lock and map
    TlsShake* hsk = nullptr;
    rdlock ();
    try {
      // create a tls handshake by state
      hsk = new TlsShake (sta->getvmaj (), sta->getvmin());
      // add the chunk block to the record
      hsk->add (TLS_HSHK_SHD, TlsChunk());
      unlock ();
      return hsk;
    } catch (...) {
      delete hsk;
      unlock ();
      throw;
    }
  }

  // send a certificate
  
  void TlsProto::sndsdon (OutputStream* os, TlsState* sta) const {
    TlsMessage* done = nullptr;
    rdlock ();
    try {
      // get the server done message and encode it
      done = getsdon (sta);
      // check for valid message
      if (done != nullptr) {
	// push the message in the message buffer [for verify]
	if (sta != nullptr) done->pushb (sta->gethvmb ());
	// encode the message
	encode (os, done);
      }
      delete done;
      unlock ();
    } catch (...) {
      delete done;
      unlock ();
      throw;
    }
  }

  // map a server change cipher spec by state

  TlsMessage* TlsProto::getsccs (TlsState* sta) const {
    // check for nil first
    if (sta == nullptr) return nullptr;
    // lock and map
    TlsCspec* ccs = nullptr;
    rdlock ();
    try {
      // create a tls change cipher spec by state
      ccs = new TlsCspec (sta->getvmaj (), sta->getvmin());
      unlock ();
      return ccs;
    } catch (...) {
      delete ccs;
      unlock ();
      throw;
    }
  }
  
  // send the server change cipher spec by state

  bool TlsProto::sndsccs (OutputStream* os, TlsState* sta) const {
    TlsMessage* sccs = nullptr;
    rdlock ();
    try {
      // get the server finish message and encode it
      sccs = getsccs (sta);
      // check for valid message and encode it
      if (sccs == nullptr) {
	unlock ();
	return false;
      }
      encode (os, sccs, sta);
      // update the state with the new cipher
      bool result = (sta == nullptr) ? false : sta->chgscs ();
      // done
      delete sccs;
      unlock ();
      return result;
    } catch (...) {
      delete sccs;
      unlock ();
      throw;
    }
  }
  
  // map a client/server finish by state

  TlsMessage* TlsProto::getxfin (TlsState* sta) const {
    // check for nil first
    if (sta == nullptr) return nullptr;
    // lock and map
    TlsShake* hsk = nullptr;
    rdlock ();
    try {
      // create a tls handshake by state
      hsk = new TlsShake (sta->getvmaj (), sta->getvmin());
      // compute the finished signature
      Buffer mbuf = sta->getmbuf();
      Buffer hvmb = sta->gethvmb();
      Buffer finb = sta->getsflg () ? sfin (mbuf, hvmb) : cfin (mbuf, hvmb);
      if (finb.length() != TLS_SIZE_FIN){
	throw Exception ("tls-error", "invalid finished buffer size");
      }
      // add the chunk block to the record
      hsk->add (TLS_HSHK_FIN, TlsChunk(finb));
      unlock ();
      return hsk;
    } catch (...) {
      delete hsk;
      unlock ();
      throw;
    }
  }
  
  // send the client/server finished message

  void TlsProto::sndxfin (OutputStream* os, TlsState* sta) const {
    TlsMessage* mfin = nullptr;
    rdlock ();
    try {
      // get the server finish message and encode it
      mfin = getxfin (sta);
      if ((sta != nullptr) && (sta->getsflg () == false)) {
	// push the message in the message buffer [for verify]
	if (mfin != nullptr) mfin->pushb (sta->gethvmb ());
      }
      // check for valid message and encode it
      if (mfin != nullptr) encode (os, mfin, sta);
      // done
      delete mfin;
      unlock ();
    } catch (...) {
      delete mfin;
      unlock ();
      throw;
    }
  }

  // pop the next available buffer

  Buffer TlsProto::popb (InputStream* is, TlsState* sta) const {
    rdlock ();
    try {
      // get the next available message
      TlsMessage* msg = getmsg (is, sta);
      if (msg == nullptr) {
	throw Exception ("tls-error", "cannot pop tls message in pop buffer");
      }
      // check for a data message
      TlsData* data = dynamic_cast<TlsData*> (msg);
      if (data != nullptr) {
	Buffer result;
	data->pushb (result);
	unlock ();
	return result;
      }
      // check for an alert
      TlsAlert* alrt = dynamic_cast<TlsAlert*> (msg);
      if (alrt != nullptr) {
	TlsAlert result = *alrt;
	throw result;
      }
      // invalid message in loop
      throw Exception ("tls-error", "invalid message in pop buffer",
		       Object::repr (msg));
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // push a buffer to the stream

  long TlsProto::pushb (OutputStream* os, TlsState* sta,
			const Buffer& buf) const {
    rdlock();
    TlsMessage* msg = nullptr;
    try {
      // check for valid state
      if (sta == nullptr) {
	throw Exception ("tls-error", "invalid nil state in push buffer");
      }
      // create a tls data message
      msg = new TlsData (sta->getvmaj(), sta->getvmin());
      // push the buffer in the message
      msg->add (buf);
      // encode the message
      long result = encode (os, msg, sta);
      // done
      delete msg;
      unlock ();
      return result;
    } catch (...) {
      delete msg;
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
  static const long QUARK_DECODE = zone.intern ("decode");
  static const long QUARK_GETMSG = zone.intern ("get-message");

  // create a new object in a generic way

  Object* TlsProto::mknew (Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();

    // check for 0 argument
    if (argc == 0) return new TlsProto;
    // too many arguments
    throw Exception ("argument-error", 
                     "too many argument with tls protocol constructor");
  }

  // return true if the given quark is defined

  bool TlsProto::isquark (const long quark, const bool hflg) const {
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
  
  Object* TlsProto::apply (Evaluable* zobj, Nameset* nset, const long quark,
			   Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();
    
    // dispatch 1 argument
    if (argc == 1) {
      if (quark == QUARK_DECODE) {
	Object*     obj = argv->get (0);
	TlsHblock* hblk = dynamic_cast<TlsHblock*> (obj);
	if (hblk == nullptr) {
	  throw Exception ("type-error", "invalid object as handshake block",
			   Object::repr (obj));
	}
	return decode (hblk);
      }
      if (quark == QUARK_GETMSG) {
	Object*    obj = argv->get (0);
	TlsRecord* rcd = dynamic_cast<TlsRecord*> (obj);
	if (rcd == nullptr) {
	  throw Exception ("type-error", "invalid object as record",
			   Object::repr (obj));
	}
	return getmsg (rcd);
      }
    }
    // call the object method
    return Object::apply (zobj, nset, quark, argv);
  }
}
