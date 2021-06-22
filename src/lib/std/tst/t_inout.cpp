// ---------------------------------------------------------------------------
// - t_inout.cpp                                                             -
// - standard object library - input-output stream class tester              -
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

#include "InputOutput.hpp"

int main (int, char**) {
  using namespace afnix;

  // create a default input stream
  Buffer sbuf = "hello";
  long   slen = sbuf.length ();
  InputOutput io (String ("hello"));
  if (io.buflen () != slen) return 1;

  // read each character
  for (long i = 0; i < slen; i++) {
    if (io.read () != sbuf.read ()) return 1;
  }
  if (io.buflen   () != 0) return 1;
  if (sbuf.length () != 0) return 1;

  // write a new string and reset buffer
  io.write ("hello");
  sbuf.add ("hello");
  if (io.buflen ()   != slen) return 1;
  if (sbuf.length () != slen) return 1;

  // read each character
  for (long i = 0; i < slen; i++) {
    if (io.read () != sbuf.read ()) return 1;
  }
  if (io.buflen () != 0)       return 1;
  if (io.iseos  ()  == false)  return 1;

  // flush and check
  io.flush ();
  if (io.iseos  ()  == false)  return 1;
  if (io.buflen () != 0)       return 1;

  // everything is fine
  return 0;
}
