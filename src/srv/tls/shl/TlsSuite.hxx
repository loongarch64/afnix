// ---------------------------------------------------------------------------
// - TlsSuite.hxx                                                            -
// - afnix:tls service - tls cipher suite internals                          -
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

#ifndef  AFNIX_TLSSUITE_HXX
#define  AFNIX_TLSSUITE_HXX

#ifndef  AFNIX_TLSSUITE_HPP
#include "TlsSuite.hpp"
#endif

#ifndef  AFNIX_TLSTYPES_HXX
#include "TlsTypes.hxx"
#endif

namespace afnix {
  // rfc::2246 cipher suite
  static const t_word TLS_NULL_WITH_NULL_NULL         = 0x0000U;
  static const t_word TLS_RSA_WITH_NULL_MD5           = 0x0001U;
  static const t_word TLS_RSA_WITH_NULL_SHA           = 0x0002U;
  static const t_word TLS_RSA_WITH_RC4_128_MD5        = 0x0004U;
  static const t_word TLS_RSA_WITH_RC4_128_SHA        = 0x0005U;
  // rfc::3268 cipher suite
  static const t_word TLS_RSA_WITH_AES_128_CBC_SHA    = 0x002FU;
  static const t_word TLS_RSA_WITH_AES_256_CBC_SHA    = 0x0035U;
  // rfc 5246 cipher suite
  static const t_word TLS_RSA_WITH_AES_128_CBC_SHA256 = 0x003CU;
  static const t_word TLS_RSA_WITH_AES_256_CBC_SHA256 = 0x003DU;
  // rfc 5288 cipher suite
  static const t_word TLS_RSA_WITH_AES_128_GCM_SHA256 = 0x009CU;
  static const t_word TLS_RSA_WITH_AES_256_GCM_SHA384 = 0x009DU;
  
  // the cipher list definition
  static const TlsSuite::s_cnfo TLS_CNFO_LST[] =
    {
      // 0x00, 0x00
      {
	"TLS_NULL_WITH_NULL_NULL", TLS_NULL_WITH_NULL_NULL,
	TLS_VCOD_V10, TLS_VCOD_ALL,
	TlsSuite::TLS_EXCH_NIL, 
	TlsSuite::TLS_CIFR_NIL, TlsSuite::TLS_CMOD_NIL, 0L, 0L, 0L,
	TlsSuite::TLS_HASH_NIL, 0L, false, false
      },
      // 0x00, 0x01
      {
	"TLS_RSA_WITH_NULL_MD5", TLS_RSA_WITH_NULL_MD5,
	TLS_VCOD_V10, TLS_VCOD_ALL,
	TlsSuite::TLS_EXCH_RSA, 
	TlsSuite::TLS_CIFR_NIL, TlsSuite::TLS_CMOD_NIL, 0L, 0L, 0L,
	TlsSuite::TLS_HASH_MD5, 16L, true, true
      },
      // 0x00, 0x02
      {
	"TLS_RSA_WITH_NULL_SHA", TLS_RSA_WITH_NULL_SHA,
	TLS_VCOD_V10, TLS_VCOD_ALL,
	TlsSuite::TLS_EXCH_RSA, 
	TlsSuite::TLS_CIFR_NIL, TlsSuite::TLS_CMOD_NIL, 0L, 0L, 0L,
	TlsSuite::TLS_HASH_SHA, 20L, true, true
      },
      // 0x00, 0x04
      {
	"TLS_RSA_WITH_RC4_128_MD5", TLS_RSA_WITH_RC4_128_MD5,
	TLS_VCOD_NIL, TLS_VCOD_NIL,
	TlsSuite::TLS_EXCH_RSA, 
	TlsSuite::TLS_CIFR_RC4, TlsSuite::TLS_CMOD_NIL, 16L, 0L, 0L,
	TlsSuite::TLS_HASH_MD5, 16L, true, true
      },
      // 0x00, 0x05
      {
	"TLS_RSA_WITH_RC4_128_SHA", TLS_RSA_WITH_RC4_128_SHA,
	TLS_VCOD_NIL, TLS_VCOD_NIL,
	TlsSuite::TLS_EXCH_RSA, 
	TlsSuite::TLS_CIFR_RC4, TlsSuite::TLS_CMOD_NIL, 16L, 0L, 0L,
	TlsSuite::TLS_HASH_SHA, 20L, true, true
      },
      // 0x00, 0x2F
      {
	"TLS_RSA_WITH_AES_128_CBC_SHA", TLS_RSA_WITH_AES_128_CBC_SHA,
	TLS_VCOD_V10, TLS_VCOD_V10,
	TlsSuite::TLS_EXCH_RSA, 
	TlsSuite::TLS_CIFR_AES, TlsSuite::TLS_CMOD_CBC, 16L, 16L, 16L,
	TlsSuite::TLS_HASH_SHA, 20L, true, true
      },
      {
	"TLS_RSA_WITH_AES_128_CBC_SHA", TLS_RSA_WITH_AES_128_CBC_SHA,
	TLS_VCOD_V11, TLS_VCOD_ALL,
	TlsSuite::TLS_EXCH_RSA, 
	TlsSuite::TLS_CIFR_AES, TlsSuite::TLS_CMOD_EBC, 16L, 16L, 0L,
	TlsSuite::TLS_HASH_SHA, 20L, true, true
      },
      // 0x00, 0x35
      {
	"TLS_RSA_WITH_AES_256_CBC_SHA", TLS_RSA_WITH_AES_256_CBC_SHA,
	TLS_VCOD_V10, TLS_VCOD_V10,
	TlsSuite::TLS_EXCH_RSA, 
	TlsSuite::TLS_CIFR_AES, TlsSuite::TLS_CMOD_CBC, 32L, 16L, 16L,
	TlsSuite::TLS_HASH_SHA, 20L, true, true
      },
      {
	"TLS_RSA_WITH_AES_256_CBC_SHA", TLS_RSA_WITH_AES_256_CBC_SHA,
	TLS_VCOD_V11, TLS_VCOD_ALL,
	TlsSuite::TLS_EXCH_RSA, 
	TlsSuite::TLS_CIFR_AES, TlsSuite::TLS_CMOD_EBC, 32L, 16L, 0L,
	TlsSuite::TLS_HASH_SHA, 20L, true, true
      },
      // 0x00, 0x3C
      {
	"TLS_RSA_WITH_AES_128_CBC_SHA256", TLS_RSA_WITH_AES_128_CBC_SHA256,
	TLS_VCOD_V12, TLS_VCOD_ALL,
	TlsSuite::TLS_EXCH_RSA, 
	TlsSuite::TLS_CIFR_AES, TlsSuite::TLS_CMOD_EBC, 16L, 16L, 0L,
	TlsSuite::TLS_HASH_SHA256, 32L, true, true
      },
      // 0x00, 0x3D
      {
	"TLS_RSA_WITH_AES_256_CBC_SHA256", TLS_RSA_WITH_AES_256_CBC_SHA256,
	TLS_VCOD_V12, TLS_VCOD_ALL,
	TlsSuite::TLS_EXCH_RSA, 
	TlsSuite::TLS_CIFR_AES, TlsSuite::TLS_CMOD_EBC, 32L, 16L, 0L,
	TlsSuite::TLS_HASH_SHA256, 32L, true
      },
      // 0x00, 0x9C
      {
	"TLS_RSA_WITH_AES_128_GCM_SHA256", TLS_RSA_WITH_AES_128_GCM_SHA256,
	TLS_VCOD_V12, TLS_VCOD_ALL,
	TlsSuite::TLS_EXCH_RSA, 
	TlsSuite::TLS_CIFR_AES, TlsSuite::TLS_CMOD_GCM, 16L, 16L, 4L,
	TlsSuite::TLS_HASH_SHA256, 32L, false, true
      },
      // 0x00, 0x9D
      {
	"TLS_RSA_WITH_AES_256_GCM_SHA384", TLS_RSA_WITH_AES_256_GCM_SHA384,
	TLS_VCOD_V12, TLS_VCOD_ALL,
	TlsSuite::TLS_EXCH_RSA, 
	TlsSuite::TLS_CIFR_AES, TlsSuite::TLS_CMOD_GCM, 32L, 16L, 4L,
	TlsSuite::TLS_HASH_SHA384, 48L, false, true
      }
    };
  // the cipher list size
  static const long TLS_CNFO_SIZ =
    sizeof (TLS_CNFO_LST) / sizeof (TlsSuite::s_cnfo);
}

#endif
