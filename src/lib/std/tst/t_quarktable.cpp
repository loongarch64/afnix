// ---------------------------------------------------------------------------
// - t_quarktable.cpp                                                        -
// - standard object library - quark table class tester                      -
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

#include "QuarkTable.hpp"

int main (int, char**) {
  using namespace afnix;

  QuarkTable* qtable = new QuarkTable (2);
  String*     hello  = new String ("hello");
  String*     world  = new String ("world");
  String*     sfo    = new String ("from sfo");
  Object*     object = nullptr;
  String*     stzobj = nullptr;

  // insert out favorite message
  qtable->add (hello->toquark (), Object::iref (hello));
  if (qtable->length  ()  != 1)       return 1;
  if (qtable->getname (0) != "hello") return 1;  
  if (qtable->getobj  (0) != hello)   return 1;
  
  // add more and check
  qtable->add (world->toquark (), Object::iref (world));
  qtable->add (sfo->toquark   (), Object::iref (sfo));
  if (qtable->length () != 3) return 1;
  
  // check for our keys
  if (qtable->exists (hello->toquark ()) == false) return 1;
  if (qtable->exists (world->toquark ()) == false) return 1;
  if (qtable->exists (sfo->toquark ())   == false) return 1;

  // get the first key
  object = qtable->get (hello->toquark ());
  stzobj = dynamic_cast <String*> (object);

  // check for string
  if (stzobj  == nullptr)    return 1;
  if (*stzobj != "hello") return 1;

  // get the second key
  object = qtable->get (world->toquark ());
  stzobj = dynamic_cast <String*> (object);
  if (stzobj  == nullptr)    return 1;
  if (*stzobj != "world") return 1;

  // remove a key
  qtable->remove (hello->toquark ());
  if (qtable->exists (hello->toquark ()) == true) return 1;

  // delete everything
  delete qtable;
  Object::dref (hello);
  Object::dref (world);
  Object::dref (sfo);
  return 0;
}
