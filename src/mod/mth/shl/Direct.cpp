// ---------------------------------------------------------------------------
// - Direct.cpp                                                              -
// - afnix:mth module - direct solver class implementation                   -
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
#include "Direct.hpp"
#include "Integer.hpp"
#include "Evaluable.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"
 
namespace afnix {

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a direct solver

  Direct::Direct (void) {
    p_cpi = nullptr;
  }

  // destroy this direct solver

  Direct::~Direct (void) {
    Object::dref (p_cpi);
  }

  // set the solver lhs

  void Direct::setlhs (Rmi* lhs) {
    wrlock ();
    try {
      // set the base lhs
      Solver::setlhs (lhs);
      // clear the permutation if any
      setcpi (nullptr);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }
  // set the solver permutation

  void Direct::setcpi (Cpi* cpi) {
    wrlock ();
    try {
      Object::iref (cpi);
      Object::dref (p_cpi);
      p_cpi = cpi;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the solver cpi

  Cpi* Direct::getcpi (void) const {
    rdlock ();
    try {
      Cpi* result = p_cpi;
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
  static const long QUARK_ZONE_LENGTH = 3;
  static QuarkZone  zone (QUARK_ZONE_LENGTH);

  // the object supported quarks
  static const long QUARK_SETCPI    = zone.intern ("set-permutation");
  static const long QUARK_GETCPI    = zone.intern ("get-permutation");
  static const long QUARK_FACTORIZE = zone.intern ("factorize");

  // return true if the given quark is defined

  bool Direct::isquark (const long quark, const bool hflg) const {
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
  
  Object* Direct::apply (Evaluable* zobj, Nameset* nset, const long quark,
			 Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();
    
    // dispatch 0 argument
    if (argc == 0) {
      if (quark == QUARK_GETCPI) {
	rdlock ();
	try {
	  Object* result = getcpi ();
	  zobj->post (result);
	  unlock ();
	  return result;
	} catch (...) {
	  unlock ();
	  throw;
	}
      }
      if (quark == QUARK_FACTORIZE) {
	factorize ();
	return nullptr;
      }
    }
    // dispatch 1 argument
    if (argc == 1) {
      if (quark == QUARK_SETCPI) {
	Object* obj = argv->get (0);
	Cpi* cpi = dynamic_cast <Cpi*> (obj);
	if ((cpi == nullptr) && (obj != nullptr)) {
	  throw Exception ("type-error", "invalid object with set-permutation",
			   Object::repr (obj));
	}
	setcpi (cpi);
	return nullptr;
      }
    }
    // call the solver methods
    return Solver::apply (zobj, nset, quark, argv);
  }
}
