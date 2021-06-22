// ---------------------------------------------------------------------------
// - cclk.hpp                                                                -
// - standard platform library - c time native function definition           -
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

#ifndef  AFNIX_CCLK_HPP
#define  AFNIX_CCLK_HPP

#ifndef  AFNIX_CCNF_HPP
#include "ccnf.hpp"
#endif

namespace afnix {
  /// This file define the operation of the afnix time coordinated or atc.
  /// The afnix system uses a time reference of 0 at Jan 1st 1BC with respect
  /// to a proleptic gregorian calendar augmented with year 0. Thus the day 0
  /// corresponds to the first day of year -1 since the 0 did not exist at
  /// that time. The day at time 0 is a saturday. Since may computer uses
  /// jan 1st 1970 as their epoch, the time correction applied corresponds
  /// to 62167219200 seconds. Normally, the year 0 is used with the
  /// astronomical gregorian calendar, but the reference in that case is
  /// not the same.
  /// @author amaury darsch

  /// @return the atc epoch
  t_long c_epoch (void);

  /// @return the current utc since the epoch in second
  t_long c_time (void);

  /// @return the current unix time since the epoch in second
  t_quad c_tunx (void);

  /// @return the timezone in seconds
  t_long c_tzone (void);

  /// pause for a certain time
  /// @param time the time to pause in milliseonds
  void c_sleep (const long time);

  /// @return a machine time stamp
  t_octa c_stamp (void);
}

#endif
