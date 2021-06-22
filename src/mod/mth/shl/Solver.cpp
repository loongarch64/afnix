// ---------------------------------------------------------------------------
// - Solver.cpp                                                              -
// - afnix:mth module - linear solver class implementation                   -
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

#include "Vector.hpp"
#include "Solver.hpp"
#include "Boolean.hpp"
#include "Evaluable.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - private section                                                       -
  // -------------------------------------------------------------------------

  // the solver clear log message
  static const String SLV_DBUG_CLR = "[afnix:mth:solver] clear called";
  
  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a default solver

  Solver::Solver (void) {
    p_lhs = nullptr;
    p_slg = nullptr;
    Object::iref (p_sla = new Analytic);
  }

  // copy construct this solver

  Solver::Solver (const Solver& that) {
    that.rdlock ();
    try {
      Object::iref (p_lhs = that.p_lhs);
      Object::iref (p_slg = that.p_slg);
      Object::iref (p_sla = that.p_sla);
      that.unlock ();
    } catch (...) {
      that.unlock ();
      throw;
    }
  }

  // destroy this solver

  Solver::~Solver (void) {
    Object::dref (p_lhs);
    Object::dref (p_slg);
    Object::dref (p_sla);
  }

  // assign a solver to this one

  Solver& Solver::operator = (const Solver& that) {
    // check for self assignation
    if (this == &that) return *this;
    // lock and assign
    wrlock ();
    that.rdlock ();
    try {
      // protect the new object
      Object::iref (that.p_lhs);
      Object::iref (that.p_slg);
      Object::iref (that.p_sla);
      // clean the old object
      Object::dref (p_lhs);
      Object::dref (p_slg);
      Object::dref (p_sla);
      // assign locally
      p_lhs = that.p_lhs;
      p_slg = that.p_slg;
      p_sla = that.p_sla;
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

  void Solver::reset (void) {
    wrlock ();
    try {
      Object::dref (p_lhs); p_lhs = nullptr;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // clear this solver

  void Solver::clear (void) {
    wrlock ();
    try {
      if (p_slg != nullptr) p_slg->add (Logger::MLVL_DBUG, SLV_DBUG_CLR);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set the solver lhs

  void Solver::setlhs (Rmi* lhs) {
    wrlock ();
    try {
      Object::iref (lhs);
      Object::dref (p_lhs);
      p_lhs = lhs;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the solver lhs

  Rmi* Solver::getlhs (void) const {
    rdlock ();
    try {
      Rmi* result = p_lhs;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set the solver logger

  void Solver::setslg (Logger* slg) {
    wrlock ();
    try {
      // set the logger locally
      Object::iref (slg);
      Object::dref (p_slg);
      p_slg = slg;
      // set the logger analytic
      if (p_sla != nullptr) p_sla->setslg (slg);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the solver logger

  Logger* Solver::getslg (void) const {
    rdlock ();
    try {
      Logger* result = p_slg;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // preset a rhs before using it

  void Solver::prerhs (Rvi& rhs) const {
    rdlock ();
    try {
      rhs.preset ();
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }
    
  // solve a linear system

  Rvi* Solver::solve (Rmi* lhs, const Rvi& rhs) {
    wrlock ();
    try {
      // set the lhs
      setlhs (lhs);
      // solve the system
      Rvi* result = solve (rhs);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // verify a system with an original solution
  
  bool Solver::verify (const Rvi& rhs, const Rvi& x) const {
    rdlock ();
    Rvi* rbv = nullptr;
    try {
      // check for a lhs
      if (p_lhs == nullptr) {
	unlock ();;
	return false;
      }
      // clone the right handside
      rbv = dynamic_cast <Rvi*> (rhs.clone ());
      rbv->clear ();
      // compute the new solution
      p_lhs->mul (*rbv, x, 1.0);
      // compare the solution
      bool result = rbv->cmp (rhs);
      // clean and return
      delete rbv;
      unlock ();
      return result;
    } catch (...) {
      delete rbv;
      unlock ();
      throw;
    }
  }

  // check a system prior being solver

  bool Solver::check (const Rvi& rhs) const {
    rdlock ();
    try {
      bool result = check (p_lhs, rhs);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // check a system prior being solver

  bool Solver::check (Rmi* lhs, const Rvi& rhs) const {
    rdlock ();
    try {
      // check for valid analytic
      if (p_sla == nullptr) {
	unlock ();
	return false;
      }
      // check for valid rhs
      bool result = !p_sla->isnan (rhs);
      // check for valid lhs
      if (lhs == nullptr) {
	result = false;
      } else {
	result &= !p_sla->isnrow (*lhs);
	result &= !p_sla->isncol (*lhs);
      }
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
  static const long QUARK_ZONE_LENGTH = 12;
  static QuarkZone  zone (QUARK_ZONE_LENGTH);

  // the object supported quarks
  static const long QUARK_RESET  = zone.intern ("reset");
  static const long QUARK_CLEAR  = zone.intern ("clear");
  static const long QUARK_SOLVE  = zone.intern ("solve");
  static const long QUARK_SETLHS = zone.intern ("set-lhs");
  static const long QUARK_GETLHS = zone.intern ("get-lhs");
  static const long QUARK_SETSLG = zone.intern ("set-logger");
  static const long QUARK_GETSLG = zone.intern ("get-logger");
  static const long QUARK_VERIFY = zone.intern ("verify");

  // return true if the given quark is defined

  bool Solver::isquark (const long quark, const bool hflg) const {
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
  
  Object* Solver::apply (Evaluable* zobj, Nameset* nset, const long quark,
			 Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();

    // dispatch 0 argument
    if (argc == 0) {
      if (quark == QUARK_RESET) {
	reset ();
	return nullptr;
      }
      if (quark == QUARK_CLEAR) {
	clear ();
	return nullptr;
      }
      if (quark == QUARK_GETLHS) {
	rdlock ();
	try {
	  Rmi* result = getlhs ();
	  zobj->post (result);
	  unlock ();
	  return result;
	} catch (...) {
	  unlock ();
	  throw;
	}
      }
      if (quark == QUARK_GETSLG) {
	rdlock ();
	try {
	  Logger* result = getslg ();
	  zobj->post (result);
	  unlock ();
	  return result;
	} catch (...) {
	  unlock ();
	  throw;
	}
      }
    }
    // dispatch 1 arguments
    if (argc == 1) {
      if (quark == QUARK_SETLHS) {
	Object* obj = argv->get (0);
	Rmi* lhs = dynamic_cast <Rmi*> (obj);
	if ((lhs == nullptr) && (obj != nullptr)){
	  throw Exception ("type-error", "invalid object with set-lhs",
			   Object::repr (obj));
	}
	setlhs (lhs);
	return nullptr;
      }
      if (quark == QUARK_SETSLG) {
	Object* obj = argv->get (0);
	Logger* slg = dynamic_cast <Logger*> (obj);
	if ((slg == nullptr) && (obj != nullptr)){
	  throw Exception ("type-error", "invalid object with set-logger",
			   Object::repr (obj));
	}
	setslg (slg);
	return nullptr;
      }
      if (quark == QUARK_SOLVE) {
	// get the rhs
	Object* obj = argv->get (0);
	Rvi* rhs = dynamic_cast <Rvi*> (obj);
	if (rhs == nullptr) {
	  throw Exception ("type-error", "invalid object with solve",
			   Object::repr (obj));
	}
	wrlock ();
	try {
	  Rvi* result = solve (*rhs);
	  zobj->post (result);
	  unlock ();
	  return result;
	} catch (...) {
	  unlock ();
	  throw;
	}
      }
    }
    // dispatch 2 arguments
    if (argc == 2) {
      if (quark == QUARK_SOLVE) {
	// get the lhs
	Object* obj = argv->get (0);
	Rmi* lhs = dynamic_cast <Rmi*> (obj);
	if (lhs == nullptr) {
	  throw Exception ("type-error", "invalid object with solve",
			   Object::repr (obj));
	}
	// get the rhs
	obj = argv->get (1);
	Rvi* rhs = dynamic_cast <Rvi*> (obj);
	if (rhs == nullptr) {
	  throw Exception ("type-error", "invalid object with solve",
			   Object::repr (obj));
	}
	wrlock ();
	try {
	  Rvi* result = solve (lhs, *rhs);
	  zobj->post (result);
	  unlock ();
	  return result;
	} catch (...) {
	  unlock ();
	  throw;
	}
      }
      if (quark == QUARK_VERIFY) {
	// get the rhs
	Object* obj = argv->get (0);
	Rvi*    rhs = dynamic_cast <Rvi*> (obj);
	if (rhs == nullptr) {
	  throw Exception ("type-error", "invalid object with verify",
			   Object::repr (obj));
	}
	// get the original solution
	obj = argv->get (1);
	Rvi* x = dynamic_cast <Rvi*> (obj);
	if (x == nullptr) {
	  throw Exception ("type-error", "invalid object with verify",
			   Object::repr (obj));
	}
	return new Boolean (verify (*rhs, *x));
      }
    }
    // call the object methods
    return Object::apply (zobj, nset, quark, argv);
  }
}
