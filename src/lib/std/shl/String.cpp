// ---------------------------------------------------------------------------
// - String.cpp                                                              -
// - standard object library - string class implementation                   -
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
#include "Stdsid.hxx"
#include "String.hpp"
#include "Strvec.hpp"
#include "Vector.hpp"
#include "Buffer.hpp"
#include "Central.hpp"
#include "Unicode.hpp"
#include "Integer.hpp"
#include "Boolean.hpp"
#include "Evaluable.hpp"
#include "Character.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"
#include "InputStream.hpp"
#include "OutputStream.hpp"
#include "csys.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - public section                                                        -
  // -------------------------------------------------------------------------
  
  // intern a string and return a quark
  
  long String::intern (const String& name) {
    return Central::intern (name);
  }
  
  // remap a string from a quark
  
  String String::qmap (const long quark) {
    return Central::qmap (quark);
  }

  // compute the hash value from a quad array

  long String::hashid (const t_quad* s) {
    return Unicode::tohash (s);
  }

  // case insensitive string comparision

  bool String::strcic (const String& s1, const String& s2) {
    String ls1 = s1.tolower ();
    String ls2 = s2.tolower ();
    return (ls1 == ls2);
  }

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create an empty string
  
  String::String (void) {
    p_sval = nullptr;
    d_nrmf = false;
  }
  
  // create a string from a character
  
  String::String (const char c) {
    p_sval = Unicode::strmak (c);
    d_nrmf = false;
  }
  
   // create a string from an unicode character
  
  String::String (const t_quad c) {
    p_sval = Unicode::strmak (c);
    d_nrmf = false;
  }
 
  // create a string from a c-string
  
  String::String (const char* s) {
    p_sval = Unicode::strdup (s, true);
    d_nrmf = true;
  }

  // create a string from an unicode string
  
  String::String (const t_quad* s) {
    p_sval = Unicode::strdup (s, true);
    d_nrmf = true;
  }

  // copy constructor
  
  String::String (const String& that) {
    that.rdlock ();
    try {
      p_sval = Unicode::strdup (that.p_sval, !that.d_nrmf);
      d_nrmf = true;
      that.unlock ();
    } catch (...) {
      that.unlock ();
      throw;
    }
  }
  
  // copy move this string
  
  String::String (String&& that) noexcept {
    that.wrlock ();
    try {
      // move base literal
      Literal::operator = (static_cast<Literal&&>(that));
      // copy move locally
      p_sval = that.p_sval; that.p_sval = nullptr;
      d_nrmf = that.d_nrmf; that.d_nrmf = false;
      that.unlock ();
    } catch (...) {
      p_sval = nullptr;
      d_nrmf = false;
      that.unlock ();
    } 
  }
  
  // destroy this string
  
  String::~String (void) {
    delete [] p_sval;
  }

  // assign a string to this string
  
  String& String::operator = (const String& that) {
    // check for self-assignation
    if (this == &that) return *this;
    // assign new string
    wrlock ();
    that.rdlock ();
    try {
      delete [] p_sval;
      p_sval = Unicode::strdup (that.p_sval, !that.d_nrmf);
      d_nrmf = true;
      that.unlock ();
      unlock ();
      return *this;
    } catch (...) {
      unlock ();
      that.unlock ();
      throw;
    }
  }
  
  // move this string into this one
  
  String& String::operator = (String&& that) noexcept {
    // check for self-move
    if (this == &that) return *this;
    // assign new string
    wrlock ();
    that.wrlock ();
    try {
      // clean locally
      delete [] p_sval;
      // base move
      Literal::operator = (static_cast<Literal&&>(that));
      // move locally
      p_sval = that.p_sval; that.p_sval = nullptr;
      d_nrmf = that.d_nrmf; that.d_nrmf = false;
      unlock ();
      that.unlock ();
    } catch (...) {
      p_sval = nullptr;
      d_nrmf = false;
      unlock ();
      that.unlock ();
    }
    return *this;
  }
  
  // assign a c-string to this string
  
  String& String::operator = (const char* s) {
    wrlock ();
    try {
      delete [] p_sval;
      p_sval = Unicode::strdup (s, true);
      d_nrmf = true;
      unlock ();
      return *this;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // assign a character to this string
  
  String& String::operator = (const char c) {
    wrlock ();
    try {
      delete [] p_sval;
      p_sval = Unicode::strmak (c);
      d_nrmf = false;
      unlock ();
      return *this;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // assign a unicode string to this string
  
  String& String::operator = (const t_quad* s) {
    wrlock ();
    try {
      delete [] p_sval;
      p_sval = Unicode::strdup (s, true);
      d_nrmf = true;
      unlock ();
      return *this;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // assign a unicode character to this string
  
  String& String::operator = (const t_quad c) {
    wrlock ();
    try {
      delete [] p_sval;
      p_sval = Unicode::strmak (c);
      d_nrmf = false;
      unlock ();
      return *this;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // get a character at a certain position

  t_quad String::operator [] (const long index) const {
    rdlock ();
    try {
      // check for good position first
      if ((index < 0) || (index >= length ())) {
	throw Exception ("index-error", "invalid string index");
      }
      // everything is fine
      t_quad result = p_sval[index];
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // add a string to the current one and return a new one
  
  String String::operator + (const String& s) const {
    rdlock   ();
    s.rdlock ();
    try {
      String result;
      result.p_sval = Unicode::strmak (p_sval, s.p_sval);
      result.d_nrmf = false;
      unlock   ();
      s.unlock ();
      return result;
    } catch (...) {
      unlock   ();
      s.unlock ();
      throw;
    }
  }
  
  // add a character to the current string and return a new one
  
  String String::operator + (const char c) const {
    rdlock ();
    try {
      String result;
      result.p_sval = Unicode::strmak (p_sval, c);
      result.d_nrmf = false;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // add a unicode character to the current string and return a new one
  
  String String::operator + (const t_quad c) const {
    rdlock ();
    try {
      String result;
      result.p_sval = Unicode::strmak (p_sval, c);
      result.d_nrmf = false;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // add an integer to the current string and return a new one
  
  String String::operator + (const long value) const {
    rdlock ();
    try {
      // create a temporary buffer to hold the string
      char* buf = Ascii::ltoa (value);
      // concatenate the two buffers
      String result;
      result.p_sval = Unicode::strmak (p_sval, buf);
      result.d_nrmf = false;
      delete [] buf;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // add a long integer to the current string and return a new one
  
  String String::operator + (const t_long value) const {
    rdlock ();
    try {
      // create a temporary buffer to hold the string
      char* buf = Ascii::lltoa (value);
      // concatenate the two buffers
      String result;
      result.p_sval = Unicode::strmak (p_sval, buf);
      result.d_nrmf = false;
      delete [] buf;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // add a string to this string
  
  String& String::operator += (const String& s) {
    wrlock   ();
    s.rdlock ();
    try {
      // create a new buffer by concatenation
      t_quad* buf = Unicode::strmak (p_sval, s.p_sval);
      // clean the old string and save the buffer
      delete [] p_sval;
      p_sval = buf;
      d_nrmf = false;
      unlock   ();
      s.unlock ();
      return *this;
    } catch (...) {
      unlock   ();
      s.unlock ();
      throw;
    }
  }

  // add a character to the current string
  
  String& String::operator += (const char c) {
    wrlock   ();
    try {
      // create a new buffer by concatenation
      t_quad* buf = Unicode::strmak (p_sval, c);
      // clean the old string and save the buffer
      delete [] p_sval;
      p_sval = buf;
      d_nrmf = false;
      unlock ();
      return *this;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // add a character to the current string
  
  String& String::operator += (const t_quad c) {
    wrlock   ();
    try {
      // create a new buffer by concatenation
      t_quad* buf = Unicode::strmak (p_sval, c);
      // clean the old string and save the buffer
      delete [] p_sval;
      p_sval = buf;
      d_nrmf = false;
      unlock ();
      return *this;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // add an integer to this string
  
  String& String::operator += (const long value) {
    wrlock ();
    try {
      // create a temporary buffer to hold the converted value
      char* ibuf = Ascii::ltoa (value);
      // create a new buffer by concatenation
      t_quad* sbuf = Unicode::strmak (p_sval, ibuf);
      // clean the old string and save the buffer
      delete [] p_sval;
      delete [] ibuf;
      p_sval = sbuf;
      d_nrmf = false;
      unlock ();
      return *this;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // add a long integer to this string
  
  String& String::operator += (const t_long value) {
    rdlock ();
    try {
      // create a temporary buffer to hold the converted value
      char* ibuf = Ascii::lltoa (value);
      // create a new buffer by concatenation
      t_quad* sbuf = Unicode::strmak (p_sval, ibuf);
      // clean the old string and save the buffer
      delete [] p_sval;
      delete [] ibuf;
      p_sval = sbuf;
      d_nrmf = false;
      unlock ();
      return *this;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // compare a string with another one
  
  bool String::operator == (const String& s) const {
    rdlock   ();
    s.rdlock ();
    try {
      bool result = Unicode::strcmp (p_sval, d_nrmf, s.p_sval, s.d_nrmf);
      unlock   ();
      s.unlock ();
      return result;
    } catch (...) {
      unlock   ();
      s.unlock ();
      throw;
    }
  }

  // compare a string with a c-string
  
  bool String::operator == (const char* s) const {
    rdlock ();
    try {
      bool result = Unicode::strcmp (p_sval, d_nrmf, s);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // compare a string with unicode array
  
  bool String::operator == (const t_quad* s) const {
    rdlock ();
    try {
      bool result = Unicode::strcmp (p_sval, d_nrmf, s, false);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // compare a string with another one
  
  bool String::operator != (const String& s) const {
    rdlock   ();
    s.rdlock ();
    try {
      bool result = Unicode::strcmp (p_sval, d_nrmf, s.p_sval, s.d_nrmf);
      unlock   ();
      s.unlock ();
      return !result;
    } catch (...) {
      unlock   ();
      s.unlock ();
      throw;
    }
  }
  
  // compare a string with a c-string
  
  bool String::operator != (const char* s) const {
    rdlock ();
    try {
      bool result = Unicode::strcmp (p_sval, d_nrmf, s);
      unlock ();
      return !result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // compare a string with a unicode array
  
  bool String::operator != (const t_quad* s) const {
    rdlock ();
    try {
      bool result = Unicode::strcmp (p_sval, d_nrmf, s, false);
      unlock ();
      return !result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // compare a string with another one
  
  bool String::operator < (const String& s) const {
    rdlock   ();
    s.rdlock ();
    try {
      bool result = Unicode::strlth (p_sval, s.p_sval);
      unlock   ();
      s.unlock ();
      return result;
    } catch (...) {
      unlock   ();
      s.unlock ();
      throw;
    }
  }

  // compare a string with a c-string
  
  bool String::operator < (const char* s) const {
    rdlock ();
    try {
      bool result = Unicode::strlth (p_sval, s);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // compare a string with a unicode array
  
  bool String::operator < (const t_quad* s) const {
    rdlock ();
    try {
      bool result = Unicode::strlth (p_sval, s);
      unlock   ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // compare a string with another one
  
  bool String::operator <= (const String& s) const {
    rdlock   ();
    s.rdlock ();
    try {
      bool result = Unicode::strleq (p_sval, s.p_sval);
      unlock   ();
      s.unlock ();
      return result;
    } catch (...) {
      unlock   ();
      s.unlock ();
      throw;
    }
  }

  // compare a string with a c-string
  
  bool String::operator <= (const char* s) const {
    rdlock ();
    try {
      bool result = Unicode::strleq (p_sval, s);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // compare a string with a unicode array
  
  bool String::operator <= (const t_quad* s) const {
    rdlock ();
    try {
      bool result = Unicode::strleq (p_sval, s);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // compare a string with another one
  
  bool String::operator > (const String& s) const {
    rdlock   ();
    s.rdlock ();
    try {
      bool result = Unicode::strleq (p_sval, s.p_sval);
      unlock   ();
      s.unlock ();
      return !result;
    } catch (...) {
      unlock   ();
      s.unlock ();
      throw;
    }
  }

  // compare a string with a c-string
  
  bool String::operator > (const char* s) const {
    rdlock ();
    try {
      bool result = Unicode::strleq (p_sval, s);
      unlock ();
      return !result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // compare a string with a unicode array
  
  bool String::operator > (const t_quad* s) const {
    rdlock ();
    try {
      bool result = Unicode::strleq (p_sval, s);
      unlock ();
      return !result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // compare a string with another one
  
  bool String::operator >= (const String& s) const {
    rdlock   ();
    s.rdlock ();
    try {
      bool result = Unicode::strlth (p_sval, s.p_sval);
      unlock   ();
      s.unlock ();
      return !result;
    } catch (...) {
      unlock   ();
      s.unlock ();
      throw;
    }
  }

  // compare a string with a c-string
  
  bool String::operator >= (const char* s) const {
    rdlock ();
    try {
      bool result = Unicode::strlth (p_sval, s);
      unlock ();
      return !result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // compare a string with a unicode array
  
  bool String::operator >= (const t_quad* s) const {
    rdlock ();
    try {
      bool result = Unicode::strlth (p_sval, s);
      unlock ();
      return !result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

    // return the class name

  String String::repr (void) const {
    return "String";
  }

  // return a clone of this object

  Object* String::clone (void) const {
    return new String (*this);
  }

  // clear this string

  void String::clear (void) {
    wrlock ();
    try {
      delete [] p_sval; p_sval = nullptr;
      d_nrmf = false;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return a literal representation of this string
  
  String String::toliteral (void) const {
    rdlock ();
    try {
      String result = '"';
      result += p_sval;
      result += '"';
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return a printable representation of this string
  
  String String::tostring (void) const {
    rdlock ();
    try {
      String result = *this;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // format a styled string representation

  String String::format (const Style& lstl) const {
    rdlock ();
    try {
      String result = lstl.truncate (*this);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // return the string did

  t_word String::getdid (void) const {
    return SRL_DEOD_STD;
  }

  // return the string sid

  t_word String::getsid (void) const {
    return SRL_STRG_SID;
  }

  // serialize this string

  void String::wrstream (OutputStream& os) const {
    rdlock ();
    try {
      char* sbuf = Unicode::encode (Encoding::EMOD_UTF8, p_sval);
      os.write (sbuf);
      os.write (nilc);
      delete [] sbuf;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // deserialize this string

  void String::rdstream (InputStream& is) {
    wrlock ();
    try {
      Buffer buffer (Encoding::EMOD_UTF8);
      char c = nilc;
      while ((c = is.read ()) != nilc) buffer.add (c);
      *this = buffer.tostring ();
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return a quark from this string

  long String::toquark (void) const {
    rdlock ();
    try {
      long result = Central::intern (*this);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return true if the string is nil

  bool String::isnil (void) const {
    rdlock ();
    try {
      bool result = (p_sval == nullptr) || (p_sval[0] == nilq);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // return true if the string is eos

  bool String::iseos (void) const {
    rdlock ();
    try {
      bool result = (p_sval == nullptr) ? false : (p_sval[0] == eosq);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // return the length of this string
  
  long String::length (void) const {
    rdlock ();
    try {
      long result = Unicode::strlen (p_sval);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // return the non combining length of this string

  long String::ncclen (void) const {
    rdlock ();
    try {
      long result = Unicode::ncclen (p_sval);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // compare a string with an array in normal form

  bool String::equals (const t_quad* s) const {
    rdlock ();
    try {
      bool result = Unicode::strcmp (p_sval, d_nrmf, s, true);
      unlock   ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return a c-string representation or nullptr for this string

  char* String::tochar (void) const {
    rdlock ();
    try {
      char* result = Ascii::strdup (p_sval);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // return an unicode string representation or nullptr for this string

  t_quad* String::toquad (void) const {
    rdlock ();
    try {
      t_quad* result = Unicode::strdup (p_sval);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return the first character

  t_quad String::first (void) const {
    rdlock ();
    try {
      t_quad result = isnil () ? nilq : p_sval[0];
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return the last character

  t_quad String::last (void) const {
    rdlock ();
    try {
      long len = length ();
      t_quad result = (len == 0) ? nilq : p_sval[len-1];
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // remove leading blank from this string
  
  String String::stripl (void) const {
    rdlock ();
    try {
      t_quad* buffer = Unicode::stripl (p_sval);
      String  result = buffer;
      delete [] buffer;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // strip left a string with a separator specification
  
  String String::stripl (const String& sep) const {
    rdlock ();
    t_quad* qsep = sep.toquad ();
    try {
      t_quad* buffer = Unicode::stripl (p_sval, qsep);
      String  result = buffer;
      delete [] qsep;
      delete [] buffer;
      unlock ();
      return result;
    } catch (...) {
      delete [] qsep;
      unlock ();
      throw;
    }
  }
  
  // remove trailing blank from this string
  
  String String::stripr (void) const {
    rdlock ();
    try {
      t_quad* buffer = Unicode::stripr (p_sval);
      String  result = buffer;
      delete [] buffer;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // strip right a string with a separator specification
  
  String String::stripr (const String& sep) const {
    rdlock ();
    t_quad* qsep = sep.toquad ();
    try {
      t_quad* buffer = Unicode::stripr (p_sval, qsep);
      String  result = buffer;
      delete [] qsep;
      delete [] buffer;
      unlock ();
      return result;
    } catch (...) {
      delete [] qsep;
      unlock ();
      throw;
    }
  }

  // remove leading and trailing blank from this string
  
  String String::strip (void) const {
    rdlock ();
    try {
      t_quad* lbuffer = Unicode::stripl (p_sval);
      t_quad* rbuffer = Unicode::stripr (lbuffer);
      String  result  = rbuffer;
      delete [] lbuffer;
      delete [] rbuffer;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // strip a string with a separator specification
  
  String String::strip (const String& sep) const {
    rdlock ();
    t_quad* qsep = sep.toquad ();
    try {
      t_quad* lbuffer = Unicode::stripl (p_sval, qsep);
      t_quad* rbuffer = Unicode::stripr (lbuffer, qsep);
      String  result  = rbuffer;
      delete [] qsep;
      delete [] lbuffer;
      delete [] rbuffer;
      unlock ();
      return result;
    } catch (...) {
      delete [] qsep;
      unlock ();
      throw;
    }
  }

  // reduce the string into a small form

  String String::redex (void) const {
    rdlock ();
    try {
      t_quad* buffer = Unicode::redex (p_sval);
      String  result = buffer;
      delete [] buffer;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // remove the quote from a string

  String String::rmquote (void) const {
    rdlock ();
    try {
      t_quad* buffer = Unicode::rmquote (p_sval);
      String  result = buffer;
      delete [] buffer;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // convert the string to upper case
  
  String String::toupper (void) const {
    rdlock ();
    try {
      t_quad* buffer = Unicode::toupper (p_sval);
      String  result = buffer;
      delete [] buffer;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // convert the string into a normal decomposition form

  String String::tonfd (void) const {
    rdlock ();
    try {
      t_quad* buffer = Unicode::strnrm (p_sval);
      String  result = buffer;
      delete [] buffer;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // convert the string to lower case
  
  String String::tolower (void) const {
    rdlock ();
    try {
      t_quad* buffer = Unicode::tolower (p_sval);
      String  result = buffer;
      delete [] buffer;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return the right substring starting at a certain index

  String String::rsubstr (const long index) const {
    rdlock ();
    try {
      String result;
      long len = length ();
      if ((len == 0) || (index >= len)) {
	unlock ();
	return result;
      }
      t_quad* sptr = p_sval + index;
      result = sptr;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return the left substring of a string

  String String::lsubstr (const long index) const {
    rdlock ();
    try {
      String result;
      long len = length ();
      if ((len == 0) || (index > len)) {
	unlock ();
	return result;
      }
      t_quad* sptr = Unicode::strdup (p_sval);
      sptr[index] = nilq;
      result = sptr;
      delete [] sptr;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return a substring of a string

  String String::substr (const long lidx, const long ridx) const {
    rdlock ();
    try {
      long len = length ();
      if ((lidx >= ridx) || (lidx < 0) || (lidx >= len) || (ridx <0) ||
	  (ridx >  len)  || (len == 0)) {
	throw Exception ("index-error", "invalid index for sub-string");
      }
      // create th result string
      String result;
      t_quad* buf = Unicode::strdup (p_sval);
      t_quad* sub = buf + lidx;
      buf[ridx] = nilq;
      result = sub;
      delete [] buf;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return the hashid for this string
  
  long String::hashid (void) const {
    rdlock ();
    try {
      long result = Unicode::tohash (p_sval);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // case insensitive string comparision

  bool String::strcic (const String& s) const {
    rdlock ();
    try {
      bool result = String::strcic (*this, s);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // fill this string with a character until a given size is reached
  
  String String::lfill (const char c, const long size) const {
    rdlock ();
    try {
      // do nothing if size exceed
      long len = size - ncclen ();
      if (len <= 0) {
	String result = *this;
	unlock ();
	return result;
      }
      // fill the string first
      String result;
      for (long i = 0; i < len; i++) result = result + c;
      result += *this;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // fill this string with a character until a given size is reached
  
  String String::lfill (const t_quad c, const long size) const {
    rdlock ();
    try {
      // do nothing if size exceed
      long len = size - ncclen ();
      if (len <= 0) {
	String result = *this;
	unlock ();
	return result;
      }
      // fill the string first
      String result;
      for (long i = 0; i < len; i++) result = result + c;
      result += *this;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // fill this string with a character until a given size is reached
  
  String String::rfill (const char c, const long size) const {
    rdlock ();
    try {
      // do nothing if size exceed
      long len = size - ncclen ();
      if (len <= 0) {
	String result = *this;
	unlock ();
	return result;
      }
      // fill the string first
      String result = *this;
      for (long i = 0; i < len; i++) result = result + c;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // fill this string with a character until a given size is reached
  
  String String::rfill (const t_quad c, const long size) const {
    rdlock ();
    try {
      // do nothing if size exceed
      long len = size - ncclen ();
      if (len <= 0) {
	String result = *this;
	unlock ();
	return result;
      }
      // fill the string first
      String result = *this;
      for (long i = 0; i < len; i++) result = result + c;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // split a string with blank and tab

  Vector* String::split (void) const {
    rdlock ();
    try {
     Vector* result = split ("");
     unlock ();
     return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // split a string according to a string break

  Vector* String::split (const String& sbrk) const {
    rdlock ();
    Vector* result = new Vector;
    try {
      Strvec vec = Strvec::split (*this, sbrk);
      long len = vec.length ();
      for (long i = 0; i < len; i++) {
	String data = vec.get (i);
	result->add (new String (data));
      }
    } catch (...) {
      delete result;
      unlock ();
      throw;
    }
    return result;
  }

  // accumulate strings between a control character

  Vector* String::extract (const t_quad cbrk) const {
    rdlock ();
    Vector* result = new Vector;
    long len = length ();
    for (long i = 0; i < len; i++) {
      t_quad c = p_sval[i];
      if (c == cbrk) {
	i++;
	String buf;
	while ((c = p_sval[i]) != cbrk) {
	  buf += c ;
	  i++;
	  if (i == len) {
	    delete result;
	    unlock ();
	    throw Exception ("extract-error", "unterminated string", *this);
	  }
	}
	result->add (new String (buf));
      }
    }
    unlock ();
    return result;
  }

  // -------------------------------------------------------------------------
  // - object section                                                        -
  // -------------------------------------------------------------------------

  // the quark zone
  static const long QUARK_ZONE_LENGTH = 31;
  static QuarkZone  zone (QUARK_ZONE_LENGTH);

  // the supported quark for this string class
  static const long QUARK_ADD       = zone.intern ("+");
  static const long QUARK_EQL       = zone.intern ("==");
  static const long QUARK_NEQ       = zone.intern ("!=");
  static const long QUARK_LTH       = zone.intern ("<");
  static const long QUARK_LEQ       = zone.intern ("<=");
  static const long QUARK_GTH       = zone.intern (">");
  static const long QUARK_GEQ       = zone.intern (">=");
  static const long QUARK_AEQ       = zone.intern ("+=");
  static const long QUARK_GET       = zone.intern ("get");
  static const long QUARK_NILP      = zone.intern ("nil-p");
  static const long QUARK_EOSP      = zone.intern ("eos-p");
  static const long QUARK_LAST      = zone.intern ("last");
  static const long QUARK_FIRST     = zone.intern ("first");
  static const long QUARK_TONFD     = zone.intern ("to-normal-form");
  static const long QUARK_SPLIT     = zone.intern ("split");
  static const long QUARK_STRIP     = zone.intern ("strip");
  static const long QUARK_REDEX     = zone.intern ("redex");
  static const long QUARK_LENGTH    = zone.intern ("length");
  static const long QUARK_NCCLEN    = zone.intern ("non-combining-length");
  static const long QUARK_HASHID    = zone.intern ("hashid");
  static const long QUARK_STRIPL    = zone.intern ("strip-left");
  static const long QUARK_STRIPR    = zone.intern ("strip-right");
  static const long QUARK_SUBSTR    = zone.intern ("substr");
  static const long QUARK_STRCIC    = zone.intern ("strcic");
  static const long QUARK_RMQUOTE   = zone.intern ("remove-quote");
  static const long QUARK_EXTRACT   = zone.intern ("extract");
  static const long QUARK_TOUPPER   = zone.intern ("to-upper");
  static const long QUARK_TOLOWER   = zone.intern ("to-lower");
  static const long QUARK_SUBLEFT   = zone.intern ("sub-left");
  static const long QUARK_SUBRIGHT  = zone.intern ("sub-right");
  static const long QUARK_FILLLEFT  = zone.intern ("fill-left");
  static const long QUARK_FILLRIGHT = zone.intern ("fill-right");

  // create a new object in a generic way

  Object* String::mknew (Vector* argv) {
    if ((argv == nullptr) || (argv->length () == 0)) return new String;
    if (argv->length () != 1) 
      throw Exception ("argument-error", 
		       "too many argument with string constructor");
    // try to map the string argument
    Object* obj = argv->get (0);
    if (obj == nullptr) return new String;

    // try a literal object
    Literal* lval = dynamic_cast <Literal*> (obj);
    if (lval != nullptr) return new String (lval->tostring ());

    // illegal object
    throw Exception ("type-error", "illegal object with string constructor",
		     obj->repr ());
  }

  // return true if the given quark is defined

  bool String::isquark (const long quark, const bool hflg) const {
    rdlock ();
    try {
      if (zone.exists (quark) == true) {
	unlock ();
	return true;
      }
      bool result = hflg ? Literal::isquark (quark, hflg) : false;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // operate this object with another object

  Object* String::oper (t_oper type, Object* object) {
    Literal* lobj = dynamic_cast <Literal*> (object);
    String*  sobj = dynamic_cast <String*>    (object);

    switch (type) {
    case Object::OPER_ADD:
      if (lobj != nullptr) return new String (*this + lobj->tostring ());
      break;  
    case Object::OPER_EQL:
    case Object::OPER_QEQ:
      if (sobj != nullptr) return new Boolean (*this == *sobj);
      break;
    case Object::OPER_NEQ:
      if (sobj != nullptr) return new Boolean (*this != *sobj);
      break;
    case Object::OPER_LTH:
      if (sobj != nullptr) return new Boolean (*this < *sobj);
      break;
    case Object::OPER_LEQ:
      if (sobj != nullptr) return new Boolean (*this <= *sobj);
      break;
    case Object::OPER_GTH:
      if (sobj != nullptr) return new Boolean (*this > *sobj);
      break;
    case Object::OPER_GEQ:
      if (sobj != nullptr) return new Boolean (*this >= *sobj);
      break;
    default:
      throw Exception ("operator-error", "unsupported string operator");
    }
    throw Exception ("type-error", "invalid operand with string",
		     Object::repr (object));
  }

  // set an object to this object

  Object* String::vdef (Evaluable* zobj, Nameset* nset, Object* object) {
    Literal* lobj = dynamic_cast <Literal*> (object);
    if (lobj != nullptr) {
      *this = lobj->tostring ();
      return this;
    }
    throw Exception ("type-error", "invalid object with string vdef",
		     Object::repr (object));
  }

  // apply this object with a set of arguments and a quark

  Object* String::apply (Evaluable* zobj, Nameset* nset, long quark,
			 Vector* argv) {
    // get the arguments length
    long argc = (argv == nullptr) ? 0 : argv->length ();

    // dispatch 0 argument
    if (argc == 0) {
      if (quark == QUARK_NILP)    return new Boolean   (isnil   ());
      if (quark == QUARK_EOSP)    return new Boolean   (iseos   ());
      if (quark == QUARK_LAST)    return new Character (last    ());
      if (quark == QUARK_FIRST)   return new Character (first   ());
      if (quark == QUARK_TONFD)   return new String    (tonfd   ());
      if (quark == QUARK_LENGTH)  return new Integer   (length  ());
      if (quark == QUARK_NCCLEN)  return new Integer   (ncclen  ());
      if (quark == QUARK_STRIPL)  return new String    (stripl  ());
      if (quark == QUARK_STRIPR)  return new String    (stripr  ());
      if (quark == QUARK_STRIP)   return new String    (strip   ());
      if (quark == QUARK_REDEX)   return new String    (redex   ());
      if (quark == QUARK_RMQUOTE) return new String    (rmquote ());
      if (quark == QUARK_TOUPPER) return new String    (toupper ());
      if (quark == QUARK_TOLOWER) return new String    (tolower ());
      if (quark == QUARK_HASHID)  return new Integer   (hashid  ());
      if (quark == QUARK_SPLIT) {
	Object* result = split ();
	zobj->post (result);
	return result;
      }
    }
    // dispatch 1 argument
    if (argc == 1) {
      if (quark == QUARK_ADD) return oper (Object::OPER_ADD, argv->get (0));
      if (quark == QUARK_EQL) return oper (Object::OPER_EQL, argv->get (0));
      if (quark == QUARK_NEQ) return oper (Object::OPER_NEQ, argv->get (0));
      if (quark == QUARK_LTH) return oper (Object::OPER_LTH, argv->get (0));
      if (quark == QUARK_LEQ) return oper (Object::OPER_LEQ, argv->get (0));
      if (quark == QUARK_GTH) return oper (Object::OPER_GTH, argv->get (0));
      if (quark == QUARK_GEQ) return oper (Object::OPER_GEQ, argv->get (0));
      if (quark == QUARK_SPLIT) {
	Object* result = split (argv->getstring (0));
	zobj->post (result);
	return result;
      }
      if (quark == QUARK_AEQ) {
	Object*   obj = argv->get (0);
	Literal* lobj = dynamic_cast <Literal*> (obj);
	if (lobj == nullptr) {
	  throw Exception ("type-error", "invalid object with operator +=",
			   Object::repr (obj));
	}
	String val = lobj->tostring ();
	*this = *this + val;
	zobj->post (this);
	return this;
      }
      if (quark == QUARK_GET) {
	t_long val = argv->getlong (0);
	char c = (*this)[val];
	return new Character (c);
      }
      if (quark == QUARK_EXTRACT) {
	t_quad cbrk = argv->getchar (0);
	Object* result = extract (cbrk);
	zobj->post (result);
	return result;
      }
      if (quark == QUARK_STRIPL) {
	String sep = argv->getstring (0);
	String result = stripl (sep);
	return new String (result);
      }
      if (quark == QUARK_STRIPR) {
	String sep = argv->getstring (0);
	String result = stripr (sep);
	return new String (result);
      }
      if (quark == QUARK_STRIP) {
	String sep = argv->getstring (0);
	String result = strip (sep);
	return new String (result);
      }
      if (quark == QUARK_SUBRIGHT) {
	t_long val = argv->getlong (0);
	String result = rsubstr (val);
	return new String (result);
      }
      if (quark == QUARK_SUBLEFT) {
	t_long val = argv->getlong (0);
	String result = lsubstr (val);
	return new String (result);
      }
      if (quark == QUARK_STRCIC) {
	String    s = argv->getstring (0);
	bool result = strcic (s);
	return new Boolean (result);
      }
    }
    // dispatch 2 arguments
    if (argc == 2) {
      if (quark == QUARK_FILLLEFT) {
	t_quad fill   = argv->getchar (0);
	t_long size   = argv->getlong (1);
	String result = lfill (fill, size);
	return new String (result);
      }
      if (quark == QUARK_FILLRIGHT) {
	t_quad fill   = argv->getchar (0);
	t_long size   = argv->getlong (1);
	String result = rfill (fill, size);
	return new String (result);
      }
      if (quark == QUARK_SUBSTR) {
	t_long lidx   = argv->getlong (0);
	t_long ridx   = argv->getlong (1);
	String result = substr (lidx, ridx);
	return new String (result);
      }
    }
    // call the literal method
    return Literal::apply (zobj, nset, quark, argv);
  }
}
