// ---------------------------------------------------------------------------
// - t_symbol.cpp                                                            -
// - afnix engine - symbol class tester module                               -
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

#include "Symbol.hpp"

int main (int, char**) {
  using namespace afnix;

  // create a simple symbol
  Symbol sym ("hello");
  if (sym.repr () != "Symbol") return 1;

  // check for members
  if (sym.getconst  () != false)   return 1;
  if (sym.getobj    () != nullptr)    return 1;
  if (sym.toliteral () != "hello") return 1;

  // success
  return 0;
}
