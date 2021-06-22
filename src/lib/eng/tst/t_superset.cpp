// ---------------------------------------------------------------------------
// - t_superset.cpp                                                         -
// - afnix engine - superset class test module                             -
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

#include "Superset.hpp"

int main (int, char**) {
  using namespace afnix;

  // create a superset
  Superset sset;
  if (sset.repr () != "Superset") return 1;
  if (sset.getparent () != nullptr)   return 1;

  // add some objects
  sset.symcst ("hello", nullptr);
  if (sset.Nameset::exists ("hello") == false) return 1;
  if (sset.Nameset::find   ("hello") == nullptr)  return 1;

  // remove and check again
  sset.Nameset::remove ("hello");
  if (sset.Nameset::exists ("hello") == true) return 1;

  // test now with quarks
  String hello = "hello";
  long   quark = hello.toquark ();

  // add some objects
  sset.symdef (quark, nullptr);
  if (sset.exists (quark) == false) return 1;
  if (sset.find   (quark) == nullptr)  return 1;

  // remove and check again
  sset.remove (quark);
  if (sset.exists (quark) == true) return 1;

  // success
  return 0;
}
