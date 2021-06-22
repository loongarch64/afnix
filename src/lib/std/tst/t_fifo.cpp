// ---------------------------------------------------------------------------
// - t_fifo.cpp                                                              -
// - standard object library - fifo class tester                             -
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

#include "Fifo.hpp"

int main (int, char**) {
  using namespace afnix;

  // create a new fifo
  Fifo fifo (2);
  String* hello  = new String ("hello");
  String* world  = new String ("world");

  Object::iref (hello);
  Object::iref (world);

  // check size and predicates
  if (fifo.getsize () != 2)     return 1;
  if (fifo.empty   () == false) return 1;
  if (fifo.full    () == true)  return 1;

  // push our favorite message
  fifo.push (hello);
  fifo.push (world);

  // check the predicate again
  if (fifo.empty   () == true)  return 1;
  if (fifo.full    () == false) return 1;

  // pop and check
  String* sobj = dynamic_cast <String*> (fifo.pop ());
  if (*sobj != *world) return 1;
  sobj = dynamic_cast <String*> (fifo.pop ());
  if (*sobj != *hello) return 1;

  // reset and check
  fifo.reset ();
  if (fifo.empty   () == false) return 1;
  if (fifo.full    () == true)  return 1;

  // push again
  fifo.push (hello);
  fifo.push (world);
  sobj = dynamic_cast <String*> (fifo.get (0));
  if (*sobj != *world) return 1;
  sobj = dynamic_cast <String*> (fifo.get (1));
  if (*sobj != *hello) return 1;

  // resize and check
  fifo.resize (100);
  if (fifo.getsize () != 100) return 1;
  if (fifo.full () == true)   return 1;

  fifo.resize (1);
  if (fifo.getsize () != 1)   return 1;
  if (fifo.full () == false)  return 1;

  sobj = dynamic_cast <String*> (fifo.get (0));
  if (*sobj != *world) return 1;

  fifo.push (hello);
  sobj = dynamic_cast <String*> (fifo.get (0));
  if (*sobj != *hello) return 1;

  // ready to clean
  Object::dref (hello);
  Object::dref (world);

  // ok - everything is fine
  return 0;
}
