// ---------------------------------------------------------------------------
// - Unicode.cpp                                                             -
// - standard object library - unicode functions class implementation        -
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

#include "Ascii.hpp"
#include "Unicode.hpp"
#include "Utility.hpp"
#include "Exception.hpp"
#include "cucd.hpp"
#include "cmem.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - private section                                                       -
  // -------------------------------------------------------------------------

  static const t_quad STR_CHAR_DQ = 0x00000022UL; // "
  static const t_quad STR_CHAR_SQ = 0x00000027UL; // '
  static const t_quad STR_CHAR_SD = 0x0000002EUL; // .
  
  // this procedure checks if a character belong to an array
  static inline bool is_csep (const char c, const char* sep) {
    while (*sep != nilc) {
      if (*sep++ == c) return true;
    }
    return false;
  }

  // this procedure checks if a character belong to an array
  static inline bool is_qsep (const t_quad c, const t_quad* sep) {
    while (*sep != nilq) {
      if (*sep++ == c) return true;
    }
    return false;
  }

  // this procedure checks if a buffer is a valid utf8 encoding
  static bool is_utf_08 (const t_byte src[], const long size) {
    // check trivial size first
    if ((size <= 0) || (size >= Unicode::MAX_UTF8_SIZE)) return false;
    // 1 byte mode
    if ((src[0] < 0x80) && (size == 1)) return true;
    // 2 bytes mode
    if ((src[0] < 0xE0) && (size == 2)) {
      if ((src[1] & 0x80) != 0x80) return false;
      return true;
    }
    // 3 bytes mode
    if ((src[0] < 0xF0) && (size == 3)) {
      if ((src[1] & 0x80) != 0x80) return false;
      if ((src[2] & 0x80) != 0x80) return false;
      return true;
    }
    // 4 bytes mode
    if ((src[0] < 0xF8) && (size == 4)) {
      if ((src[1] & 0x80) != 0x80) return false;
      if ((src[2] & 0x80) != 0x80) return false;
      if ((src[3] & 0x80) != 0x80) return false;
      return true;
    }
    // invalid buffer
    return false;
  }

  // this procedure converts a utf8 buffer into a unicode quad
  static bool utf_08_toq (t_quad& dst, const t_byte src[]) {
    // initialize destination
    dst = nilq;
    // read first byte
    t_byte b0 = src[0];
    // 1 byte mode
    if (b0 < 0x80) {
      dst = (t_quad) b0;
      return true;
    }
    // 2 bytes mode
    if (b0 < 0xE0) {
      dst = ((t_quad) (b0 & 0x3F)) << 6;
      t_byte b1 = src[1];
      if ((b1 & 0x80) != 0x80) {
	dst = nilq;
	return false;
      }
      dst |= (t_quad) (b1 & 0x3F);
      if (dst < 0x00000080UL) {
	dst = nilq;
	return false;
      }
      return true;
    }
    // 3 bytes mode
    if (b0 < 0xF0) {
      dst = ((t_quad) (b0 & 0x0f)) << 12;
      t_byte b1 = (t_byte) src[1];
      if ((b1 & 0x80) != 0x80) {
	dst = nilq;
	return false;
      }
      dst |= ((t_quad) (b1 & 0x3F)) << 6;
      t_byte b2 = (t_byte) src[2];
      if ((b2 & 0x80) != 0x80) {
	dst = nilq;
	return false;
      }
      dst |= (t_quad) (b2 & 0x3F);
      if (dst < 0x00000800UL) {
	dst = nilq;
	return false;
      }
      return true;
    }
    // 4 bytes mode
    if (b0 < 0xF8) {
      dst = ((t_quad) (b0 & 0x07)) << 18;
      t_byte b1 = (t_byte) src[1];
      if ((b1 & 0x80) != 0x80) {
	dst = nilq;
	return false;
      }
      dst |= ((t_quad) (b1 & 0x3F)) << 12;
      t_byte b2 = (t_byte) src[2];
      if ((b2 & 0x80) != 0x80) {
	dst = nilq;
	return false;
      }
      dst |= (t_quad) (b2 & 0x3F) << 6;
      t_byte b3 = (t_byte) src[3];
      if ((b3 & 0x80) != 0x80) {
	dst = nilq;
	return false;
      }
      dst |= (t_quad) (b3 & 0x3F);
      if (dst < 0x00010000UL) {
	dst = nilq;
	return false;
      }
      return true;
    }
    return false;
  }
  
  // this procedure converts a quad into a utf8 buffer
  static long qto_utf_08 (t_byte* dst, const t_quad c) {
    // check for buffer
    if (dst == nullptr) return 0;
    // initialize index
    long i = 0;
    // encode the value
    if (c < 0x00000080UL) {
      dst[i++] = (t_byte) c;
    } else if (c < 0x00000800UL) {
      dst[i++] = (t_byte) (0x000000C0UL | ((c >> 6)  & 0x0000001FUL));
      dst[i++] = (t_byte) (0x00000080UL |  (c        & 0x0000003FUL));
    } else if (c < 0x00010000UL) {
      dst[i++] = (t_byte) (0x000000E0UL | ((c >> 12) & 0x0000000FUL));
      dst[i++] = (t_byte) (0x00000080UL | ((c >> 6)  & 0x0000003FUL));
      dst[i++] = (t_byte) (0x00000080UL |  (c        & 0x0000003FUL));
    } else if (c < 0x00200000UL) {
      dst[i++] = (t_byte) (0x000000F0UL | ((c >> 18) & 0x00000007UL));
      dst[i++] = (t_byte) (0x00000080UL | ((c >> 12) & 0x0000003FUL));
      dst[i++] = (t_byte) (0x00000080UL | ((c >> 6)  & 0x0000003FUL));
      dst[i++] = (t_byte) (0x00000080UL |  (c        & 0x0000003FUL));
    } else {
      i = 0;
    }
    return i;
  }
  
  // this procedure converts a char array to a quad array in byte mode
  static t_quad* ctoq_byte (const char* s, const long size) {
    // check for null size
    if (size <= 0) return nullptr;
    // allocate the result and map
    t_quad* result = new t_quad[size+1];
    for (long i = 0; i < size; i++) result[i] = Unicode::toquad (s[i]);
    // fix end of string and return
    result[size] = nilq;
    return result;
  }

  // this procedure converts a char array to a quad array in utf8 mode
  static t_quad* ctoq_utf8 (const char* s, const long size) {
    // check for null size
    if (size <= 0) return nullptr;
    // prepare the buffer and index
    long   idx = 0;
    long   pos = 0;
    t_byte buf[Unicode::MAX_UTF8_SIZE];
    // allocate the result and map
    t_quad* result = new t_quad[size+1];
    for (long i = 0; i < size; i++) {
      // set the buffer
      buf[pos++] = s[i];
      // check for valid
      if (is_utf_08 (buf, pos) == false) {
	// check for overflow
	if (pos >= Unicode::MAX_UTF8_SIZE) {
	  throw Exception ("decode-error", "cannot decode utf8 buffer");
	}
	// continue to fill
	continue;
      }
      // here the buffer is valid - so convert it
      t_quad c = nilc;
      if (utf_08_toq (c, buf) == false) {
	throw Exception ("decode-error", "cannot decode utf8 buffer");
      }
      // save the quad and continue
      result[idx++] = c;
      pos = 0;
    }
    // fix end of string and return
    result[idx] = nilq;
    return result;
  }

  // this procedure converts a quad array to a char array in byte mode
  static char* qtoc_byte (const t_quad* s, const long size) {
    // check for null size
    if (size <= 0) return nullptr;
    // allocate the result and map
    char* result = new char[size+1];
    for (long i = 0; i < size; i++) result[i] = Unicode::tochar (s[i]);
    // fix end of string and return
    result[size] = nilc;
    return result;
  }

  // this procedure converts a quad array to a char array in utf8 mode
  static char* qtoc_utf8 (const t_quad* s, const long size) {
    // check for null size
    if (size <= 0) return nullptr;
    long len = size * Unicode::MAX_UTF8_SIZE + 1;
    long idx = 0;
    // allocate the result and map
    char* result = new char[len];
    for (long i = 0; i < size; i++) {
      // set conversion buffer
      t_byte buf[Unicode::MAX_UTF8_SIZE];
      long bsz = qto_utf_08 (buf, s[i]);
      // check for proper result
      if (bsz == 0) {
	throw Exception ("encode-error", 
			 "invalid character to encode in utf8 mode");
      }
      // add the result
      for (long j = 0; j < bsz; j++) result[idx++] = buf[j];
    }
    // fix end of string and return
    result[idx] = nilc;
    return result;
  }

  // -------------------------------------------------------------------------
  // - public section                                                        -
  // -------------------------------------------------------------------------

  // convert a unicode character to a native character if possible

  char Unicode::tochar (const t_quad value) {
    // check for 8 bit range
    if ((value & 0xFFFFFF00UL) != nilq) {
      throw Exception ("unicode-error", "cannot convert unicode character");
    }
    // map the character
    char result = (char) (value & 0x000000FFUL);
    return result;
  }

  // convert a unicode character to a bmp character if possible

  t_word Unicode::tobmp (const t_quad value) {
    // check for 16 bit range
    if ((value & 0xFFFF0000UL) != nilq) {
      throw Exception ("unicode-error", "cannot convert unicode character");
    }
    // map the character
    t_word result = (t_word) (value & 0x0000FFFFUL);
    return result;
  }

  // convert a hexadecimal character to a byte

  t_byte Unicode::htob (const t_quad value) {
    char c = Unicode::tochar (value);
    return Ascii::htob (c);
  }

  // convert a quad to a byte array

  t_byte* Unicode::qtob (long& size, const t_quad value) {
    // convert the quad
    t_byte cbuf[5];
    size = qto_utf_08 (cbuf, value);
    if (size == 0L) return nullptr;
    // copy the array
    t_byte* result = new t_byte[size];
    c_memcpy (result, size, cbuf);
    return result;
  }
  
  // convert a string into a byte array

  t_byte* Unicode::stob (long& size, const String& s) {
    // update the output size
    long slen = s.length ();
    size = slen / 2;
    if (size == 0) return nullptr;
    // check the string length
    if ((slen % 2) != 0) {
      throw Exception ("unicode-error",
		       "cannot convert string to byte array", s);
    }
    // preset start index
    long si = 0;
    // check first two bytes
    if ((s[0] == '0') && (s[1] == 'x')) {
      si    = 2;
      size -= 1;
      if (size == 0) {
	throw Exception ("unicode-error",
			 "cannot convert string to byte array", s);
      }
    }
    // allocate result array
    t_byte* result = new t_byte[size];
    for (long i = 0, j = si; i < size; i++) {
      // get upper and lower quad
      t_quad uq = s[j++];
      t_quad lq = s[j++];
      // get upper and lower byte
      t_byte ub = Unicode::htob (uq);
      t_byte lb = Unicode::htob (lq);
      // set the byte value
      result[i] = (ub << 4) | (lb & 0x0F);
    }
    // here it is
    return result;
  }

  // convert a native character to a unicode character

  t_quad Unicode::toquad (const char value) {
    t_quad result = value;
    return result & 0x000000FFUL;
  }

  // convert a string representation to a character

  t_quad Unicode::toquad (const String& value) {
    long slen = value.length ();
    // check for single character
    if (slen == 1) {
      t_quad result = value[0];
      return result;
    }
    // check for ascii representation
    if ((slen > 2) && (value[0] == '\'')) {
      t_quad result = Unicode::toquad (Ascii::tochar (value));
      return result;
    }
    // check for unicode representation
    if ((slen > 2) && (value[0] == 'U') && (value[1] == '+')) {
      // format the string
      String format = "0x";
      format += value.rsubstr (2);
      // convert to quad
      return (t_quad) Utility::tolong (format);
    }
    // invalid format
    throw Exception ("format-error",
		     "illegal unicode string representation", value);
  }

  // convert a unicode character value to a string

  String Unicode::tostring (const t_quad value) {
    // check for an ascii character
    if ((value & 0xFFFFFF00UL) == nilq) {
      char cval = (char) (value & 0x000000FFUL);
      String result = Ascii::tostring (cval);
      return result;
    }
    // we are outside the ascii range, so use the unicode representation
    String result = "U+";
    result += Utility::tohexa (value);
    return result;
  }

  // convert a native character value to a literal string

  String Unicode::toliteral (const t_quad value) {
    String result;
    if (Unicode::isascii (value) == true) {
      char cval = (char) (value & 0x000000FFUL);
      result += '\'';
      result += cval;
      result += '\'';
    } else {
      result += '"';
      result += Unicode::tostring (value);
      result += '"';
    }
    return result;
  }

  // get the size of unicode array

  long Unicode::strlen (const t_quad* s) {
    // check for nil string
    if (s == nullptr) return 0;
    // compute length
    long result = 0;
    while (*s++ != nilq) result++;
    return result;
  }

  // compare two strings and returns true if they are equals.

  bool Unicode::strcmp (const t_quad* s1, const bool n1, const char* s2) {
    // normalize the string first
    const t_quad* ns1 = n1 ? s1 : c_ucdnrm (s1, Unicode::strlen (s1));
    const t_quad* ns2 = c_ucdnrm (s2, Ascii::strlen   (s2));
    // compute string length
    long len1 = Unicode::strlen (ns1);
    long len2 = Unicode::strlen (ns2);
    // check length first
    if (len1 != len2) {
      if (n1 == false) delete [] ns1;
      delete [] ns2;
      return false;
    }
    // normal compare
    bool result = true;
    for (long i = 0; i < len1; i++) {
      if (ns1[i] != ns2[i]) {
	result = false;
	break;
      }
    }
    // clean temporaries and return
    if (n1 == false) delete [] ns1;
    delete [] ns2;
    return result;
  }

  // compare two strings and returns true if they are equals.

  bool Unicode::strcmp (const t_quad* s1, const char* s2) {
    return Unicode::strcmp (s1, false, s2);
  }

  // compare two strings and returns true if they are equals.

  bool Unicode::strcmp (const t_quad* s1, const bool n1,
			const t_quad* s2, const bool n2) {
    // normalize the string first
    const t_quad* ns1 = n1 ? s1 : c_ucdnrm (s1, Unicode::strlen (s1));
    const t_quad* ns2 = n2 ? s2 : c_ucdnrm (s2, Unicode::strlen (s2));
    // compute string length
    long len1 = Unicode::strlen (ns1);
    long len2 = Unicode::strlen (ns2);
    // check length first
    if (len1 != len2) {
      if (n1 == false) delete [] ns1;
      if (n2 == false) delete [] ns2;
      return false;
    }
    // normal compare
    bool result = true;
    for (long i = 0; i < len1; i++) {
      if (ns1[i] != ns2[i]) {
	result = false;
	break;
      }
    }
    // clean temporaries and return
    if (n1 == false) delete [] ns1;
    if (n2 == false) delete [] ns2;
    return result;
  }

  // compare two strings and returns true if they are equals.

  bool Unicode::strcmp (const t_quad* s1, const t_quad* s2) {
    return Unicode::strcmp (s1, false, s2, false);
  }

  // compare two strings upto n characters

  bool Unicode::strncmp (const t_quad* s1, const char* s2, const long size) {
    // nil case compare
    if (size == 0) return true;
    // normalize the string first
    t_quad* ns1 = c_ucdnrm (s1, Unicode::strlen (s1));
    t_quad* ns2 = c_ucdnrm (s2, Ascii::strlen   (s2));
    // compute string length
    long len1 = Unicode::strlen (ns1);
    long len2 = Unicode::strlen (ns2);
    // check length first
    if ((len1 < size) || (len2 < size)) {
      delete [] ns1;
      delete [] ns2;
      return false;
    }
    // normal compare
    bool result = true;
    for (long i = 0; i < size; i++) {
      if (ns1[i] != ns2[i]) {
	result = false;
	break;
      }
    }
    // clean temporaries and return
    delete [] ns1;
    delete [] ns2;
    return result;
  }

  // compare two strings upto n characters

  bool Unicode::strncmp (const t_quad* s1, const t_quad* s2, const long size) {
    // nil case compare
    if (size == 0) return true;
    // normalize the string first
    t_quad* ns1 = c_ucdnrm (s1, Unicode::strlen (s1));
    t_quad* ns2 = c_ucdnrm (s2, Unicode::strlen (s2));
    // compute string length
    long len1 = Unicode::strlen (ns1);
    long len2 = Unicode::strlen (ns2);
    // check length first
    if ((len1 < size) || (len2 < size)) {
      delete [] ns1;
      delete [] ns2;
      return false;
    }
    // normal compare
    bool result = true;
    for (long i = 0; i < size; i++) {
      if (ns1[i] != ns2[i]) {
	result = false;
	break;
      }
    }
    // clean temporaries and return
    delete [] ns1;
    delete [] ns2;
    return result;
  }

  // compare two strings - less than operator

  bool Unicode::strlth (const t_quad* s1, const char* s2) {
    // normalize the string first
    t_quad* ns1 = c_ucdnrm (s1, Unicode::strlen (s1));
    t_quad* ns2 = c_ucdnrm (s2, Ascii::strlen   (s2));
    // save pointers for delete
    t_quad* np1 = ns1;
    t_quad* np2 = ns2;
    // compare without equal
    bool result = false;
    while (*ns1 != nilq) {
      if (*ns1 < *ns2) {
	result = true;
	break;
      }
      if (*ns1++ > *ns2++) break;
    }
    // clean temporaries and return
    delete [] np1;
    delete [] np2;
    return result;
  }

  // compare two strings - less than operator

  bool Unicode::strlth (const t_quad* s1, const t_quad* s2) {
    // normalize the string first
    t_quad* ns1 = c_ucdnrm (s1, Unicode::strlen (s1));
    t_quad* ns2 = c_ucdnrm (s2, Unicode::strlen (s2));
    // save pointers for delete
    t_quad* np1 = ns1;
    t_quad* np2 = ns2;
    // compare without equal
    bool result = false;
    while (*ns1 != nilq) {
      if (*ns1 < *ns2) {
	result = true;
	break;
      }
      if (*ns1++ > *ns2++) break;
    }
    // clean temporaries and return
    delete [] np1;
    delete [] np2;
    return result;
  }

  // compare two strings - less equal operator

  bool Unicode::strleq (const t_quad* s1, const char* s2) {
    // normalize the string first
    t_quad* ns1 = c_ucdnrm (s1, Unicode::strlen (s1));
    t_quad* ns2 = c_ucdnrm (s2, Ascii::strlen   (s2));
    // save pointers for delete
    t_quad* np1 = ns1;
    t_quad* np2 = ns2;
    // compare with equal
    bool result = true;
    while (*ns1 != nilq) {
      if (*ns1 < *ns2) break;
      if (*ns1++ > *ns2++) {
	result = false;
	break;
      }
    }
    // clean temporaries and return
    delete [] np1;
    delete [] np2;
    return result;
  }

  // compare two strings - less equal operator

  bool Unicode::strleq (const t_quad* s1, const t_quad* s2) {
    // normalize the string first
    t_quad* ns1 = c_ucdnrm (s1, Unicode::strlen (s1));
    t_quad* ns2 = c_ucdnrm (s2, Unicode::strlen (s2));
    // save pointers for delete
    t_quad* np1 = ns1;
    t_quad* np2 = ns2;
    // compare with equal
    bool result = true;
    while (*ns1 != nilq) {
      if (*ns1 < *ns2) break;
      if (*ns1++ > *ns2++) {
	result = false;
	break;
      }
    }
    // clean temporaries and return
    delete [] np1;
    delete [] np2;
    return result;
  }

  // convert an ascii character to an unicode array

  t_quad* Unicode::strmak (const char value) {
    t_quad buf[2];
    buf[0] = Unicode::toquad (value);
    buf[1] = nilq;
    return strdup (buf, 1L);
  }

  // convert a unicode character to an unicode array

  t_quad* Unicode::strmak (const t_quad value) {
    t_quad buf[2];
    buf[0] = value;
    buf[1] = nilq;
    return strdup (buf, 1L);
  }

  // create a unicode string from a string and a character

  t_quad* Unicode::strmak (const t_quad* s, const char c) {
    t_quad value = Unicode::toquad (c);
    return Unicode::strmak (s, value);
  }

  // create a unicode string from a string one and a unicode character

  t_quad* Unicode::strmak (const t_quad* s, const t_quad c) {
    // compute size
    long    len = Unicode::strlen (s);
    t_quad* buf = new t_quad[len+2];
    // copy string directly
    for (long i = 0; i < len; i++) buf[i] = s[i];
    buf[len]   = c;
    buf[len+1] = nilq;
    // normalize and clean
    try {
      t_quad* result = strdup (buf, len+1);
      delete [] buf;
      return result;
    } catch (...) {
      delete [] buf;
      throw;
    }
  }

  // create a unicode string from a character and a string

  t_quad* Unicode::strmak (const char c, const t_quad* s) {
    t_quad value = Unicode::toquad (c);
    return Unicode::strmak (value, s);
  }

  // create a unicode string from a unicode character and a string

  t_quad* Unicode::strmak (const t_quad c, const t_quad* s) {
    // compute size
    long    len = Unicode::strlen (s);
    t_quad* buf = new t_quad[len+2];
    // add character and copy string
    buf[0] = c;
    for (long i = 0; i < len; i++) buf[i+1] = s[i];
    buf[len+1] = nilq;
    // normalize and clean
    try {
      t_quad* result = strdup (buf, len+1);
      delete [] buf;
      return result;
    } catch (...) {
      delete [] buf;
      throw;
    }
  }

  // concatenate two strings and normalize the result

  t_quad* Unicode::strmak (const t_quad* s1, const char* s2) {
    // compute arguments length
    long len1 = Unicode::strlen (s1);
    long len2 = Ascii::strlen   (s2);
    // allocate a temporary buffer and copy
    t_quad* buf = new t_quad[len1+len2+1];
    for (long i = 0; i < len1; i++) buf[i] = s1[i];
    for (long i = 0; i < len2; i++) buf[len1+i] = Unicode::toquad (s2[i]);
    buf[len1+len2] = nilq;
    // normalize and clean
    try {
      t_quad* result = strdup (buf, len1+len2);
      delete [] buf;
      return result;
    } catch (...) {
      delete [] buf;
      throw;
    }
  }

  // concatenate two strings and normalize the result

  t_quad* Unicode::strmak (const t_quad* s1, const t_quad* s2) {
    // compute arguments length
    long len1 = Unicode::strlen (s1);
    long len2 = Unicode::strlen (s2);
    // allocate a temporary buffer and copy
    t_quad* buf = new t_quad[len1+len2+1];
    for (long i = 0; i < len1; i++) buf[i]      = s1[i];
    for (long i = 0; i < len2; i++) buf[len1+i] = s2[i];
    buf[len1+len2] = nilq;
    // normalize and clean
    try {
      t_quad* result = strdup (buf, len1+len2);
      delete [] buf;
      return result;
    } catch (...) {
      delete [] buf;
      throw;
    }
  }

  // convert an ascii string to an unicode array

  t_quad* Unicode::strdup (const char* s) {
    return Unicode::strdup (s, false);
  }

  // convert a unicode string to an unicode array

  t_quad* Unicode::strdup (const t_quad* s) {
    return Unicode::strdup (s, false);
  }

  // convert an ascii string to an unicode array

  t_quad* Unicode::strdup (const char* s, const bool nrmf) {
    // get the buffer length and check for nil
    long len = Ascii::strlen (s);
    // convert the source buffer
    t_quad* buf = new t_quad[len+1];
    try {
      for (long i = 0; i < len; i++) buf[i] = Unicode::toquad (s[i]);
      buf[len] = nilq;
      t_quad* result = buf;
      if (nrmf == true) {
	result = c_ucdnrm (buf, len);
	delete [] buf;
      }
      return result;
    } catch (...) {
      delete [] buf;
      throw;
    }
  }

  // convert a unicode string to an unicode array

  t_quad* Unicode::strdup (const t_quad* s, const bool nrmf) {
    // get the string length and check for nil
    long len = Unicode::strlen (s);
    // create a new quad array
    t_quad* buf = new t_quad[len+1];
    // copy the source buffer
    try {
      for (long i = 0; i < len; i++) buf[i] = s[i];
      buf[len] = nilq;
      t_quad* result = buf;
      if (nrmf == true) {
	result = c_ucdnrm (buf, len);
	delete [] buf;
      }
      return result;
    } catch (...) {
      delete [] buf;
      throw;
    }
  }

  // convert a character buffer to an unicode array by size

  t_quad* Unicode::strdup (const char* s, const long size) {
    // create a new quad array
    t_quad* result = new t_quad[size+1];
    // convert the source buffer
    try {
      for (long i = 0; i < size; i++) result[i] = Unicode::toquad (s[i]);
      result[size] = nilq;
      return result;
    } catch (...) {
      delete [] result;
      throw;
    }
  }

  // convert a unicode string to an unicode array by size

  t_quad* Unicode::strdup (const t_quad* s, const long size) {
    // create a new quad array
    t_quad* result = new t_quad[size+1];
    // copy the source buffer
    try {
      for (long i = 0; i < size; i++) result[i] = s[i];
      result[size] = nilq;
      return result;
    } catch (...) {
      delete [] result;
      throw;
    }
  }

  // normalize a string by performing a normal form decomposition

  t_quad* Unicode::strnrm (const t_quad* s) {
    // get the string length and check for nil
    long len = Unicode::strlen (s);
    // normalize the string
    return c_ucdnrm (s, len);
  }

  // remove the leading blank and tab and return a new string

  t_quad* Unicode::stripl (const char* s) {
    if (s != nullptr) {
      while ((*s != nilc) && ((*s == blkc) || (*s == tabc))) s++;
    }
    return Unicode::strdup (s);
  }

  // remove the leading separators and return a new string

  t_quad* Unicode::stripl (const char* s, const char* sep) {
    if (s != nullptr) {
      while ((*s != nilc) && (is_csep (*s, sep) == true)) s++;
    }
    return Unicode::strdup (s);
  }
 
  // remove the leading blank and tab and return a new string

  t_quad* Unicode::stripl (const t_quad* s) {
    if (s != nullptr) {
      while ((*s != nilq) && ((*s == blkq) || (*s == tabq))) s++;
    }
    return Unicode::strdup (s);
  }

  // remove the leading separators and return a new string

  t_quad* Unicode::stripl (const t_quad* s, const t_quad* sep) {
    if (s != nullptr) {
      while ((*s != nilq) && (is_qsep (*s, sep) == true)) s++;
    }
    return Unicode::strdup (s);
  }

  // remove the trailing blank and return a new string

  t_quad* Unicode::stripr (const char* s) {
    // get the length and check
    long len = Ascii::strlen (s);
    if (len == 0) return c_ucdnil ();
    char* buf = Ascii::strdup (s);
    char* end = buf + len - 1;
    // remove trailing blank
    while ((end != s) && ((*end == blkc) || (*end == tabc))) *end-- = nilc;
    // now copy and return
    t_quad* result = Unicode::strdup (buf);
    delete [] buf;
    return result;
  }

  // remove the trailing separators and return a new string

  t_quad* Unicode::stripr (const char* s, const char* sep) {
    // get the length and check
    long len = Ascii::strlen (s);
    if (len == 0) return c_ucdnil ();
    char* buf = Ascii::strdup (s);
    char* end = buf + len - 1;
    // remove trailing blank
    while ((end != s) && (is_csep (*end, sep) == true)) *end-- = nilc;
    // now copy and return
    t_quad* result = Unicode::strdup (buf);
    delete [] buf;
    return result;
  }

  // remove the trailing blank and return a new string

  t_quad* Unicode::stripr (const t_quad* s) {
    // get the length and check
    long len = Unicode::strlen (s);
    if (len == 0) return c_ucdnil ();
    t_quad* buf = Unicode::strdup (s);
    t_quad* end = buf + len - 1;
    // remove trailing blank
    while ((end != s) && ((*end == blkq) || (*end == tabq))) *end-- = nilq;
    // now copy and return
    t_quad* result = Unicode::strdup (buf);
    delete [] buf;
    return result;
  }

  // remove the trailing separators and return a new string

  t_quad* Unicode::stripr (const t_quad* s, const t_quad* sep) {
    // get the length and check
    long len = Unicode::strlen (s);
    if (len == 0) return c_ucdnil ();
    t_quad* buf = Unicode::strdup (s);
    t_quad* end = buf + len - 1;
    // remove trailing blank
    while ((end != s) && (is_qsep (*end, sep) == true)) *end-- = nilq;
    // now copy and return
    t_quad* result = Unicode::strdup (buf);
    delete [] buf;
    return result;
  }

  // reduce the string into a small form

  t_quad* Unicode::redex (const t_quad* s) {
    // get the length and check
    long len = Unicode::strlen (s);
    if (len == 0) return c_ucdnil ();
    // check for small length
    if (len <= 8) return Unicode::strdup (s, len);
    // allocate a small result
    t_quad* result = new t_quad[13];
    for (long k = 0; k < 4; k++) {
      result[k] = s[k];
      result[8+k] = s[len-4+k];
    }
    result[4]  = STR_CHAR_SD;
    result[5]  = STR_CHAR_SD;
    result[6]  = STR_CHAR_SD;
    result[7]  = STR_CHAR_SD;
    result[12] = nilq;
    return result;
  }

  // remove the quotes from a string

  t_quad* Unicode::rmquote (const t_quad* s) {
    // get the length and check
    long len = Unicode::strlen (s);
    if (len == 0) return c_ucdnil ();
    // check for quote
    if (((s[0] == STR_CHAR_DQ) && (s[len-1] == STR_CHAR_DQ)) ||
	((s[0] == STR_CHAR_SQ) && (s[len-1] == STR_CHAR_SQ))) {
      // check for empty quoted string
      if (len == 2) return c_ucdnil ();
      // allocate and copy
      t_quad* result = new t_quad[len-1];
      for (long k = 0; k < len-2; k++) result[k] = s[k+1];
      result[len-2] = nilq;
      return result;
    }
    return Unicode::strdup (s, len);
  }

  // convert an ascii string to lower case

  t_quad* Unicode::tolower (const char* s) {
    // check for length
    if (s == nullptr) return c_ucdnil ();
    long len = Ascii::strlen (s);
    // allocate and convert
    long    size = len * UCD_LCM_MAX + 1;
    t_quad* sbuf = new t_quad[size];
    long    sidx = 0;
    t_quad  sdst[UCD_LCM_MAX];
    for (long i = 0; i < len; i++) {
      long cnvs = c_ucdtol (sdst, Unicode::toquad (s[i]));
      for (long j = 0; j < cnvs; j++) sbuf[sidx++] = sdst[j];
    }
    // mark end of string
    sbuf[sidx] = nilq;
    // copy and clean
    t_quad* result = Unicode::strdup (sbuf);
    delete [] sbuf;
    return result;
  }

  // convert an ascii string to lower case

  t_quad* Unicode::tolower (const t_quad* s) {
    // check for length
    if (s == nullptr) return c_ucdnil ();
    long len = Unicode::strlen (s);
    // allocate and convert
    long    size = len * UCD_LCM_MAX + 1;
    t_quad* sbuf = new t_quad[size];
    long    sidx = 0;
    t_quad  sdst[UCD_LCM_MAX];
    for (long i = 0; i < len; i++) {
      long cnvs = c_ucdtol (sdst, s[i]);
      for (long j = 0; j < cnvs; j++) sbuf[sidx++] = sdst[j];
    }
    // mark end of string
    sbuf[sidx] = nilq;
    // copy and clean
    t_quad* result = Unicode::strdup (sbuf);
    delete [] sbuf;
    return result;
  }

  // convert an ascii string to upper case

  t_quad* Unicode::toupper (const char* s) {
    // check for length
    if (s == nullptr) return c_ucdnil ();
    long len = Ascii::strlen (s);
    // allocate and convert
    long    size = len * UCD_UCM_MAX + 1;
    t_quad* sbuf = new t_quad[size];
    long    sidx = 0;
    t_quad  sdst[UCD_UCM_MAX];
    for (long i = 0; i < len; i++) {
      long cnvs = c_ucdtou (sdst, Unicode::toquad (s[i]));
      for (long j = 0; j < cnvs; j++) sbuf[sidx++] = sdst[j];
    }
    // mark end of string
    sbuf[sidx] = nilq;
    // copy and clean
    t_quad* result = Unicode::strdup (sbuf);
    delete [] sbuf;
    return result;
  }

  // convert an unicode string to upper case

  t_quad* Unicode::toupper (const t_quad* s) {
    // check for length
    if (s == nullptr) return c_ucdnil ();
    long len = Unicode::strlen (s);
    // allocate and convert
    long    size = len * UCD_UCM_MAX + 1;
    t_quad* sbuf = new t_quad[size];
    long    sidx = 0;
    t_quad  sdst[UCD_UCM_MAX];
    for (long i = 0; i < len; i++) {
      long cnvs = c_ucdtou (sdst, s[i]);
      for (long j = 0; j < cnvs; j++) sbuf[sidx++] = sdst[j];
    }
    // mark end of string
    sbuf[sidx] = nilq;
    // copy and clean
    t_quad* result = Unicode::strdup (sbuf);
    delete [] sbuf;
    return result;
  }

  // return true if the character is a lower character

  bool Unicode::islower (const t_quad code) {
    // get the ucd record and do nothing if it does not exist
    const s_cucd* ucd = c_getcucd (code);
    if (ucd == nullptr) return false;
    // check for lower case code
    return (ucd->d_pgcv == UCD_GCV_LL);
  }

  // return true if the character is an upper character

  bool Unicode::isupper (const t_quad code) {
    // get the ucd record and do nothing if it does not exist
    const s_cucd* ucd = c_getcucd (code);
    if (ucd == nullptr) return false;
    // check for lower case code
    return (ucd->d_pgcv == UCD_GCV_LU);
  }

  // return true if the unicode character is a letter

  bool Unicode::isletter (const t_quad code) {
    // get the ucd record and do nothing if it does not exist
    const s_cucd* ucd = c_getcucd (code);
    if (ucd == nullptr) return false;
    // get the gcv byte and check
    t_byte gcv = ucd->d_pgcv;
    if (gcv == UCD_GCV_LU) return true;
    if (gcv == UCD_GCV_LL) return true;
    if (gcv == UCD_GCV_LT) return true;
    if (gcv == UCD_GCV_LM) return true;
    if (gcv == UCD_GCV_LO) return true;
    return false;
  }

  // return true if the unicode character is a digit

  bool Unicode::isdigit (const t_quad code) {
    // get the ucd record and do nothing if it does not exist
    const s_cucd* ucd = c_getcucd (code);
    if (ucd == nullptr) return false;
    // get the gcv byte and check
    t_byte gcv = ucd->d_pgcv;
    if (gcv == UCD_GCV_ND) return true;    
    return false;
  }

  // return true if the unicode character is a combining alphanumeric

  bool Unicode::iscan (const t_quad code) {
    // get the ucd record and do nothing if it does not exist
    const s_cucd* ucd = c_getcucd (code);
    if (ucd == nullptr) return false;
    // get the gcv byte
    t_byte gcv = ucd->d_pgcv;
    // check for letter
    if (gcv == UCD_GCV_LU) return true;
    if (gcv == UCD_GCV_LL) return true;
    if (gcv == UCD_GCV_LT) return true;
    if (gcv == UCD_GCV_LM) return true;
    if (gcv == UCD_GCV_LO) return true;
    // check for marking
    if (gcv == UCD_GCV_MN) return true;
    // check for digit
    if (gcv == UCD_GCV_ND) return true;    
    // not alpha
    return false;
  }

  // return true if the unicode character is an alpha-numeric character

  bool Unicode::isalpha (const t_quad code) {
    // check for a digit
    if (Unicode::isdigit (code) == true) return true;
    // check for letter
    if (Unicode::isletter (code) == true) return true;
    // not alpha
    return false;
  }


  // return true if the unicode character is a blank or tab
  
  bool Unicode::isblank (const t_quad code) {
    if ((code == blkq) || (code == tabq)) return true;
    return false;
  }

  // return true if the unicode character is an ascii character
  
  bool Unicode::isascii (const t_quad code) {
    if ((code & 0xFFFFFF80UL) == nilq) return true;
    return false;
  }

  // return true if the unicode character is a bmp character
  
  bool Unicode::isbmp (const t_quad code) {
    if ((code & 0xFFFF0000UL) == nilq) return true;
    return false;
  }

  // return true if the unicode character is a latin character
  
  bool Unicode::islatin (const t_quad code) {
    if ((code & 0xFFFFFF00UL) == nilq) return true;
    return false;
  }

  // return true if the unicode character is a bit character
  
  bool Unicode::isbit (const t_quad code) {
    if ((code == (t_quad) '0') || (code == (t_quad) '1')) return true;
    return false;
  }

  // return true if the unicode character is an hexadecimal character
  
  bool Unicode::ishexa (const t_quad code) {
    if ((code >= (t_quad) '0') && (code <= (t_quad) '9')) return true;
    if ((code >= (t_quad) 'a') && (code <= (t_quad) 'f')) return true;
    if ((code >= (t_quad) 'A') && (code <= (t_quad) 'F')) return true;
    return false;
  }

  // return true if the character is an afnix constituent

  bool Unicode::isafnix (const t_quad code) {
    // check for an alhpa character
    if (isalpha (code) == true) return true;
    // check for other constituents
    if (code == (t_quad) '.') return true;
    if (code == (t_quad) '+') return true;
    if (code == (t_quad) '-') return true;
    if (code == (t_quad) '*') return true;
    if (code == (t_quad) '/') return true;
    if (code == (t_quad) '!') return true;
    if (code == (t_quad) '=') return true;
    if (code == (t_quad) '.') return true;
    if (code == (t_quad) '>') return true;
    if (code == (t_quad) '<') return true;
    if (code == (t_quad) '?') return true;
    return false;
  }

  // return true if the unicode character is a valid terminal character

  bool Unicode::isterm (const t_quad code) {
    // get the ucd record and do nothing if it does not exist
    const s_cucd* ucd = c_getcucd (code);
    if (ucd == nullptr) return false;
    // get the gcv byte
    t_byte gcv = ucd->d_pgcv;
    // check for letter
    if (gcv == UCD_GCV_LU) return true;
    if (gcv == UCD_GCV_LL) return true;
    if (gcv == UCD_GCV_LT) return true;
    if (gcv == UCD_GCV_LM) return true;
    if (gcv == UCD_GCV_LO) return true;
    // check for marking
    if (gcv == UCD_GCV_MN) return true;
    if (gcv == UCD_GCV_MC) return true;
    if (gcv == UCD_GCV_ME) return true;
    // check for number
    if (gcv == UCD_GCV_ND) return true;
    if (gcv == UCD_GCV_NL) return true;
    if (gcv == UCD_GCV_NO) return true;
    // check for punctuation
    if (gcv == UCD_GCV_PC) return true;
    if (gcv == UCD_GCV_PD) return true;
    if (gcv == UCD_GCV_PS) return true;
    if (gcv == UCD_GCV_PE) return true;
    if (gcv == UCD_GCV_PI) return true;
    if (gcv == UCD_GCV_PF) return true;
    if (gcv == UCD_GCV_PO) return true;
    // check for symbol
    if (gcv == UCD_GCV_SM) return true;
    if (gcv == UCD_GCV_SC) return true;
    if (gcv == UCD_GCV_SK) return true;
    if (gcv == UCD_GCV_SO) return true;
    // check for spacing
    if (gcv == UCD_GCV_ZS) return true;
    // not for a terminal
    return false;
  }


  // return true if the character is a word constituent

  bool Unicode::iswcc (const t_quad code) {
    // get the ucd record and do nothing if it does not exist
    const s_cucd* ucd = c_getcucd (code);
    if (ucd == nullptr) return false;
    // get the gcv byte
    t_byte gcv = ucd->d_pgcv;
    // check for letter
    if (gcv == UCD_GCV_LU) return true;
    if (gcv == UCD_GCV_LL) return true;
    if (gcv == UCD_GCV_LT) return true;
    if (gcv == UCD_GCV_LM) return true;
    if (gcv == UCD_GCV_LO) return true;
    // check for marking
    if (gcv == UCD_GCV_MN) return true;
    if (gcv == UCD_GCV_MC) return true;
    if (gcv == UCD_GCV_ME) return true;
    // check for number
    if (gcv == UCD_GCV_ND) return true;
    if (gcv == UCD_GCV_NL) return true;
    if (gcv == UCD_GCV_NO) return true;
    // not for a terminal
    return false;
  }

  // return true if the character is a non combining character

  bool Unicode::isncc (const t_quad code) {
    return c_ucdncc (code);
  }

  // get the non-combining length of a unicode string

  long Unicode::ncclen (const t_quad* s) {
    // check for nil string
    if (s == nullptr) return 0;
    // compute length by counting only the grapheme
    long result = 0;
    while (*s != nilq) {
      if (c_ucdncc (*s++) == true) result++;
    }
    return result;
  }

  // hash a unicode string

  long Unicode::tohash (const t_quad* s) {
    // check for null string
    if (s == nullptr) return 0L;
    // the unicode buffer
    t_byte ubuf[Unicode::MAX_UTF8_SIZE];
    // process a quad version
    if (sizeof(long) == sizeof(t_quad)) {
      // set the initial value
      t_quad cval = *s++;
      // encode the unicode buffer
      long size = qto_utf_08 (ubuf, cval);
      // preset the hash value
      t_quad hval = Utility::hashq (ubuf, size);
      // loop into the string
      while ((cval = *s++) != nilq) {
	// encode the unicode buffer
	size = qto_utf_08 (ubuf, cval);
	// preset the hash value
	hval = Utility::hashq (ubuf, size, hval);
      }
      long result = hval;
      return (result < 0) ? -result : result;
    }
    // process an octa version
    if (sizeof(long) == sizeof(t_octa)) {
      // set the initial value
      t_quad cval = *s++;
      // encode the unicode buffer
      long size = qto_utf_08 (ubuf, cval);
      // preset the hash value
      t_octa hval = Utility::hasho (ubuf, size);
      // loop into the string
      while ((cval = *s++) != nilq) {
	// encode the unicode buffer
	size = qto_utf_08 (ubuf, cval);
	// preset the hash value
	hval = Utility::hasho (ubuf, size, hval);
      }
      long result = hval;
      return (result < 0) ? -result : result;
    }
    throw Exception ("unicode-error", "cannot hash unicode string");
  }
  
  // encode a unicode character depending on the mode

  char* Unicode::encode (const Encoding::t_emod emod, const t_quad c) {
    return Unicode::encode (emod, &c, 1);
  }

  // encode a string based on a mode

  char* Unicode::encode (const Encoding::t_emod emod, const String& s) {
    t_quad* sbuf = s.toquad ();
    try {
      char* cbuf = Unicode::encode (emod, sbuf);
      delete [] sbuf;
      return cbuf;
    } catch (...) {
      delete [] sbuf;
      throw;
    }
  }

  // encode a unicode string depending on the mode

  char* Unicode::encode (const Encoding::t_emod emod, const t_quad* s) {
    // get the buffer size
    long size = Unicode::strlen (s);
    // encode by mode and size
    return encode (emod, s, size);
  }

  // encode a unicode string buffer depending on the mode
  
  char* Unicode::encode (const Encoding::t_emod emod, const t_quad* s, 
			 const long size) {
    // check the size
    if (size <= 0) return nullptr;
    // prepare the result
    char* result = nullptr;
    // encode by mode
    switch (emod) {
    case Encoding::EMOD_BYTE:
      result = qtoc_byte (s, size);
      break;
    case Encoding::EMOD_UTF8:
      result = qtoc_utf8 (s, size);
      break;
    }
    // here we are
    return result;
  }

  // check if a buffer is a valid encoding

  bool Unicode::valid (const Encoding::t_emod emod, const char* s,
		       const long size) {
    // initialize result
    bool result = false;
    // check based on mode
    switch (emod) {
    case Encoding::EMOD_BYTE:
      result = true;
      break;
    case Encoding::EMOD_UTF8:
      result = is_utf_08 ((const t_byte*) s, size);
      break;
    }
    // here we are
    return result;
  }

  // decode a unicode buffer into a quad

  t_quad Unicode::decode (const char* buf) {
    // trivial check first
    if (buf == nullptr) {
      throw Exception ("decode-error", "invalid buffer for unicode decoding");
    }
    // convert the buffer
    t_quad result = nilq;
    if (utf_08_toq (result, (const t_byte*) buf) == false) {
      throw Exception ("decode-error", "cannot decode utf8 buffer");
    }
    return result;
  }

  // decode a unicode buffer

  t_quad* Unicode::decode (const Encoding::t_emod emod, const char* s) {
    // get the buffer size
    long size = Ascii::strlen (s);
    // decode by mode and size
    return Unicode::decode (emod, s, size); 
  }

  // decode a unicode buffer by mode and size

  t_quad* Unicode::decode (const Encoding::t_emod emod, const char* s, 
			   const long size) {
    // check the size
    if (size <= 0) return nullptr;
    // prepare the result
    t_quad* result = nullptr;
    // decode by mode
    switch (emod) {
    case Encoding::EMOD_BYTE:
      result = ctoq_byte (s, size);
      break;
    case Encoding::EMOD_UTF8:
      result = ctoq_utf8 (s, size);
      break;
    }
    // here we are
    return result;
  }
}
