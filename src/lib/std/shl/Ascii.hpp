// ---------------------------------------------------------------------------
// - Ascii.hpp                                                               -
// - standard object library - ascii functions class definition              -
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

#ifndef  AFNIX_ASCII_HPP
#define  AFNIX_ASCII_HPP

#ifndef  AFNIX_STRING_HPP
#include "String.hpp"
#endif

namespace afnix {

  /// The Ascii class is a collection of static convenient functions that
  /// manipulates the ascii character representation and by extension the 
  /// iso-8859. The internal representation of a character is char type.
  /// @author amaury darsch

  class Ascii {
  public:
    /// convert a hexadecimal character to a byte
    /// @param value the character to convert
    static t_byte htob (const char value);

    /// convert a byte to a printable character
    /// @param byte the byte to convert
    /// @param lflg the low order character flag
    static char btoc (const t_byte byte, const bool lflg);

    /// convert a byte array to a string
    /// @param bval the source byte array
    /// @param size the array size
    static String btos (const t_byte* bval, const long size);
    
    /// convert an integer to a c-string representation.
    /// @param value the integer value
    /// @return the c-string representation
    static char* ltoa (const long value);
    
    /// convert a long long integer to a c-string representation
    /// @param value the integer to convert
    /// @return the character buffer
    static char* lltoa (const t_long value);

    /// convert a character byte to a character code
    /// @param byte the character byte to convert
    /// @param flag the upper control flag
    static char tochar (const char byte, const bool flag);

    /// convert a string to a native character
    /// @param value the string value representation to convert
    static char tochar (const String& value);

    /// convert a native character to a string
    /// @param value the native value to convert
    static String tostring (const char value);

    /// convert a native character to a literal string
    /// @param value the native value to convert
    static String toliteral (const char value);

    /// @return the length of an ascii string
    static long strlen (const char* s);

    /// @return the length of an ascii string in unicode element
    static long strlen (const t_quad* s);

    /// @return a c-string from an ascii character
    static char* strmak (const char value);

    /// @return a c-string from an unicode character
    static char* strmak (const t_quad value);

    /// @return a c-string from an ascii string
    static char* strdup (const char* s);

    /// @return a c-string from an unicode string
    static char* strdup (const t_quad* s);

    /// @return true if the character is alpha
    static bool isalpha (const char value);

    /// @return true if the character is a digit
    static bool isdigit (const char value);

    /// @return true if the character is hexa
    static bool ishexa (const char value);

    /// @return true if the character is a blank or tab
    static bool isblank (const char value);

    /// @return true if the unicode character is an ascii character
    static bool isascii (const t_quad value);

    /// @return true if the unicode character is a latin character
    static bool islatin (const t_quad value);
  };
}

#endif
