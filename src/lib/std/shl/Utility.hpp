// ---------------------------------------------------------------------------
// - Utility.hpp                                                             -
// - standard object library - utility functions definition                  -
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

#ifndef  AFNIX_UTILITY_HPP
#define  AFNIX_UTILITY_HPP

#ifndef  AFNIX_STRING_HPP
#include "String.hpp"
#endif

namespace afnix {

  /// The utility class is a collection of static convenient functions. There
  /// purposes is to group general purpose functions into a single umbrella.
  /// @author amaury darsch

  class Utility {
  public:
    /// @return the maximum integer value (positive)
    static long maxlong (void);

    /// @return the minimum integer value (negative)
    static long minlong (void);

    /// nullify a memory block by size
    /// @param blok the block bytes
    /// @param size the block size
    static void tonull (void* blok, const long size);

    /// copy a memory block
    /// @param pdst the block destination
    /// @param size the block size
    /// @param psrc the block source
    static void tobcpy (void* pdst, const long size, const void* psrc);

    /// duplicate a memory block
    /// @param size the block size
    /// @param psrc the block source
    static void* tobdup (const long size, const void* psrc);
    
    /// convert a character to a byte
    /// @param value the character to convert
    static t_byte tobyte (const char value);
    
    /// convert a string to a boolean
    /// @param value the string to convert
    static bool tobool (const String& value);

    /// convert a literal to a boolean
    /// @param lobj the literal to convert
    static bool tobool (Literal* lobj);

    /// convert a string to an integer boolean
    /// @param value the string to convert
    static bool toboil (const String& value);

    /// convert a literal to an integer boolean
    /// @param lobj the literal to convert
    static bool toboil (Literal* lobj);

    /// convert a string to an integer
    /// @param value the string to convert
    static t_long tolong (const String& value);

    /// convert a literal to an integer
    /// @param lobj the literal to convert
    static t_long tolong (Literal* lobj);

    /// convert a string to a real
    /// @param value the string to convert
    static t_real toreal (const String& value);

    /// convert a literal to a real
    /// @param lobj the literal to convert
    static t_real toreal (Literal* lobj);
    
    /// convert a literal to a real integer
    /// @param lobj the literal to convert
    static t_real torint (Literal* lobj);
    
    /// convert a boolean to a string
    /// @param value the integer to convert
    static String tostring (const bool value);

    /// convert an integer to a string
    /// @param value the integer to convert
    static String tostring (const long value);

    /// convert an integer to a string upto a precision
    /// @param value the integer to convert
    /// @param psize the precision size
    static String tostring (const long value, const long psize);

    /// convert an integer to a string
    /// @param value the integer to convert
    static String tostring (const t_long value);

    /// convert an integer to a string upto a precision
    /// @param value the integer to convert
    /// @param psize the precision size
    static String tostring (const t_long value, const long psize);

    /// convert a real to a string
    /// @param value the real to convert
    static String tostring (const t_real value);

    /// convert a real to a string, eventually in scientific notation
    /// @param value the real to convert
    /// @param sflg  the scientific notation flag
    static String tostring (const t_real value, const bool sflg);

    /// convert a real to a string with a precision
    /// @param value the real to convert
    /// @param psize the real precision
    static String tostring (const t_real value, const long psize);

    /// convert a real to a string with a precision, with scientific notation
    /// @param value the real to convert
    /// @param psize the real precision
    /// @param sflg  the scientific notation flag
    static String tostring (const t_real value, const long psize, 
			    const   bool sflg);

    /// truncate a string by size and ellipsis
    /// @param sval the string value to truncate
    /// @param lsiz the left size to keep
    /// @param rsiz the right size to keep
    /// @param elps the ellipsis to add between
    static String truncate (const String& sval, const long lsiz,
			    const long    rsiz, const String& elps);
    
    /// convert a byte to a hexadecimal string
    /// @param value the byte to convert
    static String tohexa (const t_byte value);

    /// convert a byte to a hexadecimal string and normalize it
    /// @param value the quad to convert
    /// @param nflg  the normalize flag
    /// @param pflg  the prefix flag
    static String tohexa (const t_byte value, const bool nflg, const bool pflg);

    /// convert a word to a hexadecimal string
    /// @param value the word to convert
    static String tohexa (const t_word value);

    /// convert a word to a hexadecimal string and normalize it
    /// @param value the word to convert
    /// @param nflg  the normalize flag
    /// @param pflg  the prefix flag
    static String tohexa (const t_word value, const bool nflg, const bool pflg);

    /// convert a quad to a hexadecimal string
    /// @param value the quad to convert
    static String tohexa (const t_quad value);

    /// convert a quad to a hexadecimal string and normalize it
    /// @param value the quad to convert
    /// @param nflg  the normalize flag
    /// @param pflg  the prefix flag
    static String tohexa (const t_quad value, const bool nflg, const bool pflg);

    /// convert an octa to a hexadecimal string
    /// @param value the integer to convert
    static String tohexa (const t_octa value);

    /// convert an octa to a hexadecimal string and normalize it
    /// @param value the quad to convert
    /// @param nflg  the normalize flag
    /// @param pflg  the prefix flag
    static String tohexa (const t_octa value, const bool nflg, const bool pflg);

    /// @return a control character by printable character
    static t_quad toctrl (const char value);

    /// @return a long random number
    static long longrnd (void);

    /// generate long random number upto a maximum
    /// @param max the maximum integer value
    static long longrnd (const long max);

    /// generate a real random number between 0.0 and 1.0
    /// @param iflg the inclusion flag
    static t_real realrnd (const bool iflg);

    /// @return a byte random number
    static t_byte byternd (void);

    /// @return a word random number
    static t_word wordrnd (void);

    /// @return a quad random number
    static t_quad quadrnd (void);

    /// @return an octa random number
    static t_octa octarnd (void);
    
    /// create a random string by size
    /// @param size the string size
    static String strnrnd (const long size);

    /// hash a byte buffer into a quad
    /// @param hbuf the buffer to hash
    /// @param size the buffer size
    static t_quad hashq (const t_byte* hbuf, const long size);

    /// hash a byte buffer into a quad
    /// @param hbuf the buffer to hash
    /// @param size the buffer size
    /// @param hval the hash initial value
    static t_quad hashq (const t_byte* hbuf, const long size,
			 const t_quad  hval);

    /// hash a byte buffer into an octa
    /// @param hbuf the buffer to hash
    /// @param size the buffer size
    static t_octa hasho (const t_byte* hbuf, const long size);

    /// hash a byte buffer into an octa
    /// @param hbuf the buffer to hash
    /// @param size the buffer size
    /// @param hval the hash initial value
    static t_octa hasho (const t_byte* hbuf, const long size,
			 const t_octa  hval);

    /// hash a byte buffer into an integer
    /// @param hbuf the buffer to hash
    /// @param size the buffer size
    static long hashl (const t_byte* hbuf, const long size);
  };
}

#endif
