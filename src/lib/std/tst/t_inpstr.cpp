// ---------------------------------------------------------------------------
// - t_inpstr.cpp                                                            -
// - standard object library - input string class tester                     -
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

#include "InputString.hpp"

int main (int, char**) {
  using namespace afnix;

  // create a default input string
  Buffer sbuf = "hello";
  long   len  = sbuf.length ();
  InputString is ("hello");
  if (is.buflen () !=  len) return 1;

  // read each character
  for (long i = 0; i < len; i++) 
    if (is.read () != sbuf.read ()) return 1;

  // everything is fine
  return 0;
}
