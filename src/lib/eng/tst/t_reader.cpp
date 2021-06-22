// ---------------------------------------------------------------------------
// - t_reader.cpp                                                            -
// - afnix engine - reader tester module                                     -
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

#include "Reader.hpp"

int main (int, char**) {
  using namespace afnix;

  // create a reader by string
  Reader rd ("assert nil nil");
  if (rd.repr () != "Reader") return 1;

  // parse a form
  Form* form = rd.parse ();
  if (form == nullptr) return 1;
  Object::cref (form);

  // check for more
  if (rd.parse () != nullptr) return 1;

  // success
  return 0;
}
