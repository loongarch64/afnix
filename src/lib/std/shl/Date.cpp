// ---------------------------------------------------------------------------
// - Date.cpp                                                                -
// - standard object library - date class implementation                     -
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
#include "Regex.hpp"
#include "Strvec.hpp"
#include "Vector.hpp"
#include "Integer.hpp"
#include "Utility.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"
#include "cclk.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - private section                                                       -
  // -------------------------------------------------------------------------

  // week day string mapping in us/week reference
  static const long  ATC_MAX_WDAY               = 7;
  static const char* ATC_DAY_NAME[ATC_MAX_WDAY] =
    {
     "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"
    };
  // months string mapping
  static const long  ATC_MAX_YMON               = 12;
  static const char* ATC_MON_NAME[ATC_MAX_YMON] =
    {
     "Jan", "Feb", "Mar", "Apr", "May", "Jun",
     "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
    };

  // number of gregorian days per year  (normal and leap)
  static const long ATC_YDAY_NORM = 365;
  static const long ATC_YDAY_LEAP = 366;
  // number of gregorian days per month (normal and leap)
  static const long ATC_MDAY_NORM[12] =
    {
     31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31
    };
  static const long ATC_MDAY_LEAP[12] =
    {
     31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31
    };

  // format the day string
  static String atc_mapwday (const long wday) {
    if ((wday < 0) || (wday >= ATC_MAX_WDAY))
      throw Exception ("date-error", "day index is out of range");
    return ATC_DAY_NAME[wday];
  }

  // get the week day value
  static long atc_getwday (const String& wday) {
    for (long k = 0; k < ATC_MAX_WDAY; k++) {
      if (wday == ATC_DAY_NAME[k]) return k;
    }
    throw Exception ("date-error", "invalid weed day to map", wday);
  }

  // format the month string
  static String atc_mapymon (const long ymon) {
    if ((ymon < 1) || (ymon > ATC_MAX_YMON))
      throw Exception ("date-error", "month index is out of range");
    return ATC_MON_NAME[ymon-1];
  }

  // get the year month value
  static long atc_getymon (const String& ymon) {
    for (long k = 0; k < ATC_MAX_YMON; k++) {
      if (ymon == ATC_MON_NAME[k]) return k;
    }
    throw Exception ("date-error", "invalid year month to map", ymon);
  }

  // return the absolute value
  static inline long abs (const long abs) {
    return (abs < 0) ? -abs : abs;
  }
  static inline t_long abs (const t_long abs) {
    return (abs < 0) ? -abs : abs;
  }

  // return true if the year is a leap year
  static inline bool atc_isleap (const long year) {
    // 0 is a special case
    if (year == 0) return true;
    // get absolute year
    long ywrk = abs (year);
    // check for leap
    if ((ywrk % 400) == 0) return true;
    if ((ywrk % 100) == 0) return false;
    if ((ywrk % 4)   == 0) return true;
    return false;
  }

  // validate a date
  static bool atc_valid (const long year, const long ymon, const long mday) {
    // check year month
    if ((ymon <= 0L) || (ymon > 12L)) return false;
    // check month day
    if ((mday <= 0L) || (mday > 31L)) return false;
    if (atc_isleap (year) == true) {
      if ((ymon == 2) && (mday > 29L)) return false;
    } else {
      if ((ymon == 2) && (mday > 28L)) return false;
    }
    if ((ymon != 2) && (ymon < 8)) {
      if (((ymon % 2) == 0) && (mday > 30)) return false;
    }
    if (ymon >= 8) {
      if (((ymon % 2) != 0) && (mday > 30)) return false;
    }
    // all good
    return true;
  }
  
  // get the number of days in a year
  static inline t_long atc_yday_in_year (const long year) {
    t_long yday = atc_isleap (year) ? ATC_YDAY_LEAP : ATC_YDAY_NORM;
    return yday;
  }

  // get the number of seconds in a year
  static inline t_long atc_ysec_in_year (const long year) {
    t_long ysec = atc_yday_in_year (year) * (t_long) Time::DSEC;
    return ysec;
  }

  // get the number of days in a year month
  static inline t_long atc_mday_in_ymon (const long year, const long ymon) {
    if (atc_isleap (year) == true) return ATC_MDAY_LEAP[ymon];
    return ATC_MDAY_NORM[ymon];
  }

  // get the number of seconds in a year month
  static inline t_long atc_msec_in_ymon (const long year, const long ymon) {
    t_long msec = atc_mday_in_ymon (year, ymon) * (t_long) Time::DSEC;
    return msec;
  }

  // trim the day in a year month
  static inline long atc_trim_in_ymon (const long year, const long ymon,
				       const long mday) {
    long maxd = (long) atc_mday_in_ymon (year, ymon);
    return (mday <= maxd) ? mday : maxd;
  }

  // get the number of days upto a year
  static t_long atc_yday_to_year (const long year) {
    // iterate upto the year
    t_long yday = 0LL;
    if (year > 0L)  {
      for (long k = 0L; k < year; k++) {
	yday += atc_yday_in_year (k);
      }
    } else {
      for (long k = -1L; k > year; k--) {
	yday += atc_yday_in_year (k);
      }
    }
    return yday;
  }

  // get the number of seconds upto a year
  static t_long atc_ysec_to_year (const long year) {
    t_long ysec = atc_yday_to_year (year) * (t_long) Time::DSEC;
    return ysec;
  }

  // get the number of days upto a month in a specific year
  static t_long atc_yday_to_ymon (const long year, const long ymon) {
    t_long yday = 0LL;
    if (year >= 0L) {
      if (atc_isleap (year) == true) {
	for (long k = 0L; k < ymon; k++) yday += ATC_MDAY_LEAP[k];
      } else {
	for (long k = 0L; k < ymon; k++) yday += ATC_MDAY_NORM[k];
      }
    } else {
      if (atc_isleap (year) == true) {
	for (long k = 11L; k > ymon; k--) yday += ATC_MDAY_LEAP[k];
      } else {
	for (long k = 11L; k > ymon; k--) yday += ATC_MDAY_NORM[k];
      }
    }
    return yday;
  }
  
  // get the number of days in a month in a specific year
  static t_long atc_yday_to_mday (const long year, const long ymon,
				  const long mday) {
    // just month day for positive year
    if (year >= 0L) return mday;
    if (atc_isleap(year) == true) {
      return ATC_MDAY_LEAP[ymon] - mday;
    }
    return ATC_MDAY_NORM[ymon] - mday;
  }

  // get the number of seconds upto a month in a specific year
  static t_long atc_ysec_to_ymon (const long year, const long ymon) {
    t_long ysec = atc_yday_to_ymon (year, ymon) * (t_long) Time::DSEC;
    return ysec;
  }

  // number of day upto a date
  static t_long atc_yday_to_date (const long year, const long ymon, 
				  const long mday) {
    t_long yday = 0LL;
    yday += atc_yday_to_year (year);
    yday += atc_yday_to_ymon (year, ymon);
    yday += atc_yday_to_mday (year, ymon, mday);
    return yday;
  }

  // number of seconds upto a date
  static t_long atc_ysec_to_date (const long year, const long ymon, 
				  const long mday) {
    t_long ysec = atc_yday_to_date (year, ymon, mday) * (t_long) Time::DSEC;
    return ysec;
  }

  // find the absolute year from an atc clock
  static long atc_year_from_tclk (const t_long tclk) {
    // get the absolute clock
    t_long wclk = abs (tclk);
    // compute the year by iteration
    long   year = 0L; t_long cclk = 0LL;
    while (cclk <= wclk) {
      cclk += atc_ysec_in_year (year);
      if (cclk > wclk) break;
      year++;
    }
    return year;
  }

  // get the number of days upto a year in a specific year
  static long atc_yday_from_tclk (const t_long secs, const long year) {
    long yday = (long) (secs / (t_long) Time::DSEC);
    if (year >= 0L) return yday;
    return atc_isleap (year) ? ATC_YDAY_LEAP - yday : ATC_YDAY_NORM - yday;
  }
  
  // find the month in a year from an atc clock
  static long atc_ymon_from_tclk (const t_long tclk, const long year) {
    // get the absolute clock
    t_long wclk = abs (tclk);
    // compute the month by iteration
    long   ymon = 0;
    t_long cclk = 0LL;
    for (long i = 0; i < ATC_MAX_YMON; i++) {
      cclk += atc_msec_in_ymon (year, i);
      if (cclk > wclk) break;
      ymon++;
    }
    return (year >= 0L) ? ymon : 11L - ymon;
  }
  
  // get the month days from an atc clock
  static long atc_mday_from_tclk (const t_long secs, const long year,
				  const long   ymon) {
    long mday = (long) (secs / (t_long) Time::DSEC);
    return (year >= 0L) ? mday : atc_mday_in_ymon(year, ymon) - mday;
  }
  
  // find the atc time from the date
  static t_long atc_time_from_date (const long year, const long ymon, 
				    const long mday) {
    // check arguments
    if (atc_valid (year, ymon, mday) == false) {
      throw Exception ("date-error", "invalid date value");
    }
    // extract the date time
    t_long tclk = atc_ysec_to_date (year, ymon-1, mday-1);
    // extract time sign from the year
    return (year >= 0) ? tclk : -tclk;
  }

  // find the atc time from the date
  static t_long atc_time_from_date (const long year, const long ymon, 
				    const long mday, const long hour, 
				    const long mins, const long secs) {
    // check arguments
    if (atc_valid (year, ymon, mday) == false) {
      throw Exception ("date-error", "invalid date value");
    }
    // extract the date time
    t_long tclk = atc_ysec_to_date (year, ymon-1, mday-1);
    // add the time
    tclk += (t_long) (hour * Time::HSEC + mins * Time::MSEC + secs);
    // extract time sign from the year
    return (year >= 0) ? tclk : -tclk;
  }

  // find the atc time from string date
  static t_long atc_time_from_date (const String& date) {
    // extract the date components
    Regex re ("[($d$d$d$d)-($d$d)-($d$d)T($d$d):($d$d):($d$d)Z]");
    if (re == date) {
      // check for consistency
      if (re.length () != 6) {
	throw Exception ("internal-error", "invalid parsed date", date);
      }
      // get the date data
      long year = re.getlong (0);
      long ymon = re.getlong (1);
      long mday = re.getlong (2);
      long hour = re.getlong (3);
      long mins = re.getlong (4);
      long secs = re.getlong (5);
      // get the atc time
      return atc_time_from_date (year, ymon, mday, hour, mins, secs);
    } else {
      throw Exception ("date-error", "invalid date format", date);
    }
  }

  // find the atc time from string web date
  static t_long atc_time_from_web (const String& date) {
    // extract the date components
    Regex re ("($a+), ($d+) ($a+) ($d+) ($d+):($d+):($d+) GMT");
    if (re == date) {
      // check for consistency
      if (re.length () != 7) {
	throw Exception ("internal-error", "invalid parsed web date", date);
      }
      // validate each field
      long wday = atc_getwday (re.getstring (0));
      // get the year
      long year = re.getlong (3);
      // get the year month
      long ymon = atc_getymon (re.getstring (2));
      // get the month day and check
      long mday = re.getlong (1);
      if ((mday < 0) || (mday > atc_mday_in_ymon (year, ymon))) {
	throw Exception ("date-error", "invalid web date format", date);
      }
      // get the time components
      long hour = re.getlong (4);
      long mins = re.getlong (5);
      long secs = re.getlong (6);
      // get the atc time
      t_long tclk = atc_time_from_date (year, ymon+1, mday, hour, mins, secs);
      // map the absolute time
      t_long wclk = abs (tclk);
      // check the wday
      if (wday != (((wclk / (t_long) Time::DSEC) + 6) % 7)) {
	throw Exception ("date-error", "inconsistent web date format", date);
      }
      return tclk;
    } else {
      throw Exception ("date-error", "invalid web date format", date);
    }
  }
  
  // the date sructure
  struct s_date {
    long d_year; // year
    long d_yday; // day in year
    long d_ymon; // month in year [0:11]
    long d_mday; // month in day  [0:30]
    long d_wday; // day in week   [0:6]
    long d_hour; // the hour
    long d_mins; // the minute
    long d_secs; // the seconds
    // create a default date
    s_date (void) {
      d_year = 0;
      d_yday = 0;
      d_ymon = 0;
      d_mday = 0;
      d_wday = 6;
      d_hour = 0;
      d_mins = 0;
      d_secs = 0;
    }
    // create a specific date
    s_date (const t_long tclk) {
      // get absolute time
      t_long wclk = abs (tclk);
      // process positive time
      if (tclk >= 0) {
	// get the year
	d_year = atc_year_from_tclk (wclk);
	// update the remaining seconds
	t_long secs = wclk - atc_ysec_to_year (d_year);
	// get the year day
	d_yday = atc_yday_from_tclk (secs, d_year);
	// get the year month
	d_ymon = atc_ymon_from_tclk (secs, d_year);
	// update the remaining seconds
	secs -= atc_ysec_to_ymon (d_year, d_ymon);
	// get the month day
	d_mday = atc_mday_from_tclk (secs, d_year, d_ymon);
	// get the week day (0000-00-00 is a saturday)
	d_wday = ((wclk / (t_long) Time::DSEC) + 6) % 7;
      } else {
	// get the year
	d_year = -(atc_year_from_tclk (wclk) + 1);
	// update the remaining seconds
	t_long secs = wclk - atc_ysec_to_year (d_year);
	// get the year day
	d_yday = atc_yday_from_tclk (secs, d_year);
	// get the year month
	d_ymon = atc_ymon_from_tclk (secs, d_year);
	// update the remaining seconds
	secs -= atc_ysec_to_ymon (d_year, d_ymon);
	// get the month day
	d_mday = atc_mday_from_tclk (secs, d_year, d_ymon);
	// get the week day (0000-00-00 is a saturday)
	d_wday = 6 - ((wclk / (t_long) Time::DSEC) % 7);
      }
      // set the time components
      d_hour = (long) ((wclk / 3600LL) % 24LL);
      d_mins = (long) ((wclk /   60LL) % 60LL);
      d_secs = (long) (wclk % 60LL);
      // fix negative time
      if (d_hour < 0) d_hour += 24LL;
      if (d_mins < 0) d_mins += 60LL;
      if (d_secs < 0) d_secs += 60LL;
    }
    // create a date by copy
    s_date (const s_date& that) {
      d_year = that.d_year;
      d_yday = that.d_yday;
      d_ymon = that.d_ymon;
      d_mday = that.d_mday;
      d_wday = that.d_wday;
      d_hour = that.d_hour;
      d_mins = that.d_mins;
      d_secs = that.d_secs;
    }
    // return the date year
    long getyear (void) const {
      return d_year;
    }
    // return the day in the year 
    long getyday (void) const {
      return d_yday + 1;
    }
    // return the month in the year 
    long getymon (void) const {
      return d_ymon + 1;
    }
    // return the day in the month
    long getmday (void) const {
      return d_mday + 1;
    }
    // return the day in the week
    long getwday (void) const {
      return d_wday;
    }
    // return the hours
    long gethour (void) const {
      return d_hour;
    }
    // return the minute
    long getmins (void) const {
      return d_mins;
    }
    // return the seconds
    long getsecs (void) const {
      return d_secs;
    }
    // return the week day name
    String mapwday (void) const {
      long wday = getwday ();
      return atc_mapwday (wday);
    }
    // return the month name
    String mapymon (void) const {
      long ymon = getymon ();
      return atc_mapymon (ymon);
    }
    // format the date
    String todate (const String& dsep) const {
      // check the year
      long year = getyear ();
      if (year < 0) {
	throw Exception ("date-error", "cannot format negative year");
      }
      if (year > 9999) {	
	throw Exception ("date-error", "cannot format year above 9999");
      }
      // format the year
      String result;
      if (year < 10) {
	result += "000";
      } else if (year < 100) {
	result += "00";
      } else if (year < 1000) {
	result += "0";
      }
      result += year;
      // add the separator
      if (dsep.isnil () == false) result += dsep;
      // format the month
      long ymon = getymon ();
      if (ymon < 10) result += "0";
      result += ymon;
      // add the separator
      if (dsep.isnil () == false) result += dsep;
      // format the day
      long mday = getmday ();
      if (mday < 10) result += "0";
      result += mday;
      // here it is
      return result;
    }
  };

  // -------------------------------------------------------------------------
  // - public section                                                        -
  // -------------------------------------------------------------------------

  String Date::todfmt (const t_long  wclk, const String& fsep,
		       const String& dsep, const String& tsep) {
    // create a wall clock date
    s_date date (wclk);
    // format the date
    String result = date.todate (dsep);
    // add the field separator
    if (fsep.isnil () == false) result += fsep;
    // format the time
    result += Time::totfmt (wclk,tsep);
    // here it is
    return result;
  }

  // -------------------------------------------------------------------------
  // - class section                                                        -
  // -------------------------------------------------------------------------

  // create a current date

  Date::Date (void) {
    setdate (Time::TNOW);
  }

  // create a specific date

  Date::Date (const t_long tclk) : Time (tclk) {}

  // create a date by iso specification

  Date::Date (const String& date) {
    setdate (date);
  }

  // create a date by specific elements
  
  Date::Date (const long year, const long ymon, const long mday) : Time (0) {
    setdate (year, ymon, mday);
  }

  // create a date by specific elements
  
  Date::Date (const long year, const long ymon, const long mday,
	      const long hour, const long mins, const long secs) : Time (0) {
    setdate (year, ymon, mday, hour, mins, secs);
  }

  // copy construct a date object

  Date::Date (const Date& that) {
    that.rdlock ();
    try {
      Time::operator = (that);
      that.unlock ();
    } catch (...) {
      that.unlock ();
      throw;
    }
  }

  // return the class name

  String Date::repr (void) const {
    return "Date";
  }

  // return a clone of this object

  Object* Date::clone (void) const {
    return new Date (*this);
  }
  
  // assign a date object to this one

  Date& Date::operator = (const Date& that) {
    // check for self assignation
    if (this == &that) return *this;
    // lock and assign
    wrlock ();
    that.rdlock ();
    try {
      Time::operator = (that);
      unlock ();
      that.unlock ();
      return *this;
    } catch (...) {
      unlock ();
      that.unlock ();
      throw;
    }
  }

  // format the date in a general form
  
  String Date::format (const bool utc) const {
    rdlock ();
    try {
      // create an operating date
      s_date date (gettime (utc));
      // format the week day
      String result = date.mapwday ();
      // add the month
      result += ' ';
      result += date.mapymon ();
      // add the day
      result += ' ';
      result += date.getmday ();
      // format the time
      result += ' ';
      result += totime (utc);
      // add the year
      result += ' ';
      result += date.getyear ();
      // unlock and return
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // format the date in the ISO-8601 form
  
  String Date::toiso (const bool utc) const {
    rdlock ();
    try {
      // format the date
      String result = todate (utc);
      // format the time
      result += 'T';
      result += Time::toiso (utc);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // format the date in the RFC-2822 form

  String Date::torfc (const bool utc) const {
    rdlock ();
    try {
      // create an operating date
      s_date date (gettime (utc));
      // map the week day
      String result = date.mapwday ();
      // add the month day
      long mday = date.getmday ();
      if (mday < 10) {
	result += ", 0";
      } else {
	result += ", ";
      }
      result += mday;
      // add the month
      result += ' ';
      result += date.mapymon ();
      // add the year
      long year = date.getyear ();
      if (year < 10) {
	result += " 000";
      } else if (year < 100) {
	result += " 00";
      } else if (year < 1000) {
	result += " 0";
      } else {
	result += " ";
      }
      result += year;
      // add the time/zone
      result += ' ';
      result += Time::torfc (utc);
      // unlock and return
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    } 
  }
  
  // format the date in the extended form
  
  String Date::toext (const bool utc) const {
    rdlock ();
    try {
      // format the date
      String result = todate (utc);
      // format the time
      result += 'T';
      result += Time::toext (utc);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // set a date by an iso specification

  void Date::setdate (const String& date) {
    wrlock ();
    try {
      // check for now
      if (date == Time::TNOW) {
	Time::settime (Time::TNOW);
	unlock ();
	return;
      }
      // check for zulu time
      if (date.last () != 'Z') {
	throw Exception ("date-error", "missing zulu date postfix", date);
      }
      // split date and extended clock
      String sdat = date;
      Strvec svec = Strvec::split (date, ".");
      if (svec.length () > 2) {
	throw Exception ("time-error", "invalid date string", date);
      }
      if (svec.length () == 2) {
	sdat = svec.get (0) + "Z";
	String eclk = svec.get (1); eclk = eclk.lsubstr (eclk.length()-1);
	d_eclk = Utility::toreal (String (".") + eclk);
      }
      // extract the atc time
      t_long tclk = atc_time_from_date (sdat);
      // set the time
      settime (tclk);
      // unlock and return
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set the date by specific elements
  
  void Date::setdate (const long year, const long ymon, const long mday) {
    wrlock ();
    try {
      // extract the atc time
      t_long tclk = atc_time_from_date (year, ymon, mday);
      // set the time
      settime (tclk);
      // unlock and return
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set the date by specific elements
  
  void Date::setdate (const long year, const long ymon, const long mday,
		      const long hour, const long mins, const long secs) {
    wrlock ();
    try {
      // extract the atc time
      t_long tclk = atc_time_from_date (year, ymon, mday, hour, mins, secs);
      // set the time
      settime (tclk);
      // unlock and return
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // add years to the current date

  void Date::addyear (const long num) {
    wrlock ();
    try {
      // create an operating date
      s_date date (d_tclk);
      // get the current date
      long year = date.getyear ();
      long ymon = date.getymon ();
      long mday = date.getmday ();
      long hour = date.gethour ();
      long mins = date.getmins ();
      long secs = date.getsecs ();
      // add the year
      year += num;
      // check if the day is valid
      mday = atc_trim_in_ymon (year, ymon, mday);
      // set the date by components
      setdate (year, ymon, mday, hour, mins, secs);
      // unlock and return
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // add months to the current date

  void Date::addymon (const long num) {
    wrlock ();
    try {
      // create an operating date
      s_date date (d_tclk);
      // get the current date
      long year = date.getyear ();
      long ymon = date.getymon ();
      long mday = date.getmday ();
      long hour = date.gethour ();
      long mins = date.getmins ();
      long secs = date.getsecs ();
      // compute number of year and remaining month
      long ynum = num / 12;
      long mnum = num % 12;
      // add the year and month
      year += ynum;
      ymon += mnum;
      // update year to add and month
      year += ymon / 12;
      ymon  = ymon % 12;
      // check if the day is valid
      mday = atc_trim_in_ymon (year, ymon, mday);
      // update the date
      setdate (year, ymon, mday, hour, mins, secs);
      // unlock and return
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return the date year

  long Date::getyear (const bool utc) const {
    rdlock ();
    try {
      // create an operating date
      s_date date (gettime (utc));
      // get the year
      long result = date.getyear ();
      // unlock and return
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return the day in the year 

  long Date::getyday (const bool utc) const {
    rdlock ();
    try {
      // create an operating date
      s_date date (gettime (utc));
      // get the day in the year
      long result = date.getyday ();
      // unlock and return
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return the month in the year 

  long Date::getymon (const bool utc) const {
    rdlock ();
    try {
      // create an operating date
      s_date date (gettime (utc));
      // get the month in the year
      long result = date.getymon ();
      // unlock and return
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return the day in the month

  long Date::getmday (const bool utc) const {
    rdlock ();
    try {
      // create an operating date
      s_date date (gettime (utc));
      // get the day in the month
      long result = date.getmday ();
      // unlock and return
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return the day in the week

  long Date::getwday (const bool utc) const {
    rdlock ();
    try {
      // create an operating date
      s_date date (gettime (utc));
      // get the day in the week
      long result = date.getwday ();
      // unlock and return
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return the week day name

  String Date::mapwday (const bool utc) const {
    rdlock ();
    try {
      // get the day in the week
      long wday = getwday (utc);
      // map the day name
      String result = atc_mapwday (wday);
      // unlock and return
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return the month name

  String Date::mapymon (const bool utc) const {
    rdlock ();
    try {
      // get the month in the year
      long ymon = getymon (utc);
      // map the month name
      String result = atc_mapymon (ymon);
      // unlock and return
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // format the date

  String Date::todate (const bool utc) const {
    rdlock ();
    try {
      // create an operating date
      s_date date (gettime (utc));
      // format the date
      String result = date.todate ('-');
      // unlock and return
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // format the time

  String Date::totime (const bool utc) const {
    rdlock ();
    try {
      String result = Time::format (utc);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // format the date in web form as specified by RFC-1123

  String Date::toweb (void) const {
    rdlock ();
    try {
      // create an operating date
      s_date date (d_tclk);
      // get the day of week
      String result = date.mapwday ();
      result = result + ", ";
      // get the day in month
      long mday = date.getmday ();
      if (mday < 10) {
	result = result + '0' + mday + ' ';
      } else {
	result = result + mday + ' ';
      }
      // get the year month
      result = result + date.mapymon () + ' ';
      // get the year
      long year = date.getyear ();
      result = result + year + ' ';
      // get the time
      result = result + Time::format (true) + " GMT";
      // unlock and return
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // set a date by by web specification

  void Date::setweb (const String& date) {
    wrlock ();
    try {
      // extract the atc time
      t_long tclk = atc_time_from_web (date);
      // set the time
      settime (tclk);
      // unlock and return
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // -------------------------------------------------------------------------
  // - object section                                                        -
  // -------------------------------------------------------------------------

  // the quark zone
  static const long QUARK_ZONE_LENGTH = 15;
  static QuarkZone  zone (QUARK_ZONE_LENGTH);

  // the object supported quarks
  static const long QUARK_TORFC   = zone.intern ("to-rfc");
  static const long QUARK_TOWEB   = zone.intern ("to-web");
  static const long QUARK_TODATE  = zone.intern ("to-date");
  static const long QUARK_TOTIME  = zone.intern ("to-time");
  static const long QUARK_SETWEB  = zone.intern ("set-web");
  static const long QUARK_GETYEAR = zone.intern ("year");
  static const long QUARK_GETYDAY = zone.intern ("year-day");
  static const long QUARK_GETYMON = zone.intern ("month");
  static const long QUARK_GETMDAY = zone.intern ("day");
  static const long QUARK_ADDYEAR = zone.intern ("add-years");
  static const long QUARK_ADDYMON = zone.intern ("add-months");  
  static const long QUARK_GETWDAY = zone.intern ("week-day");
  static const long QUARK_MAPWDAY = zone.intern ("map-day");
  static const long QUARK_MAPYMON = zone.intern ("map-month");
  static const long QUARK_SETDATE = zone.intern ("set-date");

  // create a new object in a generic way
 
  Object* Date::mknew (Vector* argv) {
    long argc = (argv == nullptr) ? 0 : argv->length ();
    // create a default time object
    if (argc == 0) return new Date;
    if (argc == 1) {
      Object* obj = argv->get (0);
      // check for an integer
      Integer* iobj = dynamic_cast <Integer*> (obj);
      if (iobj != nullptr) return new Date (iobj->tolong ());
      // check for a string
      String* sobj = dynamic_cast <String*> (obj);
      if (sobj != nullptr) return new Date (*sobj);
      // invalid object
      throw Exception ("type-error", "invalid object for date",
		       Object::repr (obj));
    }
    // check for 3 arguments
    if (argc == 3) {
      long year = argv->getlong (0);
      long ymon = argv->getlong (1);
      long mday = argv->getlong (2);
      return new Date (year, ymon, mday);
    }
    // check for 6 arguments
    if (argc == 6) {
      long year = argv->getlong (0);
      long ymon = argv->getlong (1);
      long mday = argv->getlong (2);
      long hour = argv->getlong (3);
      long mins = argv->getlong (4);
      long secs = argv->getlong (5);
      return new Date (year, ymon, mday, hour, mins, secs);
    }
    throw Exception ("argument-error",
                     "too many argument with date constructor");
  }
 
  // return true if the given quark is defined

  bool Date::isquark (const long quark, const bool hflg) const {
    rdlock ();
    try {
      if (zone.exists (quark) == true) {
	unlock ();
	return true;
      }
      bool result = hflg ? Time::isquark (quark, hflg) : false;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // apply this object with a set of arguments and a quark
  
  Object* Date::apply (Evaluable* zobj, Nameset* nset, const long quark,
		       Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();

    // check for 0 arguments
    if (argc == 0) {
      if (quark == QUARK_TOWEB)   return new String  (toweb   ());
      if (quark == QUARK_TORFC)   return new String  (torfc   (false));
      if (quark == QUARK_TODATE)  return new String  (todate  (false));
      if (quark == QUARK_TOTIME)  return new String  (totime  (false));
      if (quark == QUARK_GETYEAR) return new Integer (getyear (false));
      if (quark == QUARK_GETYDAY) return new Integer (getyday (false));
      if (quark == QUARK_GETYMON) return new Integer (getymon (false));
      if (quark == QUARK_GETMDAY) return new Integer (getmday (false));
      if (quark == QUARK_GETWDAY) return new Integer (getwday (false));
      if (quark == QUARK_MAPWDAY) return new String  (mapwday (false));
      if (quark == QUARK_MAPYMON) return new String  (mapymon (false));
    }
    // check for 1 argument
    if (argc == 1) {
      if (quark == QUARK_TORFC) {
	bool utc = argv->getbool (0);
	return new String (torfc (utc));
      }
      if (quark == QUARK_TODATE) {
	bool utc = argv->getbool (0);
	return new String (todate (utc));
      }
      if (quark == QUARK_TOTIME) {
	bool utc = argv->getbool (0);
	return new String (totime (utc));
      }
      if (quark == QUARK_GETYEAR) {
	bool utc = argv->getbool (0);
	return new Integer (getyear (utc));
      }
      if (quark == QUARK_GETYDAY) {
	bool utc = argv->getbool (0);
	return new Integer (getyday (utc));
      }
      if (quark == QUARK_GETYMON) {
	bool utc = argv->getbool (0);
	return new Integer (getymon (utc));
      }
      if (quark == QUARK_GETMDAY) {
	bool utc = argv->getbool (0);
	return new Integer (getmday (utc));
      }
      if (quark == QUARK_GETWDAY) {
	bool utc = argv->getbool (0);
	return new Integer (getwday (utc));
      }
      if (quark == QUARK_MAPWDAY) {
	bool utc = argv->getbool (0);
	return new String (mapwday (utc));
      }
      if (quark == QUARK_MAPYMON) {
	bool utc = argv->getbool (0);
	return new String (mapymon (utc));
      }
      if (quark == QUARK_ADDYEAR) {
	long num = argv->getlong (0);
	addyear (num);
	return nullptr;
      }
      if (quark == QUARK_ADDYMON) {
	long num = argv->getlong (0);
	addymon (num);
	return nullptr;
      }
      if (quark == QUARK_SETDATE) {
	String date = argv->getstring (0);
	setdate (date);
	return nullptr;
      }
      if (quark == QUARK_SETWEB) {
	String date = argv->getstring (0);
	setweb (date);
	return nullptr;
      }
    }
    // check for 3 arguments
    if (argc == 3) {
      if (quark == QUARK_SETDATE) {
	long year = argv->getlong (0);
	long ymon = argv->getlong (1);
	long mday = argv->getlong (2);
	setdate (year, ymon, mday);
	return nullptr;
      }
    }
    // check for 6 arguments
    if (argc == 6) {
      if (quark == QUARK_SETDATE) {
	long year = argv->getlong (0);
	long ymon = argv->getlong (1);
	long mday = argv->getlong (2);
	long hour = argv->getlong (3);
	long mins = argv->getlong (4);
	long secs = argv->getlong (5);
	setdate (year, ymon, mday, hour, mins, secs);
	return nullptr;
      }
    }
    // call the time method
    return Time::apply (zobj, nset, quark, argv);
  }
}
