// ---------------------------------------------------------------------------
// - t_return.cpp                                                            -
// - afnix engine - return object tester module                              -
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

#include "Return.hpp"

int main (int, char**) {
  using namespace afnix;

  // create a default return
  Return rtn;
  if (rtn.repr () != "Return") return 1;

  // check the object
  if (rtn.getobj () != nullptr) return 1;

  // success
  return 0;
}
