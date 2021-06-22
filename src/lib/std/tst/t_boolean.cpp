// ---------------------------------------------------------------------------
// - t_boolean.cpp                                                           -
// - standard object library - boolean class tester module                   -
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

#include "Boolean.hpp"
#include "InputOutput.hpp"

int main (int, char**) {
  using namespace afnix;

  // create a default boolean
  Boolean b1;

  // check for initialized value at false
  if (b1 != false) return 1;

  // create another boolean and compare
  Boolean b2 = true;
  if (b1 == b2) return 1;
  
  // check for assign
  b1 = b2;
  if (b1 != b2) return 1;

  // check serialization
  InputOutput io;
  b2.serialize (io);
  auto bs = dynamic_cast<Boolean*> (Serial::deserialize (io));
  if (bs == nullptr) return 1;
  if (b2 != *bs) return 1;
  delete bs;
  
  // we are done
  return 0;
}
