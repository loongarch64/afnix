// ---------------------------------------------------------------------------
// - Iterative.cpp                                                           -
// - afnix:mth module - iterative solver class implementation                -
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
#include "Vector.hpp"
#include "Rvector.hpp"
#include "Evaluable.hpp"
#include "Iterative.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"
 
namespace afnix {

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a default iterative solver

  Iterative::Iterative (void) {
    d_aps = Math::d_aeps;
    d_rps = Math::d_reps;
    d_mni = 0LL;
    p_ovp = nullptr;
  }

  // copy construct this objet

  Iterative::Iterative (const Iterative& that) {
    that.rdlock ();
    try {
      // assign the base solver
      Solver::operator = (that);
      // assign locally
      d_aps = that.d_aps;
      d_rps = that.d_rps;
      d_mni = that.d_mni;
      Object::iref (p_ovp = that.p_ovp);
      // unlock
      that.unlock ();
    } catch (...) {
      that.unlock ();
      throw;
    }
  }

  // destroy this object

  Iterative::~Iterative (void) {
    Object::dref (p_ovp);
  }

  // assign an object to this one

  Iterative& Iterative::operator = (const Iterative& that) {
    // check for self assignation
    if (this == &that) return *this;
    // lock and assign
    wrlock ();
    that.rdlock ();
    try {
      // assign base object
      Solver::operator = (that);
      // protect new object
      Object::iref (that.p_ovp);
      // clean and assign
      Object::dref (p_ovp);
      d_aps = that.d_aps;
      d_rps = that.d_rps;
      p_ovp = that.p_ovp;
      // unlock
      unlock ();
      that.unlock ();
      return *this;
    } catch (...) {
      unlock ();
      that.unlock ();
      throw;
    } 	
  }

  // reset this solver

  void Iterative::reset (void) {
    wrlock ();
    try {
      // reset base
      Solver::reset ();
      // reset locally
      d_mni = 0LL;
      Object::dref (p_ovp); p_ovp = nullptr;
    } catch (...) {
      unlock ();
      throw;
    }
  }
      
  // set the solver lhs

  void Iterative::setlhs (Rmi* lhs) {
    wrlock ();
    try {
      // set the base lhs
      Solver::setlhs (lhs);
      // update locally
      d_mni = (p_lhs == nullptr) ? 0LL : p_lhs->getrsiz ();
      Object::dref (p_ovp); p_ovp = nullptr;      
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set the maximum iterations

  void Iterative::setmni (const t_long mni) {
    wrlock ();
    try {
      d_mni = (mni < 0LL) ? 0LL : mni;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set the solver absolute precision

  void Iterative::setaps (const t_real aps) {
    wrlock ();
    try {
      if (((Math::isnan (aps) == true) || Math::isinf (aps) == true) ||
	  (aps <= 0.0)) d_aps = Math::d_aeps;
      else d_aps = aps;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }
	
  // get the solver absolute precision

  t_real Iterative::getaps (void) const {
    rdlock ();
    try {
      t_real result = d_aps;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set the solver relative precision

  void Iterative::setrps (const t_real rps) {
    wrlock ();
    try {
      if (((Math::isnan (rps) == true) || Math::isinf (rps) == true) ||
	  (rps <= 0.0)) d_rps = Math::d_reps;
      else d_rps = rps;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }
	
  // get the solver relative precision

  t_real Iterative::getrps (void) const {
    rdlock ();
    try {
      t_real result = d_rps;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
	
  // set the solver preconditionner

  void Iterative::setovp (Rvi* ovp) {
    wrlock ();
    try {
      Object::iref (ovp);
      Object::dref (p_ovp);
      p_ovp = ovp;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the solver preconditionner

  Rvi* Iterative::getovp (void) const {
    rdlock ();
    try {
      Rvi* result = p_ovp;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set the solver lhs with preconditionning

  void Iterative::setlhp (Rmi* lhs, const bool pcf) {
    wrlock ();
    try {
      // set the solver lhs
      setlhs (lhs);
      // set the preconditionner
      if (pcf == true) setovp (getjp ());
      // done
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get a jacobi vector preconditionner from the lhs
  
  Rvi* Iterative::getjp (void) const {
    rdlock ();
    Rvi* jp = nullptr;
    try {
      // get the lhs size by row and check
      long size = (p_lhs == nullptr) ? 0L : p_lhs->getrsiz ();
      if (size == 0L) {
	unlock ();
	return nullptr;
      }
      // create a null vector
      jp = new Rvector (size);
      // loop in the matrix diagonal
      for (long k = 0; k < size; k++) {
	t_real lhv = p_lhs->get (k,k);
	t_real pvv = (lhv == 0.0) ? 1.0 : (lhv < 0.0) ? -lhv : lhv;
	jp->set (k, 1.0 / pvv);
      }
      unlock ();
      return jp;
    } catch (...) {
      delete jp;
      unlock ();
      throw;
    }
  }

  // -------------------------------------------------------------------------
  // - object section                                                        -
  // -------------------------------------------------------------------------
  
  // the quark zone
  static const long QUARK_ZONE_LENGTH = 9;
  static QuarkZone  zone (QUARK_ZONE_LENGTH);

  // the object supported quarks
  static const long QUARK_GETJP  = zone.intern ("get-jacobi-preconditionner");
  static const long QUARK_SETMNI = zone.intern ("set-max-iterations");
  static const long QUARK_SETOVP = zone.intern ("set-preconditionner");
  static const long QUARK_GETOVP = zone.intern ("get-preconditionner");
  static const long QUARK_SETLHP = zone.intern ("set-lhs-preconditionner");
  static const long QUARK_SETAPS = zone.intern ("set-absolute-precision");
  static const long QUARK_GETAPS = zone.intern ("get-absolute-precision");
  static const long QUARK_SETRPS = zone.intern ("set-relative-precision");
  static const long QUARK_GETRPS = zone.intern ("get-relative-precision");

  // return true if the given quark is defined

  bool Iterative::isquark (const long quark, const bool hflg) const {
    rdlock ();
    if (zone.exists (quark) == true){
      unlock ();
      return true;
    }
    bool result = hflg ? Solver::isquark (quark, hflg) : false;
    unlock ();
    return result;
  }

  // apply this object with a set of arguments and a quark
  
  Object* Iterative::apply (Evaluable* zobj, Nameset* nset, const long quark,
			    Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();
    
    // dispatch 0 argument
    if (argc == 0) {
      if (quark == QUARK_GETJP) return getjp ();
      if (quark == QUARK_GETAPS) return new Real (getaps ());
      if (quark == QUARK_GETRPS) return new Real (getrps ());
      if (quark == QUARK_GETOVP) {
	rdlock ();
	try {
	  Rvi* result = getovp ();
	  zobj->post (result);
	  unlock ();
	  return result;
	} catch (...) {
	  unlock ();
	  throw;
	}
      }
    }
    // dispatch 1 argument
    if (argc == 1) {
      if (quark == QUARK_SETMNI) {
	t_long mni = argv->getlong (0);
	setmni (mni);
	return nullptr;
      }
      if (quark == QUARK_SETAPS) {
	t_real aps = argv->getreal (0);
	setaps (aps);
	return nullptr;
      }
      if (quark == QUARK_SETRPS) {
	t_real rps = argv->getreal (0);
	setrps (rps);
	return nullptr;
      }
      if (quark == QUARK_SETOVP) {
	Object* obj = argv->get (0);
	Rvi* ovp = dynamic_cast <Rvi*> (obj);
	if ((ovp == nullptr) && (obj != nullptr)){
	  throw Exception ("type-error", 
			   "invalid object with set-preconditionner",
			   Object::repr (obj));
	}
	setovp (ovp);
	return nullptr;
      }
    }
    // dispatch 2 arguments
    if (argc == 2) {
      if (quark == QUARK_SETLHP) {
	Object* obj = argv->get (0);
	Rmi* lhs = dynamic_cast <Rmi*> (obj);
	if ((lhs == nullptr) && (obj != nullptr)){
	  throw Exception ("type-error", "invalid object with set-lhs",
			   Object::repr (obj));
	}
	bool pcf = argv->getbool (1);
	setlhp (lhs, pcf);
	return nullptr;
      }
    }
    // call the solver methods
    return Solver::apply (zobj, nset, quark, argv);
  }
}
