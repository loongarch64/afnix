// ---------------------------------------------------------------------------
// - t_plist.cpp                                                             -
// - standard object library - property list class tester                    -
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

#include "Plist.hpp"

int main (int, char**) {
  using namespace afnix;

  // the property name and value
  String name = "afnix";
  String pval = "programming language";

  // create a new plist
  Plist plist;
  
  // insert a property
  plist.add (name, pval);
  if (plist.length () != 1) return 1;

  // check existence
  if (plist.exists  (name)  == false) return 1;
  if (plist.exists  (name, pval) == false) return 1;
  if (plist.getpval (name) != pval)  return 1;

  // get the property
  Property* prop = plist.lookup (name);
  if (prop == nullptr) return 1;
  if (prop->getname () != name) return 1;
  if (prop->getpval () != pval) return 1;

  // reset everything and check
  plist.reset ();
  if (plist.length () != 0) return 1;
  
  // ok - everything is fine
  return 0;
}
