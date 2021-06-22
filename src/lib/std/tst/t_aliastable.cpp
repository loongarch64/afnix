// ---------------------------------------------------------------------------
// - t_aliastable.cpp                                                        -
// - standard object library - alias table class tester                      -
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

#include "AliasTable.hpp"

int main (int, char**) {
  using namespace afnix;

  // the property name and value
  String name = "afnix";
  String anam = "aws";
  String pval = "programming language";

  // create an alias table
  AliasTable atbl;
  if (atbl.empty () == false) return 1;
  atbl.add (name, anam);
  if (atbl.empty () == true)  return 1;
  
  // check existence
  if (atbl.exists  (name) == false) return 1;
  if (atbl.isalias (anam) == false) return 1;

  // check mapping
  if (atbl.map   (name) != anam) return 1;
  if (atbl.remap (anam) != name) return 1;

  // create a new plist
  Plist plst;  
  plst.add (name, pval);
  
  // map the plist
  Plist alst = atbl.map (plst);
  if (alst.length () != 1) return 1;
  
  // check existence
  if (alst.exists (anam) == false) return 1;
  if (alst.exists (anam, pval) == false) return 1;
  
  // ok - everything is fine
  return 0;
}
