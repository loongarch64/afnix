// ---------------------------------------------------------------------------
// - Ecp.cpp                                                                 -
// - afnix:sec module - elliptic curve point class implementation            -
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

#include "Ecp.hpp"
#include "Ecc.hxx"
#include "Vector.hpp"
#include "Boolean.hpp"
#include "Integer.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a default point

  Ecp::Ecp (void) {
    d_x = 0;
    d_y = 0;
  }

  // create a point by value

  Ecp::Ecp (const Relatif& x, const Relatif& y) {
    d_x = x;
    d_y = y;
  }
  
  // copy construct this point

  Ecp::Ecp (const Ecp& that) {
    that.rdlock ();
    try {
      d_x = that.d_x;
      d_y = that.d_y;
      that.unlock ();
    } catch (...) {
      that.unlock ();
      throw;
    }
  }

  // copy move this point

  Ecp::Ecp (Ecp&& that) noexcept {
    that.wrlock ();
    try {
      d_x = that.d_x; that.d_x = 0;
      d_y = that.d_y; that.d_y = 0;
    } catch (...) {
      d_x = 0;
      d_y = 0;
    }
    that.unlock ();
  }

  // return the class name

  String Ecp::repr (void) const {
    return "Ecp";
  }

  // return a clone of this object

  Object* Ecp::clone (void) const {
    return new Ecp (*this);
  }

  // assign a ecp to this one

  Ecp& Ecp::operator = (const Ecp& that) {
    // check for self assignation
    if (this == &that) return *this;
    // lock and assign
    wrlock ();
    that.rdlock ();
    try {
      d_x = that.d_x;
      d_y = that.d_y;
      unlock ();
      that.unlock ();
      return *this;
    } catch (...) {
      unlock ();
      that.unlock ();
      throw;
    }
  }

  // move a point to this one

  Ecp& Ecp::operator = (Ecp&& that) noexcept {
    // check for self assignation
    if (this == &that) return *this;
    // lock and assign
    wrlock ();
    that.rdlock ();
    try {
      d_x = that.d_x; that.d_x = 0;
      d_y = that.d_y; that.d_y = 0;
    } catch (...) {
      d_x = 0;
      d_y = 0;
    }
    unlock ();
    that.unlock ();
    return *this;
  }
  
  // reset this point

  void Ecp::reset (void) {
    wrlock ();
    try {
      d_x = 0;
      d_y = 0;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // check if a point is nil

  bool Ecp::isnil (void) const {
    rdlock ();
    try {
      bool result = d_x.iszero () && d_y.iszero ();
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
    
  // set the point coordinates

  void Ecp::set (const Relatif& x, const Relatif& y) {
    wrlock ();
    try {
      d_x = x;
      d_y = y;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // set the point x coordinate

  void Ecp::setx (const Relatif& x) {
    wrlock ();
    try {
      d_x = x;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the point x coordinate

  Relatif Ecp::getx (void) const {
    rdlock ();
    try {
      Relatif result = d_x;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set the point y coordinate

  void Ecp::sety (const Relatif& y) {
    wrlock ();
    try {
      d_y = y;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // get the point y coordinate

  Relatif Ecp::gety (void) const {
    rdlock ();
    try {
      Relatif result = d_y;
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
  static const long QUARK_ZONE_LENGTH = 7;
  static QuarkZone  zone (QUARK_ZONE_LENGTH);

  // the ecp supported quarks
  static const long QUARK_SET   = zone.intern ("set");
  static const long QUARK_SETX  = zone.intern ("set-x");
  static const long QUARK_GETX  = zone.intern ("get-x");
  static const long QUARK_SETY  = zone.intern ("set-y");
  static const long QUARK_GETY  = zone.intern ("get-y");
  static const long QUARK_NILP  = zone.intern ("nil-p");
  static const long QUARK_RESET = zone.intern ("reset");
  
  // create a new object in a generic way
  
  Object* Ecp::mknew (Vector* argv) {
    long argc = (argv == nullptr) ? 0 : argv->length ();
    // check for 0 argument
    if (argc == 0) return new Ecp;
    // check for 2 arguments
    if (argc == 2) {
      Relatif x = ecc_to_relatif (argv->get (0));
      Relatif y = ecc_to_relatif (argv->get (1));
      return new Ecp (x, y);
    }
    // invalid arguments
    throw Exception ("argument-error", "too many arguments with ecp");
  }

  // return true if the given quark is defined

  bool Ecp::isquark (const long quark, const bool hflg) const {
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
  
  Object* Ecp::apply (Evaluable* zobj, Nameset* nset, const long quark,
		      Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();
    
    // check for 0 argument
    if (argc == 0) {
      if (quark == QUARK_NILP) return new Boolean (isnil());
      if (quark == QUARK_GETX) return new Relatif (getx ());
      if (quark == QUARK_GETY) return new Relatif (gety ());
      if (quark == QUARK_RESET) {
	reset ();
	return nullptr;
      }
    }
    // check for 1 argument
    if (argc == 1) {
      if (quark == QUARK_SETX) {
	Relatif x = 0;
	Object* ox = argv->get (0);
	// check for a relatif
	auto rx = dynamic_cast<Relatif*>(ox);
	if (rx != nullptr ) x = *rx;
	else {
	  // check for an integer
	  auto ix = dynamic_cast<Integer*>(ox);
	  if (ix != nullptr) x = ix->tolong(); else {
	    throw Exception ("ecp-error", "invalid object as x coordinate",
			     Object::repr (ox));
	  }
	}
	setx (x);
	return nullptr;
      }
      if (quark == QUARK_SETY) {
	Relatif y = 0;
	Object* oy = argv->get (0);
	// check for a relatif
	auto ry = dynamic_cast<Relatif*>(oy);
	if (ry != nullptr ) y = *ry;
	else {
	  // check for an integer
	  auto iy = dynamic_cast<Integer*>(oy);
	  if (iy != nullptr) y = iy->tolong(); else {
	    throw Exception ("ecp-error", "invalid object as x coordinate",
			     Object::repr (oy));
	  }
	}
	sety (y);
	return nullptr;
      }
    }
    // check for 2 arguments
    if (argc == 2) {
      if (quark == QUARK_SET) {
	// initialize point
	Relatif x = 0; Relatif y = 0;
	// collect x
	Object* ox = argv->get (0);
	// check for a relatif
	auto rx = dynamic_cast<Relatif*>(ox);
	if (rx != nullptr ) x = *rx;
	else {
	  // check for an integer
	  auto ix = dynamic_cast<Integer*>(ox);
	  if (ix != nullptr) x = ix->tolong(); else {
	    throw Exception ("ecp-error", "invalid object as x coordinate",
			     Object::repr (ox));
	  }
	}
	// collect y
	Object* oy = argv->get (1);
	// check for a relatif
	auto ry = dynamic_cast<Relatif*>(oy);
	if (ry != nullptr ) y = *ry;
	else {
	  // check for an integer
	  auto iy = dynamic_cast<Integer*>(oy);
	  if (iy != nullptr) y = iy->tolong(); else {
	    throw Exception ("ecp-error", "invalid object as y coordinate",
			     Object::repr (oy));
	  }
	}
	set (x, y);
	return nullptr;
      }
    }
    // call the object method
    return Object::apply (zobj, nset, quark, argv);
  }
}
