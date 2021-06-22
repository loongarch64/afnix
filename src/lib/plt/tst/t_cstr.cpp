// ---------------------------------------------------------------------------
// - t_cstr.cpp                                                              -
// - standard platform library - cstring tester module                       -
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

int main (int, char**) {
  using namespace afnix;

  // we use the famous string for our test
  const char* hello = "hello world";

  // test length
  if (c_strlen (0)   != 0)  return 1;
  if (c_strlen (hello) != 11) return 1;

  // test duplicate
  char* world = c_strdup (hello);
  if (c_strlen (world) != 11) return 1;

  // test compare
  if (c_strcmp (0,0)         != true)  return 1;
  if (c_strcmp (0,hello)     != false) return 1;
  if (c_strcmp (hello,0)     != false) return 1;
  if (c_strcmp (hello,world) != true)  return 1;

  // test remove leading blank
  char* data = c_rmlead ("  hello world");
  if (c_strcmp (hello,data) == false) return 1;
  delete [] data;

  // test remove trailing blank
  data = c_rmtrail ("hello world  ");
  if (c_strcmp (hello,data) == false) return 1;
  delete [] data;

  // test convert to upper case
  data = c_toupper ("hello world");
  if (c_strcmp ("HELLO WORLD",data) == false) return 1;
  delete [] data;

  // test convert to upper case
  data = c_tolower ("HELLO WORLD");
  if (c_strcmp (hello,data) == false) return 1;
  delete [] data;

  const char* src = "hello world";
  const char* hs = c_strstr (src, "hello");
  if (hs != src) return 1;
  const char* ws = c_strstr (src, "world");
  if (ws != &src[6]) return 1;
  const char* gs = c_strstr (src, "afnix");
  if (gs != nullptr) return 1;
  
  // here we are saved - what an accomplishment !!!
  return 0;
}
