// ---------------------------------------------------------------------------
// - t_hashtable.cpp                                                         -
// - standard object library - hash table class tester                       -
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

#include "HashTable.hpp"

int main (int, char**) {
  using namespace afnix;

  HashTable* htable = new HashTable (2L);
  String*    hello  = new String ("hello");
  String*    world  = new String ("world");
  String*    sfo    = new String ("from sfo");
  Object*    object = nullptr;
  String*    stzobj = nullptr;

  // insert out favorite message
  htable->add ("hello", hello);
  if (htable->length ()  != 1)       return 1;
  if (htable->getkey (0) != "hello") return 1;  
  if (htable->getobj (0) != hello)   return 1;

  // add more and check
  htable->add ("world",world);
  htable->add (*sfo, sfo);
  if (htable->length () != 3) return 1;

  // check for our keys
  if (htable->exists ("hello") == false) return 1;
  if (htable->exists ("world") == false) return 1;
  if (htable->exists (*sfo)    == false) return 1;

  // get the first key
  object = htable->get ("hello");
  stzobj = dynamic_cast <String*> (object);

  // check for string
  if (stzobj  == nullptr)    return 1;
  if (*stzobj != "hello") return 1;

  // get the second key
  object = htable->get ("world");
  stzobj = dynamic_cast <String*> (object);
  if (stzobj  == nullptr)    return 1;
  if (*stzobj != "world") return 1;

  // remove a key
  htable->remove ("hello");
  if (htable->exists ("hello") == true) return 1;

  // check a case insensitive hash table
  HashTable cih (true);
  cih.add ("world", world);
  if (cih.length () != 1) return 1;
  if (cih.exists ("World") == false) return 1;
  cih.add ("WoRlD", world);
  if (cih.length () != 1) return 1;
  if (cih.exists ("WORLD") == false) return 1;

  // delete everything
  delete htable;
  return 0;
}
