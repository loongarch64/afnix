// ---------------------------------------------------------------------------
// - Date.hpp                                                                -
// - standard object library - date class definition                         -
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

#ifndef  AFNIX_DATE_HPP
#define  AFNIX_DATE_HPP

#ifndef  AFNIX_TIME_HPP
#include "Time.hpp"
#endif

namespace afnix {

  /// The Date class is a derived class designed to manipulate dates. The 
  /// date computation is based on an modified proleptic gregorian calendar.
  /// This proleptic feature means that the actual calendar (gregorian) is 
  /// extended beyond year 1582 (its introduction year) and modified in order
  /// to support the year 0. This kind of calendar is somehow similar to the 
  /// astronomical gregorian calendar except that the reference date is 0 
  /// for the atc system (afnix time coordinated). This method presents the
  /// advantage to support negative time. It should be noted that the 0 
  /// reference does not means year 1BC since year 0 did not exist at that 
  /// time (the concept of zero is fairly new) and more important, the date 
  /// expressed in the form 1BC generally refers to the Julian calendar 
  /// since the date is before 1582. Although, the class provides several 
  /// methods to access the individual fields, it is also possible to get a
  /// string representation that conforms to ISO-8601 or to RFC-2822. 
  /// @author amaury darsch

  class Date : public Time {
  public:
    /// format a wall date by value and separators
    /// @param wclk the wall clock date to format
    /// @param fsep the field separator
    /// @param dsep the date separator
    /// @param tsep the time separator
    static String todfmt (const t_long  wclk, const String& fsep,
			  const String& dsep, const String& tsep);
  public:
    /// create a current date
    Date (void);

    /// create a specific date
    /// @param tclk the specific time
    Date (const t_long tclk);

    /// set a date by iso specification
    /// @param date the iso specification date
    Date (const String& date);
    
    /// create a new date by specific elements
    /// @param year the specific year
    /// @param ymon the specific month in year
    /// @param mday the specific day in month
    Date (const long year, const long ymon, const long mday);

    /// create a new date by specific elements
    /// @param year the specific year
    /// @param ymon the specific month in year
    /// @param mday the specific day in month
    /// @param hour the specific hour
    /// @param mins the specific minutes
    /// @param secs the specific seconds
    Date (const long year, const long ymon, const long mday,
	  const long hour, const long mins, const long secs);

    /// copy constructor for this date
    /// @param that the date to copy
    Date (const Date& that);

    /// @return the class name
    String repr (void) const override;

    /// @return a clone of this object
    Object* clone (void) const override;

    /// assign a date object to this one
    /// @param that the date object to assign
    Date& operator = (const Date& that);

    /// format the date in a general form
    /// @param utc return in utc form if true
    String format (const bool utc) const override;

    /// format the time in ISO-8601 like form
    /// @param utc return in utc form if true
    String toiso (const bool utc) const override;
    
    /// format the date in RFC-2822 like form
    /// @param utc return in utc form if true
    String torfc (const bool utc) const override;

    /// format the date as an extended time
    /// @param utc return in utc form if true
    String toext (const bool utc) const override;
    
    /// set the date by an iso specification
    /// @param the date string to convert
    virtual void setdate (const String& date);

    /// set the date by specific elements
    /// @param year the specific year
    /// @param ymon the specific month in year
    /// @param mday the specific day in month
    virtual void setdate (const long year, const long ymon, const long mday);

    /// set the date by specific elements
    /// @param year the specific year
    /// @param ymon the specific month in year
    /// @param mday the specific day in month
    /// @param hour the specific hour
    /// @param mins the specific minutes
    /// @param secs the specific seconds
    virtual void setdate (const long year, const long ymon, const long mday,
			  const long hour, const long mins, const long secs);

    /// add year to the date
    /// @param num the number of years to add
    virtual void addyear (const long num);

    /// add months to the date
    /// @param num the number of months to add
    virtual void addymon (const long num);

    /// get the date year
    /// @param utc return in utc form if true
    virtual long getyear (const bool utc) const;

    /// get the day in the year
    /// @param utc return in utc form if true
    virtual long getyday (const bool utc) const;

    /// get the month in the year
    /// @param utc return in utc form if true
    virtual long getymon (const bool utc) const;

    /// get the day in the month
    /// @param utc return in utc form if true
    virtual long getmday (const bool utc) const;

    /// get the day index in the week
    /// @param utc return in utc form if true
    virtual long getwday (const bool utc) const;

    /// get the day name
    /// @param utc return in utc form if true
    virtual String mapwday (const bool utc) const;

    /// get the month name
    /// @param utc return in utc form if true
    virtual String mapymon (const bool utc) const;

    /// get a date string representation
    /// @param utc return in utc form if true
    virtual String todate (const bool utc) const;

    /// get a time string representation
    /// @param utc return in utc form if true
    virtual String totime (const bool utc) const;

    /// format the time in web form (aka RFC-1223)
    /// @param utc return in utc form if true
    virtual String toweb (void) const;
    
    /// set the date by a web specification
    /// @param the date string to convert
    virtual void setweb (const String& date);

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
