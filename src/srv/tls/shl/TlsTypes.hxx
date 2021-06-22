// ---------------------------------------------------------------------------
// - TlsTypes.hxx                                                            -
// - afnix:tls service - tls content types definition                        -
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

#ifndef  AFNIX_TLSTYPES_HXX
#define  AFNIX_TLSTYPES_HXX

#ifndef  AFNIX_UTILITY_HPP
#include "Utility.hpp"
#endif

namespace afnix {
  /// the tls major version
  static const t_byte TLS_VMAJ_3XX = 0x03U;
  /// the tls minor version
  static const t_byte TLS_VMIN_301 = 0x01U;
  static const t_byte TLS_VMIN_302 = 0x02U;
  static const t_byte TLS_VMIN_303 = 0x03U;
  // tls version code
  static const t_word TLS_VCOD_NIL = 0x0000U;
  static const t_word TLS_VCOD_V10 = 0x0301U;
  static const t_word TLS_VCOD_V11 = 0x0302U;
  static const t_word TLS_VCOD_V12 = 0x0303U;
  static const t_word TLS_VCOD_ALL = 0xFFFFU;
  
  /// the default tls major version
  static const t_byte TLS_VMAJ_DEF = TLS_VMAJ_3XX;
  /// the default tls minor version
  static const t_byte TLS_VMIN_DEF = TLS_VMIN_303;
  
  // the message content types
  static const t_byte TLS_TYPE_CCS = 0x14U; // change cipher spec
  static const t_byte TLS_TYPE_ALT = 0x15U; // alert
  static const t_byte TLS_TYPE_HSK = 0x16U; // handshake
  static const t_byte TLS_TYPE_APP = 0x17U; // application

  // the message handshake type
  static const t_byte TLS_HSHK_HLR = 0x00U; // hello request
  static const t_byte TLS_HSHK_CLH = 0x01U; // client hello
  static const t_byte TLS_HSHK_SRH = 0x02U; // server hello
  static const t_byte TLS_HSHK_CRT = 0x0BU; // certificate
  static const t_byte TLS_HSHK_SKE = 0x0CU; // server key exchange
  static const t_byte TLS_HSHK_CRR = 0x0DU; // certificate request
  static const t_byte TLS_HSHK_SHD = 0x0EU; // server hello done
  static const t_byte TLS_HSHK_CRV = 0x0FU; // certificate verify
  static const t_byte TLS_HSHK_CKE = 0x10U; // client key exchange
  static const t_byte TLS_HSHK_FIN = 0x14U; // finished
    
  // maximum record length
  static const long   TLS_RLEN_MAX = 16384L;
  // the random sequence size
  static const long   TLS_SIZE_RND = 32L;
  // the (pre)master secret size [1.0/1.1/1.2]
  static const long   TLS_SIZE_MSX = 48L;
  // the finished buffer size
  static const long   TLS_SIZE_FIN = 12L;
  
  // this procedure validates a tls record type
  static inline bool tls_type_valid (const t_byte type) {
    bool result = false;
    switch (type) {
    case TLS_TYPE_CCS:
    case TLS_TYPE_ALT:
    case TLS_TYPE_HSK:
    case TLS_TYPE_APP:
      result = true;
      break;
    default:
      result = false;
      break;
    }
    return result;
  }

  // this procedure validates a tls version
  static inline bool tls_vers_valid (const t_byte vmaj, const t_byte vmin) {
    // check major version
    if (vmaj == TLS_VMAJ_3XX) {
      // check minor version
      if ((vmin == TLS_VMIN_301) || (vmin == TLS_VMIN_302) ||
	  (vmin == TLS_VMIN_303)) return true;
    }
    // invalid major version
    return false;
  }
  
  // this procedure converts a tls version into a string
  static inline String tls_vers_tostring (const t_byte vmaj, 
					  const t_byte vmin) {
    String result = Utility::tostring ((long) vmaj) + '.';
    result += Utility::tostring ((long) vmin);
    return result;
  }
}

#endif
