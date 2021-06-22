// ---------------------------------------------------------------------------
// - t_system.cpp                                                            -
// - standard object library - system class tester                           -
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

#include "System.hpp"
#include "Exception.hpp"

int main (int, char**) {
  using namespace afnix;

  // check for unique id
  if (System::uniqid () == System::uniqid ()) return 1;

  // check for file name extension removal
  String name = System::rmext ("hello.als");
  if (name != "hello") return 1;
  
  // join back the names
  if (System::join (name,"world") != "hello/world") return 1;

  // check if we can daemonize
  if (System::daemon () == false) return 1;

  // normally we should exit with status 0
  System::exit (0);

  // if we are here something went wrong
  return 1;
}
