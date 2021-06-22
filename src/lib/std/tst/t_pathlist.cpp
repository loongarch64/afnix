// ---------------------------------------------------------------------------
// - t_pathlist.cpp                                                          -
// - standard object library - path list class tester                        -
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

#include "System.hpp"
#include "Pathlist.hpp"

int main (int, char**) {
  using namespace afnix;

  // create a path list
  Pathlist plst (true);
  
  // check for local and length
  if (plst.length  () != 0)     return 1;
  if (plst.getlsch () == false) return 1;

  // check for the makefile
  if (plst.isfile ("Makefile") == false) return 1;

  // disable the local seach and check again
  plst.setlsch (false);
  if (plst.getlsch () == true)          return 1;
  if (plst.isfile ("Makefile") == true) return 1;

  // add a component for search
  plst.addpath (".");
  if (plst.length  () != 1)              return 1;
  if (plst.isfile ("Makefile") == false) return 1;

  // finally - no failure
  return 0;
}
