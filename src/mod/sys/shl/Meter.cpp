// ---------------------------------------------------------------------------
// - Meter.cpp                                                               -
// - afnix:sys module - performance meter class implementation               -
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
#include "Meter.hpp"
#include "Vector.hpp"
#include "Integer.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a default performance meter

  Meter::Meter (void) {
    d_mnum = 1L;
    d_cnum = 0L;
    p_tref = new t_long[d_mnum];
    p_pmtr = new t_long[d_mnum];
    p_pcnt = nullptr;
    p_nmtr = new String[d_mnum];
    p_imtr = new String[d_mnum];
    p_ncnt = nullptr;
    p_icnt = nullptr;
    reset ();
  }

  // create a performance meter by counter number

  Meter::Meter (const long cnum) {
    d_mnum = 1L;
    d_cnum = (cnum < 0L) ? 0L : cnum;
    p_tref = new t_long[d_mnum];
    p_pmtr = new t_long[d_mnum];
    p_pcnt = (cnum == 0L) ? nullptr : new t_long[d_cnum];
    p_nmtr = new String[d_mnum];
    p_imtr = new String[d_mnum];
    p_ncnt = (cnum == 0L) ? nullptr : new String[d_cnum];
    p_icnt = (cnum == 0L) ? nullptr : new String[d_cnum];
    reset ();
  }

  // create a performance meter by time and counter number

  Meter::Meter (const long cnum, const long mnum) {
    d_mnum = (mnum < 0L) ? 0L : mnum;
    d_cnum = (cnum < 0L) ? 0L : cnum;
    p_tref = (mnum == 0L) ? nullptr : new t_long[d_mnum];
    p_pmtr = (mnum == 0L) ? nullptr : new t_long[d_mnum];
    p_pcnt = (cnum == 0L) ? nullptr : new t_long[d_cnum];
    p_nmtr = (mnum == 0L) ? nullptr : new String[d_mnum];
    p_imtr = (mnum == 0L) ? nullptr : new String[d_mnum];
    p_ncnt = (cnum == 0L) ? nullptr : new String[d_cnum];
    p_icnt = (cnum == 0L) ? nullptr : new String[d_cnum];
    reset ();
  }

  // destroy this performance meter 

  Meter::~Meter (void) {
    delete [] p_tref;
    delete [] p_pmtr;
    delete [] p_pcnt;
    delete [] p_nmtr;
    delete [] p_imtr;
    delete [] p_ncnt;
    delete [] p_icnt;
  }

  // return the class name

  String Meter::repr (void) const {
    return "Meter";
  }

  // reset the performance meter

  void Meter::reset (void) {
    wrlock ();
    try {
      for (long k = 0L; k < d_mnum; k++) p_tref[k] = 0LL;
      for (long k = 0L; k < d_mnum; k++) p_pmtr[k] = 0LL;
      for (long k = 0L; k < d_cnum; k++) p_pcnt[k] = 0LL;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the number of time meters

  long Meter::getmnum (void) const {
    rdlock ();
    try {
      long result = d_mnum;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the number of performance counters

  long Meter::getcnum (void) const {
    rdlock ();
    try {
      long result = d_cnum;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
     
  // set the reference time

  t_long Meter::settref (void) {
    wrlock ();
    try {
      // reset the time meters
      for (long k = 0L; k < d_mnum; k++) p_pmtr[k] = 0LL;
      // set the reference time
      t_long result = Time::gettclk ();
      for (long k = 0L; k < d_mnum; k++) p_tref[k] = result;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    } 
  }

  // set the reference time by index

  t_long Meter::settref (const long midx) {
    wrlock ();
    try {
      // check index
      if ((midx < 0L) || (midx >= d_mnum)) {
	throw Exception ("meter-error", "invalid time meter index in stamp");
      }
      // reset the time meters
      p_pmtr[midx] = 0ULL;
      // set the reference time
      t_long result = p_tref[midx] = Time::gettclk ();
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    } 
  }

  // get the reference time

  t_long Meter::gettref (const long midx) const {
    rdlock ();
    try {
      // check index
      if ((midx < 0L) || (midx >= d_mnum)) {
	throw Exception ("meter-error", "invalid time meter index in stamp");
      }
      t_long result = p_tref[midx];
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // stamp a time meter by index

  t_long Meter::stamp (const long midx) {
    wrlock ();
    try {
      // check index
      if ((midx < 0L) || (midx >= d_mnum)) {
	throw Exception ("meter-error", "invalid time meter index in stamp");
      }
      t_long result = p_pmtr[midx] = Time::gettclk ();
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    } 
  }

  // mark a performance counter

  t_long Meter::mark (const long cidx) {
    wrlock ();
    try {
      // check index
      if ((cidx < 0L) || (cidx >= d_cnum)) {
	throw Exception ("meter-error", "invalid counter index in mark");
      }
      t_long result = ++p_pcnt[cidx];
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    } 
  }

  // get the performance meter by index
  
  t_long Meter::getpmtr (const long midx) const {
    rdlock ();
    try {
      // check index
      if ((midx < 0L) || (midx >= d_mnum)) {
	throw Exception ("meter-error", "invalid time meter index in get");
      }
      t_long result = p_pmtr[midx];
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    } 
  }

  // get the performance relative time by index

  t_long Meter::getprtm (const long midx) const {
    rdlock ();
    try {
      // check index
      if ((midx < 0L) || (midx >= d_mnum)) {
	throw Exception ("meter-error", "invalid time meter index in get");
      }
      t_long   pmtr = p_pmtr[midx];
      t_long   tref = p_tref[midx];
      t_long result = (pmtr > tref) ? (pmtr - tref) : 0LL;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    } 
  }
  
  // get the performance counter by index
  
  t_long Meter::getpcnt (const long cidx) const {
    rdlock ();
    try {
      // check index
      if ((cidx < 0L) || (cidx >= d_cnum)) {
	throw Exception ("meter-error", "invalid counter index in get");
      }
      t_long result = p_pcnt[cidx];
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    } 
  }

  // set the time meter name and info by index

  void Meter::setnimtr (const long    midx, 
			const String& name, const String& info) {
    wrlock ();
    try {
      // check index
      if ((midx < 0L) || (midx >= d_mnum)) {
	throw Exception ("meter-error", "invalid time meter index in setni");
      }
      p_nmtr[midx] = name;
      p_imtr[midx] = info;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set the time meter name and info by index

  void Meter::setnicnt (const long    cidx, 
			const String& name, const String& info) {
    wrlock ();
    try {
      // check index
      if ((cidx < 0L) || (cidx >= d_cnum)) {
	throw Exception ("meter-error", "invalid counter index in setni");
      }
      p_ncnt[cidx] = name;
      p_icnt[cidx] = info;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // create a performance meter plist

  Plist Meter::getplst (void) const {
    rdlock ();
    try {
      // create a result plist
      Plist result (d_name, d_info);
      // fill in by meters
      for (long k = 0L; k < d_mnum; k++) {
	t_long pmtr = p_pmtr[k];
	t_long tref = p_tref[k];
	t_long rtim = (pmtr > tref) ? (pmtr - tref) : 0LL;
	result.add (p_nmtr[k], p_imtr[k], rtim);
      }
      // fill in by counters
      for (long k = 0L; k < d_cnum; k++) {
	result.add (p_ncnt[k], p_icnt[k], p_pcnt[k]);
      }
      // here it is
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
  static const long QUARK_ZONE_LENGTH = 13;
  static QuarkZone  zone (QUARK_ZONE_LENGTH);

  // the object supported quarks
  static const long QUARK_MARK     = zone.intern ("mark");
  static const long QUARK_RESET    = zone.intern ("reset");
  static const long QUARK_STAMP    = zone.intern ("stamp");
  static const long QUARK_GETPMTR  = zone.intern ("get-meter");
  static const long QUARK_GETPCNT  = zone.intern ("get-counter");
  static const long QUARK_GETPLST  = zone.intern ("get-plist");
  static const long QUARK_GETPRTM  = zone.intern ("get-meter-relative");
  static const long QUARK_GETMNUM  = zone.intern ("get-meter-number");
  static const long QUARK_GETCNUM  = zone.intern ("get-counter-number");
  static const long QUARK_SETTREF  = zone.intern ("set-reference-time");
  static const long QUARK_GETTREF  = zone.intern ("get-reference-time");
  static const long QUARK_SETNIMTR = zone.intern ("set-meter-name-info");
  static const long QUARK_SETNICNT = zone.intern ("set-counter-name-info");

  // create a new object in a generic way

  Object* Meter::mknew (Vector* argv) {
    long argc = (argv == nullptr) ? 0 : argv->length ();
    // check for 0 arguments
    if (argc == 0) return new Meter;
    // check for 1 argument
    if (argc == 1) {
      long cnum = argv->getlong (0);
      return new Meter (cnum);
    }
    // check for 2 arguments
    if (argc == 2) {
      long cnum = argv->getlong (0);
      long mnum = argv->getlong (1);
      return new Meter (cnum, mnum);
    }
    throw Exception ("argument-error",
		     "too many argument with meter constructor");
  }
  
  // return true if the given quark is defined

  bool Meter::isquark (const long quark, const bool hflg) const {
    rdlock ();
    try {
      if (zone.exists (quark) == true) {
	unlock ();
	return true;
      }
      bool result = hflg ? Infoable::isquark (quark, hflg) : false;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // apply this object with a set of arguments and a quark
  
  Object* Meter::apply (Evaluable* zobj, Nameset* nset, const long quark,
			Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();
 
    // check for 0 argument
    if (argc == 0) {
      if (quark == QUARK_GETMNUM) return new Integer (getmnum ());
      if (quark == QUARK_GETCNUM) return new Integer (getcnum ());
      if (quark == QUARK_SETTREF) return new Integer (settref ());
      if (quark == QUARK_GETPLST) return new Plist   (getplst ());
      if (quark == QUARK_RESET) {
	reset ();
	return nullptr;
      }
    }
    // check for 1 argument
    if (argc == 1) {
      if (quark == QUARK_MARK) {
	long cidx = argv->getlong (0);
	return new Integer (mark (cidx));
      }
      if (quark == QUARK_STAMP) {
	long midx = argv->getlong (0);
	return new Integer (stamp (midx));
      }
      if (quark == QUARK_GETPMTR) {
	long midx = argv->getlong (0);
	return new Integer (getpmtr (midx));
      }
      if (quark == QUARK_GETPRTM) {
	long midx = argv->getlong (0);
	return new Integer (getprtm (midx));
      }
      if (quark == QUARK_GETPCNT) {
	long cidx = argv->getlong (0);
	return new Integer (getpcnt (cidx));
      }
      if (quark == QUARK_SETTREF) {
	long midx = argv->getlong (0);
	return new Integer (settref (midx));
      }
      if (quark == QUARK_GETTREF) {
	long midx = argv->getlong (0);
	return new Integer (gettref (midx));
      }
    }
    // check for 3 arguments
    if (argc == 3) {
      if (quark == QUARK_SETNIMTR) {
	long   midx = argv->getlong   (0);
	String name = argv->getstring (1);
	String info = argv->getstring (2);
	setnimtr (midx, name, info);
	return nullptr;
      }
      if (quark == QUARK_SETNICNT) {
	long   cidx = argv->getlong   (0);
	String name = argv->getstring (1);
	String info = argv->getstring (2);
	setnicnt (cidx, name, info);
	return nullptr;
      }
    }
    // call the nameable method 
    return Infoable::apply (zobj, nset, quark, argv);
  }
}
