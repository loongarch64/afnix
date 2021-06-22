// ---------------------------------------------------------------------------
// - Ecc.cpp                                                                 -
// - afnix:sec module - elliptic curve cryptography class implementation     -
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

#include "Ecc.hpp"
#include "Ecc.hxx"
#include "Vector.hpp"
#include "Boolean.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - class section                                                        -
  // -------------------------------------------------------------------------

  // multiply a point by a scalar

  Ecp Ecc::mul (const Relatif& s, const Ecp& p) const {
    rdlock ();
    try {
      // check for nil
      if ((s.iszero () ==  true) || (p.isnil () == true)) {
	Ecp zero;
	unlock ();
	return zero;
      }
      // double and add
      Ecp d = p; Ecp result;
      long smsb = s.getmsb ();
      for (long k = 0; k < smsb; k++) {
	if (s.getbit (k) == true) result = add (result, d);
	d = add (d, d);
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
  static const long QUARK_ZONE_LENGTH = 4;
  static QuarkZone  zone (QUARK_ZONE_LENGTH);

  // the ecc supported quarks
  static const long QUARK_ADD    = zone.intern ("add");
  static const long QUARK_NEG    = zone.intern ("neg");
  static const long QUARK_MUL    = zone.intern ("mul");
  static const long QUARK_VALIDP = zone.intern ("valid-p");
  
  // return true if the given quark is defined

  bool Ecc::isquark (const long quark, const bool hflg) const {
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
  
  Object* Ecc::apply (Evaluable* zobj, Nameset* nset, const long quark,
		      Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();
    
    // check for 0 argument
    if (argc == 0) {
      if (quark == QUARK_VALIDP) return new Boolean (valid ());
    }
    // check for 1 argument
    if (argc == 1) {
      if (quark == QUARK_VALIDP) {
	Ecp p = ecc_to_ecp (argv->get (0));
	return new Boolean (valid (p));
      }
      if (quark == QUARK_NEG) {
	Ecp p = ecc_to_ecp (argv->get (0));
	return new Ecp (neg (p));
      }
    }
    // check for 2 arguments
    if (argc == 2) {
      if (quark == QUARK_ADD) {
	Ecp px = ecc_to_ecp (argv->get (0));
	Ecp py = ecc_to_ecp (argv->get (1));
	return new Ecp (add (px, py));
      }
      if (quark == QUARK_NEG) {
	Ecp px = ecc_to_ecp (argv->get (0));
	Ecp py = ecc_to_ecp (argv->get (1));
	return new Ecp (neg (px, py));
      }
      if (quark == QUARK_MUL) {
	Object* obj = argv->get (0);
	Ecp p = ecc_to_ecp (argv->get (1));
	// check for an integer
	auto iobj = dynamic_cast<Integer*>(obj);
	if (iobj != nullptr) return new Ecp (mul (iobj->tolong(), p));
	// check for a relatif
	auto s = dynamic_cast<Relatif*>(obj);
	if (s != nullptr) return new Ecp (mul (*s, p));
	// invalid object
	throw Exception ("type-error", "invalid scalar with ecc mul",
			 Object::repr (s));
      }
    }
    // call the object method
    return Object::apply (zobj, nset, quark, argv);
  }
}
