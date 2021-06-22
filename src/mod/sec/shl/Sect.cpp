// ---------------------------------------------------------------------------
// - Sect.cpp                                                                -
// - afnix:sec module - ecc galois field class implementation                -
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

#include "Ecc.hxx"
#include "Sect.hpp"
#include "Vector.hpp"
#include "Boolean.hpp"
#include "Integer.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - private section                                                       -
  // -------------------------------------------------------------------------

  // this procedure performs a point addition
  static bool ecc_add_point (Relatif& rx, Relatif& ry,
			     const Relatif& xx, const Relatif& xy,
			     const Relatif& yx, const Relatif& yy,
			     const Relatif& a,  const Galois&   f) {
    // compute lambda (yy + xy) / (yx + xx)
    Relatif n = f.add (yy, xy); Relatif d = f.add (yx, xx);
    Relatif l = f.div (n, d);
    // update result
    rx = f.add (f.add (f.add (f.add (f.mul (l, l), l), xx), yx), a);
    ry = f.add (f.add (f.mul (l, f.add (xx, rx)), rx), xy);
    return true;
  }

  // this procedure performs a point doubling
  static bool ecc_dbl_point (Relatif& rx, Relatif& ry,
			     const Relatif& xx, const Relatif& xy,
			     const Relatif&  a, const Galois&   f) {
    // compute lambda xx + xy/xx
    Relatif l = f.add (xx, f.div (xy, xx));
    // update result
    rx = f.add (f.add (f.mul (l, l), l), a);
    ry = f.add (f.add (f.mul (l, f.add (xx, rx)), rx), xy);
    return true;
  }
  
  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a default curve

  Sect::Sect (void) {
    d_p = 0;
    d_a = 0;
    d_b = 0;
    d_g = Ecp (0, 0);
    d_n = 0;
    d_h = 0;
  }

  // create a curve by parameters

  Sect::Sect (const Relatif& p, const Relatif& a, const Relatif& b,
	      const Ecp& g,     const Relatif& n, const Relatif& h) {
    d_p = p;
    d_a = a;
    d_b = b;
    d_g = g;
    d_n = n;
    d_h = h;
    d_f = Galois (d_p);
  }

  // copy construct this curve

  Sect::Sect (const Sect& that) {
    that.rdlock ();
    try {
      d_p = that.d_p;
      d_a = that.d_a;
      d_b = that.d_b;
      d_g = that.d_g;
      d_n = that.d_n;
      d_h = that.d_h;
      d_f = that.d_f;
      that.unlock ();
    } catch (...) {
      that.unlock ();
      throw;
    }
  }
  
  // assign a curve to this one

  Sect& Sect::operator = (const Sect& that) {
    // check for self-assignation
    if (this == &that) return *this;
    // lock ad assign
    wrlock ();
    that.rdlock ();
    try {
      d_p = that.d_p;
      d_a = that.d_a;
      d_b = that.d_b;
      d_g = that.d_g;
      d_n = that.d_n;
      d_h = that.d_h;
      d_f = that.d_f;
      unlock ();
      that.unlock ();
      return *this;
    } catch (...) {
      unlock ();
      that.unlock ();
      throw;
    }
  }
  // return the class name

  String Sect::repr (void) const {
    return "Sect";
  }

  // return a clone of this object

  Object* Sect::clone (void) const {
    return new Sect (*this);
  }

  // add two points

  Ecp Sect::add (const Ecp& px, const Ecp& py) const {
    rdlock ();
    try {
      // check for null
      if (px.isnil () == true) {
	Ecp result = py;
	unlock ();
	return result;
      }
      if (py.isnil () == true) {
	Ecp result = px;
	unlock ();
	return result;
      }
      // validate the x point
      if (valid (px) == false) {
	throw Exception ("ecc-error", "invalid curve point for add");
      }
      // validate the y point
      if (valid (py) == false) {
	throw Exception ("ecc-error", "invalid curve point for add");
      }
      // collect point coordinates
      Relatif xx = px.getx(); Relatif xy = px.gety ();
      Relatif yx = py.getx(); Relatif yy = py.gety ();
      // check for opposite
      if ((xx == yx) && (xy == d_f.add (yx, yy))) {
	Ecp result;
	unlock ();
	return result;
      }
      // double or add
      Relatif rx; Relatif ry;
      if ((xx == yx) && (xy == yy)) {
	if (ecc_dbl_point (rx, ry, xx, xy, d_a, d_f) == false) {
	  throw Exception ("ecc-error", "cannot double ecc points");
	}
      } else {
	if (ecc_add_point (rx, ry, xx, xy, yx, yy, d_a, d_f) == false) {
	  throw Exception ("ecc-error", "cannot add ecc points");
	}
      }
      // set result
      Ecp result (rx, ry);
      if (valid (result) == false) {
	throw Exception ("ecc-error", "invalid curve point after add");
      }
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // negate a point

  Ecp Sect::neg (const Ecp& p) const {
    rdlock ();
    try {
      // validate the x point
      if (valid (p) == false) {
	throw Exception ("ecc-error", "invalid curve point for add");
      }
      // collect point coordinates
      Relatif x = p.getx(); Relatif y = d_f.add (p.getx (), p.gety ());
      // set result
      Ecp result (x, y);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // substract two points

  Ecp Sect::neg (const Ecp& px, const Ecp& py) const {
    rdlock ();
    try {
      Ecp result = add (px, neg(py));
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // check that a curve is valid

  bool Sect::valid (void) const {
    rdlock ();
    try {
      // check for null curve
      if ((d_p.iszero () == true) || (d_g.isnil  () == true) ||
	  (d_n.iszero () == true) || (d_h.iszero () == true)) {
	unlock ();
	return false;
      }
      // check generator
      bool result = valid (d_g);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // validate a point

  bool Sect::valid (const Ecp& p) const {
    rdlock ();
    try {
      // extract coordinates
      Relatif x = p.getx (); Relatif y = p.gety ();
      // check for zero
      if ((x.iszero () == true) && (y.iszero () == true)) {
	unlock ();
	return true;
      }
      // compute rhs = x^3 + a.x^2 + b
      Relatif  xx = d_f.mul(x, x);
      Relatif rhs = d_f.add(d_f.add(d_f.mul(xx, x), d_f.mul(d_a, xx)), d_b);
      // compute lhs = y^2 + x.y
      Relatif lhs = d_f.add(d_f.mul(y, y), d_f.mul(x,  y));
      // compare lhs with rhs 
      bool result = (rhs == lhs) ? true : false;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the curve polynomial

  Relatif Sect::getp (void) const {
    rdlock ();
    try {
      Relatif result = d_p;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the curve coefficient a

  Relatif Sect::geta (void) const {
    rdlock ();
    try {
      Relatif result = d_a;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the curve coefficient b

  Relatif Sect::getb (void) const {
    rdlock ();
    try {
      Relatif result = d_b;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the curve generator point

  Ecp Sect::getg (void) const {
    rdlock ();
    try {
      Ecp result = d_g;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the curve order n

  Relatif Sect::getn (void) const {
    rdlock ();
    try {
      Relatif result = d_n;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the curve cofactor

  Relatif Sect::geth (void) const {
    rdlock ();
    try {
      Relatif result = d_h;
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
  static const long QUARK_ZONE_LENGTH = 6;
  static QuarkZone  zone (QUARK_ZONE_LENGTH);

  // the sect supported quarks
  static const long QUARK_GETP = zone.intern ("get-polynomial");
  static const long QUARK_GETA = zone.intern ("get-coefficient-a");
  static const long QUARK_GETB = zone.intern ("get-coefficient-b");
  static const long QUARK_GETG = zone.intern ("get-generator");
  static const long QUARK_GETN = zone.intern ("get-order");
  static const long QUARK_GETH = zone.intern ("get-cofactor");

  // create a new object in a generic way
  
  Object* Sect::mknew (Vector* argv) {
    long argc = (argv == nullptr) ? 0 : argv->length ();
    // check for 0 argument
    if (argc == 0) return new Sect;
    // check for 6 arguments
    if (argc == 6) {
      Relatif p = ecc_to_relatif (argv->get(0));
      Relatif a = ecc_to_relatif (argv->get(1));
      Relatif b = ecc_to_relatif (argv->get(2));
      Ecp     g = ecc_to_ecp     (argv->get(3));
      Relatif n = ecc_to_relatif (argv->get(4));
      Relatif h = ecc_to_relatif (argv->get(5));
      return new Sect (p, a, b, g, n, h);
    }
    // invalid arguments
    throw Exception ("argument-error", "too many arguments with ecp");
  }
  
  // return true if the given quark is defined

  bool Sect::isquark (const long quark, const bool hflg) const {
    rdlock ();
    try {
      if (zone.exists (quark) == true) {
	unlock ();
	return true;
      }
      bool result = hflg ? Ecc::isquark (quark, hflg) : false;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // apply this object with a set of arguments and a quark
  
  Object* Sect::apply (Evaluable* zobj, Nameset* nset, const long quark,
		       Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();
    
    // check for 0 argument
    if (argc == 0) {
      if (quark == QUARK_GETP) return new Relatif (getp ());
      if (quark == QUARK_GETA) return new Relatif (geta ());
      if (quark == QUARK_GETB) return new Relatif (getb ());
      if (quark == QUARK_GETG) return new Ecp (getg ());
      if (quark == QUARK_GETN) return new Relatif (getn ());
      if (quark == QUARK_GETH) return new Relatif (geth ());
    }
    // call the ecc method
    return Ecc::apply (zobj, nset, quark, argv);
  }
}
