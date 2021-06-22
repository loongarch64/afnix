// ---------------------------------------------------------------------------
// - Lufit.cpp                                                               -
// - afnix:mth module - linear univariate model fitting class implementation -
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
#include "Lufit.hpp"
#include "Vector.hpp"
#include "Integer.hpp"
#include "Evaluable.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"
 
namespace afnix {

  // -------------------------------------------------------------------------
  // - class section                                                        -
  // -------------------------------------------------------------------------

  // create a default fitter

  Lufit::Lufit (void) {
    reset ();
  }
  
  // copy construct this object

  Lufit::Lufit (const Lufit& that) {
    that.rdlock ();
    try {
      d_dcov = that.d_dcov;
      d_pv   = that.d_pv;
      d_cs   = that.d_cs;
      d_ci   = that.d_ci;
      that.unlock ();
    } catch (...) {
      that.unlock ();
      throw;
    }
  }

  // copy move this object

  Lufit::Lufit (Lufit&& that) {
    that.rdlock ();
    try {
      d_dcov = that.d_dcov;
      d_pv   = that.d_pv; that.d_pv = Math::CV_NAN;
      d_cs   = that.d_cs; that.d_cs = Math::CV_NAN;
      d_ci   = that.d_ci; that.d_ci = Math::CV_NAN;
      that.unlock ();
    } catch (...) {
      that.unlock ();
      throw;
    }
  }
  
  // assign a fitter to this object

  Lufit& Lufit::operator = (const Lufit& that) {
    // check for self-assignation
    if (this == &that) return *this;
    // lock and assign
    wrlock ();
    that.rdlock ();
    try {
      d_dcov = that.d_dcov;
      d_pv   = that.d_pv;
      d_cs   = that.d_cs;
      d_ci   = that.d_ci;
      unlock ();
      that.unlock ();
      return *this;
    } catch (...) {
      unlock ();
      that.unlock ();
      throw;
    }
  }
  
  // move a fitter to this object

  Lufit& Lufit::operator = (Lufit&& that) {
    // check for self-assignation
    if (this == &that) return *this;
    // lock and assign
    wrlock ();
    that.wrlock ();
    try {
      d_dcov = that.d_dcov;
      d_pv   = that.d_pv; that.d_pv = Math::CV_NAN;
      d_cs   = that.d_cs; that.d_cs = Math::CV_NAN;
      d_ci   = that.d_ci; that.d_ci = Math::CV_NAN;
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

  String Lufit::repr (void) const {
    return "Lufit";
  }
  
  // return a clone of this object
  
  Object* Lufit::clone (void) const {
    return new Lufit (*this);
  }
  
  // reset this fitter

  void Lufit::reset (void) {
    wrlock ();
    try {
      clear ();
      d_dcov.reset ();
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }	
  }

  // clear this fitter

  void Lufit::clear (void) {
    wrlock ();
    try {
      d_pv = Math::CV_NAN;
      d_cs = Math::CV_NAN;
      d_ci = Math::CV_NAN;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }	
  }

  // get the number of predictors

  long Lufit::getsize (void) const {
    wrlock ();
    try {
      long result = 1;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // add a predictor value

  void Lufit::addp (const t_real pv) {
    wrlock ();
    try {
      d_pv = pv;
      d_cs = Math::CV_NAN;
      d_ci = Math::CV_NAN;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // add a response value

  void Lufit::addr (const t_real rv) {
    wrlock ();
    try {
      if (Math::isnan (d_pv) == true) {
	throw Exception ("lufit-error",
			 "cannot push response without predictor");
      }
      d_dcov.push (d_pv, rv);
      d_pv = Math::CV_NAN;
      d_cs = Math::CV_NAN;
      d_ci = Math::CV_NAN;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // factorize this model fitter

  bool Lufit::factorize (void) {
    wrlock ();
    try {
      // factorize the regression effects
      d_cs  = d_dcov.getcov () / d_dcov.getxv ();
      d_ci  = d_dcov.getcyv () - d_cs * d_dcov.getcxv ();
      d_ci /= d_dcov.getsnum();
      // validate result
      bool result = !Math::isnan(d_cs) && !Math::isnan(d_ci);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // push a regressor value and regress

  t_real Lufit::regress (const t_real rv) const {
    rdlock();
    try {
      // check for cached value
      if ((Math::isnan (d_cs) == true) || (Math::isnan (d_ci) == true)) {
	throw Exception ("lufit-error", "model is not factorized");
      }
      // evaluate the regressand
      t_real result = d_ci + d_cs * rv;
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
  
  // create a new object in a generic way

  Object* Lufit::mknew (Vector* argv) {
    long argc = (argv == nullptr) ? 0 : argv->length ();
    
    // check for 0 argument
    if (argc == 0) return new Lufit;
    // invalid arguments
    throw Exception ("argument-error",
		     "invalid arguments with lufit object");
  }
}

