// ---------------------------------------------------------------------------
// - t_counter.cpp                                                           -
// - afnix engine - counter object tester module                             -
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

#include "Counter.hpp"

int main (int, char**) {
  using namespace afnix;

  // create a upward counter
  Counter cntu (10);
  if (cntu.repr () != "Counter") return 1;
  if (cntu.valid () == false)    return 1;
  // check the object
  long stpu = 0;
  while (cntu.step () == true) stpu++;
  if (stpu != 10) return 1;
  
  // create a upward counter
  Counter cntd (0, -10);
  if (cntd.repr () != "Counter") return 1;
  if (cntd.valid () == false)    return 1;
  // check the object
  long stpd = 0;
  while (cntd.step () == true) stpd++;
  if (stpd != 10) return 1;

  // success
  return 0;
}
