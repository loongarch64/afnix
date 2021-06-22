// ---------------------------------------------------------------------------
// - t_property.cpp                                                          -
// - standard object library - property class tester                         -
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

#include "Property.hpp"

int main (int, char**) {
  using namespace afnix;

  // create a new property
  Property prop;

  // check name and value
  prop.setname ("afnix");
  if (prop.getname () != "afnix")   return 1;
  prop.setpval (String ("language"));
  if (prop.getpval () != "language") return 1;

  // check pair at once
  prop.set ("hello", String ("world"));
  if (prop.getname () != "hello")   return 1;
  if (prop.getpval () != "world")   return 1;

  // ok - everything is fine
  return 0;
}
