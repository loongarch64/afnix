// ---------------------------------------------------------------------------
// - Newton.cpp                                                              -
// - afnix:mth module - newton based zero solver class implementation        -
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
#include "Math.hpp"
#include "Vector.hpp"
#include "Newton.hpp"
#include "Linear.hpp"
#include "Boolean.hpp"
#include "Integer.hpp"
#include "Evaluable.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"

namespace afnix {
  // the iteration factor for the newton solver
  static const long NTW_MNI_DEF = 100L;

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a default newton solver

  Newton::Newton (void) {
    d_mni = NTW_MNI_DEF;
    Object::iref (p_lnr = new Linear);
  }
 
  // create a newton solver with a linear solver

  Newton::Newton (Solver* lnr) {
    d_mni = NTW_MNI_DEF;
    Object::iref (p_lnr = lnr);
  } 

  // copy construct this object

  Newton::Newton (const Newton& that) {
    that.rdlock ();
    try {
      d_mni = that.d_mni;
      Object::iref (p_lnr = that.p_lnr);
      that.unlock ();
    } catch (...) {
      that.unlock ();
      throw;
    }
  }

  // destroy this newton solver

  Newton::~Newton (void) {
    Object::dref (p_lnr);
  }

  // return the class name
  
  String Newton::repr (void) const {
    return "Newton";
  }
  
  // return a clone of this object
  
  Object* Newton::clone (void) const {
    return new Newton (*this);
  }

  // assign an object to this one

  Newton& Newton::operator = (const Newton& that) {
    // check for self assignation
    if (this == &that) return *this;
    // lock and assign
    wrlock ();
    that.rdlock ();
    try {
      d_mni = that.d_mni;
      Object::iref (that.p_lnr);
      Object::dref (p_lnr);
      p_lnr = that.p_lnr;
      unlock ();
      that.unlock ();
      return *this;
    } catch (...) {
      unlock ();
      that.unlock ();
      throw;
    }
  }

  // set the newton linear solver

  void Newton::setlnr (Solver* lnr) {
    wrlock ();
    try {
      Object::iref (lnr);
      Object::dref (p_lnr);
      p_lnr = lnr;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the newton linear solver

  Solver* Newton::getlnr (void) const {
    rdlock ();
    try {
      Solver* result = p_lnr;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set the number of newton iterations

  void Newton::setmni (const long mni) {
    wrlock ();
    try {
      d_mni = mni;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the number of newton iterations

  long Newton::getmni (void) const {
    rdlock ();
    try {
      long result = d_mni;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // find a function root with the newton method

  t_real Newton::solve (const Rfi& fo, const t_real xi) {
    rdlock ();
    try {
      // initialize current point
      t_real xn = xi;
      // newton loop
      for (long k = 0L; k < d_mni; k++) {
	// check for zero
	t_real xz = fo.compute (xn);
	if (Math::rcmp (0, xz) == true) {
	  unlock ();
	  return xn;
	}
	// check for null derivative
	t_real xd = fo.derivate (xn);
	if (Math::rcmp (0, xd) == true) {
	  throw Exception ("newton-error", 
			   "null derivative in convergence loop");
	}
	// update delta
	xz /= xd;
	xn -= xz;
      }
      throw Exception ("newton-eror", "zero loop convergence failure");
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // find a system root with the newton method

  Rvi* Newton::solve (const Rni& so, const Rvi& xi) {
    rdlock ();
    // initialize the current point
    Rvi* xn = dynamic_cast <Rvi*> (xi.clone ());
    Object::iref (xn);
    // initialize delta, lhs and rhs
    Rvi* xd  = nullptr;
    Rvi* rhs = nullptr;
    Rmi* lhs = nullptr;
    try {
      // check for a linear solver
      if (p_lnr == nullptr) {
	throw Exception ("newton-error", 
			 "no linear solver bound to newton solver");
      }
      // newton loop
      for (long k = 0L; k < d_mni; k++) {
	// check rhs for zero
	rhs = so.getrhs (xn); Object::iref (rhs);
	if (rhs == nullptr) {
	  throw Exception ("newton-error", "null rhs in convergence loop");
	}
	if (Math::rcmp (0, rhs->norm ()) == true) {
	  Object::tref (xn);
	  Object::dref (rhs);
	  unlock ();
	  return xn;
	}
	// get the lhs by operand
	Rmi* lhs = so.getlhs (xn); Object::iref (lhs);
	// try to solve the system
	xd = p_lnr->solve (lhs, *rhs);
	// clean elements
	Object::dref (lhs); lhs = nullptr;
	Object::dref (rhs); rhs = nullptr;
	// check for null delta
	if (xd == nullptr) {
	  throw Exception ("newton-error", "newton linear solver failure");
	}
	// update delta
	*xn += *xd;
	delete xd; xd = nullptr;
      }
      throw Exception ("newton-error", "newton loop convergence failure");
    } catch (...) {
      delete xd;
      Object::dref (xn);
      Object::dref (lhs);
      Object::dref (rhs);
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
  static const long QUARK_SOLVE  = zone.intern ("solve");
  static const long QUARK_SETLNR = zone.intern ("set-linear-solver");
  static const long QUARK_GETLNR = zone.intern ("get-linear-solver");
  static const long QUARK_SETMNI = zone.intern ("set-newton-iterations");
  static const long QUARK_GETMNI = zone.intern ("get-newton-iterations");

  // create a new object in a generic way
  
  Object* Newton::mknew (Vector* argv) {
    long argc = (argv == nullptr) ? 0 : argv->length ();
    
    // check for 0 argument
    if (argc == 0) return new Newton;
    // check for 1 argument
    if (argc == 1) {
      Object* obj = argv->get (0);
      // check for a solver
      Solver* slv = dynamic_cast <Solver*> (obj);
      if (slv != nullptr) return new Newton (slv);
      // invalid type
      throw Exception ("type-error", "invalid object for newton solver",
		       Object::repr (obj));
    }
    // invalid arguments
    throw Exception ("argument-error", "invalid arguments with newton object");
  }

  // return true if the given quark is defined

  bool Newton::isquark (const long quark, const bool hflg) const {
    rdlock ();
    if (zone.exists (quark) == true){
      unlock ();
      return true;
    }
    bool result = hflg ? Object::isquark (quark, hflg) : false;
    unlock ();
    return result;
  }
  
  // apply this object with a set of arguments and a quark
  
  Object* Newton::apply (Evaluable* zobj, Nameset* nset, const long quark,
			 Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();

    // dipatch 0 argument
    if (argc == 0) {
      if (quark == QUARK_GETMNI) return new Integer (getmni ());
      if (quark == QUARK_GETLNR) {
	rdlock ();
	try {
	  Solver* result = getlnr ();
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
	long mni = argv->getlong (0);
	setmni (mni);
	return nullptr;
      }
      if (quark == QUARK_SETLNR) {
	Object* obj = argv->get (0);
	Solver* lnr = dynamic_cast <Solver*> (obj);
	if (lnr == nullptr) {
	  throw Exception ("type-error", 
			   "invalid object with set-linear-solver",
			   Object::repr (obj));
	}
	setlnr (lnr);
	return nullptr;
      }
    }
    // dispatch 2 arguments
    if (argc == 2) {
      if (quark == QUARK_SOLVE) {
	Object* obj = argv->get (0);
	// check for rfi object
	Rfi* fo = dynamic_cast <Rfi*> (obj);
	if (fo != nullptr) {
	  t_real xi = argv->getreal (1);
	  return new Real (solve (*fo, xi));
	}
	// check for rni object
	Rni* so = dynamic_cast <Rni*> (obj);
	if (so == nullptr) {
	  throw Exception ("type-error", "invalid object with solve",
			   Object::repr (obj));
	}
	// check for rvi point
	obj = argv->get (1);
	Rvi* xi = dynamic_cast <Rvi*> (obj);
	if (xi == nullptr) {
	  throw Exception ("type-error", "invalid object with solve",
			   Object::repr (obj));
	}
	// try to solve the system
	return solve (*so, *xi);
      }
    }
    // call the object
    return Object::apply (zobj, nset, quark, argv);
  }
}
