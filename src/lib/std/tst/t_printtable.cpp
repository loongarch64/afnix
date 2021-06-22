// ---------------------------------------------------------------------------
// - t_printtable.cpp                                                        -
// - standard object library - printtable class tester                       -
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

#include "PrintTable.hpp"

int main (int, char**) {
  using namespace afnix;

  // the property name and value
  String name = "afnix";
  String pval = "programming language";

  // create a new print table
  PrintTable ptbl (2);
  if (ptbl.repr () != "PrintTable") return 1;

  // check rows and columns
  if (ptbl.getrows () != 0L) return 1;
  if (ptbl.getcols () != 2L) return 1;

  // set the head and check
  ptbl.sethead (0, "name");
  ptbl.sethead (1, "value");
  if (ptbl.gethead (0) != "name")  return 1;
  if (ptbl.gethead (1) != "value") return 1;

  // add a new row
  long row = ptbl.add ();
  if (row != 0L) return 1;
  if (ptbl.getrows () != 1L) return 1;

  // set the row value
  ptbl.set (row, 0, name);
  ptbl.set (row, 1, pval);
  if (ptbl.get (row, 0) != name) return 1;
  if (ptbl.get (row, 1) != pval) return 1;

  // add a 2 column
  ptbl.addcols (2);
  if (ptbl.getrows () != 1L) return 1;
  if (ptbl.getcols () != 4L) return 1;

  if (ptbl.get (row, 0) != name) return 1;
  if (ptbl.get (row, 1) != pval) return 1;
  if (ptbl.get (row, 2) != "")   return 1;
  if (ptbl.get (row, 3) != "")   return 1;

  // ok - everything is fine
  return 0;
}
