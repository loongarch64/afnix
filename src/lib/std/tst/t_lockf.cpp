// ---------------------------------------------------------------------------
// - t_lockf.cpp                                                             -
// - standard object library - lock file class tester                        -
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

#include "Lockf.hpp"
#include "System.hpp"

int main (int, char**) {
  using namespace afnix;

  // create a lock file
  static const String path = "#t_lockf";
  Lockf lock (path);

  // check representatiom
  if (lock.repr () != "Lockf") return 1;

  // lock for reading
  try {
    lock.rdlock ();
    lock.unlock ();
    lock.wrlock ();
    lock.unlock ();
  } catch (...) {
    System::rmfile (path);
    return 1;
  }
  System::rmfile (path);

  // everything is fine
  return 0;
}
