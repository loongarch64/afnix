// ---------------------------------------------------------------------------
// - t_inpmap.cpp                                                            -
// - standard object library - input mapped file class tester                -
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

#include "InputFile.hpp"
#include "InputMapped.hpp"

int main (int, char**) {
  using namespace afnix;

  // create a null mapped file
  InputMapped nm;
  if (nm.repr () != "InputMapped") return 1;

  // check the stream
  if (nm.valid () == false) return 1;
  if (nm.iseos () == true)  return 1;
  // check value and length
  if (nm.read  ()  != nilc)  return 1;
  if (nm.length () != 0)     return 1;

  // this source file name
  String name = "t_inpmap.cpp";

  // create an input file and an input file
  InputFile   fs (name);
  InputMapped ms (name);

  // check the size
  long msize = ms.length ();
  long fsize = fs.length ();
  if (msize != fsize) return 1;

  // loop on character and check count
  long count = 0;
  while ((ms.iseos () == false) && (fs.iseos () == false)) {
    if (ms.read () != fs.read ()) return 1;
    count++;
  }
  if (count != msize) return 1;
  if (count != fsize) return 1;

  // create a simple buffer
  Buffer buf (name);
  // map the buffer
  InputMapped mb (buf);

  // check the length
  long mlen = mb.length  ();
  long blen = buf.length ();
  if (mlen != blen) return 1;

  // loop on character and check count
  count = 0;
  while ((mb.iseos () == false) && (buf.empty () == false)) {
    if (mb.read () != buf.read ()) return 1;
    count++;
  }
  if (count != mlen) return 1;
  if (count != blen) return 1;
  if (buf.length () != 0) return 1;

  // everything is fine
  return 0;
}
