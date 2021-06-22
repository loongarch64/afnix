// ---------------------------------------------------------------------------
// - Cov.cpp                                                          -
// - afnix:mth module - samples cov class implementation              -
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

#include "Cov.hpp"
#include "Real.hpp"
#include "Vector.hpp"
#include "Integer.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a default covariance

  Cov::Cov (void) {
    d_cxv  = 0.0;
    d_cyv  = 0.0;
    d_cxxv = 0.0;
    d_cyyv = 0.0;
    d_cxyv = 0.0;
    d_snum = 0L;
  }

  // copy construct this object

  Cov::Cov (const Cov& that) {
    that.rdlock ();
    try {
      d_cxv  = that.d_cxv;
      d_cyv  = that.d_cyv;
      d_cxxv = that.d_cxxv;
      d_cyyv = that.d_cyyv;
      d_cxyv = that.d_cxyv;
      d_snum = that.d_snum;
      that.unlock ();
    } catch (...) {
      that.unlock ();
      throw;
    }
  }

  // copy move this obejct

  Cov::Cov (Cov&& that) {
    that.wrlock ();
    try {
      d_cxv  = that.d_cxv;  that.d_cxv  = 0.0;
      d_cyv  = that.d_cyv;  that.d_cyv  = 0.0;
      d_cxxv = that.d_cxyv; that.d_cxxv = 0.0;
      d_cyyv = that.d_cxyv; that.d_cyyv = 0.0;
      d_cxyv = that.d_cxyv; that.d_cxyv = 0.0;
      d_snum = that.d_snum; that.d_snum = 0L;
      that.unlock ();
    } catch (...) {
      that.unlock ();
      throw;
    }
  }
  
  // assign a covariance to this object

  Cov& Cov::operator = (const Cov& that) {
    // check for self-assignation
    if (this == &that) return *this;
    // lock and assign
    wrlock ();
    that.rdlock ();
    try {
      d_cxv  = that.d_cxv;
      d_cyv  = that.d_cyv;
      d_cxxv = that.d_cxxv;
      d_cyyv = that.d_cyyv;
      d_cxyv = that.d_cxyv;
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

  // move a covariance to this object

  Cov& Cov::operator = (Cov&& that) {
    // check for self-assignation
    if (this == &that) return *this;
    // lock and assign
    wrlock ();
    that.wrlock ();
    try {
      d_cxv  = that.d_cxv;  that.d_cxv  = 0.0;
      d_cyv  = that.d_cyv;  that.d_cyv  = 0.0;
      d_cxxv = that.d_cxyv; that.d_cxxv = 0.0;
      d_cyyv = that.d_cxyv; that.d_cyyv = 0.0;
      d_cxyv = that.d_cxyv; that.d_cxyv = 0.0;
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

  String Cov::repr (void) const {
    return "Cov";
  }

  // return a clone of this object

  Object* Cov::clone (void) const {
    return new Cov (*this);
  }

  // reset this covariance

  void Cov::reset (void) {
    wrlock ();
    try {
      d_cxv  = 0.0;
      d_cyv  = 0.0;
      d_cxxv = 0.0;
      d_cyyv = 0.0;
      d_cxyv = 0.0;
      d_snum = 0L;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the cumulative x value

  t_real Cov::getcxv (void) const {
    rdlock();
    try {
      t_real result = d_cxv;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the cumulative y value

  t_real Cov::getcyv (void) const {
    rdlock();
    try {
      t_real result = d_cyv;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the cumulative xx value

  t_real Cov::getcxxv (void) const {
    rdlock();
    try {
      t_real result = d_cxxv;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // get the cumulative yy value

  t_real Cov::getcyyv (void) const {
    rdlock();
    try {
      t_real result = d_cyyv;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // get the cumulative xy value

  t_real Cov::getcxyv (void) const {
    rdlock();
    try {
      t_real result = d_cxyv;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // get the number of samples

  long Cov::getsnum (void) const {
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

  // get the x variance

  t_real Cov::getxv (void) const {
    try {
      t_real cxxm = (d_snum == 0L) ? 0.0 : d_cxxv / d_snum;
      t_real cxm  = (d_snum == 0L) ? 0.0 : d_cxv  / d_snum;
      t_real result = cxxm - cxm*cxm;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // get the y variance

  t_real Cov::getyv (void) const {
    try {
      t_real cyym = (d_snum == 0L) ? 0.0 : d_cyyv / d_snum;
      t_real cym  = (d_snum == 0L) ? 0.0 : d_cyv  / d_snum;
      t_real result = cyym - cym*cym;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // get the covariance value

  t_real Cov::getcov (void) const {
    rdlock();
    try {
      t_real cxym = (d_snum == 0L) ? 0.0 : d_cxyv / d_snum;
      t_real cxm  = (d_snum == 0L) ? 0.0 : d_cxv  / d_snum;
      t_real cym  = (d_snum == 0L) ? 0.0 : d_cyv  / d_snum;
      t_real result = cxym - cxm*cym;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // push a sample x, yvalue

  void Cov::push (const t_real xval, const t_real yval) {
    wrlock ();
    try {
      d_cxxv += (xval * xval);
      d_cyyv += (yval * yval);
      d_cxyv += (xval * yval);
      d_cxv  += xval;
      d_cyv  += yval;
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
  static const long QUARK_ZONE_LENGTH = 11;
  static QuarkZone  zone (QUARK_ZONE_LENGTH);

  // the object supported quarks
  static const long QUARK_PUSH    = zone.intern ("push");
  static const long QUARK_RESET   = zone.intern ("reset");
  static const long QUARK_GETXV   = zone.intern ("get-x-variance");
  static const long QUARK_GETYV   = zone.intern ("get-y-variance");
  static const long QUARK_GETCOV  = zone.intern ("get-covariance");
  static const long QUARK_GETCXV  = zone.intern ("get-x-cumulative-value");
  static const long QUARK_GETCYV  = zone.intern ("get-y-cumulative-value");
  static const long QUARK_GETCXXV = zone.intern ("get-xx-cumulative-value");
  static const long QUARK_GETCYYV = zone.intern ("get-yy-cumulative-value");
  static const long QUARK_GETCXYV = zone.intern ("get-xy-cumulative-value");
  static const long QUARK_GETSNUM = zone.intern ("get-samples");

  // create a new object in a generic way

  Object* Cov::mknew (Vector* argv) {
    long argc = (argv == nullptr) ? 0 : argv->length ();
    
    // check for 0 argument
    if (argc == 0) return new Cov;
    // invalid arguments
    throw Exception ("argument-error",
		     "invalid arguments with covariance object");
  }

  // return true if the given quark is defined

  bool Cov::isquark (const long quark, const bool hflg) const {
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
  
  Object* Cov::apply (Evaluable* zobj, Nameset* nset, const long quark,
		      Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();

    // dispatch 0 argument
    if (argc == 0) {
      if (quark == QUARK_GETXV)   return new Real (getxv());
      if (quark == QUARK_GETYV )  return new Real (getyv());
      if (quark == QUARK_GETCOV)  return new Real (getcov());
      if (quark == QUARK_GETCXV)  return new Real (getcxv());
      if (quark == QUARK_GETCYV)  return new Real (getcyv());
      if (quark == QUARK_GETCXXV) return new Real (getcxxv());
      if (quark == QUARK_GETCYYV) return new Real (getcyyv());
      if (quark == QUARK_GETCXYV) return new Real (getcxyv());
      if (quark == QUARK_GETSNUM) return new Integer (getsnum());
      if (quark == QUARK_RESET) {
	reset ();
	return nullptr;
      }
    }
    // dispatch 2 arguments
    if (argc == 2) {
      if (quark == QUARK_PUSH) {
	t_real xval = argv->getrint(0);
	t_real yval = argv->getrint(1);
	push (xval, yval);
	return nullptr;
      }
    }
    // call the object methods
    return Object::apply (zobj, nset, quark, argv);
  }
}
