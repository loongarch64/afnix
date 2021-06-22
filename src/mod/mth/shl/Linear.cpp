// ---------------------------------------------------------------------------
// - Linear.cpp                                                              -
// - afnix:mth module - generalize linear solver class implementation        -
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

#include "Qr.hpp"
#include "Mgs.hpp"
#include "Cgs.hpp"
#include "Bcs.hpp"
#include "Tqmr.hpp"
#include "Linear.hpp"
#include "Boolean.hpp"
#include "Evaluable.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"
 
namespace afnix {
 
  // -------------------------------------------------------------------------
  // - private section                                                       -
  // -------------------------------------------------------------------------

  /// the all solver type
  static const String LNR_STYP_ALL = "ALL";
  /// the direct solver type
  static const String LNR_STYP_DCT = "DCT";
  /// the iterative solver type
  static const String LNR_STYP_ITR = "ITR";

  // -------------------------------------------------------------------------
  // - public section                                                       -
  // -------------------------------------------------------------------------

  // the default verification flag
  const bool Linear::AVF_DEF = true;
  // the default preconditionnign flag
  const bool Linear::PCF_DEF = true;

  // map a string to a solver type

  Linear::t_styp Linear::tostyp (const String& styp) {
    String s = styp.toupper ();
    // check for official name
    if (s == LNR_STYP_ALL) return Linear::STYP_ALL;
    if (s == LNR_STYP_DCT) return Linear::STYP_DCT;
    if (s == LNR_STYP_ITR) return Linear::STYP_ITR;
    // check for aliases
    if ((s == "") || (s == "DEFAULT")) return Linear::STYP_ALL;
    if (s == "DIRECT") return Linear::STYP_DCT;
    if (s == "ITERATIVE") return Linear::STYP_ITR;
    // invalid name
    throw Exception ("linear-error", "cannot map solver type by name", styp);
  }

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a default linear solver

  Linear::Linear (void) {
    reset ();
  }
  
  // create a linear solver by flag

  Linear::Linear (const bool avf) {
    reset ();
    d_avf = avf;
  }

  // create a linear solver by flags

  Linear::Linear (const bool avf, const bool pcf) {
    reset ();
    d_avf = avf;
    d_pcf = pcf;
  }

  // create a linear solver by flags and type

  Linear::Linear (const bool avf, const bool pcf, const t_styp styp) { 
    d_avf = avf;
    d_pcf = pcf;
    setstyp (styp);
  }

  // create a linear solver by flags and type

  Linear::Linear (const bool avf, const bool pcf, const t_styp styp, 
		  Logger* slg) {
    d_avf = avf;
    d_pcf = pcf;
    setstyp (styp);
    setslg (slg);
  }

  // copy construct this object

  Linear::Linear (const Linear& that) {
    that.rdlock ();
    try {
      // assign base solver
      Solver::operator = (that);
      // copy locally
      d_slv = that.d_slv;
      d_avf = that.d_avf;
      d_pcf = that.d_pcf;
      that.unlock ();
    } catch (...) {
      that.unlock ();
      throw;
    }
  }

  // return the class name
  
  String Linear::repr (void) const {
    return "Linear";
  }
  
  // return a clone of this object
  
  Object* Linear::clone (void) const {
    return new Linear (*this);
  }

  // assign an object to this one

  Linear& Linear::operator = (const Linear& that) {
    // check for self assignation
    if (this == &that) return *this;
    // lock and assign
    wrlock ();
    that.rdlock ();
    try {
      // assign base solver
      Solver::operator = (that);
      // assign locally
      d_slv = that.d_slv;
      d_avf = that.d_avf;
      d_pcf = that.d_pcf;
      unlock ();
      that.unlock ();
      return *this;
    } catch (...) {
      unlock ();
      that.unlock ();
      throw;
    }
  }

  // reset this linear solver

  void Linear::reset (void) {
    wrlock ();
    try {
      // reset the defaults
      d_slv.reset ();
      d_avf = Linear::AVF_DEF;
      d_pcf = Linear::PCF_DEF;
      // bind all solvers
      setstyp (STYP_ALL);
      // done
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set the linear system lhs

  void Linear::setlhs (Rmi* lhs) {
    wrlock ();
    try {
      // set the base solver
      Solver::setlhs (lhs);
      // get the number of solvers
      long slen = d_slv.length ();
      // loop in the solvers
      for (long k = 0L; k < slen; k++) {
	// get an iterative solver
	Iterative* isv = dynamic_cast <Iterative*> (d_slv.get (k));
	if (isv != nullptr) {
	  isv->setlhp (lhs, d_pcf);
	  continue;
	}
	// get the base solver
	Solver* slv = dynamic_cast <Solver*> (d_slv.get (k));
	if (slv == nullptr) continue;
	// set the lhs
	slv->setlhs (lhs);
      }
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // solve a system with several iterable solvers
  
  Rvi* Linear::solve (const Rvi& rhs) {
    rdlock ();
    Rvi* result = nullptr;
    try {
      // get the number of solvers
      long slen = d_slv.length ();
      // loop in the solvers
      for (long k = 0L; k < slen; k++) {
	// get the appropriate solver
	Solver* slv = dynamic_cast <Solver*> (d_slv.get (k));
	if (slv == nullptr) continue;
	// reset the solver logger
	slv->setslg (p_slg);
	// solve the system
	if ((result = slv->solve (rhs)) == nullptr) continue;
	// eventually verify the system
	if (d_avf == false) break;
	if (slv->verify (rhs, *result) == true) break;
	// failure - try another one
	delete result; result = nullptr;
      }
      unlock ();
      return result;
    } catch (...) {
      delete result;
      unlock ();
      throw;
    }
  }

  // set the solver type

  void Linear::setstyp (const t_styp styp) {
    wrlock ();
    try {
      // reset the solver vector
      d_slv.reset ();
      // set by type
      switch (styp) {
      case STYP_ALL:
	d_slv.add (new Qr);
	d_slv.add (new Mgs);
	d_slv.add (new Cgs);
	d_slv.add (new Bcs);
	break;
      case STYP_DCT:
	d_slv.add (new Qr);
	d_slv.add (new Mgs);
	break;
      case STYP_ITR:
	d_slv.add (new Cgs);
	d_slv.add (new Bcs);
	break;
      }
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // -------------------------------------------------------------------------
  // - object section                                                        -
  // -------------------------------------------------------------------------

  // create a new object in a generic way
  
  Object* Linear::mknew (Vector* argv) {
    long argc = (argv == nullptr) ? 0 : argv->length ();
    
    // check for 0 argument
    if (argc == 0) return new Linear;    
    // check for 1 argument
    if (argc == 1) {
      bool avf = argv->getbool (0);
      return new Linear (avf);
    }
    // check for 2 arguments
    if (argc == 2) {
      bool avf = argv->getbool (0);
      bool pcf = argv->getbool (1);
      return new Linear (avf, pcf);
    }
    // invalid arguments
    throw Exception ("argument-error", "invalid arguments with linear object");
  }
}
