// ---------------------------------------------------------------------------
// - t_unitabler.cpp                                                         -
// - standard object library - unicode table class tester                    -
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

#include "Unitabler.hpp"

int main (int, char**) {
  using namespace afnix;

  Unitabler* table  = new Unitabler (2);
  String*    hello  = new String ("hello");
  String*    world  = new String ("world");
  String*    sfo    = new String ("from lfpg");
  Object*    object = nullptr;
  String*    stzobj = nullptr;

  // insert out favorite message
  table->add (0x00000000, Object::iref (hello));
  if (table->length ()  != 1)          return 1;
  if (table->getkey (0) != 0x00000000) return 1;  
  if (table->getobj (0) != hello)      return 1;
  
  // add more and check
  table->add (0x10000001, Object::iref (world));
  table->add (0x11000002, Object::iref (sfo));
  if (table->length () != 3) return 1;
  
  // check for our keys
  if (table->exists (0x00000000) == false) return 1;
  if (table->exists (0x10000001) == false) return 1;
  if (table->exists (0x11000002) == false) return 1;

  // get the first element
  object = table->get (0x00000000);
  stzobj = dynamic_cast <String*> (object);

  // check for string
  if (stzobj  == nullptr)    return 1;
  if (*stzobj != "hello") return 1;

  // get the second element
  object = table->get (0x10000001);
  stzobj = dynamic_cast <String*> (object);
  if (stzobj  == nullptr)    return 1;
  if (*stzobj != "world") return 1;

  // remove a key
  table->remove (0x00000000);
  if (table->exists (0x00000000) == true) return 1;

  // delete everything
  delete table;
  Object::dref (hello);
  Object::dref (world);
  Object::dref (sfo);
  return 0;
}
