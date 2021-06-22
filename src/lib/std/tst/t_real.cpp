// ---------------------------------------------------------------------------
// - t_real.cpp                                                              -
// - standard object library - real class tester module                      -
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

#include "Real.hpp"
#include "InputOutput.hpp"

int main (int, char**) {
  using namespace afnix;

  // create a default null real
  Real f1;

  // check for initialized value at 0
  if ((f1 == 0.0) == false) return 1;

  // create another real and compare
  Real f2 = 1.0;
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

  // check for automatic conversion
  f2 = 2000;
  if (f2 != 2000.0) return 1;

  // check for string constructor
  Real f3 ("1.0");    if (f3 !=  1.0)  return 1;
  Real f4 ("-1.0");   if (f4 != -1.0)  return 1;
  Real f5 ("1.");     if (f5 !=  1.0)  return 1;
  Real f6 ("1.0e1");  if (f6 != 10.0)  return 1;

  // check serialization
  InputOutput io;
  f6.serialize (io);
  auto rs = dynamic_cast<Real*> (Serial::deserialize (io));
  if (rs == nullptr) return 1;
  if (f6 != *rs) return 1;
  delete rs;
  
  // we are done
  return 0;
}
