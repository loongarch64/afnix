// ---------------------------------------------------------------------------
// - t_fileinfo.cpp                                                          -
// - standard object library - file info class tester                        -
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

#include "FileInfo.hpp"

int main (int, char**) {
  using namespace afnix;

  // create a self file info object
  FileInfo info ("t_fileinfo.cpp");
  if (info.repr () != "FileInfo") return 1;

  // check the size
  if (info.length () != 1478) return 1;

  // everything is fine
  return 0;
}
