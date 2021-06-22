// ---------------------------------------------------------------------------
// - cclk.cxx                                                                -
// - standard platform library - c time function implementation              -
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
#include "cclk.hxx"

// ---------------------------------------------------------------------------  
// - private section                                                         -
// ---------------------------------------------------------------------------  

#ifdef AFNIX_HAVE_RT
namespace afnix {
  // this procedure returns the system time since the epoch in seconds
  static time_t cclk_get_tunx (void) {
    struct timespec ts;
    if (clock_gettime (CLOCK_REALTIME, &ts) == -1) return -1;
    return ts.tv_sec;
  }

  // this procedure returns the system time stamp in ns
  static t_octa cclk_get_stamp (void) {
    struct timespec ts;
    if (clock_gettime (CLOCK_REALTIME, &ts) == -1) return -1;
    t_octa result = (((t_octa) ts.tv_sec) << 32) | (t_octa) ts.tv_nsec;
    return result;
  }
}
#else
namespace afnix {
  static time_t cclk_get_tunx (void) {
    struct timeval tval;
    if (gettimeofday (&tval, NULL) == -1) return -1;
    t_long result = (t_long) tval.tv_sec;
    return result;
  }
  // this procedure returns the system time stamp in ns
  static t_octa cclk_get_stamp (void) {
    struct timeval tval;
    if (gettimeofday (&tval, NULL) == -1) return 0;
    t_octa result = (((t_octa) tval.tv_sec) << 32) | (t_octa) tval.tv_usec;
    return result;
  }
}
#endif  

// ---------------------------------------------------------------------------  
// - public section                                                          -
// ---------------------------------------------------------------------------  

namespace afnix {

  // return the atc epoch

  t_long c_epoch (void) {
    return AFNIX_ATC_EPOCH;
  }

  // return the atc time in seconds since the origin

  t_long c_time (void) {
    t_long result = (t_long) cclk_get_tunx () + AFNIX_ATC_EPOCH;
    return result;
  }

  // return the unix time in second since the epoch

  t_quad c_tunx (void) {
    t_long result = (t_long) cclk_get_tunx ();
    return result;
  }

  // return the timezone in seconds

  t_long c_tzone (void) {
    // get the time at the call
    time_t time = cclk_get_tunx ();
    if (time == -1) return 0LL;
    // initialize local and utc structure
    struct tm utm;
    struct tm ltm;
    if (gmtime_r    (&time, &utm) == NULL) return 0;
    if (localtime_r (&time, &ltm) == NULL) return 0;
    // compute local time in second
    t_long lts = (ltm.tm_yday * 86400) + (ltm.tm_hour * 3600) + 
      (ltm.tm_min  * 60) + ltm.tm_sec;
    // compute universal time in second
    t_long uts = (utm.tm_yday * 86400) + (utm.tm_hour * 3600) + 
      (utm.tm_min  * 60) + utm.tm_sec;
    // compute timezeone
    return (lts - uts);
  }

  // pause for a certain time

  void c_sleep (const long time) {
    struct timespec tval;
    if (time <= 0) return;
    long sval = time / 1000;
    long nval = (time % 1000) * 1000000;
    tval.tv_sec  = sval;
    tval.tv_nsec = nval;
    nanosleep (&tval, NULL);
  }

  // return the machine time stamp

  t_octa c_stamp (void) {
    t_octa result = cclk_get_stamp ();
    return result;
  }
}
