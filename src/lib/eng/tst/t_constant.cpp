// ---------------------------------------------------------------------------
// - t_constant.cpp                                                          -
// - afnix engine - constant class tester module                             -
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

#include "Constant.hpp"

int main (int, char**) {
  using namespace afnix;

  // create a constant
  String*   lobj = new String ("hello");
  Constant* cobj = new Constant (lobj); 

  // check the string value
  if (cobj->tostring () != *lobj) return 1;

  // success
  delete cobj;
  return 0;
}
