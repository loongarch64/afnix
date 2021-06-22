// ---------------------------------------------------------------------------
// - Utility.cpp                                                             -
// - standard object library - utility function class implementation         -
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

#include "Real.hpp"
#include "Boolean.hpp"
#include "Integer.hpp"
#include "Utility.hpp"
#include "Exception.hpp"
#include "cgen.hpp"
#include "ccnv.hpp"
#include "cmem.hpp"
#include "cmth.hpp"

namespace afnix {

  // return the maximum long integer value

  long Utility::maxlong (void) {
    // shift index to trick the compiler
    long sidx = 0;
    if (sizeof (long) == 8) sidx = 63;
    if (sizeof (long) == 4) sidx = 31;
    if (sidx == 0) {
      throw Exception ("internal-error", "unknown integer size");
    }
    // compute max value
    return ~(1L << sidx);
  }

  // return the minimum long integer value

  long Utility::minlong (void) {
    // shift index to trick the compiler
    long sidx = 0;
    if (sizeof (long) == 8) sidx = 63;
    if (sizeof (long) == 4) sidx = 31;
    if (sidx == 0) {
      throw Exception ("internal-error", "unknown integer size");
    }
    // compute min value
    return (1L << sidx);
  }

  // nullify a memory block

  void Utility::tonull (void* blok, const long size) {
    c_memset (blok, size, nilc);
  }

  // copy a memory block

  void Utility::tobcpy (void* pdst, const long size, const void* psrc) {
    c_memcpy (pdst, size, psrc);
  }

  // duplicate a memory block

  void* Utility::tobdup (const long size, const void* psrc) {
    // check for nil
    if ((size <= 0L) || (psrc == nullptr)) return nullptr;
    // allocate and copy
    void* result = new t_byte[size]; c_memcpy (result, size, psrc);
    // here it is
    return result;
  }
  
  // convert a character to a byte

  t_byte Utility::tobyte (const char value) {
    if ((value >= '0') && (value <= '9')) return  (t_byte) (value - '0');
    if ((value >= 'a') && (value <= 'f')) return ((t_byte) (value - 'a')) + 10;
    if ((value >= 'A') && (value <= 'F')) return ((t_byte) (value - 'A')) + 10;
    throw Exception ("literal-error", "invalid character to convert to byte",
		     value);
  }
  
  // convert a string to a boolean

  bool Utility::tobool (const String& value) {
    if (value.tolower () == "true")  return true;
    if (value.tolower () == "false") return false;
    throw Exception ("literal-error", "cannot convert to boolean", value);
  }
  
  // convert a literal to a boolean

  bool Utility::tobool (Literal* lobj) {
    Boolean* bobj = dynamic_cast <Boolean*> (lobj);
    if (bobj != nullptr) return bobj->tobool ();
    throw Exception ("literal-error", "cannot convert to boolean",
		     Object::repr(lobj));
  }
  
  // convert a string to an integer boolean

  bool Utility::toboil (const String& value) {
    // check integer value
    if (value == "0") return false;
    if (value == "1") return true;
    // check string value
    if (value.tolower () == "true")  return true;
    if (value.tolower () == "false") return false;
    throw Exception ("literal-error", "cannot convert to boilean", value);
  }
  
  // convert a literal to a boolean

  bool Utility::toboil (Literal* lobj) {
    Boolean* bobj = dynamic_cast <Boolean*> (lobj);
    if (bobj != nullptr) return bobj->tobool ();
    Integer* iobj = dynamic_cast <Integer*> (lobj);
    if (iobj != nullptr) {
      long ival = iobj->tolong ();
      if (ival == 0L) return false;
      if (ival == 1L) return true;
    }
    throw Exception ("literal-error", "cannot convert to boilean",
		     Object::repr(lobj));
  }
  
  // convert a string to an integer

  t_long Utility::tolong (const String& value) {
    bool  status     = false;
    const char* data = value.tochar ();
    t_long result = c_atoll (data, status);
    delete [] data;
    if (status == false) {
      throw Exception ("literal-error",
		       "illegal string integer number", value);
    }
    return result;
  }

  // convert a literal to an integer

  t_long Utility::tolong (Literal* lobj) {
    Integer* iobj = dynamic_cast <Integer*> (lobj);
    if (iobj != nullptr) return iobj->tolong ();
    throw Exception ("literal-error", "cannot convert to integer",
		     Object::repr(lobj));
  }
  
  // convert a string to a real

  t_real Utility::toreal (const String& value) {
    bool status = false;
    const char* data = value.tochar ();
    t_real result = c_atod (data, status);
    delete [] data;
    if (status == false) {
      throw Exception ("literal-error", "illegal string real number", value);
    }
    return result;
  }
  
  // convert a literal to a real

  t_real Utility::toreal (Literal* lobj) {
    Real* zobj = dynamic_cast <Real*> (lobj);
    if (zobj != nullptr) return zobj->toreal ();
    // cannot convert
    String msg = (lobj == nullptr) ? Object::repr(lobj) : lobj->tostring ();
    throw Exception ("literal-error", "cannot convert to real", msg);
  }

  // convert a literal to a real integer

  t_real Utility::torint (Literal* lobj) {
    Real* zobj = dynamic_cast <Real*> (lobj);
    if (zobj != nullptr) return zobj->toreal ();
    Integer* iobj = dynamic_cast <Integer*> (lobj);
    if (iobj != nullptr) return (t_real) iobj->tolong ();
    // cannot convert
    String msg = (lobj == nullptr) ? Object::repr(lobj) : lobj->tostring ();
    throw Exception ("literal-error", "cannot convert to rint", msg);
  }
  
  // convert a boolean to a string

  String Utility::tostring (const bool value) {
    String result = value ? "true" : "false";
    return result;
  }

  // convert an integer to a string

  String Utility::tostring (const long value) {
    char*  buffer = c_ltoa (value);
    String result = buffer;
    delete [] buffer;
    return result;
  }
  
  // convert an integer with a precision

  String Utility::tostring (const long value, const long psize) {
    String result = Utility::tostring (value);
    // check for additional precision
    if (psize > 0) {
      result += '.';
      for (long i = 0; i < psize; i++) result += '0';
    }
    return result;
  }
  
  // convert an integer to a string

  String Utility::tostring (const t_long value) {
    char*  buffer = c_lltoa (value);
    String result = buffer;
    delete [] buffer;
    return result;
  }

  // convert an integer with a precision

  String Utility::tostring (const t_long value, const long psize) {
    String result = Utility::tostring (value);
    // check for additional precision
    if (psize > 0) {
      result += '.';
      for (long i = 0; i < psize; i++) result += '0';
    }
    return result;
  }

  // convert a real to a string

  String Utility::tostring (const t_real value) {
    return Utility::tostring (value, false);
  }

  // convert a real to a string, eventually in scientific notation

  String Utility::tostring (const t_real value, const bool sflg) {
    char* buffer = sflg ? c_dtos (value) : c_dtoa (value);
    String result (buffer);
    delete [] buffer;
    return result;
  }

  // convert a real to a string with a certain precision

  String Utility::tostring (const t_real value, const long psize) {
    return Utility::tostring (value, psize, false);
  }

  // convert a real to a string with a certain precision

  String Utility::tostring (const t_real value, const long psize, 
			    const bool   sflg) {
    // check the precision
    if (psize < 0) {
      throw Exception ("precision-error", 
		       "invalid negative precision with conversion");
    }
    // check for null precision
    if ((psize == 0) && (c_isnan (value) == false) && 
	(c_isinf (value) == false)) {
      t_long data = (t_long) value;
      return Utility::tostring (data);
    }
    // use base precision
    char* buffer = sflg ? c_dtosp (value, psize) : c_dtoap (value, psize);
    String result (buffer);
    delete [] buffer;
    return result;
  }

  String Utility::truncate (const String& sval, const long lsiz,
			    const long    rsiz, const String& elps) {
    // strip the input string
    String sstr = sval.strip ();
    // get the string length
    long slen = sstr.length ();
    // check if the string needs to be truncated
    if (slen <= (lsiz + rsiz + elps.length ())) return sstr;
    // extract string components
    String lval = sstr.lsubstr (lsiz);
    String rval = sstr.rsubstr (slen-rsiz);
    // build the result string
    String result = lval + elps + rval;
    return result;
  }
  
  // convert a byte to a hexadecimal string

  String Utility::tohexa (const t_byte value) {
    char*  buffer = c_btoh (value, false);
    String result = buffer;
    delete [] buffer;
    return result;
  }

  // convert a byte to a hexadecimal string and normalize it

  String Utility::tohexa (const t_byte value,const bool nflg,const bool pflg) {
    String lval = Utility::tohexa (value);
    lval = nflg ? lval.lfill ('0', 2) : lval;
    if (pflg == true) {
      String result = "0x";
      result += lval;
      return result;
    }
    return lval;
  }

  // convert a word to a hexadecimal string

  String Utility::tohexa (const t_word value) {
    char*  buffer = c_wtoh (value, false);
    String result = buffer;
    delete [] buffer;
    return result;
  }

  // convert a word to a hexadecimal string and normalize it

  String Utility::tohexa (const t_word value,const bool nflg,const bool pflg) {
    String lval = Utility::tohexa (value);
    lval = nflg ? lval.lfill ('0', 4) : lval;
    if (pflg == true) {
      String result = "0x";
      result += lval;
      return result;
    }
    return lval;
  }

  // convert a quad to a hexadecimal string

  String Utility::tohexa (const t_quad value) {
    char*  buffer = c_qtoh (value, false);
    String result = buffer;
    delete [] buffer;
    return result;
  }

  // convert a quad to a hexadecimal string and normalize it

  String Utility::tohexa (const t_quad value,const bool nflg,const bool pflg) {
    String lval = Utility::tohexa (value);
    lval = nflg ? lval.lfill ('0', 8) : lval;
    if (pflg == true) {
      String result = "0x";
      result += lval;
      return result;
    }
    return lval;
  }

  // convert an octa to a hexadecimal string

  String Utility::tohexa (const t_octa value) {
    char*  buffer = c_otoh (value, false);
    String result = buffer;
    delete [] buffer;
    return result;
  }

  // convert an octa to a hexadecimal string and normalize it

  String Utility::tohexa (const t_octa value,const bool nflg,const bool pflg) {
    String lval = Utility::tohexa (value);
    lval = nflg ? lval.lfill ('0', 16) : lval;
    if (pflg == true) {
      String result = "0x";
      result += lval;
      return result;
    }
    return lval;
  }

  // return a control character from a printable character

  t_quad Utility::toctrl (const char value) {
    return (t_quad) (value - 'a' + 1);
  }

  // return a long random number

  long Utility::longrnd (void) {
    long result = 0;
    // check for quad
    if (sizeof (long) == 8) {
      result = c_octarnd ();
    } else {
      result = c_quadrnd ();
    }
    // force to a positive value
    if (result < 0) result = -result;
    // here it is
    return result;
  }

  // return a long random number upto a maximum

  long Utility::longrnd (const long max) {
    long result = (long) (c_realrnd (true) * max);
    // force to a positive value
    if (result < 0) result = -result;
    // here it is
    return result;
  }

  // return a real random number

  t_real Utility::realrnd (const bool iflg) {
    return c_realrnd (iflg);
  }

  // return a byte random number

  t_byte Utility::byternd (void) {
    return c_byternd ();
  }

  // return a word random number

  t_word Utility::wordrnd (void) {
    return c_wordrnd ();
  }

  // return a quad random number

  t_quad Utility::quadrnd (void) {
    return c_quadrnd ();
  }

  // return an octa random number

  t_octa Utility::octarnd (void) {
    return c_octarnd ();
  }

  // return a random string by size

  String Utility::strnrnd (const long size) {
    // create the result string
    if (size <= 0L) return String();
    // create an array buffer
    char data[size+1L];
    for (long k = 0L; k < size; k++) {
      char c = nilc;
      while (c == nilc) c = byternd ();
      data[k] = c;
    }
    data[size] = nilc;
    String result(data);
    return result;
  }

  // hash a byte buffer into a quad

  t_quad Utility::hashq (const t_byte* hbuf, const long size) {
    // check for null first
    if ((hbuf == nullptr) || (size <= 0L)) return nilq;
    // Fowler-Noll-Vo (fnv) 32 bits
    return hashq (hbuf, size, 0x811C9DC5U);
  }

  // hash a byte buffer into a quad with an initial value

  t_quad Utility::hashq (const t_byte* hbuf, const long size,
			 const t_quad  hval) {
    // check for null first
    if ((hbuf == nullptr) || (size <= 0L)) return hval;
    // Fowler-Noll-Vo (fnv) 32 bits
    t_quad result = hval;
    for (long k = 0L; k < size; k++) {
      // multiply with prime
      result += (result << 1) + (result << 4) + (result << 7) +
	        (result << 8) + (result << 24);
      // xor with byte
      result ^= hbuf[k];
    }
    return result;
  }

  // hash a byte buffer into an octa

  t_octa Utility::hasho (const t_byte* hbuf, const long size) {
    // check for null first
    if ((hbuf == nullptr) || (size <= 0L)) return nilq;
    // Fowler-Noll-Vo (fnv) 64 bits
    return hasho (hbuf, size, 0XCBF29CE484222325U);
  }

  // hash a byte buffer into an octa

  t_octa Utility::hasho (const t_byte* hbuf, const long size,
			 const t_octa  hval) {
    // check for null first
    if ((hbuf == nullptr) || (size <= 0L)) return hval;
    // Fowler-Noll-Vo (fnv) 64 bits
    t_octa result = hval;
    for (long k = 0L; k < size; k++) {
      // multiply with prime - 0x100000001B3U
      result += (result << 1) + (result << 4) + (result << 5) +
	        (result << 7) + (result << 8) + (result << 40);
      // xor with byte buffer
      result ^= hbuf[k];
    }
    return result;
  }

  // hash a byte buffer into an integer

  long Utility::hashl (const t_byte* hbuf, const long size) {
    long result = (sizeof (long) == sizeof (t_quad))
      ? hashq (hbuf, size)
      : hasho (hbuf, size);
    return (result < 0L) ? -result : result;
  }
}
