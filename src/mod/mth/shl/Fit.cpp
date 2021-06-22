// ---------------------------------------------------------------------------
// - Fit.cpp                                                                 -
// - afnix:mth module - Model fitting interface implementation               -
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

#include "Fit.hpp"
#include "Real.hpp"
#include "Vector.hpp"
#include "Boolean.hpp"
#include "Integer.hpp"
#include "Evaluable.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"
 
namespace afnix {

  // -------------------------------------------------------------------------
  // - object section                                                        -
  // -------------------------------------------------------------------------

  // the quark zone
  static const long QUARK_ZONE_LENGTH = 7;
  static QuarkZone  zone (QUARK_ZONE_LENGTH);

  // the object supported quarks
  static const long QUARK_ADDP    = zone.intern ("add-predictor");
  static const long QUARK_ADDR    = zone.intern ("add-response");
  static const long QUARK_RESET   = zone.intern ("reset");
  static const long QUARK_CLEAR   = zone.intern ("clear");
  static const long QUARK_FACTOR  = zone.intern ("factorize");
  static const long QUARK_REGRESS = zone.intern ("regress");
  static const long QUARK_GETSIZE = zone.intern ("get-size");

  // return true if the given quark is defined

  bool Fit::isquark (const long quark, const bool hflg) const {
    rdlock ();
    try {
      if (zone.exists (quark) == true) {
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
  
  Object* Fit::apply (Evaluable* zobj, Nameset* nset, const long quark,
		      Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();

    // dispatch 0 argument
    if (argc == 0) {
      if (quark == QUARK_FACTOR) return new Boolean (factorize ());
      if (quark == QUARK_GETSIZE) return new Integer (getsize());
      if (quark == QUARK_CLEAR) {
	clear ();
	return nullptr;
      }
      if (quark == QUARK_RESET) {
	reset ();
	return nullptr;
      }
    }
    // dispatch 1 argument
    if (argc == 1) {
      if (quark == QUARK_ADDP) {
	t_real pv = argv->getrint (0);
	addp (pv);
	return nullptr;
      }
      if (quark == QUARK_ADDR) {
	t_real rv = argv->getrint (0);
	addr (rv);
	return nullptr;
      }
      if (quark == QUARK_REGRESS) {
	t_real rv = argv->getrint (0);
	return new Real (regress (rv));
      }
    }
    // call the object method
    return Object::apply (zobj, nset, quark, argv);
  }
}

