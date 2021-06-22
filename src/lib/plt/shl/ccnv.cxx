// ---------------------------------------------------------------------------
// - ccnv.cxx                                                                -
// - standard platform library - c conversion function implementation        -
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

#include "cstr.hpp"
#include "ccnv.hpp"
#include "ccnv.hxx"

namespace afnix {

  // this function convert a character according to a base
  static inline long ctol (const char c, const int base, bool& status) {
    status = true;
    switch (base) {
    case 2:
      if (c == '0') return 0;
      if (c == '1') return 1;
      break;
    case 10:
      if ((c >= '0') && (c <= '9')) return (long) (c - '0');
      break;
    case 16:
      if ((c >= '0') && (c <= '9')) return  (long) (c - '0');
      if ((c >= 'a') && (c <= 'f')) return ((long) (c - 'a')) + 10;
      if ((c >= 'A') && (c <= 'F')) return ((long) (c - 'A')) + 10;
      break;
    }
    status = false;
    return 0;
  }
  
  t_long c_atoll (const char* s, bool& status) {
    // initialize result
    int       base   = 10;
    long long basval = 1;
    long long result = 0;  
    status           = true;
    
    // check for size first
    long len = 0;
    if ((s == 0) || ((len = c_strlen (s)) == 0)) return 0;
    
    // process one character
    if (len == 1) return ctol (s[0],10,status);
    
    // here we have at least two characters - it can be the sign, the format
    // or a normal number
    bool negative   = false;
    const char* ptr = s;
    
    // check for the sign
    if (ptr[0] == '-') {
      ptr++; len--;
      negative = true;
      goto format;
    }
    if (ptr[0] == '+') {
      ptr++; len--;
      goto format;
    }
    
    // check for the format
  format:
    if (ptr[0] != '0') goto number;
    ptr++; len--;
    if (len == 0) return 0;
    if ((ptr[0] == 'x') || (ptr[0] == 'X')) {
      ptr++; len--;
      if (len == 0) {
	status = false;
	return 0;
      }
      base = 16;
      goto number;
    }
    if ((ptr[0] == 'b') || (ptr[0] == 'B')) {
      ptr++; len--;
      if (len == 0) {
	status = false;
	return 0;
      }
      base = 2;
      goto number;
    }
    
    // compute the number value
  number:
    if (len == 0) return 0;
    for (long i = len-1; i >= 0; i--) {
      result  += basval * ctol (ptr[i], base, status);
      basval  *= base;
      if (status == false) return 0;
    };
    return (negative) ? -result : result;
  }

  // this procedure convert a long value to a character
  static inline char ltoc (const long value, bool& status) {
    if ((value >=  0) && (value <= 9)) return (char) ('0' + value);
    status = false;
    return nilc;
  }

  // convert a long long integer to a string

  char* c_lltoa (const t_long value) {
    char buffer[512];
    bool status       = true;
    long index        = 0;
    bool sign         = (value < 0) ? true : false;
    t_octa baseval = 10;
    t_octa dataval = sign ? (t_octa) -value : (t_octa) value;
    
    // let start in good shape
    buffer[0] = nilc;
    index     = 0;
    // loop until we reach 0
    while (dataval != 0) {
      long val = (long) (dataval % baseval);
      buffer[index++] = ltoc (val, status);
      if (status == false) return nullptr;
      // readjust index
      dataval /= baseval;
    }
    if (status == false) return nullptr;
    // readjust for sign and null value
    if (sign == true) buffer[index++] = '-';
    if (index == 0)   buffer[index++] = '0';
    // prepare the result string
    char* result = new char[index+1];
    for (long i = 0; i < index; i++) result[i] = buffer[index-i-1];
    result[index] = nilc;
    return result;
  }

  // convert a long integer to a string

  char* c_ltoa (const long value) {
    return c_lltoa (value);
  }

  // this procedure convert a long value to a hexa character
  static inline char ltoh (const long value, bool& status) {
    if ((value >=  0) && (value <= 9)) return (char) ('0' + value);
    if (value == 10) return 'A';
    if (value == 11) return 'B';
    if (value == 12) return 'C';
    if (value == 13) return 'D';
    if (value == 14) return 'E';
    if (value == 15) return 'F';
    status = false;
    return nilc;
  }

  // convert a byte to a hexadecimal string

  char* c_btoh (const t_byte value, const bool pflag) {
    return c_otoh (value, pflag);
  }

  // convert a word to a hexadecimal string

  char* c_wtoh (const t_word value, const bool pflag) {
    return c_otoh (value, pflag);
  }

  // convert a quad to a hexadecimal string

  char* c_qtoh (const t_quad value, const bool pflag) {
    return c_otoh (value, pflag);
  }

  // convert an octa to an hexadecimal string

  char* c_otoh (const t_octa value, const bool pflag) {
    char buffer[512];
    bool status    = true;
    long index     = 0;
    t_octa baseval = 16;
    t_octa dataval = value;
    
    // let start in good shape
    buffer[0] = nilc;
    index     = 0;
    // loop until we reach 0
    while (dataval != 0) {
      long val = dataval % baseval;
      buffer[index++] = ltoh (val, status);
      if (status == false) return nullptr;
      // readjust index
      dataval /= baseval;
    }
    if (status == false) return nullptr;
    // readjust extension
    if (index == 0) buffer[index++] = '0';
    if (pflag == true) {
      buffer[index++] = 'x';
      buffer[index++] = '0';
    }
    // prepare the result string
    char* result = new char[index+1];
    for (long i = 0; i < index; i++) result[i] = buffer[index-i-1];
    result[index] = nilc;
    return result;
  }

  // convert a real to a c-string representation

  char* c_dtoa (const t_real value) {
    char buffer[512];
    sprintf (buffer,"%f",value);
    return c_strdup (buffer);
  }

  // convert a real to a scientific representation

  char* c_dtos (const t_real value) {
    char buffer[512];
    sprintf (buffer,"%e",value);
    return c_strdup (buffer);
  }

  // convert a real to a c-string with a precision

  char* c_dtoap (const t_real value, const long psize) {
    char format[512];
    char buffer[512];
    if (psize < 0) return nullptr;
    sprintf (format, "%%.%ldf", psize);
    if (psize  == 0)
      sprintf (buffer, "%f", value);
    else
      sprintf (buffer, format, value);
    return c_strdup (buffer);
  }

  // convert a real to a scientific notation with a precision

  char* c_dtosp (const t_real value, const long psize) {
    char format[512];
    char buffer[512];
    if (psize < 0) return nullptr;
    sprintf (format, "%%.%lde", psize);
    if (psize == 0)
      sprintf (buffer,"%e", value);
    else
      sprintf (buffer, format, value);
    return c_strdup (buffer);
  }

  // convert a char string to a real

  t_real c_atod (const char* buffer, bool& status) {
    char* check;
    errno = 0;
    if (c_strlen (buffer) == 0) return 0.0;
    t_real result = strtod (buffer,&check);
    if ((*check != nilc) || (errno != 0)) {
      status = false;
      return 0.0;
    }
    status = true;
    return result;
  }

  // return true if the platform is in big endian sex
  
  bool c_isbe (void) {
    union {
      t_word wval;
      t_byte bval[2];
    } data;
    data.wval = 0x1234;
    if ((data.bval[0] == 0x12) && (data.bval[1] == 0x34)) return true;
    return false;
  }

  // convert a word to an array on bytes

  void c_whton (const t_word value, t_byte array[2]) {
    union {
      t_word wval;
      t_byte bval[2];
    } data;
    data.wval = value;
    if (c_isbe () == true) {
      array[1] = data.bval[1];
      array[0] = data.bval[0];
    } else {
      array[1] = data.bval[0];
      array[0] = data.bval[1];
    }
  }

  // convert an array of bytes to a word

  t_word c_wntoh (const t_byte array[2]) {
    union {
      t_word wval;
      t_byte bval[2];
    } data;
    if (c_isbe () == true) {
      data.bval[1] = array[1];
      data.bval[0] = array[0];
    } else {
      data.bval[1] = array[0];
      data.bval[0] = array[1];
    }
    return data.wval;
  }

  // swap a word between network order and host order

  t_word c_wswap (const t_word value) {
    // do nothing in big endian mode
    if (c_isbe () == true) return value;
    // swap in little endian mode
    union {
      t_word wval;
      t_byte bval[2];
    } data;
    data.wval    = value;
    t_byte tmp   = data.bval[0];
    data.bval[0] = data.bval[1];
    data.bval[1] = tmp;
    return data.wval;
  }

  // convert a quad to an array on bytes

  void c_qhton (const t_quad value, t_byte array[4]) {
    union {
      t_quad qval;
      t_byte bval[4];
    } data;
    data.qval = value;
    if (c_isbe () == true) {
      array[3] = data.bval[3];
      array[2] = data.bval[2];
      array[1] = data.bval[1];
      array[0] = data.bval[0];
    } else {
      array[3] = data.bval[0];
      array[2] = data.bval[1];
      array[1] = data.bval[2];
      array[0] = data.bval[3];
    }
  }

  // convert an array of bytes to a quad

  t_quad c_qntoh (const t_byte array[4]) {
    union {
      t_quad qval;
      t_byte bval[4];
    } data;
    if (c_isbe () == true) {
      data.bval[3] = array[3];
      data.bval[2] = array[2];
      data.bval[1] = array[1];
      data.bval[0] = array[0];
    } else {
      data.bval[3] = array[0];
      data.bval[2] = array[1];
      data.bval[1] = array[2];
      data.bval[0] = array[3];
    }
    return data.qval;
  }

  // swap a quad between network order and host order

  t_quad c_qswap (const t_quad value) {
    // do nothing in big endian mode
    if (c_isbe () == true) return value;
    // swap in little endian mode
    union {
      t_quad qval;
      t_byte bval[4];
    } data;
    data.qval    = value;
    t_byte tmp   = data.bval[3];
    data.bval[3] = data.bval[0];
    data.bval[0] = tmp;
    tmp          = data.bval[2];
    data.bval[2] = data.bval[1];
    data.bval[1] = tmp;
    return data.qval;
  }

  // convert an octa to an array on bytes

  void c_ohton (const t_octa value, t_byte array[8]) {
    union {
      t_octa oval;
      t_byte bval[8];
    } data;
    data.oval = value;
    if (c_isbe () == true) {
      array[7] = data.bval[7];
      array[6] = data.bval[6];
      array[5] = data.bval[5];
      array[4] = data.bval[4];
      array[3] = data.bval[3];
      array[2] = data.bval[2];
      array[1] = data.bval[1];
      array[0] = data.bval[0];
    } else {
      array[7] = data.bval[0];
      array[6] = data.bval[1];
      array[5] = data.bval[2];
      array[4] = data.bval[3];
      array[3] = data.bval[4];
      array[2] = data.bval[5];
      array[1] = data.bval[6];
      array[0] = data.bval[7];
    }
  }

  // convert an array of bytes to an octa

  t_octa c_ontoh (const t_byte array[8]) {
    union {
      t_octa oval;
      t_byte bval[8];
    } data;
    if (c_isbe () == true) {
      data.bval[7] = array[7];
      data.bval[6] = array[6];
      data.bval[5] = array[5];
      data.bval[4] = array[4];
      data.bval[3] = array[3];
      data.bval[2] = array[2];
      data.bval[1] = array[1];
      data.bval[0] = array[0];
    } else {
      data.bval[7] = array[0];
      data.bval[6] = array[1];
      data.bval[5] = array[2];
      data.bval[4] = array[3];
      data.bval[3] = array[4];
      data.bval[2] = array[5];
      data.bval[1] = array[6];
      data.bval[0] = array[7];
    }
    return data.oval;
  }

  // swap an octa between network order and host order
  
  t_octa c_oswap (const t_octa value) {
    // do noting in big endian mode
    if (c_isbe () == true) return value;
    // swap in little endian mode
    union {
      t_octa oval;
      t_byte bval[8];
    } data;
    data.oval    = value;
    t_byte tmp   = data.bval[7];
    data.bval[7] = data.bval[0];
    data.bval[0] = tmp;
    tmp          = data.bval[6];
    data.bval[6] = data.bval[1];
    data.bval[1] = tmp;
    tmp          = data.bval[5];
    data.bval[5] = data.bval[2];
    data.bval[2] = tmp;
    tmp          = data.bval[4];
    data.bval[4] = data.bval[3];
    data.bval[3] = tmp;
    return data.oval;
  }

  // convert a real to an array of bytes

  void c_rhton (const t_real value, t_byte array[8]) {
    // map the real value as an octa
    union {
      t_real rval;
      t_octa oval;
    } data;
    data.rval = value;
    // map the octa to the array
    c_ohton (data.oval, array);
  }

  // convert an array of bytes to a real

  t_real c_ontor (const t_byte array[8]) {
    // map the array as an octa
    union {
      t_real rval;
      t_octa oval;
    } data;
    data.oval = c_ontoh (array);
    // map the real value
    return data.rval;
  }
}
