// ---------------------------------------------------------------------------
// - Parallel.cpp                                                            -
// - afnix:mth module - parallel solver class implementation                 -
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
#include "Integer.hpp"
#include "Parallel.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"
 
namespace afnix {

  // -------------------------------L------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a default parallel solver

  Parallel::Parallel (void) {
    d_tnum = 0L;
  }

  // reset this solver

  void Parallel::reset (void) {
    wrlock ();
    try {
      // reset base
      Solver::reset ();
      // reset locally
      d_tnum = 0L;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set the number of tasks

  void Parallel::settnum (const long tnum) {
    wrlock ();
    try {
      d_tnum = (tnum < 0) ? 0L : tnum;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the number of tasks

  long Parallel::gettnum (void) const {
    rdlock ();
    try {
      long result = d_tnum;
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
  static const long QUARK_ZONE_LENGTH = 2;
  static QuarkZone  zone (QUARK_ZONE_LENGTH);

  // the object supported quarks
  static const long QUARK_SETTNUM = zone.intern ("set-task-number");
  static const long QUARK_GETTNUM = zone.intern ("get-task-number");

  // return true if the given quark is defined

  bool Parallel::isquark (const long quark, const bool hflg) const {
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
  
  Object* Parallel::apply (Evaluable* zobj, Nameset* nset, const long quark,
			   Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();
    
    // dispatch 0 argument
    if (argc == 0) {
      if (quark == QUARK_GETTNUM) return new Integer (gettnum ());
    }
    // dispatch 1 argument
    if (argc == 1) {
      if (quark == QUARK_SETTNUM) {
	t_long mni = argv->getlong (0);
	settnum (mni);
	return nullptr;
      }
    }
    // call the solver methods
    return Solver::apply (zobj, nset, quark, argv);
  }
}
