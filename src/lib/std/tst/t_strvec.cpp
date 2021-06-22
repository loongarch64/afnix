// ---------------------------------------------------------------------------
// - t_strvec.cpp                                                            -
// - standard object library - string vector class tester                    -
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

#include "Strvec.hpp"

int main (int, char**) {
  using namespace afnix;

  // create a new string vector
  Strvec vector;

  // insert out favorite message
  vector.add ("hello");
  vector.add ("world");

  // check vector size
  if (vector.length () != 2) return 1;

  // check elements
  if (vector.get (0) != "hello") return 1;
  if (vector.get (1) != "world") return 1;

  // check for existence
  if (vector.exists ("hello") == false) return 1;
  if (vector.exists ("world") == false) return 1;

  // check for split operation
  Strvec svec = Strvec::split ("hello:world", ":");
  if (svec.length () != 2) return 1;
  // get first object
  if (svec.get (0) != "hello") return 1;
  // get second object
  if (svec.get (1) != "world") return 1;

  // check for corner case - notice the two spaces
  Strvec ccsv = Strvec::split ("hello  world");
  if (ccsv.length () != 2) return 1;
  ccsv = Strvec::split ("hello::world", ":");
  if (ccsv.length () != 3) return 1;

  // check for concat
  if (svec.concat ()    != "helloworld")  return 1;
  if (svec.concat (' ') != "hello world") return 1;

  // success
  return 0;
}
