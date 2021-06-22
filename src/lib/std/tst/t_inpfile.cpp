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

int main (int, char**) {
  using namespace afnix;

  // create an input mapped on ourself
  InputFile is ("t_inpfile.cpp");

  // get the file size
  long size = is.length ();

  // loop on character and check count
  long count = 0;
  while (is.iseos () == false) {
    is.read ();
    count++;
  }
  if (count != size) return 1;

  // everything is fine
  return 0;
}
