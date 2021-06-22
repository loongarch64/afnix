// ---------------------------------------------------------------------------
// - t_cclk.cpp                                                              -
// - standard platform library - time function tester                        -
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

#include "cclk.hpp"

int main (int, char**) {
  using namespace afnix;

  // get the utc time since the atc
  t_long tclk = c_time ();
  if (tclk <= 0) return -1;

  // get the timezeone and check it is bounded within a day
  t_long tz = c_tzone ();
  if ((tz < -86400) || (tz > 86400)) return 1;

  // sleep for one second
  c_sleep (1000);

  // everything is fine
  return 0;
}
