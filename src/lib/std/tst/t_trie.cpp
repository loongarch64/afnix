// ---------------------------------------------------------------------------
// - t_trie.cpp                                                              -
// - standard object library - trie class tester                             -
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

#include "Trie.hpp"

int main (int, char**) {
  using namespace afnix;

  // create a new trie
  Trie trie;
  if (trie.length () != 0) return 1;
  if (trie.count  () != 0) return 1;
  String* hello = new String ("hello");
  String* world = new String ("world");
  
  // insert out favorite message
  trie.add (*hello, hello);
  trie.add (*world, world);

  // check trie size
  if (trie.length () != 2) return 1;
  if (trie.count  () != 2) return 1;
  // check count by name
  if (trie.count (*hello) != 1) return 1;
  if (trie.count (*world) != 1) return 1;
  // check index by name
  if (trie.index (*hello) != 0) return 1;
  if (trie.index (*world) != 1) return 1;
  // check existence
  if (trie.exists (*hello) == false) return 1;
  if (trie.exists (*world) == false) return 1;
  // check marking
  trie.mark (*hello);
  if (trie.exists (*hello) == false) return 1;
  if (trie.length () != 2) return 1;
  if (trie.count  () != 3) return 1;
  if (trie.count (*hello) != 2) return 1;
  trie.mark (*world);
  if (trie.exists (*world) == false) return 1;
  if (trie.length () != 2) return 1;
  if (trie.count  () != 4) return 1;
  if (trie.count (*world) != 2) return 1;

  // add extra marking
  trie.mark ("hell");
  if (trie.exists ("hell") == false) return 1;
  if (trie.length () != 3) return 1;
  if (trie.count  () != 5) return 1;
  if (trie.index  ("hell") != 2) return 1;

  trie.mark ("word");
  if (trie.exists ("word") == false) return 1;
  if (trie.length () != 4) return 1;
  if (trie.count  () != 6) return 1;
  if (trie.index  ("word") != 3) return 1;

  // check the string vector
  Strvec* sv = trie.tonames ();
  if (sv == nullptr)         return 1;
  if (sv->length () != 4) return 1;
  if (sv->exists (*hello) == false) return 1;
  if (sv->exists (*world) == false) return 1;
  if (sv->exists ("hell") == false) return 1;
  if (sv->exists ("word") == false) return 1;
  delete sv;
  // check non existence
  if (trie.exists ("afnix") == true) return 1;
  if (trie.count  ("afnix") != 0L)   return 1;
  if (trie.count  ("hel")   != 0L)   return 1;
  if (trie.count  ("hell")  != 1L)   return 1;

  // ok - everything is fine
  return 0;
}
