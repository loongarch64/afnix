// ---------------------------------------------------------------------------
// - Cryptics.hxx                                                            -
// - afnix:sec module - private cryptographic utilities                      -
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

#ifndef AFNIX_CRYPTICS_HXX
#define AFNIX_CRYPTICS_HXX

#ifndef  AFNIX_UNICODE_HPP
#include "Unicode.hpp"
#endif

namespace afnix {

  // -------------------------------------------------------------------------
  // - generic bytes mapping functions                                       -
  // -------------------------------------------------------------------------

  // little endian byte to quad mapping function - the size is in bytes
  static inline void lebtoq (t_quad* dst, const t_byte* src, const long size) {
    for (long i = 0, j = 0; j < size; i++, j += 4)
      dst[i] =   ((t_quad) src[j])          | 
	        (((t_quad) src[j+1]) << 8)  |
	        (((t_quad) src[j+2]) << 16) | 
	        (((t_quad) src[j+3]) << 24);
  }

  // little endian quad to bytes mapping functions - the size is in quad
  static inline void leqtob (t_byte* dst, const t_quad* src, const long size) {
    for (long i = 0, j = 0; i < size; i++, j += 4) {
      dst[j]   = (t_byte)  (src[i]        & 0xff);
      dst[j+1] = (t_byte) ((src[i] >> 8)  & 0xff);
      dst[j+2] = (t_byte) ((src[i] >> 16) & 0xff);
      dst[j+3] = (t_byte) ((src[i] >> 24) & 0xff);
    }
  }
  
  static inline void lebtoo (t_octa* dst, const t_byte* src, const long size) {
    for (long i = 0, j = 0; j < size; i++, j += 8)
      dst[i] =   ((t_octa) src[j])          | 
	        (((t_octa) src[j+1]) << 8)  |
	        (((t_octa) src[j+2]) << 16) | 
	        (((t_octa) src[j+3]) << 24) |
	        (((t_octa) src[j+4]) << 32) |
	        (((t_octa) src[j+5]) << 40) |
	        (((t_octa) src[j+6]) << 48) |
	        (((t_octa) src[j+7]) << 56);
  }
  // little endian octa to bytes mapping function - the size is in octa
  static inline void leotob (t_byte* dst, const t_octa* src, const long size) {
    for (long i = 0, j = 0; i < size; i++, j += 8) {
      dst[j]   = (t_byte)  (src[i]        & 0xff);
      dst[j+1] = (t_byte) ((src[i] >> 8)  & 0xff);
      dst[j+2] = (t_byte) ((src[i] >> 16) & 0xff);
      dst[j+3] = (t_byte) ((src[i] >> 24) & 0xff);
      dst[j+4] = (t_byte) ((src[i] >> 32) & 0xff);
      dst[j+5] = (t_byte) ((src[i] >> 40) & 0xff);
      dst[j+6] = (t_byte) ((src[i] >> 48) & 0xff);
      dst[j+7] = (t_byte) ((src[i] >> 56) & 0xff);
    }
  }
  
  // little endian octa to bytes mapping function - the size is in bytes
  static inline void leocpb (t_byte* dst, const t_octa* src, const long size) {
    for (long k = 0; k < size; k++) {
      long i = k / 8;
      long j = (k % 8) << 3;
      if (j == 0) {
	dst[k]   = (t_byte) src[i] & 0xff;
      } else {
	dst[k] = (t_byte) ((src[i] >> j) & 0xff);
      }
    }
  }

  // big endian byte to quad mapping function - the size is in bytes
  static inline void bebtoq (t_quad* dst, const t_byte* src, const long size) {
    for (long i = 0, j = 0; j < size; i++, j += 4)
      dst[i] =   ((t_quad) src[j+3])        | 
	        (((t_quad) src[j+2]) << 8)  |
	        (((t_quad) src[j+1]) << 16) | 
	        (((t_quad) src[j])   << 24);
  }

  // big endian quad to bytes mapping functions - the size is in quad
  static inline void beqtob (t_byte* dst, const t_quad* src, const long size) {
    for (long i = 0, j = 0; i < size; i++, j += 4) {
      dst[j+3] = (t_byte)  (src[i]        & 0xff);
      dst[j+2] = (t_byte) ((src[i] >> 8)  & 0xff);
      dst[j+1] = (t_byte) ((src[i] >> 16) & 0xff);
      dst[j]   = (t_byte) ((src[i] >> 24) & 0xff);
    }
  }

  // big endian byte to octa mapping function - the size is in bytes
  static inline void bebtoo (t_octa* dst, const t_byte* src, const long size) {
    for (long i = 0, j = 0; j < size; i++, j += 8)
      dst[i] =   ((t_octa) src[j+7])        | 
	        (((t_octa) src[j+6]) << 8)  |
	        (((t_octa) src[j+5]) << 16) | 
	        (((t_octa) src[j+4]) << 24) |
	        (((t_octa) src[j+3]) << 32) |
	        (((t_octa) src[j+2]) << 40) |
	        (((t_octa) src[j+1]) << 48) |
	        (((t_octa) src[j])   << 56);
  }

  // big endian octa to bytes mapping function - the size is in octa
  static inline void beotob (t_byte* dst, const t_octa* src, const long size) {
    for (long i = 0, j = 0; i < size; i++, j += 8) {
      dst[j+7] = (t_byte)  (src[i]        & 0xff);
      dst[j+6] = (t_byte) ((src[i] >> 8)  & 0xff);
      dst[j+5] = (t_byte) ((src[i] >> 16) & 0xff);
      dst[j+4] = (t_byte) ((src[i] >> 24) & 0xff);
      dst[j+3] = (t_byte) ((src[i] >> 32) & 0xff);
      dst[j+2] = (t_byte) ((src[i] >> 40) & 0xff);
      dst[j+1] = (t_byte) ((src[i] >> 48) & 0xff);
      dst[j]   = (t_byte) ((src[i] >> 56) & 0xff);
    }
  }

  // big endian octa to bytes mapping function - the size is in bytes
  static inline void beocpb (t_byte* dst, const t_octa* src, const long size) {
    for (long k = 0; k < size; k++) {
      long i = k / 8;
      long j = (k % 8);
      if (j == 0) {
	dst[k + 7 - j] = (t_byte) src[i] & 0xff;
      } else {
	dst[k + 7 - j] = (t_byte) ((src[i] >> (j << 3)) & 0xff);
      }
    }
  }


  // -------------------------------------------------------------------------
  // - logical functions                                                     -
  // -------------------------------------------------------------------------

  // word rotate left
  static inline t_word wrotl (t_word x, long n) {
    return ((x << n) | (x >> (16 - n)));
  }

  // word rotate right
  static inline t_quad wrotr (t_word x, long n) {
    return ((x >> n) | (x << (16 - n)));
  }

  // quad rotate left
  static inline t_quad qrotl (t_quad x, long n) {
    return ((x << n) | (x >> (32 - n)));
  }

  // quad rotate right
  static inline t_quad qrotr (t_quad x, long n) {
    return ((x >> n) | (x << (32 - n)));
  }

  // octa rotate left
  static inline t_octa orotl (t_octa x, long n) {
    return ((x << n) | (x >> (64 - n)));
  }

  // octa rotate right
  static inline t_octa orotr (t_octa x, long n) {
    return ((x >> n) | (x << (64 - n)));
  }

  // -------------------------------------------------------------------------
  // - string functions                                                      -
  // -------------------------------------------------------------------------

  // check if a string hash is well formatted
  static inline bool ishhs (const String& s, const long size) {
    // check for length
    long slen = s.length ();
    if (slen != size) return false;
    // check for content
    for (long i = 0; i < slen; i++) {
      if (Unicode::ishexa (s[i]) == false) return false;
    }
    // could work
    return true;
  }
}

#endif
