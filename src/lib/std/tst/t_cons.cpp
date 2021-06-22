// ---------------------------------------------------------------------------
// - t_cons.cpp                                                              -
// - standard object library - cons class tester module                      -
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

#include "Cons.hpp"
#include "Integer.hpp"

int main (int, char**) {
  using namespace afnix;

  // create a simple cons cell
  Cons cell;
  if (cell.getcar () != nullptr)  return 1;
  if (cell.isnil  () == false) return 1;
  if (cell.length () != 1)     return 1;

  // set the car and add
  cell.setcar (new Integer);
  cell.add    (new Integer);
  if (cell.length () != 2) return 1;

  // get the car and cadr and check
  if (dynamic_cast <Integer*> (cell.getcar  ()) == nullptr) return 1;
  if (dynamic_cast <Integer*> (cell.getcadr ()) == nullptr) return 1;

  // copy constructor and assignment operator
  Cons c1 = cell;
  if (c1.length () != 2) return 1;
  if (dynamic_cast <Integer*> (c1.getcar  ()) == nullptr) return 1;
  if (dynamic_cast <Integer*> (c1.getcadr ()) == nullptr) return 1;

  cell = c1;
  if (c1.length () != 2) return 1;
  if (dynamic_cast <Integer*> (cell.getcar  ()) == nullptr) return 1;
  if (dynamic_cast <Integer*> (cell.getcadr ()) == nullptr) return 1;
  
  // success
  return 0;
}
