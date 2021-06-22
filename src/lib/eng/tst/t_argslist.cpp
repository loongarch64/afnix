// ---------------------------------------------------------------------------
// - t_argslist.cpp                                                          -
// - afnix engine - argument list class tester module                        -
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

#include "ArgsList.hpp"

int main (int, char**) {
  using namespace afnix;

  // create a new argument list
  ArgsList argl;
  if (argl.length () != 0) return 1;

  // add quarks and check
  argl.add (0, false);
  argl.add (1, true);

  if (argl.length () != 2)      return 1;
  if (argl.exists (0) == false) return 1;
  if (argl.exists (1) == false) return 1;
  if (argl.exists (2) == true)  return 1;

  long id0 = argl.find (0);
  if (argl.getquark (id0) != 0) return 1;
  if (argl.getconst (id0) != false) return 1;

  long id1 = argl.find (1);
  if (argl.getquark (id1) != 1) return 1;
  if (argl.getconst (id1) != true) return 1;
  
  long id2 = argl.find (2);
  if (id2 != -1) return 1;

  // success
  return 0;
}
