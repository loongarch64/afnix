// ---------------------------------------------------------------------------
// - Unicode.hpp                                                             -
// - standard object library - unicode functions class definition            -
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

#ifndef  AFNIX_UNICODE_HPP
#define  AFNIX_UNICODE_HPP

#ifndef  AFNIX_ENCODING_HPP
#include "Encoding.hpp"
#endif

namespace afnix {

  /// The Unicode class is a collection of static convenient functions that
  /// manipulates the unicode representation. The internal representation of
  /// a unicode character is quad type in host representation.
  /// @author amaury darsch

  class Unicode {
  public:
    /// the maximum utf-8 buffer size
    static const long MAX_UTF8_SIZE = 5;

  public:
    /// convert a unicode character to a native character if possible
    /// @param value the value to convert
    static char tochar (const t_quad value);

    /// convert a unicode character to a bmp character if possible
    /// @param value the value to convert
    static t_word tobmp (const t_quad value);

    /// convert a hexadecimal character to a byte
    /// @param value the character to convert
    static t_byte htob (const t_quad value);

    /// convert a quad into a byte array
    /// @param value the character to convert
    static t_byte* qtob (long& size, const t_quad value);

    /// convert a string into a byte array
    /// @param s the string to convert
    static t_byte* stob (long& size, const String& s);

    /// convert a native character to a unicode character
    /// @param value the value to convert
    static t_quad toquad (const char value);

    /// convert a string to a unicode character
    /// @param value the string value representation to convert
    static t_quad toquad (const String& value);

    /// convert a unicode character to a string
    /// @param value the native value to convert
    static String tostring (const t_quad value);

    /// convert a unicode character to a literal string
    /// @param value the native value to convert
    static String toliteral (const t_quad value);

    /// @return the length of an unicode string
    static long strlen (const t_quad* s);

    /// compare two strings and returns true if they are equals.
    /// @param s1 the first string
    /// @param n1 the first normal flag
    /// @param s2 the second string
    /// @return true if the string are equals or both null
    static bool strcmp (const t_quad* s1, const bool n1, const char* s2);

    /// compare two strings and returns true if they are equals.
    /// @param s1 the first string
    /// @param s2 the second string
    /// @return true if the string are equals or both null
    static bool strcmp (const t_quad* s1, const char* s2);

    /// compare two strings and returns true if they are equals.
    /// @param s1 the first string
    /// @param n1 the first normal flag
    /// @param s2 the second string
    /// @param n2 the second normal flag
    /// @return true if the string are equals or both null
    static bool strcmp (const t_quad* s1, const bool n1,
			const t_quad* s2, const bool n2);

    /// compare two strings and returns true if they are equals.
    /// @param s1 the first string
    /// @param s2 the second string
    /// @return true if the string are equals or both null
    static bool strcmp (const t_quad* s1, const t_quad* s2);
    
    /// compare two strings and returns true if they are equals for a number of
    /// characters. This function is safe with null pointer.
    /// @param s1 the first string
    /// @param s2 the second string
    /// @param size the number of characters to compare
    /// @return true if the string are equals or both null
    static bool strncmp (const t_quad* s1, const char* s2, const long size);

    /// compare two strings and returns true if they are equals for a number of
    /// characters. This function is safe with null pointer.
    /// @param s1 the first string
    /// @param s2 the second string
    /// @param size the number of characters to compare
    /// @return true if the string are equals or both null
    static bool strncmp (const t_quad* s1, const t_quad* s2, const long size);

    /// compare two strings and returns true if s1 is less than s2.
    /// @param s1 the first string
    /// @param s2 the second string
    /// @return true if the string are equals or both null
    static bool strlth (const t_quad* s1, const char* s2);
 
    /// compare two strings and returns true if s1 is less than s2.
    /// @param s1 the first string
    /// @param s2 the second string
    /// @return true if the string are equals or both null
    static bool strlth (const t_quad* s1, const t_quad* s2);
    
    /// compare two strings and returns true if s1 is less equal than s2.
    /// @param s1 the first string
    /// @param s2 the second string
    /// @return true if the string are equals or both null
    static bool strleq (const t_quad* s1, const char* s2);

    /// compare two strings and returns true if s1 is less equal than s2.
    /// @param s1 the first string
    /// @param s2 the second string
    /// @return true if the string are equals or both null
    static bool strleq (const t_quad* s1, const t_quad* s2);

    /// @return an unicode array from an ascii character
    static t_quad* strmak (const char value);

    /// @return an unicode array from an unicode character
    static t_quad* strmak (const t_quad value);

    /// create a unicode string from a unicode string and a character
    /// @param s the original string to use
    /// @param c the character to add
    static t_quad* strmak (const t_quad* s, const char c);

    /// create a unicode string from a unicode string and a unicode character
    /// @param s the original string to use
    /// @param c the character to add
    static t_quad* strmak (const t_quad* s, const t_quad c);

    /// create a unicode string from a character and a unicode string
    /// @param c the character to add
    /// @param s the original string to use
    static t_quad* strmak (const char c, const t_quad* s);

    /// create a unicode string from a unicode character and a unicode string
    /// @param c the character to add
    /// @param s the original string to use
    static t_quad* strmak (const t_quad c, const t_quad* s);

    /// concatenate two strings and normalize the result
    /// @param s1 the first string
    /// @param s1 the second string
    static t_quad* strmak (const t_quad* s1, const char* s2);

    /// concatenate two strings and normalize the result
    /// @param s1 the first string
    /// @param s2 the second string
    static t_quad* strmak (const t_quad* s1, const t_quad* s2);

    /// @return an unicode array from an ascii string
    static t_quad* strdup (const char* s);

    /// @return an unicode array from an unicode string
    static t_quad* strdup (const t_quad* s);

    /// @return an unicode array from an ascii string
    static t_quad* strdup (const char* s, const bool nrmf);

    /// @return an unicode array from an unicode string
    static t_quad* strdup (const t_quad* s, const bool nrmf);

    /// @return an unicode array from a character buffer and size
    static t_quad* strdup (const char* s, const long size);

    /// @return an unicode array from an unicode string and size
    static t_quad* strdup (const t_quad* s, const long size);

    /// @return a string in a normal decomposition form
    static t_quad* strnrm (const t_quad* s);

    /// remove the leading blank and tab and return a new string
    /// @param s the original string
    /// @return a new clean string
    static t_quad* stripl (const char* s);

    /// remove the leading separators and return a new string
    /// @param s   the original string
    /// @param sep the character separators
    /// @return a new clean string
    static t_quad* stripl (const char* s, const char* sep);
    
    /// remove the leading blank and tab and return a new string
    /// @param s the original string
    /// @return a new clean string
    static t_quad* stripl (const t_quad* s);

    /// remove the leading separators and return a new string
    /// @param s   the original string
    /// @param sep the character separators
    /// @return a new clean string
    static t_quad* stripl (const t_quad* s, const t_quad* sep);
    
    /// remove the trailing blank and tab and return a new string
    /// @param s the original string
    /// @return a new clean string
    static t_quad* stripr (const char* s);
    
    /// remove the trailing separators and return a new string
    /// @param s   the original string
    /// @param sep the space characters
    /// @return a new clean string
    static t_quad* stripr (const char* s, const char* sep);

    /// remove the trailing blank and tab and return a new string
    /// @param s the original string
    /// @return a new clean string
    static t_quad* stripr (const t_quad* s);

    /// remove the trailing separators and return a new string
    /// @param s   the original string
    /// @param sep the space characters
    /// @return a new clean string
    static t_quad* stripr (const t_quad* s, const t_quad* sep);
    
    /// reduce the string into a small form
    /// @param s the original string
    /// @return a new clean string
    static t_quad* redex (const t_quad* s);

    /// remove the quote and return a new string
    /// @param s the original string
    /// @return a new clean string
    static t_quad* rmquote (const t_quad* s);

    /// convert the string to lower case
    /// @param s the original string
    /// @return a new lower case string
    static t_quad* tolower (const char* s);

    /// convert the string to lower case
    /// @param s the original string
    /// @return a new lower case string
    static t_quad* tolower (const t_quad* s);

    /// convert the string to upper case
    /// @param s the original string
    /// @return a new upper case string
    static t_quad* toupper (const char* s);

    /// convert the string to upper case
    /// @param s the original string
    /// @return a new upper case string
    static t_quad* toupper (const t_quad* s);

    /// @return true if the unicode character is a lower character
    static bool islower (const t_quad code);

    /// @return true if the unicode character is an upper character
    static bool isupper (const t_quad code);

    /// @return true if the unicode character is a letter
    static bool isletter (const t_quad code);

    /// @return true if the unicode character is a digit
    static bool isdigit (const t_quad code);

    /// @return true if the unicode character is a combining alphanumeric
    static bool iscan (const t_quad code);

    /// @return true if the unicode character is alpha
    static bool isalpha (const t_quad code);

    /// @return true if the unicode character is a blank or tab
    static bool isblank (const t_quad code);

    /// @return true if the unicode character is an ascii character
    static bool isascii (const t_quad code);

    /// @return true if the unicode character is a bmp character
    static bool isbmp (const t_quad code);

    /// @return true if the unicode character is a latin character
    static bool islatin (const t_quad code);

    /// @return true if the unicode character is a bit code
    static bool isbit (const t_quad code);

    /// @return true if the unicode character is an hexadecimal character
    static bool ishexa (const t_quad code);

    /// @return true if the unicode character is an afnix character
    static bool isafnix (const t_quad code);

    /// @return true if the unicode character is a terminal character
    static bool isterm (const t_quad code);

    /// @return true if the unicode character is a word constituent
    static bool iswcc (const t_quad code);

    /// @return true if the unicode character is a non combining character
    static bool isncc (const t_quad code);

    /// @return the non-combining length of an unicode string
    static long ncclen (const t_quad* s);

    /// @return the hash value of a unicode string
    static long tohash (const t_quad* s);
    
    /// @return a unicode character encoding
    static char* encode (const Encoding::t_emod emod, const t_quad c);
    
    /// @return a string encoding of a string
    static char* encode (const Encoding::t_emod emod, const String& s);

    /// @return a string encoding of a unicode string
    static char* encode (const Encoding::t_emod emod, const t_quad* s);
    
    /// @return a string encoding of a buffer by size
    static char* encode (const Encoding::t_emod emod, const t_quad* s, 
			 const long size);

    /// @return true if a unicode buffer is valid
    static bool valid (const Encoding::t_emod emod, const char* s, 
		       const long size);
    
    /// decode a unicode buffer into a quad
    /// @param buf the utf8 buffer to decode
    static t_quad decode (const char* buf);
    
    /// @return a unicode string by decoding a character buffer
    static t_quad* decode (const Encoding::t_emod emod, const char* s);
    
    /// @return a unicode string by decoding a character buffer by size
    static t_quad* decode (const Encoding::t_emod emod, const char* s, 
			   const long size);
  };
}

#endif
