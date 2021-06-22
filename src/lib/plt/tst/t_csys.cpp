// ---------------------------------------------------------------------------
// - t_csys.cpp                                                              -
// - standard platform library - system function tester                      -
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

#include "csys.hpp"

int main (int, char**) {
  using namespace afnix;

  // check the uniq id
  if (c_uniqid () == c_uniqid ()) return 1;

  // try to get a temporary file name
  if (c_tempnam ("afnix") == nullptr) return 1;

  // check if we can daemonize
  if (c_daemon () == false) return 1;

  // check if we can call exit
  c_exit (0);

  // we should never be here
  return 1;
}
