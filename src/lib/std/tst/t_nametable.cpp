// ---------------------------------------------------------------------------
// - t_nametable.cpp                                                         -
// - standard object library - name table class tester                       -
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

#include "NameTable.hpp"

int main (int, char**) {
  using namespace afnix;

  NameTable* ntable = new NameTable;
  String*    hello  = new String ("hello");
  String*    world  = new String ("world");
  Object*    object = nullptr;
  String*    stzobj = nullptr;

  // insert out favorite message
  ntable->add ("hello", hello);
  if (ntable->length   ()  != 1)      return 1;
  if (ntable->getname (0) != "hello") return 1;  
  if (ntable->getobj  (0) != hello)   return 1;

  // second key
  ntable->add ("world", world);
  if (ntable->length () != 2) return 1;

  // check for our keys
  if (ntable->exists ("hello") == false) return 1;
  if (ntable->exists ("world") == false) return 1;

  // get the first key
  object = ntable->get ("hello");
  stzobj = dynamic_cast <String*> (object);

  // check for stzobj
  if (stzobj  == nullptr)    return 1;
  if (*stzobj != "hello") return 1;

  // get the second key
  object = ntable->get ("world");
  stzobj = dynamic_cast <String*> (object);
  if (stzobj  == nullptr)    return 1;
  if (*stzobj != "world") return 1;

  // remove a key
  ntable->remove ("hello");
  if (ntable->exists ("hello") == true) return 1;

  // delete everything
  delete ntable;
  return 0;
}
