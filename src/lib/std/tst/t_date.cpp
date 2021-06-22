// ---------------------------------------------------------------------------
// - t_date.cpp                                                              -
// - standard object library - date class tester                             -
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

#include "Date.hpp"
#include "Exception.hpp"

int main (int, char**) {
  using namespace afnix;

  // check the origin (0000-00-00 is a saturday)
  Date date (0LL);
  if (date.getyear (true) != 0)     return 1;
  if (date.getymon (true) != 1)     return 1;
  if (date.getmday (true) != 1)     return 1;
  if (date.getwday (true) != 6)     return 1;
  if (date.mapwday (true) != "Sat") return 1;
  if (date.mapymon (true) != "Jan") return 1;
  // check the generic format
  if (Date::todfmt (0LL, "", "", "")    != "00000101000000")      return 1;
  if (Date::todfmt (0LL, 'T', '-', ':') != "0000-01-01T00:00:00") return 1;
  // check the method format
  if (date.todate  (true) != "0000-01-01") return 1;
  if (date.totime  (true) != "00:00:00")   return 1;
  if (date.toiso   (true) != "0000-01-01T00:00:00Z") return 1;
  if (date.format  (true) != "Sat Jan 1 00:00:00 0") return 1;
  if (date.toweb   ()     != "Sat, 01 Jan 0 00:00:00 GMT") return 1;

  // check my birthday (1964-11-20 is a friday)
  date.setdate (1964,11,20);
  if (date.getyear (true) != 1964)  return 1;
  if (date.getymon (true) != 11)    return 1;
  if (date.getmday (true) != 20)    return 1;
  if (date.getwday (true) != 5)     return 1;
  if (date.mapwday (true) != "Fri") return 1;
  if (date.mapymon (true) != "Nov") return 1;
  // check the string format
  if (date.todate  (true) != "1964-11-20") return 1;
  if (date.totime  (true) != "00:00:00")   return 1;
  if (date.toiso   (true) != "1964-11-20T00:00:00Z")     return 1;
  if (date.format  (true) != "Fri Nov 20 00:00:00 1964") return 1;
  if (date.toweb   ()     != "Fri, 20 Nov 1964 00:00:00 GMT") return 1;

  // check the epoch (1970-1-1 is a thursday)
  date.setdate (1970,1,1);
  if (date.gettime (true) != 62167219200LL) return 1;
  if (date.getyear (true) != 1970)  return 1;
  if (date.getyday (true) != 1)     return 1;
  if (date.getymon (true) != 1)     return 1;
  if (date.getmday (true) != 1)     return 1;
  if (date.getwday (true) != 4)     return 1;
  if (date.mapwday (true) != "Thu") return 1;
  if (date.mapymon (true) != "Jan") return 1;
  // check the string format
  if (date.todate  (true) != "1970-01-01") return 1;
  if (date.totime  (true) != "00:00:00")   return 1;
  if (date.toiso   (true) != "1970-01-01T00:00:00Z")    return 1;
  if (date.format  (true) != "Thu Jan 1 00:00:00 1970") return 1;
  if (date.toweb   ()     != "Thu, 01 Jan 1970 00:00:00 GMT") return 1;

  // check the millenium bug!! (1999-12-31 is a friday)
  date.setdate (1999,12,31);
  if (date.getyear (true) != 1999)  return 1;
  if (date.getyday (true) != 365)   return 1;
  if (date.getymon (true) != 12)    return 1;
  if (date.getmday (true) != 31)    return 1;
  if (date.getwday (true) != 5)     return 1;
  if (date.mapwday (true) != "Fri") return 1;
  if (date.mapymon (true) != "Dec") return 1;
  // check the string format
  if (date.todate  (true) != "1999-12-31") return 1;
  if (date.totime  (true) != "00:00:00")   return 1;
  if (date.toiso   (true) != "1999-12-31T00:00:00Z")     return 1;
  if (date.format  (true) != "Fri Dec 31 00:00:00 1999") return 1;
  if (date.toweb   ()     != "Fri, 31 Dec 1999 00:00:00 GMT") return 1;

  // check the millenium!! (2000-12-31 is a sunday and is a leap year)
  date.setdate (2000,12,31);
  if (date.getyear (true) != 2000 ) return 1;
  if (date.getyday (true) != 366)   return 1;
  if (date.getymon (true) != 12)    return 1;
  if (date.getmday (true) != 31)    return 1;
  if (date.getwday (true) != 0)     return 1;
  if (date.mapwday (true) != "Sun") return 1;
  if (date.mapymon (true) != "Dec") return 1;
  // check the string format
  if (date.todate  (true) != "2000-12-31") return 1;
  if (date.totime  (true) != "00:00:00")   return 1;
  if (date.toiso   (true) != "2000-12-31T00:00:00Z")     return 1;
  if (date.format  (true) != "Sun Dec 31 00:00:00 2000") return 1;
  if (date.toweb   ()     != "Sun, 31 Dec 2000 00:00:00 GMT") return 1;
 
  // check the date in reverse
  date.setdate ("2000-12-31T23:59:00Z");
  if (date.getyear (true) != 2000)  return 1;
  if (date.getymon (true) != 12)    return 1;
  if (date.getmday (true) != 31)    return 1;
  if (date.gethour (true) != 23)    return 1;
  if (date.getmins (true) != 59)    return 1;
  if (date.getsecs (true) != 0)     return 1;

  // check the web date in reverse
  date.setweb ("Sun, 31 Dec 2000 23:59:00 GMT");
  if (date.getyear (true) != 2000)  return 1;
  if (date.getymon (true) != 12)    return 1;
  if (date.getmday (true) != 31)    return 1;
  if (date.gethour (true) != 23)    return 1;
  if (date.getmins (true) != 59)    return 1;
  if (date.getsecs (true) != 0)     return 1;

  // check the last negative day
  date.setdate (-1,12,31);
  if (date.getyear (true) != -1)    return 1;
  if (date.getyday (true) != 365)   return 1;
  if (date.getymon (true) != 12)    return 1;
  if (date.getmday (true) != 31)    return 1;
  if (date.getwday (true) != 5)     return 1;
  if (date.mapwday (true) != "Fri") return 1;
  if (date.mapymon (true) != "Dec") return 1;

  // check the gregorian proleptic epoch
  date.setdate (-4714,11,24);
  if (date.getyear (true) != -4714) return 1;
  if (date.getyday (true) != 328)   return 1;
  if (date.getymon (true) != 11)    return 1;
  if (date.getmday (true) != 24)    return 1;

  // check extended date
  date.setdate ("2000-01-01T12:00:00.5Z");
  if (date.getyear (true) != 2000) return 1;
  if (date.getyday (true) != 1)    return 1;
  if (date.getymon (true) != 1)    return 1;
  if (date.getmday (true) != 1)    return 1;
  if (date.toext (true) != "2000-01-01T12:00:00.500000Z") return 1;
  // success
  return 0;
}
