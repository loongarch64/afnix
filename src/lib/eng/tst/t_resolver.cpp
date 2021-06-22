// ---------------------------------------------------------------------------
// - t_resolver.cpp                                                          -
// - afnix engine - resolver tester module                                   -
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

#include "Resolver.hpp"

int main (int, char**) {
  using namespace afnix;

  // create a default resolver
  Resolver rsv;
  if (rsv.repr () != "Resolver") return 1;

  // success
  return 0;
}
