// ---------------------------------------------------------------------------
// - Ascii.cpp                                                               -
// - standard object library - ascii functions class implementation          -
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
#include "Exception.hpp"
#include "cstr.hpp"
#include "ccnv.hpp"

#include "transient.tcc"

namespace afnix {

  // convert an hexadecimal character to a byte

  t_byte Ascii::htob (const char value) {
    if ((value >= '0') && (value <= '9')) return (t_byte) (value - '0');
    if ((value >= 'a') && (value <= 'f')) return (t_byte) (value - 'a' + 10);
    if ((value >= 'A') && (value <= 'F')) return (t_byte) (value - 'A' + 10);
    throw Exception ("ascii-error", 
		     "invalid character for hexadecimal conversion", value);
  }

  // convert a byte to a printable character
  
  char Ascii::btoc (const t_byte byte, const bool lflg) {
    char data = lflg ? byte & 0x0f : (byte >> 4) & 0x0f;
    if (data < 10) return '0' + data;
    return 'A' + (data - 10);
  }

  // convert a byte array to a string

  String Ascii::btos (const t_byte* src, const long size) {
    // check for nil
    if ((src == nullptr) || (size <= 0)) return "";
    // create a character buffer
    long  blen = size * 2;
    t_transient<char> data = new char[blen+1];
    for (long i = 0, j = 0; i < size; i++, j+=2) {
      data[j]   = Ascii::btoc (src[i], false);
      data[j+1] = Ascii::btoc (src[i], true);
    }
    data[blen] = nilc;
    String result = *data;
    return result;
  }

  // convert an integer to a c-string representation

  char* Ascii::ltoa (const long value) {
    return c_ltoa (value);
  }

  // convert a long integer to a c-string representation

  char* Ascii::lltoa (const t_long value) {
    return c_lltoa (value);
  }

  // byte to character mapping function

  char Ascii::tochar (const char byte, const bool flag) {
    char data = flag ? byte & 0x0f : (byte >> 4) & 0x0f;
    if (data < 10) return '0' + data;
    return 'A' + (data - 10);
  }

  // convert a string representation to a character

  char Ascii::tochar (const String& value) {
    char result = nilc;
    if (value.length () == 1) {
      result = value[0];
    } else if ((value.length () == 3) && (value[0] == '\'') && 
	       (value[2] == '\'')) {
      result = value[1];
    } else {
      throw Exception ("format-error",
		       "illegal character representation", value);
    }
    return result;
  }

  // convert a native character value to a string

  String Ascii::tostring (const char value) {
    String result = value;
    return result;
  }

  // convert a native character value to a literal string

  String Ascii::toliteral (const char value) {
    String result = "'";
    if (value == '\'') {
      result += "\\'";
    } else {
      result += Ascii::tostring (value);
    }
    result += "'";
    return result;
  }

  // return the size of an ascii string

  long Ascii::strlen (const char* s) {
    // check for nil string
    if (s == nullptr) return 0;
    // compute length
    long result = 0;
    while (*s++ != nilc) result++;
    return result;
  }

  // return the size of an ascii string

  long Ascii::strlen (const t_quad* s) {
    // check for nil string
    if (s == nullptr) return 0;
    // compute length
    long result = 0;
    while (*s != nilq) {
      if (Ascii::islatin (*s++) == false)
	throw Exception ("ascii-error", "non latin unicode character");
      else
	result++;
    }
    return result;
  }

  // return a c-string from a character

  char* Ascii::strmak (const char value) {
    return c_strmak (value);
  }

  // return a c-string from an unicode character

  char* Ascii::strmak (const t_quad value) {
    if (Ascii::islatin (value) == true) return c_strmak (value);
    throw Exception ("ascii-error", "non latin unicode character");
  }

  // return an ascii string from a c-string

  char* Ascii::strdup (const char* s) {
    return c_strdup (s);
  }

  // return an ascii string from a unicode string

  char* Ascii::strdup (const t_quad* s) {
    // check the string
    long  size = Ascii::strlen (s);
    // allocate and copy
    char* sbuf = new char[size+1];
    for (long i = 0; i < size; i++) {
      t_quad value = s[i];
      if (Ascii::islatin (value) == false) {
	delete [] sbuf;
	throw Exception ("ascii-error", "non latin unicode character");
      }
      sbuf[i] = (char) value;
    }
    sbuf[size] = nilc;
    return sbuf;
  }

  // return true if the character is alpha

  bool Ascii::isalpha (const char value) {
    // check regular ascii
    if ((value >= 'a') && (value <= 'z')) return true;
    if ((value >= 'A') && (value <= 'Z')) return true;
    return false;
  }

  // return true if the character is a digit

  bool Ascii::isdigit (const char value) {
    if ((value >= '0') && (value <= '9')) return true;
    return false;
  }

  // return true if the character is hexadecimal

  bool Ascii::ishexa (const char value) {
    if ((value >= '0') && (value <= '9')) return true;
    if ((value >= 'a') && (value <= 'f')) return true;
    if ((value >= 'A') && (value <= 'F')) return true;
    return false;
  }

  // return true if the character is a blank or tab
  
  bool Ascii::isblank (const char value) {
    if ((value == ' ') || (value == '\t')) return true;
    return false;
  }

  // return true if the unicode character is an ascii character
  
  bool Ascii::isascii (const t_quad value) {
    if ((value & 0xFFFFFF80) == 0x00000000) return true;
    return false;
  }

  // return true if the unicode character is a latin character
  
  bool Ascii::islatin (const t_quad value) {
    if ((value & 0xFFFFFF00) == 0x00000000) return true;
    return false;
  }
}
