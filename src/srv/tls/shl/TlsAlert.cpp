// ---------------------------------------------------------------------------
// - TlsAlert.cpp                                                            -
// - afnix:tls service - tls alert message class implementation              -
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
#include "TlsAlert.hpp"
#include "TlsTypes.hxx"
#include "QuarkZone.hpp"
#include "Exception.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - private section                                                       -
  // -------------------------------------------------------------------------

  // the alert level code
  static const t_byte TLS_ALRT_WRN = 0x01U; // warning
  static const t_byte TLS_ALRT_FTL = 0x02U; // fatal
  // the alert description code
  static const t_byte TLS_ALRT_CLS = 0x00U; // close notify
  static const t_byte TLS_ALRT_UMS = 0x0AU; // unexpected message
  static const t_byte TLS_ALRT_BRM = 0x14U; // bad record mac
  static const t_byte TLS_ALRT_CFL = 0x15U; // decryption failed
  static const t_byte TLS_ALRT_ROF = 0x16U; // record overflow
  static const t_byte TLS_ALRT_DFL = 0x1EU; // decompression failure
  static const t_byte TLS_ALRT_HFL = 0x28U; // handshake failure
  static const t_byte TLS_ALRT_BCT = 0x2AU; // bad certificate
  static const t_byte TLS_ALRT_NCT = 0x2BU; // unsupported certificate
  static const t_byte TLS_ALRT_RCT = 0x2CU; // certificate revoked
  static const t_byte TLS_ALRT_ECT = 0x2DU; // certificate expired
  static const t_byte TLS_ALRT_UCT = 0x2EU; // certificate unknown
  static const t_byte TLS_ALRT_PRM = 0x2FU; // illegal parameter
  static const t_byte TLS_ALRT_UCA = 0x30U; // unknown ca
  static const t_byte TLS_ALRT_DND = 0x31U; // access denied
  static const t_byte TLS_ALRT_DEC = 0x32U; // decode error
  static const t_byte TLS_ALRT_DCY = 0x33U; // decrypt error
  static const t_byte TLS_ALRT_EXP = 0x3CU; // export restriction
  static const t_byte TLS_ALRT_PVR = 0x46U; // protocol version
  static const t_byte TLS_ALRT_SEC = 0x47U; // insufficient security
  static const t_byte TLS_ALRT_ERR = 0x50U; // internal error
  static const t_byte TLS_ALRT_USR = 0x5AU; // user canceled
  static const t_byte TLS_ALRT_NRG = 0x64U; // no renegotiation

  // this procedure converts a byte into a level
  static TlsAlert::t_levl tls_to_levl(const t_byte b) {
    TlsAlert::t_levl levl;
    switch (b) {
    case TLS_ALRT_WRN:
      levl = TlsAlert::TLS_LEVL_WRN;
      break;
    case TLS_ALRT_FTL:
      levl = TlsAlert::TLS_LEVL_FTL;
      break;
    default:
      throw Exception ("tls-error", "invalid tls alert level code");
      break;
    }
    return levl;
  }
  
  // this procedure converts an alert level to a byte
  static t_byte tls_to_byte (const TlsAlert::t_levl levl) {
    t_byte result = nilc;
    switch (levl) {
    case TlsAlert::TLS_LEVL_WRN:
      result = TLS_ALRT_WRN;
      break;
    case TlsAlert::TLS_LEVL_FTL:
      result = TLS_ALRT_FTL;
      break;
    }
    return result;
  }

  // this procedure converts a byte into a description
  static TlsAlert::t_desc tls_to_desc(const t_byte b) {
    TlsAlert::t_desc desc;
    switch (b) {
    case TLS_ALRT_CLS:
      desc = TlsAlert::TLS_DESC_CLS;
      break;
    case TLS_ALRT_UMS:
      desc = TlsAlert::TLS_DESC_UMS;
      break;
    case TLS_ALRT_BRM:
      desc = TlsAlert::TLS_DESC_BRM;
      break;
    case TLS_ALRT_CFL:
      desc = TlsAlert::TLS_DESC_CFL;
      break;
    case TLS_ALRT_ROF:
      desc = TlsAlert::TLS_DESC_ROF;
      break;
    case TLS_ALRT_DFL:
      desc = TlsAlert::TLS_DESC_DFL;
      break;
    case TLS_ALRT_HFL:
      desc = TlsAlert::TLS_DESC_HFL;
      break;
    case TLS_ALRT_BCT:
      desc = TlsAlert::TLS_DESC_BCT;
      break;
    case TLS_ALRT_NCT:
      desc = TlsAlert::TLS_DESC_NCT;
      break;
    case TLS_ALRT_RCT:
      desc = TlsAlert::TLS_DESC_RCT;
      break;
    case TLS_ALRT_ECT:
      desc = TlsAlert::TLS_DESC_ECT;
      break;
    case TLS_ALRT_UCT:
      desc = TlsAlert::TLS_DESC_UCT;
      break;
    case TLS_ALRT_PRM:
      desc = TlsAlert::TLS_DESC_PRM;
      break;
    case TLS_ALRT_UCA:
      desc = TlsAlert::TLS_DESC_UCA;
      break;
    case TLS_ALRT_DND:
      desc = TlsAlert::TLS_DESC_DND;
      break;
    case TLS_ALRT_DEC:
      desc = TlsAlert::TLS_DESC_DEC;
      break;
    case TLS_ALRT_DCY:
      desc = TlsAlert::TLS_DESC_DCY;
      break;
    case TLS_ALRT_EXP:
      desc = TlsAlert::TLS_DESC_EXP;
      break;
    case TLS_ALRT_PVR:
      desc = TlsAlert::TLS_DESC_PVR;
      break;
    case TLS_ALRT_SEC:
      desc = TlsAlert::TLS_DESC_SEC;
      break;
    case TLS_ALRT_ERR:
      desc = TlsAlert::TLS_DESC_ERR;
      break;
    case TLS_ALRT_USR:
      desc = TlsAlert::TLS_DESC_USR;
      break;
    case TLS_ALRT_NRG:
      desc = TlsAlert::TLS_DESC_NRG;
      break;
    default:
      throw Exception ("tls-error", "invalid tls alert description code");
      break;
    }
    return desc;
  }
  
  // this procedure converts an alert description
  static t_byte tls_to_byte (const TlsAlert::t_desc desc) {
    t_byte result = nilc;
    switch (desc) {
    case TlsAlert::TLS_DESC_CLS:
      result = TLS_ALRT_CLS;
      break;
    case TlsAlert::TLS_DESC_UMS:
      result = TLS_ALRT_UMS;
      break;
    case TlsAlert::TLS_DESC_BRM:
      result = TLS_ALRT_BRM;
      break;
    case TlsAlert::TLS_DESC_CFL:
      result = TLS_ALRT_CFL;
      break;
    case TlsAlert::TLS_DESC_ROF:
      result = TLS_ALRT_ROF;
      break;
    case TlsAlert::TLS_DESC_DFL:
      result = TLS_ALRT_DFL;
      break;
    case TlsAlert::TLS_DESC_HFL:
      result = TLS_ALRT_HFL;
      break;
    case TlsAlert::TLS_DESC_BCT:
      result = TLS_ALRT_BCT;
      break;
    case TlsAlert::TLS_DESC_NCT:
      result = TLS_ALRT_NCT;
      break;
    case TlsAlert::TLS_DESC_RCT:
      result = TLS_ALRT_RCT;
      break;
    case TlsAlert::TLS_DESC_ECT:
      result = TLS_ALRT_ECT;
      break;
    case TlsAlert::TLS_DESC_UCT:
      result = TLS_ALRT_UCT;
      break;
    case TlsAlert::TLS_DESC_PRM:
      result = TLS_ALRT_PRM;
      break;
    case TlsAlert::TLS_DESC_UCA:
      result = TLS_ALRT_UCA;
      break;
    case TlsAlert::TLS_DESC_DND:
      result = TLS_ALRT_DND;
      break;
    case TlsAlert::TLS_DESC_DEC:
      result = TLS_ALRT_DEC;
      break;
    case TlsAlert::TLS_DESC_DCY:
      result = TLS_ALRT_DCY;
      break;
    case TlsAlert::TLS_DESC_EXP:
      result = TLS_ALRT_EXP;
      break;
    case TlsAlert::TLS_DESC_PVR:
      result = TLS_ALRT_PVR;
      break;
    case TlsAlert::TLS_DESC_SEC:
      result = TLS_ALRT_SEC;
      break;
    case TlsAlert::TLS_DESC_ERR:
      result = TLS_ALRT_ERR;
      break;
    case TlsAlert::TLS_DESC_USR:
      result = TLS_ALRT_USR;
      break;
    case TlsAlert::TLS_DESC_NRG:
      result = TLS_ALRT_NRG;
      break;
    }
    return result;
  }
  
  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create an empty alert - fatal close notify

  TlsAlert::TlsAlert (void) {
    d_levl = TLS_LEVL_FTL;
    d_desc = TLS_DESC_CLS;
  }

  // create an alert by description

  TlsAlert::TlsAlert (const t_desc desc) : TlsMessage (TLS_TYPE_ALT,
						       TLS_VMAJ_DEF,
						       TLS_VMIN_DEF) {
    // set the alert level
    d_levl = TLS_LEVL_FTL;
    d_desc = desc;
    // add a chunk to the record
    add (tochunk());
  }
  
  // create an alert by description

  TlsAlert::TlsAlert (const t_byte vmaj, const t_byte vmin, const t_desc desc) :
    TlsMessage (TLS_TYPE_ALT, vmaj, vmin) {
    // set the alert level
    d_levl = TLS_LEVL_FTL;
    d_desc = desc;
    // add a chunk to the record
    add (tochunk());
  }
  
  // create a message by record

  TlsAlert::TlsAlert (TlsRecord* rcd) : TlsMessage (rcd) {
    d_levl = TLS_LEVL_FTL;
    d_desc = TLS_DESC_CLS;
    decode ();
  }

  // copy construct this alert

  TlsAlert::TlsAlert (const TlsAlert& that) {
    that.rdlock ();
    try {
      // copy the message
      TlsMessage::operator = (that);
      // copy localy
      d_levl = that.d_levl;
      d_desc = that.d_desc;
      that.unlock ();
    } catch (...) {
      that.unlock ();
      throw;
    }
  }

  // assign a alert to this one

  TlsAlert& TlsAlert::operator = (const TlsAlert& that) {
    // check for self-assignation
    if (this == &that) return *this;
    // lock and assign
    wrlock ();
    that.rdlock ();
    try {
      // copy the message
      TlsMessage::operator = (that);
      // copy localy
      d_levl = that.d_levl;
      d_desc = that.d_desc;
      unlock ();
      that.unlock ();
      return *this;
    } catch (...) {
      unlock ();
      that.unlock ();
      throw;
    }
  }

  // get the class name

  String TlsAlert::repr (void) const {
    return "TlsAlert";
  }

  // clone this object

  Object* TlsAlert::clone (void) const {
    return new TlsAlert (*this);
  }

  // reset the alert

  void TlsAlert::reset (void) {
    wrlock ();
    try {
      // reset base message
      TlsMessage::reset ();
      // reset locally
      d_levl = TLS_LEVL_FTL;
      d_desc = TLS_DESC_CLS;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the message info as a plist

  Plist TlsAlert::getinfo (void) const {
    rdlock ();
    try {
      // create a header result plist
      Plist plst = TlsHeader::getinfo ();
      // set the message info
      plst.add ("TLS-MSG-TYPE", "TLS ALERT", (t_long) gettype  ());
      // here it is
      unlock ();
      return plst;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // decode a tls alert message

  void TlsAlert::decode (void) {
    wrlock ();
    try {
      d_levl = tls_to_levl (getbyte (0));
      d_desc = tls_to_desc (getbyte (1));
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // map an alert to a chunk block

  TlsChunk TlsAlert::tochunk (void) const {
    rdlock ();
    try {
      // the result chunk
      TlsChunk result;
      // add the level
      result.add (tls_to_byte(d_levl));
      // add the description
      result.add (tls_to_byte(d_desc));
      // here it is
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the alert decsriptor

  TlsAlert::t_desc TlsAlert::getdesc (void) const {
    rdlock ();
    try {
      t_desc result = d_desc;
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

  Object* TlsAlert::mknew (Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();

    // check for 0 argument
    if (argc == 0) return new TlsAlert;
    // check for 1 argument
    if (argc == 1) {
      Object*    obj = argv->get (0);
      TlsRecord* rcd = dynamic_cast<TlsRecord*> (obj);
      if ((rcd == nullptr) && (obj != nullptr)) {
	throw Exception ("type-error", "invalid object as tls record",
			 Object::repr (obj));
      }
      return new TlsAlert (rcd);
    }
    // too many arguments
    throw Exception ("argument-error", 
                     "too many argument with tls alert constructor");
  }
}
