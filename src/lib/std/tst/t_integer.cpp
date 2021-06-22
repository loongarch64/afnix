// ---------------------------------------------------------------------------
// - t_integer.cpp                                                           -
// - standard object library - integer class tester module                   -
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

#include "Integer.hpp"
#include "InputOutput.hpp"

int main (int, char**) {
  using namespace afnix;

  // create a default null integer
  Integer i1;

  // check for initialized value at 0
  if ((i1 == 0)    == false) return 1;
  if (i1.iszero () == false) return 1;
  if (i1.iseven () == false) return 1;
  if (i1.isodd  () == true)  return 1;

  // check for conversion
  Integer* p = new Integer (1);
  if (p->tostring() != "1") return 1;
  delete p;

  // create another integer and compare
  Integer i2 = 1;
  if (i1 == i2) return 1;
  if (i2 <  i1) return 1;
  if (i2 <= i1) return 1;
  if (i1 >  i2) return 1;
  if (i1 >= i2) return 1;

  // check for assign
  i1 = i2;
  if (i1 != i2) return 1;

  // check for add
  if ((i1 + i2) != 2) return 1;
  if ((i1 + 2)  != 3) return 1;

  // check for sub
  if ((i1 - i2) != 0)  return 1;
  if ((i1 - 2)  != -1) return 1;

  // check for mul
  if ((i1 * i2) != 1) return 1;
  if ((i2 * 2)  != 2) return 1;

  // check for div
  if ((i1 / i2) != 1) return 1;
  if ((i2 / 2)  != 0) return 1;

  // check big number
  Integer i3 = 0x1000000000LL;
  Integer i4 = String ("0x1000000000");
  if (i3 != i4) return 1;
  if (i3.tostring () != "68719476736") return 1;

  // check serialization
  InputOutput io;
  i3.serialize (io);
  auto is = dynamic_cast<Integer*> (Serial::deserialize (io));
  if (is == nullptr) return 1;
  if (i3 != *is) return 1;
  delete is;
  
  // we are done
  return 0;
}
