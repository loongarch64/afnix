// ---------------------------------------------------------------------------
// - t_globalset.cpp                                                         -
// - afnix engine - global set class test module                             -
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

#include "Globalset.hpp"

int main (int, char**) {
  using namespace afnix;

  // create a global set
  Globalset gset;
  if (gset.repr () != "Globalset") return 1;
  if (gset.getparent () != nullptr)   return 1;

  // add some objects
  gset.symcst ("hello", nullptr);
  if (gset.Nameset::exists ("hello") == false) return 1;
  if (gset.Nameset::find   ("hello") == nullptr)  return 1;

  // remove and check again
  gset.Nameset::remove ("hello");
  if (gset.Nameset::exists ("hello") == true) return 1;

  // test now with quarks
  String hello = "hello";
  long   quark = hello.toquark ();

  // add some objects
  gset.symdef (quark, nullptr);
  if (gset.exists (quark) == false) return 1;
  if (gset.find   (quark) == nullptr)  return 1;

  // remove and check again
  gset.remove (quark);
  if (gset.exists (quark) == true) return 1;

  // success
  return 0;
}
