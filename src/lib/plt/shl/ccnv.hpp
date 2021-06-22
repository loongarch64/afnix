// ---------------------------------------------------------------------------
// - ccnv.hpp                                                                -
// - standard platform library - c conversion function definition            -
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

#ifndef  AFNIX_CCNV_HPP
#define  AFNIX_CCNV_HPP

#ifndef  AFNIX_CCNF_HPP
#include "ccnf.hpp"
#endif

namespace afnix {
  /// This file contains various conversion functions from one type to
  /// to another. It is primarily designed to handle the basic conversion
  /// from number types to strings and the opposite.
  /// @author amaury darsch

  /// convert an integer to a c-string representation.
  /// @param value the integer value
  /// @return the c-string representation
  char* c_ltoa (const long value);
  
  /// convert a long long integer to a c-string representation
  /// @param value the integer to convert
  /// @return the character buffer
  char* c_lltoa (const t_long value);

  /// convert a byte to a c-string hexadecimal representation.
  /// @param value the byte value
  /// @param pflag the prefix flag
  /// @return the c-string representation
  char* c_btoh (const t_byte value, const bool pflag);
  
  /// convert a word to a c-string hexadecimal representation.
  /// @param value the word value
  /// @param pflag the prefix flag
  /// @return the c-string representation
  char* c_wtoh (const t_word value, const bool pflag);
  
  /// convert a quad to a c-string hexadecimal representation.
  /// @param value the quad value
  /// @param pflag the prefix flag
  /// @return the c-string representation
  char* c_qtoh (const t_quad value, const bool pflag);
  
  /// convert an octa to a c-string hexadecimal representation
  /// @param value the octa to convert
  /// @param pflag the prefix flag
  /// @return the character buffer
  char* c_otoh (const t_octa value, const bool pflag);
  
  /// convert a char string to a long long integer
  /// @param buffer the buffer to convert
  /// @param status a boolean flag set to true if the conversion succeds
  /// @return a long long integer value
  t_long c_atoll (const char* buffer, bool& status);
  
  /// convert a real to a c-string representation
  /// @param value the real to convert
  /// @return the character buffer
  char* c_dtoa (const t_real value);
  
  /// convert a real to a scientific representation
  /// @param value the real to convert
  /// @return the character buffer
  char* c_dtos (const t_real value);

  /// convert a real to a c-string representation with a format
  /// @param value the real to convert
  /// @param psize the precision size
  /// @return the character buffer
  char* c_dtoap (const t_real value, const long psize);

  /// convert a real to a scientific representation with a format
  /// @param value the real to convert
  /// @param psize the conversion precision
  /// @return the character buffer
  char* c_dtosp (const t_real value, const long psize);
  
  /// convert a char string to a real
  /// @param buffer the buffer to convert
  /// @param status a boolean flag set to true if the conversion succeds
  /// @return a real value
  t_real c_atod (const char* buffer, bool& status);

  /// @return true if the platform operates in big endian mode
  bool c_isbe (void);

  /// convert a word to an array of bytes
  /// @param value the value to convert
  /// @param array the resulting array
  void c_whton (const t_word value, t_byte array[2]);

  /// convert an array of bytes to a word
  /// @param array the array to convert
  /// @return the word value
  t_word c_wntoh (const t_byte array[2]);

  /// swap a word between network and host format
  /// @param value the value to convert
  /// @return the swapped word
  t_word c_wswap (const t_word value);

  /// convert a quad to an array of bytes
  /// @param value the value to convert
  /// @param array the resulting array
  void c_qhton (const t_quad value, t_byte array[4]);

  /// convert an array of bytes to a quad
  /// @param array the array to convert
  /// @return the quad value
  t_quad c_qntoh (const t_byte array[4]);

  /// swap a quad between network and host format
  /// @param value the value to convert
  /// @return the swapped quad
  t_quad c_qswap (const t_quad value);

  /// convert an octa to an array of bytes
  /// @param value the value to convert
  /// @param array the resulting array
  void c_ohton (const t_octa value, t_byte array[8]);

  /// convert an array of bytes to an octa
  /// @param array the array to convert
  /// @return the integer value
  t_octa c_ontoh (const t_byte array[8]);

  /// swap an octa between network and host format
  /// @param value the value to convert
  /// @return the swapped octa
  t_octa c_oswap (const t_octa value);

  /// convert a real to an array of bytes
  /// @param value the value to convert
  /// @param array the resulting array
  void c_rhton (const t_real value, t_byte array[8]);

  /// convert an array of bytes to a real
  /// @param array the array to convert
  /// @return the real value
  t_real c_ontor (const t_byte array[8]);
}

#endif
