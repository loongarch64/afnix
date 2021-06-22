// ---------------------------------------------------------------------------
// - Mean.cpp                                                                -
// - afnix:mth module - samples mean class implementation                    -
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

#include "Mean.hpp"
#include "Real.hpp"
#include "Vector.hpp"
#include "Integer.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a default mean

  Mean::Mean (void) {
    d_cval = 0.0;
    d_snum = 0L;
  }

  // copy construct this object

  Mean::Mean (const Mean& that) {
    that.rdlock ();
    try {
      d_cval = that.d_cval;
      d_snum = that.d_snum;
      that.unlock ();
    } catch (...) {
      that.unlock ();
      throw;
    }
  }
  
  // copy move this object

  Mean::Mean (Mean&& that) {
    that.wrlock ();
    try {
      d_cval = that.d_cval; that.d_cval = 0.0;
      d_snum = that.d_snum; that.d_snum = 0L;
      that.unlock ();
    } catch (...) {
      that.unlock ();
      throw;
    }
  }

  // assign a mean to this obejct

  Mean& Mean::operator = (const Mean& that) {
    // check for self-assignation
    if (this == &that) return *this;
    // lock and assign
    wrlock ();
    that.rdlock ();
    try {
      d_cval = that.d_cval;
      d_snum = that.d_snum;
      unlock ();
      that.unlock ();
      return *this;
    } catch (...) {
      unlock ();
      that.unlock ();
      throw;
    }
  }

  // move a mean to this obejct

  Mean& Mean::operator = (Mean&& that) {
    // check for self-assignation
    if (this == &that) return *this;
    // lock and assign
    wrlock ();
    that.wrlock ();
    try {
      d_cval = that.d_cval; that.d_cval = 0.0;
      d_snum = that.d_snum; that.d_snum = 0L;
      unlock ();
      that.unlock ();
      return *this;
    } catch (...) {
      unlock ();
      that.unlock ();
      throw;
    }
  }

  // return the class name

  String Mean::repr (void) const {
    return "Mean";
  }

  // return a clone of this object

  Object* Mean::clone (void) const {
    return new Mean (*this);
  }

  // reset this mean

  void Mean::reset (void) {
    wrlock ();
    try {
      d_cval = 0.0;
      d_snum = 0L;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the cumulative value

  t_real Mean::getcval (void) const {
    rdlock();
    try {
      t_real result = d_cval;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the number of samples

  long Mean::getsnum (void) const {
    rdlock();
    try {
      long result = d_snum;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // get the mean value

  t_real Mean::getmean (void) const {
    rdlock();
    try {
      t_real result = (d_snum == 0L) ? 0L : d_cval / d_snum;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // push a sample value

  void Mean::push (const t_real sval) {
    wrlock ();
    try {
      d_cval += sval;
      d_snum++;
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
  static const long QUARK_ZONE_LENGTH = 5;
  static QuarkZone  zone (QUARK_ZONE_LENGTH);

  // the object supported quarks
  static const long QUARK_PUSH    = zone.intern ("push");
  static const long QUARK_RESET   = zone.intern ("reset");
  static const long QUARK_GETMEAN = zone.intern ("get-mean");
  static const long QUARK_GETCVAL = zone.intern ("get-cumulative-value");
  static const long QUARK_GETSNUM = zone.intern ("get-samples");

  // create a new object in a generic way

  Object* Mean::mknew (Vector* argv) {
    long argc = (argv == nullptr) ? 0 : argv->length ();
    
    // check for 0 argument
    if (argc == 0) return new Mean;
    // invalid arguments
    throw Exception ("argument-error", "invalid arguments with mean object");
  }

  // return true if the given quark is defined

  bool Mean::isquark (const long quark, const bool hflg) const {
    rdlock ();
    try {
      if (zone.exists (quark) == true){
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
  
  Object* Mean::apply (Evaluable* zobj, Nameset* nset, const long quark,
		       Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();

    // dispatch 0 argument
    if (argc == 0) {
      if (quark == QUARK_GETMEAN) return new Real (getmean());
      if (quark == QUARK_GETCVAL) return new Real (getcval());
      if (quark == QUARK_GETSNUM) return new Integer (getsnum());
      if (quark == QUARK_RESET) {
	reset ();
	return nullptr;
      }
    }
    // dispatch 1 arguments
    if (argc == 1) {
      if (quark == QUARK_PUSH) {
	t_real sval = argv->getrint(0);
	push (sval);
	return nullptr;
      }
    }
    // call the object methods
    return Object::apply (zobj, nset, quark, argv);
  }
}
