// ---------------------------------------------------------------------------
// - TlsExtension.hxx                                                        -
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

#ifndef  AFNIX_TLSEXTENSION_HXX
#define  AFNIX_TLSEXTENSION_HXX

#ifndef  AFNIX_CRYPTO_HPP
#include "Crypto.hpp"
#endif

namespace afnix {

  // the supported extension code
  static const t_word TLS_EXTN_00 = 0x0000U; // no extension
  static const t_word TLS_EXTN_13 = 13;      // signature and hash

  // convert a byte to a hash code
  static Crypto::t_hash tls_extn_hash (const t_byte code) {
    if (code == 0x00U) return Crypto::HASH_NIL;
    else if (code == 0x01U) return Crypto::HASH_MD5;
    else if (code == 0x02U) return Crypto::HASH_SHA1;
    else if (code == 0x03U) return Crypto::HASH_SHA224;
    else if (code == 0x04U) return Crypto::HASH_SHA256;
    else if (code == 0x05U) return Crypto::HASH_SHA384;
    else if (code == 0x06U) return Crypto::HASH_SHA512;
    throw Exception ("tls-error", "invalid hash code to decode");
  }

  // convert a byte to a signature code
  static Crypto::t_sign tls_extn_sign (const t_byte code) {
    if (code == 0x00U) return Crypto::SIGN_NIL;
    else if (code == 0x01U) return Crypto::SIGN_RSA;
    else if (code == 0x02U) return Crypto::SIGN_DSA;
    throw Exception ("tls-error", "invalid sign code to decode");
  }
  
  // the base extension structure
  struct s_extn {
    // the extension code
    t_word d_code;
    // create a nil extension
    s_extn (void) {
      d_code = TLS_EXTN_00;
    }
  };

  // 13: signature and hash algorithm

  struct s_extn13 : s_extn {
    // the hasher code
    Crypto::t_hash d_hash;
    // the signer code
    Crypto::t_sign d_sign;
    // create a default extension
    s_extn13 (void) {
      d_code = TLS_EXTN_13;
      d_hash = Crypto::HASH_SHA256;
      d_sign = Crypto::SIGN_RSA;
    }
    // create an extension by data
    s_extn13 (const long size, const t_byte* data) {
      d_hash = Crypto::HASH_NIL;
      d_sign = Crypto::SIGN_NIL;
      decode (size, data);
    }
    // decode an extension by data
    void decode (const long size, const t_byte* data) {
      if (size != 2) {
	throw Exception ("tls-error", "invalid extension size to decode");
      }
      d_hash = tls_extn_hash (data[0]);
      d_sign = tls_extn_sign (data[1]);
    }
  };

  // decode an extension by code and data
  static s_extn* tls_extn_xx (const t_word code,
			      const long size, const t_byte* data) {
    if (code == TLS_EXTN_00) return nullptr;
    else if (code == TLS_EXTN_13) return new s_extn13 (size, data);
    throw Exception ("tls-error", "invalid extension to decode");
  }
}

#endif
