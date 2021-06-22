// ---------------------------------------------------------------------------
// - Time.cpp                                                                -
// - standard object library - time class implementation                     -
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

#include "Math.hpp"
#include "Real.hpp"
#include "Time.hpp"
#include "Strvec.hpp"
#include "Vector.hpp"
#include "Regex.hpp"
#include "Utility.hpp"
#include "Integer.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"
#include "cclk.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - private section                                                       -
  // -------------------------------------------------------------------------

  // the default utc zone
  static const String UTC_ZONE_DEF = "+0000";
  
  // return the absolute value
  static inline t_long abs (const t_long abs) {
    return (abs < 0) ? -abs : abs;
  }

  // -------------------------------------------------------------------------
  // - public section                                                        -
  // -------------------------------------------------------------------------

  // the time now
  const char* Time::TNOW = "now";
  
  // return the time in seconds since the origin

  t_long Time::gettclk (void) {
    return c_time ();
  }

  // return the unix time in seconds

  t_quad Time::getuclk (void) {
    return c_tunx ();
  }

  // return the timezone in seconds
  
  t_long Time::getzone (void) {
    return c_tzone ();
  }
  
  // return a formated time by value

  String Time::totfmt (const t_long wclk, const String& tsep) {
    // compute the seconds
    long secs = (long) (wclk % 60LL);
    // compute minute
    long mins = (long) ((wclk / 60LL) % 60LL);
    // compute hours
    long hour = (long) ((wclk / 3600LL) % 24LL);
    // fix negative time
    if (secs < 0) secs += 60LL;
    if (mins < 0) mins += 60LL;
    if (hour < 0) hour += 24LL;
    // initialize the result
    String result;
    // format the hour
    if (hour < 10) {
      result = result + '0' + hour;
    } else {
      result = result + hour;
    }
    if (result.isnil () == false) result += tsep;
    // format the minute
    if (mins < 10) {
      result = result + '0' + mins;
    } else {
      result = result + mins;
    }
    if (result.isnil () == false) result += tsep;
    // format the seconds
    if (secs < 10) {
      result = result + '0' + secs;
    } else {
      result = result + secs;
    }
    return result;
  }

  // return a formated zone by value
  
  String Time::tozfmt (const long zclk, const String& zsep) {
    // save the sign
    bool negt = (zclk < 0);
    long wclk = negt ? -zclk : zclk;
    // get the minutes
    long mins = (long) ((wclk / 60LL) % 60LL);
    // compute hours
    long hour = (long) ((wclk / 3600LL) % 24LL);
    // format the result
    String result = negt ? '-' : '+';
    // add the hour
    if (hour < 10) result += '0';
    result += hour;
    // add the separator
    if (zsep.isnil () == false) result += zsep;
    // add the minutes
    if (mins < 10) result += '0';
    result += mins;
    // here we are
    return result;
  }

  // return a formated string of the time zone

  String Time::tozone (void) {
    // get the time zone difference
    long zclk = c_tzone ();
    // format the zone
    return Time::tozfmt (zclk, "");
  }

  // -------------------------------------------------------------------------
  // - class section                                                        -
  // -------------------------------------------------------------------------

  // create a current atc clock

  Time::Time (void) {
    settime (TNOW);
  }

  // create a specific time clock

  Time::Time (const t_long tclk) {
    d_tclk = tclk;
    d_eclk = 0.0;
  }
  
  // create a specific time clock with an extended value

  Time::Time (const t_long tclk, const t_real eclk) {
    d_tclk = tclk; d_eclk = 0.0;
    addext (eclk);
  }
  // create a specific time clock

  Time::Time (const long hour, const long mins, const long secs) {
    settime (hour, mins, secs);
    d_eclk = 0.0;
  }

  // copy construct this time

  Time::Time (const Time& that) {
    that.rdlock ();
    try {
      d_tclk = that.d_tclk;
      d_eclk = that.d_eclk;
      that.unlock ();
    } catch (...) {
      that.unlock ();
      throw;
    }
  }

  // return the class name

  String Time::repr (void) const {
    return "Time";
  }

  // return a clone of this object

  Object* Time::clone (void) const {
    return new Time (*this);
  }

  // assign a time object to this one

  Time& Time::operator = (const Time& that) {
    // check for self assignation
    if (this == &that) return *this;
    // lock and assign
    wrlock ();
    that.rdlock ();
    try {
      d_tclk = that.d_tclk;
      d_eclk = that.d_eclk;
      unlock ();
      that.unlock ();
      return *this;
    } catch (...) {
      unlock ();
      that.unlock ();
      throw;
    }
  }
  
  // compare two times

  bool Time::operator == (const Time& that) const {
    rdlock ();
    that.rdlock ();
    try {
      bool status = (d_tclk == that.d_tclk) && (d_eclk == that.d_eclk);
      unlock ();
      that.unlock ();
      return status;
    } catch (...) {
      unlock ();
      that.unlock ();
      throw;
    }
  }
  // compare two times

  bool Time::operator < (const Time& that) const {
    rdlock ();
    that.rdlock ();
    try {
      bool status = (d_tclk < that.d_tclk);
      if (d_tclk == that.d_tclk) status = (d_eclk < that.d_eclk);
      unlock ();
      that.unlock ();
      return status;
    } catch (...) {
      unlock ();
      that.unlock ();
      throw;
    }
  }

  // compare two times

  bool Time::operator <= (const Time& that) const {
    rdlock ();
    that.rdlock ();
    try {
      bool status = (d_tclk <= that.d_tclk);
      if (d_tclk == that.d_tclk) status = (d_eclk <= that.d_eclk);
      unlock ();
      that.unlock ();
      return status;
    } catch (...) {
      unlock ();
      that.unlock ();
      throw;
    }
  }   

  // add a specific time in second to the atc clock

  void Time::add (const t_long tclk) {
    wrlock ();
    try {
      d_tclk += tclk;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // add a time extension to the atc clock

  void Time::addext (const t_real eclk) {
    wrlock ();
    try {
      if (eclk >= 0.0) {
	t_real ival = Math::floor (eclk);
	d_tclk += (t_long) ival;
	d_eclk += (eclk - ival);
      } else {
	t_real ival = Math::floor (-eclk);
	d_tclk -= (t_long) ival;
	d_eclk -= (-eclk - ival);
      }
      if (d_eclk >= 1.0) {
	t_real ival = Math::floor (d_eclk);
	d_tclk += (t_long) ival;
	d_eclk  = (d_eclk - ival);
      }
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // set the atc clock in seconds

  void Time::settime (const t_long tclk) {
    wrlock ();
    try {
      d_tclk = tclk;
      unlock ();
     } catch (...) {
      unlock ();
      throw;
    } 
  }

  // set the atc clock by iso representation

  void Time::settime (const String& time) {
    wrlock ();
    try {
      // check for now
      if (time == TNOW) {
	d_tclk = c_time ();
	d_eclk = 0.0;
	unlock ();
	return;
      }
      // check for zulu time
      if (time.last () != 'Z') {
	throw Exception ("time-error", "missing zulu time postfix", time);
      }
      // split time and extended clock
      String stim = time;
      Strvec svec = Strvec::split (time, ".");
      if (svec.length () > 2) {
	throw Exception ("time-error", "invalid time string", time);
      }
      if (svec.length () == 2) {
	stim = svec.get (0) + "Z";
	String eclk = svec.get (1); eclk = eclk.lsubstr (eclk.length()-1);
	d_eclk = Utility::toreal (String (".") + eclk);
      }
      // create a time regex
      Regex re ("($d$d):($d$d):($d$d)Z");
      // check the time
      if (re == stim) {
	// extract tme information
	long hour = Utility::tolong (re.getstring (0));
	long mins = Utility::tolong (re.getstring (1));
	long secs = Utility::tolong (re.getstring (2));
	// set the atc time
	settime (hour, mins,secs);
	unlock ();
      } else {
	throw Exception ("time-error", "invalid time format", time);
      }
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }
  // set the time by specific elements

  void Time::settime (const long hour, const long mins, const long secs) {
    wrlock ();
    try {
      t_long tclk = hour * HSEC + mins * MSEC + secs;
      settime (tclk);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }
      
  // return the atc clock

  t_long Time::gettime (const bool utc) const {
    rdlock ();
    try {
      t_long result = d_tclk;
      if (utc == false) result += (t_long) c_tzone ();
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the extended clock

  t_real Time::geteclk (const bool utc) const {
    rdlock ();
    try {
      t_real result = (t_real) gettime (utc) + d_eclk;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // get the number of seconds
  
  long Time::getsecs (const bool utc) const {
    rdlock ();
    try {
      // compute the working clock based on the timezone
      t_long wclk = abs (d_tclk);
      if (utc == false) wclk += (t_long) c_tzone ();
      // compute the seconds
      long result = (long) (wclk % 60LL);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the number of minutes

  long Time::getmins (const bool utc) const {
    rdlock ();
    try {
      // compute the working clock based on the timezone
      t_long wclk = abs (d_tclk);
      if (utc == false) wclk += (t_long) c_tzone ();
      // compute the minutes
      long result = (long) ((wclk / 60LL) % 60LL);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }  
  }

  // get the number of hour

  long Time::gethour (const bool utc) const {
    rdlock ();
    try {
      // compute the working clock based on the timezone
      t_long wclk = abs (d_tclk);
      if (utc == false) wclk += (t_long) c_tzone ();
      // compute the hour
      long result = (long) ((wclk / 3600LL) % 24LL);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return the base day reference time
  
  t_long Time::getbday (void) const {
    rdlock ();
    try {
      t_long result = (d_tclk / DSEC) * DSEC;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // format the time
  
  String Time::format (const bool utc) const {
    rdlock ();
    try {
      // compute the working clock based on the timezone
      t_long wclk = abs (d_tclk);
      if (utc == false) wclk += (t_long) c_tzone ();
      // format the time
      String result = Time::totfmt (wclk, ':');
      // unlock and return
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // format the time in a form similar to ISO-8601
  
  String Time::toiso (const bool utc) const {
    rdlock ();
    try {
      // get the time representation (local)
      String result = Time::format (utc);
      // add the suffix if any
      if (utc == true) result += 'Z';
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // format the time in a form similar to RFC-2822

  String Time::torfc (const bool utc) const {
    rdlock ();
    try {
      // get the time representation (local)
      String result = Time::format (utc);
      // add the timezone difference (including daylight if any)
      result += ' ';
      if (utc == true) {
	result += UTC_ZONE_DEF;
      } else {
	result += Time::tozone ();
      }
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // format the time in an extended form
  
  String Time::toext (const bool utc) const {
    rdlock ();
    try {
      // get the time representation (local)
      String result = Time::format (utc);
      // add the fraction
      result += '.';
      result += Strvec::split (Utility::tostring (d_eclk), ".").get(1);
      // add the suffix if any
      if (utc == true) result += 'Z';
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // -------------------------------------------------------------------------
  // - object section                                                        -
  // -------------------------------------------------------------------------

  // the quark zone
  static const long QUARK_ZONE_LENGTH = 16;
  static QuarkZone  zone (QUARK_ZONE_LENGTH);

  // the object supported quarks
  static const long QUARK_ADD     = zone.intern ("add");
  static const long QUARK_TOISO   = zone.intern ("to-iso");
  static const long QUARK_TORFC   = zone.intern ("to-rfc");
  static const long QUARK_TOEXT   = zone.intern ("to-extended");
  static const long QUARK_ADDEXT  = zone.intern ("add-extended");
  static const long QUARK_FORMAT  = zone.intern ("format");
  static const long QUARK_ADDMNS  = zone.intern ("add-minutes");
  static const long QUARK_ADDHRS  = zone.intern ("add-hours");
  static const long QUARK_ADDDAYS = zone.intern ("add-days");
  static const long QUARK_SETTIME = zone.intern ("set-time");
  static const long QUARK_GETTIME = zone.intern ("get-time");
  static const long QUARK_GETECLK = zone.intern ("get-extended");
  static const long QUARK_GETSECS = zone.intern ("seconds");
  static const long QUARK_GETMINS = zone.intern ("minutes");
  static const long QUARK_GETHOUR = zone.intern ("hours");
  static const long QUARK_GETBDAY = zone.intern ("get-base-day");

  // create a new object in a generic way
 
  Object* Time::mknew (Vector* argv) {
    long argc = (argv == nullptr) ? 0 : argv->length ();
    // create a default time object
    if (argc == 0) return new Time;
    if (argc == 1) {
      t_long tval = argv->getlong (0);
      return new Time (tval);
    }
    if (argc == 2) {
      t_long tval = argv->getlong (0);
      t_real eclk = argv->getreal (1);
      return new Time (tval, eclk);
    }
    if (argc == 3) {
      long hour = argv->getlong (0);
      long mins = argv->getlong (1);
      long secs = argv->getlong (2);
      return new Time (hour, mins, secs);
    }
    throw Exception ("argument-error",
                     "too many argument with time constructor");
  }
 
  // return true if the given quark is defined

  bool Time::isquark (const long quark, const bool hflg) const {
    rdlock ();
    try {
      if (zone.exists (quark) == true) {
	unlock ();
	return true;
      }
      bool result = hflg ? Object::isquark (quark, hflg) : false;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // apply this object with a set of arguments and a quark
  
  Object* Time::apply (Evaluable* zobj, Nameset* nset, const long quark,
		       Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();

    // check for 0 arguments
    if (argc == 0) {
      if (quark == QUARK_TOISO)   return new String  (toiso   (false));
      if (quark == QUARK_TORFC)   return new String  (torfc   (false));
      if (quark == QUARK_TOEXT)   return new String  (toext   (false));
      if (quark == QUARK_FORMAT)  return new String  (format  (false));
      if (quark == QUARK_GETTIME) return new Integer (gettime (true));
      if (quark == QUARK_GETECLK) return new Real    (geteclk (true));
      if (quark == QUARK_GETSECS) return new Integer (getsecs (true));
      if (quark == QUARK_GETMINS) return new Integer (getmins (true));
      if (quark == QUARK_GETHOUR) return new Integer (gethour (true));
      if (quark == QUARK_GETBDAY) return new Integer (getbday ());
    }
    // check for 1 argument
    if (argc == 1) {
      if (quark == QUARK_ADD) {
	t_long tval = argv->getlong (0);
	add (tval);
	return nullptr;
      }
      if (quark == QUARK_ADDEXT) {
	t_real eclk = argv->getreal (0);
	addext (eclk);
	return nullptr;
      }
      if (quark == QUARK_GETTIME) {
	bool utc = argv->getbool (0);
	return new Integer (gettime (utc));
      }
      if (quark == QUARK_GETECLK) {
	bool utc = argv->getbool (0);
	return new Real (geteclk (utc));
      }
      if (quark == QUARK_SETTIME) {
	t_long tclk = argv->getlong (0);
	settime (tclk);
	return nullptr;
      }
      if (quark == QUARK_ADDMNS) {
	t_long num = argv->getlong (0);
	t_long sec = num * (t_long) MSEC;
	add (sec);
	return nullptr;
      }
      if (quark == QUARK_ADDHRS) {
	t_long num = argv->getlong (0);
	t_long sec = num * (t_long) HSEC;
	add (sec);
	return nullptr;
      }
      if (quark == QUARK_ADDDAYS) {
	t_long num = argv->getlong (0);
	t_long sec = num * (t_long) DSEC;
	add (sec);
	return nullptr;
      }
      if (quark == QUARK_FORMAT) {
	bool utc = argv->getbool (0);
	return new String (format (utc));
      }
      if (quark == QUARK_TOISO) {
	bool utc = argv->getbool (0);
	return new String (toiso (utc));
      }
      if (quark == QUARK_TORFC) {
	bool utc = argv->getbool (0);
	return new String (torfc (utc));
      }
      if (quark == QUARK_TOEXT) {
	bool utc = argv->getbool (0);
	return new String (toext (utc));
      }
      if (quark == QUARK_GETSECS) {
	bool utc = argv->getbool (0);
	return new Integer (getsecs (utc));
      }
      if (quark == QUARK_GETMINS) {
	bool utc = argv->getbool (0);
	return new Integer (getmins (utc));
      }
      if (quark == QUARK_GETHOUR) {
	bool utc = argv->getbool (0);
	return new Integer (gethour (utc));
      }
    }
    // call the object method
    return Object::apply (zobj, nset, quark, argv);
  }
}
