// ---------------------------------------------------------------------------
// - t_vector.cpp                                                            -
// - standard object library - vector class tester                           -
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

#include "Vector.hpp"
#include "String.hpp"
#include "Integer.hpp"

int main (int, char**) {
  using namespace afnix;

  // create a new vector
  Vector  vobj;
  String* hello  = new String ("hello");
  String* world  = new String ("world");
  
  // insert out favorite message
  vobj.add (hello);
  vobj.add (world);

  // check vector size
  if (vobj.length () != 2) return 1;

  // get first object
  Object* object = vobj.get (0);
  String* stzobj = dynamic_cast <String*> (object);

  // check for string
  if (stzobj  == nullptr)    return 1;
  if (*stzobj != "hello") return 1;

  // query next string
  object = vobj.get (1);
  stzobj = dynamic_cast <String*> (object);

  // check for string
  if (stzobj  == nullptr)    return 1;
  if (*stzobj != "world") return 1;

  // add a string in the middle
  vobj.add (1, new String ("new"));
  if (vobj.length () != 3) return 1;

  // check the new string
  object = vobj.get (1);
  stzobj = dynamic_cast <String*> (object);
  if (*stzobj != "new") return 1;

  // test the iterator
  for (auto s : vobj) {
    if (dynamic_cast <String*> (s) == nullptr) return 1;
  }
  // reset and fill with integer
  vobj.reset();
  long vsum = 0L;
  for (long k = 0; k < 10; k++) {
    vobj.add (new Integer(k));
    vsum += k;
  }
  if (vobj.length () != 10) return 1;
  long isum = 0L;
  for (auto iobj : vobj) {
    isum += dynamic_cast<Integer*>(iobj)->tolong();
  }
  if (isum != vsum) return 1;
  // ok - everything is fine
  return 0;
}
