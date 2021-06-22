// ---------------------------------------------------------------------------
// - TlsAlert.hpp                                                            -
// - afnix:tls service - tls alert message class definition                  -
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

#ifndef  AFNIX_TLSALERT_HPP
#define  AFNIX_TLSALERT_HPP

#ifndef  AFNIX_TLSMESSAGE_HPP
#include "TlsMessage.hpp"
#endif

namespace afnix {

  /// The TlsAlert class is the  alert message class for the tls protocol.
  /// A alert message contains only a level and a description.
  /// @author amaury darsch

  class TlsAlert : public TlsMessage {
  public:
    /// the alert level type
    enum t_levl {
      TLS_LEVL_WRN, // warning
      TLS_LEVL_FTL  // fatal
    };

    /// the alert description
    enum t_desc {
      TLS_DESC_CLS, // close notify
      TLS_DESC_UMS, // unexpected message
      TLS_DESC_BRM, // bad record mac
      TLS_DESC_CFL, // decryption failed
      TLS_DESC_ROF, // record overflow
      TLS_DESC_DFL, // decompression failure
      TLS_DESC_HFL, // handshake failure
      TLS_DESC_BCT, // bad certificate
      TLS_DESC_NCT, // unsupported certificate
      TLS_DESC_RCT, // certificate revoked
      TLS_DESC_ECT, // certificate expired
      TLS_DESC_UCT, // certificate unknown
      TLS_DESC_PRM, // illegal parameter
      TLS_DESC_UCA, // unknown ca
      TLS_DESC_DND, // access denied
      TLS_DESC_DEC, // decode error
      TLS_DESC_DCY, // decrypt error
      TLS_DESC_EXP, // export restriction
      TLS_DESC_PVR, // protocol version
      TLS_DESC_SEC, // insufficient security
      TLS_DESC_ERR, // internal error
      TLS_DESC_USR, // user canceled
      TLS_DESC_NRG  // no renegotiation
    };

  protected:
    /// the alert level
    t_levl d_levl;
    /// the alert description
    t_desc d_desc;

  public:
    /// create an empty alert
    TlsAlert (void);

    /// create a default version alert
    /// @param desc the alert description
    TlsAlert (const t_desc desc);

    /// create an alert by version and description
    /// @param vmaj the major version
    /// @param vmin the minor version
    /// @param desc the alert description
    TlsAlert (const t_byte vmaj, const t_byte vmin, const t_desc desc);
    
    /// create a alert by record
    /// @param rcd the tls record
    TlsAlert (TlsRecord* rcd);

    /// copy construct this alert
    /// @param that the object to copy
    TlsAlert (const TlsAlert& that);

    /// assign a record to this one
    /// @param that the record to assign
    TlsAlert& operator = (const TlsAlert& that);

    /// @return the class name
    String repr (void) const;

    /// @return a clone of this object
    Object* clone (void) const;

    /// reset this alert
    void reset (void);

    /// @return the message info as a plist
    Plist getinfo (void) const;
    
    /// decode the alert message
    /// @param rcd the record to decode
    virtual void decode (void);

    /// map an alert to a chunk block
    virtual TlsChunk tochunk (void) const;
    
    /// @return the alert descriptor
    virtual t_desc getdesc (void) const;
    
  public:
    /// create a new object in a generic way
    /// @param argv the argument vector
    static Object* mknew (Vector* argv);
  };
}

#endif
