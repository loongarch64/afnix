// ---------------------------------------------------------------------------
// - Xoid.hpp                                                                -
// - afnix:tls service - standard tls oid class definition                   -
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

#ifndef  AFNIX_XOID_HPP
#define  AFNIX_XOID_HPP

#ifndef  AFNIX_OID_HPP
#include "Oid.hpp"
#endif

namespace afnix {

  /// The Xoid class is a class design to manage the set of standard and
  /// registered oid. Standard oid are predefined in this system An oid can
  /// be accessed by string or by a oid object. This class provides only
  /// static functions for manipulating the oid.
  /// @author amaury darsch

  class Xoid {
  public:
    enum t_toid {
      TLS_ATTR_C,    // country
      TLS_ATTR_L,    // locality
      TLS_ATTR_O,    // organization
      TLS_ATTR_CN,   // common name
      TLS_ATTR_SN,   // serial number
      TLS_ATTR_ST,   // state
      TLS_ATTR_OU,   // organization unit
      TLS_ATTR_SRN,  // surname
      TLS_ATTR_STR,  // street
      TLS_ALGO_RSAE, // RSA encryption
      TLS_ALGO_RSA2, // MD2/RSA encryption
      TLS_ALGO_RSA4, // MD4/RSA encryption
      TLS_ALGO_RSA5, // MD5/RSA encryption
      TLS_ALGO_RSAS  // SHA1/RSA signature
    };

  public:
    /// check if a oid is registered
    /// @param oid the oid representation
    static bool exists (const Oid& oid);
    
    /// check if a oid is registered
    /// @param xoid the oid string representation
    static bool exists (const String& xoid);

    /// map an oid to a oid type
    /// @param oid the oid representation
    static t_toid totoid (const Oid& oid);

    /// map an oid to a oid type
    /// @param xoid the oid string representation
    static t_toid totoid (const String& xoid);
  };
}

#endif
