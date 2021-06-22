// ---------------------------------------------------------------------------
// - t_unimapper.cpp                                                         -
// - standard object library - unicode character mapper class tester         -
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

#include "Unimapper.hpp"

int main (int, char**) {
  using namespace afnix;
  
  // create a mapping table
  Unimapper* table  = new Unimapper (2);

  // insert nil character first
  table->add (0x00000000, 0xFFFFFFFF);
  if (table->length ()  != 1)          return 1;
  if (table->getkey (0) != 0x00000000) return 1;  
  if (table->getchr (0) != 0xFFFFFFFF)      return 1;
  
  // add more and check
  table->add (0x10000001, 'H');
  table->add (0x10000010, 'E');
  table->add (0x10000100, 'L');
  table->add (0x10001000, 'L');
  table->add (0x10010000, 'O');
  if (table->length () != 6) return 1;
  
  // check for our keys
  if (table->exists (0x00000000) == false) return 1;
  if (table->exists (0x10000001) == false) return 1;
  if (table->exists (0x10000010) == false) return 1;
  if (table->exists (0x10000100) == false) return 1;
  if (table->exists (0x10001000) == false) return 1;
  if (table->exists (0x10010000) == false) return 1;

  // get the elements
  if (table->get (0x10000001) != 'H') return 1;
  if (table->get (0x10000010) != 'E') return 1;
  if (table->get (0x10000100) != 'L') return 1;
  if (table->get (0x10001000) != 'L') return 1;
  if (table->get (0x10010000) != 'O') return 1;

  // remove a key
  table->remove (0x00000000);
  if (table->exists (0x00000000) == true) return 1;

  // delete everything
  delete table;
  return 0;
}
