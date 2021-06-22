// ---------------------------------------------------------------------------
// - t_strbuf.cpp                                                            -
// - standard object library - string buffer class tester                    -
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

#include "Strbuf.hpp"

int main (int, char**) {
  using namespace afnix;

  // create a default buffer
  Strbuf* buffer = new Strbuf;

  // check for length equal to 0
  if (buffer->length () != 0) return 1;

  // add the magic string into it
  String magic = "hello world";
  buffer->add (magic);

  // check for the length again
  if (buffer->length () != magic.length ()) return 1;
  // check the result string
  String value = buffer->tostring ();
  if (value != magic) return 1;

  // reset the buffer and check string and size
  buffer->reset ();
  if (buffer->length () != 0)   return 1;

  // finally - no failure
  delete buffer;
  return 0;
}
