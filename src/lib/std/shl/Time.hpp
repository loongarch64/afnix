// ---------------------------------------------------------------------------
// - Time.hpp                                                                -
// - standard object library - time class definition                         -
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

#ifndef  AFNIX_TIME_HPP
#define  AFNIX_TIME_HPP

#ifndef  AFNIX_STRING_HPP
#include "String.hpp"
#endif

namespace afnix {

  /// The Time class is a simple class used to manipulate time. The afnix
  /// system operates with the afnix time coordinated or ATC which uses the
  /// reference of Jan 1st 0000 in a modified proleptic gregorian calendar.
  /// Note that the time can be negative. Although, the class provides several
  /// methods to access the time fields, it is also ossible to get a string
  /// representation that conforms to ISO-8601 or to RFC-2822.
  /// @author amaury darsch

  class Time : public virtual Object {
  public:
    /// the number of seconds per minute
    static const long  MSEC = 60;
    /// the number of minute per hour
    static const long  HMNS = 60;
    /// the number of seconds per hour
    static const long  HSEC = HMNS * MSEC;
    /// the number of hours per day
    static const long  DHRS = 24;
    /// the number of seconds per day
    static const long  DSEC = DHRS * HSEC;
    /// the time now
    static const char* TNOW;
    
    /// @return the current atc time
    static t_long gettclk (void);

    /// @return the current unix time
    static t_quad getuclk (void);

    /// @return the current timezone difference
    static t_long getzone (void);

    /// format a wall time by value and separator
    /// @param wclk the clock to format
    /// @param tsep the time separator
    static String totfmt (const t_long wclk, const String& tsep);

    /// format a zone time by value and separator
    /// @param wclk the zone to format
    /// @param tsep the zone separator
    static String tozfmt (const long zclk, const String& zsep);

    /// @return a formated timezone
    static String tozone (void);

  protected:
    /// the atc time
    t_long d_tclk;
    /// the extension time
    t_real d_eclk;
    
  public:
    /// create the current time clock
    Time (void);

    /// create a specific time clock
    /// @param tclk the atc time to use
    Time (const t_long tclk);

    /// create a specific time clock
    /// @param tclk the atc time to use
    /// @param eclk the extended time
    Time (const t_long tclk, const t_real eclk);
    
    /// create a specific time clock
    /// @param hour the time hour
    /// @param mins the time minutes
    /// @param secs the time seconds
    Time (const long hour, const long mins, const long secs);

    /// copy construct this time
    /// @param that the time to copy
    Time (const Time& that);

    /// @return the class name
    String repr (void) const override;

    /// @return a clone of this object
    Object* clone (void) const override;

    /// assign a time to this one
    /// @param that the time object to assign
    Time& operator = (const Time& that);

    /// compare two times
    /// @param that the time to compare
    bool operator == (const Time& that) const;

    /// compare two times
    /// @param that the time to compare
    bool operator < (const Time& that) const;
    
    /// compare two times
    /// @param that the time to compare
    bool operator <= (const Time& that) const;
    
    /// add a certain time to the atc clock
    /// @param tclk the time to add in seconds
    virtual void add (const t_long tclk);

    /// add a time extension to the atc clock
    /// @param eclk the extension to add
    virtual void addext (const t_real eclk);

    /// set the atc clock in seconds
    /// @param tclk the time to set
    virtual void settime (const t_long tclk);

    /// set the atc clock by iso representation
    /// @param time the iso time representation
    virtual void settime (const String& time);
    
    /// set the time by elements
    /// @param hour the time hour
    /// @param mins the time minutes
    /// @param secs the time seconds
    virtual void settime (const long hour, const long mins, const long secs);

    /// get the atc clock in seconds
    /// @param utc the utc flag
    virtual t_long gettime (const bool utc) const;

    /// get the extended clock in seconds
    /// @param utc the utc flag
    virtual t_real geteclk (const bool utc) const;

    /// @return the number of seconds
    virtual long getsecs (const bool utc) const;

    /// @return the number of minutes
    virtual long getmins (const bool utc) const;

    /// @return the number of hours
    virtual long gethour (const bool utc) const;

    /// @return the base day reference time
    virtual t_long getbday (void) const;

    /// format the time in hh:mm:ss format
    /// @param utc return in utc form if true
    virtual String format (const bool utc) const;
    
    /// format the time in ISO-8601 like form
    /// @param utc return in utc form if true
    virtual String toiso (const bool utc) const;

    /// format the time in RFC-2822 like form
    /// @param utc return in utc form if true
    virtual String torfc (const bool utc) const;

    /// format the time as an extended time
    /// @param utc return in utc form if true
    virtual String toext (const bool utc) const;
    
  public:
    /// create a new object in a generic way
    /// @param argv the argument vector
    static Object* mknew (Vector* argv);

    /// @return true if the given quark is defined
    bool isquark (const long quark, const bool hflg) const override;

    /// apply this object with a set of arguments and a quark
    /// @param zobj  the current evaluable
    /// @param nset  the current nameset    
    /// @param quark the quark to apply these arguments
    /// @param argv  the arguments to apply
    Object* apply (Evaluable* zobj, Nameset* nset, const long quark,
		   Vector* argv) override;
  };
}

#endif
