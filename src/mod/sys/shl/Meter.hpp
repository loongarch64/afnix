// ---------------------------------------------------------------------------
// - Meter.hpp                                                               -
// - afnix:sys module - performance meter class definition                   -
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

#ifndef  AFNIX_METER_HPP
#define  AFNIX_METER_HPP

#ifndef  AFNIX_PLIST_HPP
#include "Plist.hpp"
#endif

#ifndef  AFNIX_INFOABLE_HPP
#include "Infoable.hpp"
#endif

namespace afnix {

  /// The Meter class is a performance meter storage class. The class can
  /// be used for time performance analysis as well as general purpose
  /// metering. In both cases, a set of counters are allocated by the class.
  /// By default, there is only time meter and no general purpose counters.
  /// For time measurement, the stamp method without argument is used to set
  /// the reference time.
  /// @author amaury darsch

  class Meter : public Infoable {
  protected:
    /// the number of time meters
    long    d_mnum;
    /// the number of counters
    long    d_cnum;
    /// the reference time
    t_long* p_tref;
    /// the time meters
    t_long* p_pmtr;
    /// the performance counters
    t_long* p_pcnt;
    /// the time meter name
    String* p_nmtr;
    /// the time meter info
    String* p_imtr;
    /// the counter name
    String* p_ncnt;
    /// the counter info
    String* p_icnt;

  public:
    /// create a default meter
    Meter (void);

    /// create a meter by counter number
    /// @param cnum the number of counters
    Meter (const long cnum);

    /// create a meter by counter and time numbers
    /// @param cnum the number of counters
    /// @param tnum the number of time meters
    Meter (const long cnum, const long tnum);

    /// destroy this performance meter
    ~Meter (void);

    /// @return the class name
    String repr (void) const;

    /// reset the performance meter
    virtual void reset (void);

    /// @return the number of time meters
    virtual long getmnum (void) const;

    /// @return the number of performance counters
    virtual long getcnum (void) const;

    /// set the reference time
    virtual t_long settref (void);

    /// set the reference time
    /// @param midx the meter index
    virtual t_long settref (const long midx);

    /// get the reference time
    /// @param midx the meter index
    virtual t_long gettref (const long midx) const;

    /// stamp a time meter by index
    /// @param midx the meter index
    virtual t_long stamp (const long midx);

    /// mark a performance counter by index
    /// @param cidx the counter index
    virtual t_long mark (const long cidx);

    /// get the performance meter by index
    /// @param midx the meter index
    virtual t_long getpmtr (const long midx) const;

    /// get the performance meter relative time by index
    /// @param midx the meter index
    virtual t_long getprtm (const long midx) const;

    /// get the performance counter by index
    /// @param cidx the counter index
    virtual t_long getpcnt (const long midx) const;

    /// set the time meter name and info by index
    /// @param midx the meter index
    /// @param name the meter name
    /// @param info the meter info
    virtual void setnimtr (const long    midx,
			   const String& name, const String& info);

    /// set the counter name and info by index
    /// @param cidx the counter index
    /// @param name the counter name
    /// @param info the counter info
    virtual void setnicnt (const long    cidx,
			   const String& name, const String& info);

    /// @return a performance meter plist
    virtual Plist getplst (void) const;

  private:
    // make the copy constructor private
    Meter (const Meter&);
    // make the assignment operator private
    Meter& operator = (const Meter&);

  public:
    /// create a new object in a generic way
    /// @param argv the argument vector
    static Object* mknew (Vector* argv);

    /// @return true if the given quark is defined
    bool isquark (const long quark, const bool hflg) const;

    /// apply this object with a set of arguments and a quark
    /// @param zobj  the current evaluable
    /// @param nset  the current nameset    
    /// @param quark the quark to apply these arguments
    /// @param argv  the arguments to apply
    Object* apply (Evaluable* zobj, Nameset* nset, const long quark,
		   Vector* argv);
  };
}

#endif
