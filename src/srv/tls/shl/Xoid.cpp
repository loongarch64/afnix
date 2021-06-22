// ---------------------------------------------------------------------------
// - Xoid.cpp                                                                -
// - afnix:tls service - standard tld oid class implementation               -
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

#include "Xoid.hpp"
#include "Xoid.hxx"
#include "Exception.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - public section                                                        -
  // -------------------------------------------------------------------------

  // check if a oid exists in the definition

  bool Xoid::exists (const Oid& oid) {
    return Xoid::exists (oid.format ());
  }

  // check if a oid exists in the definition

  bool Xoid::exists (const String& xoid) {
    // loop in the oid table
    for (long k = 0; k < TLS_XOID_SIZE; k++) {
      if (xoid == TLS_XOID_BASE[k].d_xoid) return true;
    }
    return false;
  }

  // map an oid to a oid type

  Xoid::t_toid Xoid::totoid (const Oid& oid) {
    return Xoid::totoid (oid.format ());
  }

  // map an oid description to a oid type

  Xoid::t_toid Xoid::totoid (const String& xoid) {
    // check for rsa encryption
    if (xoid == XOID_ALGO_RSAE) return TLS_ALGO_RSAE;
    if (xoid == XOID_ALGO_RSA2) return TLS_ALGO_RSA2;
    if (xoid == XOID_ALGO_RSA4) return TLS_ALGO_RSA4;
    if (xoid == XOID_ALGO_RSA5) return TLS_ALGO_RSA5;
    if (xoid == XOID_ALGO_RSAS) return TLS_ALGO_RSAS;
    if (xoid == XOID_ATTR_C)    return TLS_ATTR_C;
    if (xoid == XOID_ATTR_L)    return TLS_ATTR_L;
    if (xoid == XOID_ATTR_O)    return TLS_ATTR_O;
    if (xoid == XOID_ATTR_CN)   return TLS_ATTR_CN;
    if (xoid == XOID_ATTR_SN)   return TLS_ATTR_SN;
    if (xoid == XOID_ATTR_ST)   return TLS_ATTR_ST;
    if (xoid == XOID_ATTR_OU)   return TLS_ATTR_OU;
    if (xoid == XOID_ATTR_SRN)  return TLS_ATTR_SRN;
    if (xoid == XOID_ATTR_STR)  return TLS_ATTR_STR;
    // not found
    throw Exception ("xoid-error", "cannot map oid description", xoid);
  }
}
