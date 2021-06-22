// ---------------------------------------------------------------------------
// - t_time.cpp                                                              -
// - standard object library - time class tester                             -
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

#include "Time.hpp"
#include "Exception.hpp"

int main (int, char**) {
  using namespace afnix;

  // create a specific time object
  Time tm (0);
  if (tm.gettime (true) != 0LL)   return 1;
  if (tm.getsecs (true) != 0) return 1;
  if (tm.getmins (true) != 0) return 1;
  if (tm.gethour (true) != 0) return 1;
  // check the generic format
  if (Time::totfmt (0LL, "")  != "000000")   return 1;
  if (Time::totfmt (0LL, ':') != "00:00:00") return 1;
  // check the format methods
  if (tm.format (true) != "00:00:00")  return 1;
  if (tm.toiso  (true) != "00:00:00Z") return 1;
  
  // check for a specific time
  tm.settime (13*Time::HSEC + 10*Time::MSEC + 35);
  if (tm.getsecs (true) != 35) return 1;
  if (tm.getmins (true) != 10) return 1;
  if (tm.gethour (true) != 13) return 1;
  // check the string
  if (tm.format (true) != "13:10:35")  return 1;
  if (tm.toiso  (true) != "13:10:35Z") return 1;

  // check for a specific time with days
  tm.settime (6*Time::DSEC + 19*Time::HSEC + 27*Time::MSEC + 43);
  if (tm.getsecs (true) != 43) return 1;
  if (tm.getmins (true) != 27) return 1;
  if (tm.gethour (true) != 19) return 1;
  // check the string
  if (tm.format (true) != "19:27:43") return 1;

  // remove time and check
  tm.add (-19*Time::HSEC);
  if (tm.gethour (true) != 0) return 1;
  tm.add (-27*Time::MSEC);
  if (tm.getmins (true) != 0) return 1;
  tm.add (-43);
  if (tm.getsecs (true) != 0) return 1;
  // remove remaining time
  tm.add (-6*Time::DSEC);
  if (tm.gettime (true) != 0LL) return 1;

  // check for iso format
  tm.settime ("12:34:45Z");
  if (tm.gethour (true) != 12) return 1;
  if (tm.getmins (true) != 34) return 1;
  if (tm.getsecs (true) != 45) return 1;

  // check for extended time
  if (tm.geteclk (true) != (t_real) tm.gettime (true)) return 1;
  tm.addext (0.5);
  if (tm.geteclk (true) != (t_real) tm.gettime (true) + 0.5) return 1;
  tm.addext (1.5);
  if (tm.geteclk (true) != (t_real) tm.gettime (true)) return 1;

  // check for extended format
  tm.settime ("12:34:45.5Z");
  if (tm.gethour (true) != 12) return 1;
  if (tm.getmins (true) != 34) return 1;
  if (tm.getsecs (true) != 45) return 1;
  if (tm.toext (true) != "12:34:45.500000Z") return 1;
  
  // success
  return 0;
}
