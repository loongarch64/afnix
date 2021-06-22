// ---------------------------------------------------------------------------
// - t_cursor.cpp                                                            -
// - standard object library - string buffer class tester                    -
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

#include "Cursor.hpp"

int main (int, char**) {
  using namespace afnix;

  // create a default cursor
  Cursor* cursor = new Cursor;

  // check for length equal to 0
  if (cursor->length () != 0) return 1;
  if (cursor->getabs () != 0) return 1;
  if (cursor->getrel () != 0) return 1;
  if (cursor->getrem () != 0) return 1;

  // add the magic string into it
  String magic = "hello world";
  cursor->Strbuf::add (magic);

  // check for the length again
  if (cursor->length () != magic.length ()) return 1;
  if (cursor->getrel () != magic.length ()) return 1;
  // check the result string
  String value = cursor->tostring ();
  if (value != magic) return 1;

  // move left and check position
  if (cursor->movel  () == false) return 1;
  if (cursor->getrel () != (magic.length () - 1)) return 1;

  // move right and check position
  if (cursor->mover  () == false) return 1;
  if (cursor->getrel () != magic.length ()) return 1;
  if (cursor->mover  () == true)  return 1;

  // finally - no failure
  delete cursor;
  return 0;
}
