// ---------------------------------------------------------------------------
// - t_localset.cpp                                                          -
// - afnix engine - local set class tester module                            -
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

#include "Localset.hpp"

int main (int, char**) {
  using namespace afnix;

  // create a local set
  Localset lset;
  if (lset.repr () != "Localset") return 1;
  if (lset.getparent () != nullptr)   return 1;

  // add some objects
  lset.Nameset::bind ("hello", (Object*) nullptr);
  if (lset.Nameset::exists ("hello") == false) return 1;
  if (lset.Nameset::find ("hello") != nullptr)    return 1;

  // remove and check again
  lset.Nameset::remove ("hello");
  if (lset.Nameset::exists ("hello") == true) return 1;

  // test now with quarks
  String hello = "hello";
  long   quark = hello.toquark ();

  // add some objects
  lset.bind (quark, (Object*) nullptr);
  if (lset.exists (quark) == false) return 1;
  if (lset.find (quark) != nullptr)    return 1;

  // remove and check again
  lset.remove (quark);
  if (lset.exists (quark) == true) return 1;

  // success
  return 0;
}
