// ---------------------------------------------------------------------------
// - t_buffer.cpp                                                            -
// - standard object library - buffer class tester                           -
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

#include "Ascii.hpp"
#include "Buffer.hpp"
#include "Exception.hpp"

int main (int, char**) {
  using namespace afnix;

  // try to create a buffer with negative size
  try {
    Buffer* wrong = new Buffer ((long) -1);
    delete wrong;
  }
  catch (const Exception& e) {
    // we should catch this exception
  } catch (...) {
    // if we are here, it is an error
    return 1;
  }

  // create a default buffer with 0 size
  Buffer* buffer = new Buffer ((long) 0);

  // check for length equal to 0
  if (buffer->length () != 0) return 1;

  // add the magic string into it
  const char* magic = "hello world";
  long        size  = Ascii::strlen (magic);
  for (long i = 0; i < size; i++) buffer->add (magic[i]);

  // check for the length again
  if (buffer->length () != size) return 1;
  // check the result string
  String value = buffer->tostring ();
  if (value != magic) return 1;

  // reset the buffer and check string and size
  buffer->reset ();
  if (buffer->length () != 0)   return 1;

  // add in full mode
  buffer->add (magic);
  // check for the length again
  if (buffer->length () != size) return 1;
  // check the result string
  value = buffer->tostring ();
  if (value != magic) return 1;

  // reset the buffer
  buffer->reset ();
  // add and check format
  buffer->add ("afnix");
  if (buffer->format () != "61666E6978") return 1;

  // reset the buffer
  buffer->reset ();
  // add a natural word
  buffer->add ((t_word) 0x1234U, false);
  if (buffer->length () != 2)  return 1;
  if (buffer->read () != (char) 0x12) return 1;
  if (buffer->read () != (char) 0x34) return 1;

  // reset the buffer
  buffer->reset ();
  // add a natural quad
  buffer->add ((t_quad) 0x12345678UL, false);
  if (buffer->length () != 4)  return 1;
  if (buffer->read () != (char) 0x12) return 1;
  if (buffer->read () != (char) 0x34) return 1;
  if (buffer->read () != (char) 0x56) return 1;
  if (buffer->read () != (char) 0x78) return 1;

  // reset the buffer
  buffer->reset ();
  // add a natural quad
  buffer->add ((t_octa) 0x123456789ABCDEF0ULL, false);
  if (buffer->length () != 8)  return 1;
  if (buffer->read () != (char) 0x12) return 1;
  if (buffer->read () != (char) 0x34) return 1;
  if (buffer->read () != (char) 0x56) return 1;
  if (buffer->read () != (char) 0x78) return 1;
  if (buffer->read () != (char) 0x9A) return 1;
  if (buffer->read () != (char) 0xBC) return 1;
  if (buffer->read () != (char) 0xDE) return 1;
  if (buffer->read () != (char) 0xF0) return 1;
  // reset the buffer and add a matching string
  buffer->reset ();
  buffer->add ("hello \nsuper world");
  Buffer* bbuf = buffer->cpbnds ("super ");
  if (bbuf->pop () != eolc) return 1;
  if (bbuf->tostring   () != "hello ") return 1;
  if (buffer->tostring () != "super world") return 1;
  // reset the buffer and test extract/collect
  buffer->reset ();
  buffer->add ("hello:world:");
  if (buffer->extract(0L, 5L).tostring () != "hello") return 1;
  if (buffer->collect(7L).tostring () != ":world:") return 1;
  if (buffer->tostring() != "hello") return 1;
  // finally - no failure
  delete bbuf;
  delete buffer;
  return 0;
}
