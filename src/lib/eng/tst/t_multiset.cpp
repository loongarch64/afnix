// ---------------------------------------------------------------------------
// - t_multiset.cpp                                                          -
// - afnix engine - multiset class tester module                             -
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

#include "Multiset.hpp"

int main (int, char**) {
  using namespace afnix;

  // create a multiset
  Multiset mset;
  if (mset.repr () != "Multiset") return 1;
  if (mset.getparent () != nullptr)   return 1;

  // add some objects
  mset.Nameset::bind ("hello", (Object*) nullptr);
  if (mset.Nameset::exists ("hello") == false) return 1;
  if (mset.Nameset::find ("hello") != nullptr)    return 1;

  // remove and check again
  mset.Nameset::remove ("hello");
  if (mset.Nameset::exists ("hello") == true) return 1;

  // test now with quarks
  String hello = "hello";
  long   quark = hello.toquark ();

  // add some objects
  mset.bind (quark, (Object*) nullptr);
  if (mset.exists (quark) == false) return 1;
  if (mset.find (quark) != nullptr)    return 1;

  // remove and check again
  mset.remove (quark);
  if (mset.exists (quark) == true) return 1;

  // success
  return 0;
}
