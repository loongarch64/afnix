// ---------------------------------------------------------------------------
// - t_set.cpp                                                               -
// - standard object library - set class tester                              -
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

#include "Set.hpp"
#include "String.hpp"

int main (int, char**) {
  using namespace afnix;

  // create a new set
  Set sobj;
  String* hello  = new String ("hello");
  String* world  = new String ("world");
  
  // insert out favorite message
  sobj.add (hello);
  sobj.add (world);
  // check set size
  if (sobj.length () != 2) return 1;

  // check existence
  if (sobj.exists (hello) == false) return 1;
  if (sobj.exists (world) == false) return 1;

  // create another set to merge
  Set as;
  as.add (new String ("bonjour"));
  as.add (hello);
  as.add (world);
  if (as.length () != 3) return 1;
  sobj.merge (as);
  if (sobj.length () != 3) return 1;
  
  // remove both common elements
  sobj.remove (hello);
  sobj.remove (world);
  if (sobj.length () != 1) return 1;
  if (as.length   () != 3) return 1;

  // create a subset and check cardinal
  Set* rss = as.getrss (2);
  if (rss->length () > 2) return 1;
  delete rss;

  // ok - everything is fine
  return 0;
}
