// ---------------------------------------------------------------------------
// - Rpolynom.cpp                                                            -
// - afnix:mth module - real polynom class implementation                    -
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

#include "Real.hpp"
#include "Vector.hpp"
#include "Integer.hpp"
#include "Rpolynom.hpp"
#include "Evaluable.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"
 
namespace afnix {

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a default real polynom

  Rpolynom::Rpolynom (void) {
    d_pdgr = 0;
    d_ndgr = 0;
    p_coef = new t_real[1];
    p_coef[0] = 0.0;
  }

  // create a real polynom by signed degree

  Rpolynom::Rpolynom (const long sdgr) {
    // set the polynom degree
    d_pdgr = (sdgr >= 0) ?  sdgr : 0;
    d_ndgr = (sdgr <  0) ? -sdgr : 0;
    // set the total size
    long size = d_pdgr + d_ndgr + 1;
    p_coef = new t_real[size];
    // initialize to null
    for (long i = 0; i < size; i++) p_coef[i] = 0.0;
  }

  // create a real polynom by positive and negative degree

  Rpolynom::Rpolynom (const long pdgr, const long ndgr) {
    // check for valid degree
    if ((pdgr < 0) || (ndgr > 0)) {
      throw Exception ("polynom-error", "invalid signed polynom degree");
    }
    // set the polynom degree
    d_pdgr =  pdgr;
    d_ndgr = -ndgr;
    // set the total size
    long size = d_pdgr + d_ndgr + 1;
    p_coef = new t_real[size];
    // initialize to null
    for (long i = 0; i < size; i++) p_coef[i] = 0.0;
  }

  // copy construct this real polynom

  Rpolynom::Rpolynom (const Rpolynom& that) {
    that.rdlock ();
    try {
      // copy the polynom degree
      d_pdgr = that.d_pdgr;
      d_ndgr = that.d_ndgr;
      // set the total size
      long size = d_pdgr + d_ndgr + 1;
      p_coef = new t_real[size];
      // copy the coefficients
      for (long i = 0; i < size; i++) p_coef[i] = that.p_coef[i];
      that.unlock ();
    } catch (...) {
      that.unlock ();
      throw;
    }
  }
  
  // destroy this polynom

  Rpolynom::~Rpolynom (void) {
    delete [] p_coef;
  }

  // return the class name
  
  String Rpolynom::repr (void) const {
    return "Rpolynom";
  }
  
  // return a clone of this object
  
  Object* Rpolynom::clone (void) const {
    return new Rpolynom (*this);
  }
  
  // assign a real polynom to this one

  Rpolynom& Rpolynom::operator = (const Rpolynom& that) {
    // check for self assignation
    if (this == &that) return *this;
    // lock and assign
    wrlock ();
    that.rdlock ();
    try {
      // clean the old polynom
      delete [] p_coef; p_coef = nullptr;
      // copy the polynom degree
      d_pdgr = that.d_pdgr;
      d_ndgr = that.d_ndgr;
      // set the total size
      long size = d_pdgr + d_ndgr + 1;
      p_coef = new t_real[size];
      // copy the coefficients
      for (long i = 0; i < size; i++) p_coef[i] = that.p_coef[i];
      // unlock and return
      unlock ();
      that.unlock ();
      return *this;
    } catch (...) {
      unlock ();
      that.unlock ();
      throw;
    }
  }

  // compute the polynom at a point

  t_real Rpolynom::compute (const t_real x) const {
    rdlock ();
    try {
      // set the constant value
      t_real result = p_coef[d_ndgr];
      // evaluate negative degree
      if (d_ndgr > 0) {
	t_real nx = 1.0;
	for (long i = 1; i <= d_ndgr; i++) {
	  nx /= x;
	  result += (p_coef[d_ndgr-i] * nx);
	}
      }
      // evaluate positive degree
      if (d_pdgr > 0) {
	t_real px = 1.0;
	for (long i = 1 ; i <= d_pdgr; i++) {
	  px *= x;
	  result += (p_coef[i] * px);
	}
      }
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // compute the derivate at a point

  t_real Rpolynom::derivate (const t_real x) const {
    rdlock ();
    try {
      // set the result value
      t_real result = 0.0;
      // evaluate negative degree
      if (d_ndgr > 0) {
	t_real nx = 1.0 / x;
	for (long i = 1; i <= d_ndgr; i++) {
	  nx /= x;
	  result += (-i * p_coef[d_ndgr-i] * nx);
	}
      }
      // evaluate positive degree
      if (d_pdgr > 0) {
	t_real px = 1.0;
	for (long i = 1 ; i <= d_pdgr; i++) {
	  result += (i * p_coef[i] * px);
	  px *= x;
	}
      }
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the polynom positive degre

  long Rpolynom::getpdgr (void) const {
    rdlock ();
    try {
      long result = d_pdgr;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the polynom negative degre

  long Rpolynom::getndgr (void) const {
    rdlock ();
    try {
      long result = d_ndgr;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set the polynom coefficient

  void Rpolynom::setcoef (const long index, const t_real coef) {
    wrlock ();
    try {
      // check for valid range
      if (((index > 0) && (index > d_pdgr)) ||
	  ((index < 0) && (index < d_ndgr))) {
	throw Exception ("polynom-error", "invalid position in polynom set");
      }
      // set by index
      p_coef[d_ndgr+index] = coef;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the polynom coefficient

  t_real Rpolynom::getcoef (const long index) const {
    rdlock ();
    try {
      // check for valid range
      if (((index > 0) && (index > d_pdgr)) ||
	  ((index < 0) && (index < d_ndgr))) {
	throw Exception ("polynom-error", "invalid position in polynom set");
      }
      // set by index
      t_real result = p_coef[d_ndgr+index];
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
  static const long QUARK_ZONE_LENGTH = 4;
  static QuarkZone  zone (QUARK_ZONE_LENGTH);

  // the object supported quarks
  static const long QUARK_GETPDGR  = zone.intern ("get-positive-degree");
  static const long QUARK_GETNDGR  = zone.intern ("get-negative-degree");
  static const long QUARK_SETCOEF  = zone.intern ("set-coefficient");
  static const long QUARK_GETCOEF  = zone.intern ("get-coefficient");

  // create a new object in a generic way
  
  Object* Rpolynom::mknew (Vector* argv) {
    long argc = (argv == nullptr) ? 0 : argv->length ();
    
    // check for 0 argument
    if (argc == 0) return new Rpolynom;    
    // check for 1 argument
    if (argc == 1) {
      long sdgr = argv->getlong (0);
      return new Rpolynom (sdgr);
    }
    // check for 2 arguments
    if (argc == 2) {
      long pdgr = argv->getlong (0);
      long ndgr = argv->getlong (1);
      return new Rpolynom (pdgr,ndgr);
    }
    // invalid arguments
    throw Exception ("argument-error", 
		     "invalid arguments with real polynom object");
  }

  // return true if the given quark is defined

  bool Rpolynom::isquark (const long quark, const bool hflg) const {
    rdlock ();
    if (zone.exists (quark) == true){
      unlock ();
      return true;
    }
    bool result = hflg ? Rfi::isquark (quark, hflg) : false;
    unlock ();
    return result;
  }
  
  // apply this object with a set of arguments and a quark
  
  Object* Rpolynom::apply (Evaluable* zobj, Nameset* nset, const long quark,
                      Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();
    
    // dispatch 0 argument
    if (argc == 0) {
      if (quark == QUARK_GETPDGR) return new Integer (getpdgr ());
      if (quark == QUARK_GETNDGR) return new Integer (getndgr ());
    }
    // dispatch 1 argument
    if (argc == 1) {
      if (quark == QUARK_GETCOEF) {
        long index = argv->getlong (0);
        return new Real (getcoef (index));
      }
    }
    // dispatch 2 arguments
    if (argc == 2) {
      if (quark == QUARK_SETCOEF) {
        long   index = argv->getlong (0);
        t_real  coef = argv->getreal (1);
        setcoef (index, coef);
	return nullptr;
      }
    }
    // call the interface method
    return Rfi::apply (zobj, nset, quark, argv);
  }
}
