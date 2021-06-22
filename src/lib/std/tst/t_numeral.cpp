// ---------------------------------------------------------------------------
// - t_numeral.cpp                                                           -
// - standard object library - numeral class tester module                   -
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

#include "Numeral.hpp"
#include "InputOutput.hpp"

int main (int, char**) {
  using namespace afnix;

  // create a default null numeral
  Numeral i1;

  // check for initialized value at 0
  if ((i1 == 0LL)    == false) return 1;
  if (i1.iszero () == false) return 1;
  if (i1.iseven () == false) return 1;
  if (i1.isodd  () == true)  return 1;

  // check for conversion
  Numeral* p = new Numeral (1LL);
  if (p->tostring() != "1") return 1;
  delete p;

  // create another numeral and compare
  Numeral i2 = 1LL;
  if (i1 == i2) return 1;
  if (i2 <  i1) return 1;
  if (i2 <= i1) return 1;
  if (i1 >  i2) return 1;
  if (i1 >= i2) return 1;

  // check for assign
  i1 = i2;
  if (i1 != i2) return 1;

  // check for add
  if ((i1 + i2) != 2LL) return 1;
  if ((i1 + 2LL)  != 3LL) return 1;

  // check for sub
  if ((i1 - i2) != 0LL)  return 1;
  if ((i1 - 2LL)  != -1LL) return 1;

  // check for mul
  if ((i1 * i2) != 1LL) return 1;
  if ((i2 * 2LL)  != 2LL) return 1;

  // check for div
  if ((i1 / i2) != 1LL) return 1;
  if ((i2 / 2LL)  != 0LL) return 1;

  // check big number
  Numeral i3 = 0x10000000LL;
  // check serialization
  InputOutput iio;
  i3.serialize (iio);
  auto is = dynamic_cast<Numeral*> (Serial::deserialize (iio));
  if (is == nullptr) return 1;
  if (i3 != *is) return 1;
  delete is;

  // create a default null float numeral
  Numeral f1;

  // check for initialized value at 0
  if ((f1 == 0.0) == false) return 1;

  // create another numeral and compare
  Numeral f2 = 1.0;
  if (f1 == f2) return 1;
  if (f2 <  f1) return 1;
  if (f2 <= f1) return 1;
  if (f1 >  f2) return 1;
  if (f1 >= f2) return 1;

  // check for assign
  f1 = f2;
  if (f1 != f2) return 1;

  // check for add
  if ((f1 + f2)  != 2.0) return 1;
  if ((f1 + 2.0) != 3.0) return 1;
  if ((f1 - 1.0) != 0.0) return 1;

  // check serialization
  InputOutput fio;
  f1.serialize (fio);
  auto rs = dynamic_cast<Numeral*> (Serial::deserialize (fio));
  if (rs == nullptr) return 1;
  if (f1 != *rs) return 1;
  delete rs;
  
  // we are done
  return 0;
}
