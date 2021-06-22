// ---------------------------------------------------------------------------
// - cstr.hpp                                                                -
// - standard platform library - c string function definitions               -
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

#ifndef  AFNIX_CSTR_HPP
#define  AFNIX_CSTR_HPP

#ifndef  AFNIX_CCNF_HPP
#include "ccnf.hpp"
#endif

namespace afnix {
  /// This file contains the functions that handle the null terminated
  /// c-string. The interface permit to count, copy and compare strings.
  /// The allocated string can be freed by the standard platform memory
  /// cleanup functions.
  /// @author amaury darsch

  /// get the size of the c string. The function is safe with null pointer.
  /// @param s the string to evaluate
  /// @return the string length or 0 if null pointer.
  long c_strlen (const char* s);

  /// compare two strings and returns true if they are equals. This function 
  /// is safe with null pointer.
  /// @param s1 the first string
  /// @param s2 the second string
  /// @return true if the string are equals or both null
  bool c_strcmp (const char* s1, const char* s2);
 
  /// compare two strings and returns true if they are equals for a number of
  /// characters. This function is safe with null pointer.
  /// @param s1 the first string
  /// @param s2 the second string
  /// @param size the number of characters to compare
  /// @return true if the string are equals or both null
  bool c_strncmp (const char* s1, const char* s2, const long size);
 
  /// compare two strings and returns true if s1 is less than s2.
  /// @param s1 the first string
  /// @param s2 the second string
  /// @return true if the string are equals or both null
  bool c_strlth (const char* s1, const char* s2);

  /// compare two strings and returns true if s1 is less equal than s2.
  /// @param s1 the first string
  /// @param s2 the second string
  /// @return true if the string are equals or both null
  bool c_strleq (const char* s1, const char* s2);
 
  /// duplicate a string. If the string is null or has a 0 length, the 
  /// function returns the null pointer. The delete operator cleans the 
  /// strings
  /// @param s the string to duplicate
  /// @return a copy of the string
  char* c_strdup (const char* s);
  
  /// create a string from a character.
  /// @param c the character to map to a string
  /// @return a copy of the string
  char* c_strmak (const char c);
  
  /// copy a string from a source to destination
  /// @param dst the destination string
  /// @param src the source string
  void c_strcpy (char* dst, const char* src);
  
  /// concatenate a string with another. The string must allocated enough.
  /// @param dst the destination string
  /// @param src the source string
  void c_strcat (char* dst, const char* src);

  /// truncate a string upto n characters
  /// @param src the source string
  /// @param max the maximum length
  char* c_strtrc (const char* src, const long max);

  /// find a string inside a string
  /// @param src the source haystack
  /// @param str the needle string
  const char* c_strstr (const char* src, const char* str);
  
  /// remove the leading blank and tab and return a new string
  /// @param the original string
  /// @return a new clean string
  char* c_rmlead (const char* s);
  
  /// remove the trailing blank and tab and return a new string
  /// @param the original string
  /// @return a new clean string
  char* c_rmtrail (const char* s);
  
  /// convert the string to upper case
  /// @param the original string
  /// @return a new upper case string
  char* c_toupper (const char* s);
  
  /// convert the string to lower case
  /// @param the original string
  /// @return a new lower case string
  char* c_tolower (const char* s);
}

#endif
