// ---------------------------------------------------------------------------
// - t_byte.cpp                                                              -
// - standard object library - byte class test module                        -
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

#include "Byte.hpp"
#include "InputOutput.hpp"

int main (int, char**) {
  using namespace afnix;

  // create a default byte and check for value
  Byte b1;
  if (b1.isnil () == false) return 1;
  if (b1.iseos () == true)  return 1;

  // create a byte from a character
  Byte b2 ('a');
  if (b2 != 'a') return 1;

  // check byte and quad
  if (b2.tobyte () != (t_byte) 'a') return 1;
  if (b2.toquad () != (t_quad) 'a') return 1;

  // move up and check again
  b2++;
  if (b2.tobyte () != (t_byte) 'b') return 1;
  if (b2.toquad () != (t_quad) 'b') return 1;
  // move down and check again
  b2--;
  if (b2.tobyte () != (t_byte) 'a') return 1;
  if (b2.toquad () != (t_quad) 'a') return 1;

  // check serialization
  InputOutput io;
  b2.serialize (io);
  auto bs = dynamic_cast<Byte*> (Serial::deserialize (io));
  if (bs == nullptr) return 1;
  if (b2 != *bs) return 1;
  delete bs;
  
  // everything is fine
  return 0;
}
