// ---------------------------------------------------------------------------
// - cstr.cxx                                                                -
// - standard platform library - c string function implementation            -
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
#include "cstr.hxx"

namespace afnix {

  // get the size of the c string. The function is safe with null pointer.

  long c_strlen (const char* s) {
    if (s == nullptr) return 0;
    long result = 0;
    while (*s++ != nilc) result++;
    return result;
  }

  // Compare two strings and returns true if they are equals. 

  bool c_strcmp (const char* s1, const char* s2) {
    // nullptr case first
    const char* ss1 = (s1 == nullptr) ? "" : s1;
    const char* ss2 = (s2 == nullptr) ? "" : s2;    
    // check first character for fast compare
    if (*ss1 != *ss2) return false;
    // normal compare now
    while (*ss1 != nilc) {
      if (*ss2 == nilc) break;
      if (*ss1++ != *ss2++) return false;
    }
    return (*ss1 == *ss2) ? true : false;
  }

  // compare two strings to n characters

  bool c_strncmp (const char* s1, const char* s2, const long size) {
    // nil case compare
    if (size == 0) return true;
    long len1 = c_strlen (s1);
    long len2 = c_strlen (s2);
    if ((len1 == 0) && (len2 == 0)) return false;
    if ((len1 == 0) && (len2 != 0)) return false;
    if ((len1 != 0) && (len2 == 0)) return false;
    // normal compare
    int status = strncmp (s1,s2,size);
    return (status == 0) ? true : false;
  }

  // compare two strings - less than operator
  bool c_strlth (const char* s1, const char* s2) {
    // nullptr case first
    const char* ss1 = (s1 == nullptr) ? "" : s1;
    const char* ss2 = (s2 == nullptr) ? "" : s2;
    // check first character for fast compare
    if (*ss1 >= *ss2) return false;
    // normal compare now
    while (*ss1 != nilc) {
      if (*ss2 == nilc)     return true;
      if (*ss1++ >= *ss2++) return true;
    }
    if ((*ss1 == nilc) && (*ss2 == nilc)) return true;
    return (*ss1 < *ss2) ? true : false;
  }

  // compare two strings - less equal operator

  bool c_strleq (const char* s1, const char* s2) {
    // nullptr case first
    const char* ss1 = (s1 == nullptr) ? "" : s1;
    const char* ss2 = (s2 == nullptr) ? "" : s2;
    // check first character for fast compare
    if (*ss1 > *ss2) return false;
    // normal compare now
    while (*ss1 != nilc) {
      if (*ss2 == nilc)    return true;
      if (*ss1++ > *ss2++) return true;
    }
    return (*ss1 <= *ss2) ? true : false;
  }

  // create a string from a character.

  char* c_strmak (const char c) {
    char* result = new char[2];
    result[0] = c;
    result[1] = nilc;
    return result;
  }

  // duplicate a string. If the string is null or has a 0 length, 
  // the function returns the null pointer.
  
  char* c_strdup (const char* s) {
    // check for null string if the length is null
    long len = c_strlen (s);
    // allocate a new string with new so delete can be used
    char* result = new char[len + 1];
    if (len > 0) {
      for (long i = 0; i < len + 1; i++) result[i] = s[i];
    }
    result[len] = nilc;
    return result;
  }

  // copy a string from a source to destination
  
  void c_strcpy (char* dst, const char* src) {
    // standard check as usual
    if (dst == nullptr) return;
    // get the length and copy
    long len = c_strlen (src);
    if (len > 0) {
      for (long i = 0; i < len; i++) dst[i] = src[i];
    }
    dst[len] = nilc;
  }

  // concatenate a string with another. The string must allocated enough.

  void c_strcat (char* dst, const char* src) {
    // standard check as usual
    if (dst == nullptr) return;
    // get length and position 
    long len = c_strlen (src);
    long pos = c_strlen (dst);
    if (len > 0) {
      for (long i = 0; i < len; i++) dst[pos + i] = src[i];
    }
    dst[pos+len] = nilc;
  }

  // truncate a string upto n characters

  char* c_strtrc (const char* src, const long max) {
    // standard check as usual
    long size = c_strlen (src);
    // do not change anything if the size is less than max
    if (size <= max) return c_strdup (src);
    // here we are bigger - so better control the allocation
    char* result = new char[max+1];
    for (long i = 0; i < max; i++) result[i] = src[i];
    result[max] = nilc;
    // here is the resule
    return result;
  }

  // find a string niside a string

  const char* c_strstr (const char* src, const char* str) {
    // check for nil
    if ((str == nullptr) || (*str == nilc)) return src;
    if ((src == nullptr) || (*src == nilc)) return nullptr;
    // compare at position
    if ((*src == *str) && (c_strstr (src+1,str+1) == src+1)) {
      return src;
    }
    return c_strstr (src+1, str);
  }
  
  // remove the leading blank from a string and return a new string

  char* c_rmlead (const char* s) {
    long len = c_strlen (s);
    if (len != 0) {
      // remove leading blank
      while ((*s != nilc) && ((*s == blkc) || (*s == tabc))) s++;
    }
    // now copy and return
    return c_strdup (s);
  }

  // remove the trailing blank from a string and return a new string

  char* c_rmtrail (const char* s) {
    long len = c_strlen (s);
    if (len == 0) return c_strdup (s);
    char* buffer = c_strdup (s);
    char* end    = buffer + len - 1;
    // remove trailing blank
    while ((end != s) && ((*end == blkc) || (*end == tabc))) *end-- = nilc;
    // now copy and return
    char* result = c_strdup (buffer);
    delete [] buffer;
    return result;    
  }

  // convert a string to upper case
  
  char* c_toupper (const char* s) {
    long len = c_strlen (s);
    if (len == 0) return c_strdup (s);
    char* result = new char[len + 1];
    for (long i = 0; i < len; i++) {
      char c = s[i];
      if ((c >= 'a') && (c <= 'z')) c = c - 'a' + 'A';
      result[i] = c;
    }
    result[len] = nilc;
    return result;
  }

  // convert a string to lower case
  
  char* c_tolower (const char* s) {
    long len = c_strlen (s);
    if (len == 0) return (char*) 0;
    char* result = new char[len + 1];
    for (long i = 0; i < len; i++) {
      char c = s[i];
      if ((c >= 'A') && (c <= 'Z')) c = c - 'A' + 'a';
      result[i] = c;
    }
    result[len] = nilc;
    return result;
  }
}
