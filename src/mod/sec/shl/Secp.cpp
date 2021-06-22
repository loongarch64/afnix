// ---------------------------------------------------------------------------
// - Secp.cpp                                                                -
// - afnix:sec module - ecc primary field class implementation               -
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
#include "Secp.hpp"
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
			     const Relatif& p) {
    // compute lambda (yy - xy) / (yx - xx)
    Relatif n = yy - xy; Relatif d = yx - xx;
    Relatif l = (n * Relatif::mmi (d, p)) % p;
    // update result
    rx = (l.pow (2) - xx - yx); while (rx < 0) rx+= p; rx = rx % p;
    ry = (l * (xx - rx)  - xy); while (ry < 0) ry+= p; ry = ry % p;
    return true;
  }

  // this procedure performs a point doubling
  static bool ecc_dbl_point (Relatif& rx, Relatif& ry,
			     const Relatif& xx, const Relatif& xy,
			     const Relatif& p,  const Relatif& a) {
    // compute lambda (3*xx^2 + a) / (2*xy)
    Relatif l = ((3 * xx.pow(2) + a) * Relatif::mmi (2*xy, p)) % p;
    // update result
    rx = (l.pow (2) - 2 * xx); while (rx < 0) rx+= p; rx = rx % p;
    ry = (l * (xx - rx) - xy); while (ry < 0) ry+= p; ry = ry % p;
    return true;
  }
  
  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a default curve

  Secp::Secp (void) {
    d_p = 0;
    d_a = 0;
    d_b = 0;
    d_g = Ecp (0, 0);
    d_n = 0;
    d_h = 0;
  }

  // create a curve by parameters

  Secp::Secp (const Relatif& p, const Relatif& a, const Relatif& b,
	      const Ecp& g,     const Relatif& n, const Relatif& h) {
    d_p = p;
    d_a = a;
    d_b = b;
    d_g = g;
    d_n = n;
    d_h = h;
  }

  // copy construct this curve

  Secp::Secp (const Secp& that) {
    that.rdlock ();
    try {
      d_p = that.d_p;
      d_a = that.d_a;
      d_b = that.d_b;
      d_g = that.d_g;
      d_n = that.d_n;
      d_h = that.d_h;
      that.unlock ();
    } catch (...) {
      that.unlock ();
      throw;
    }
  }
  
  // assign a curve to this one

  Secp& Secp::operator = (const Secp& that) {
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

  String Secp::repr (void) const {
    return "Secp";
  }

  // return a clone of this object

  Object* Secp::clone (void) const {
    return new Secp (*this);
  }

  // add two points

  Ecp Secp::add (const Ecp& px, const Ecp& py) const {
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
      Relatif xx = px.getx() % d_p; Relatif xy = px.gety () % d_p;
      Relatif yx = py.getx() % d_p; Relatif yy = py.gety () % d_p;
      // check for opposite
      if ((xx == yx) && (xy == -yy)) {
	Ecp result;
	unlock ();
	return result;
      }
      // double or add
      Relatif rx; Relatif ry;
      if ((xx == yx) && (xy == yy)) {
	if (ecc_dbl_point (rx, ry, xx, xy, d_p, d_a) == false) {
	  throw Exception ("ecc-error", "cannot double ecc points");
	}
      } else {
	if (ecc_add_point (rx, ry, xx, xy, yx, yy, d_p) == false) {
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

  Ecp Secp::neg (const Ecp& p) const {
    rdlock ();
    try {
      // validate the x point
      if (valid (p) == false) {
	throw Exception ("ecc-error", "invalid curve point for add");
      }
      // collect point coordinates
      Relatif x = p.getx() % d_p; Relatif y = p.gety() % d_p;
      // set result
      Ecp result (x, -y);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // substract two points

  Ecp Secp::neg (const Ecp& px, const Ecp& py) const {
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

  bool Secp::valid (void) const {
    rdlock ();
    try {
      // check for null curve
      if ((d_p.iszero () == true) || (d_g.isnil  () == true) ||
	  (d_n.iszero () == true) || (d_h.iszero () == true)) {
	unlock ();
	return false;
      }
      // validate discriminanr
      Relatif d = (16 * (4 * d_a.pow(3) + 27 * d_b.pow (2))) % d_p;
      if (d.iszero () == true) {
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

  bool Secp::valid (const Ecp& p) const {
    rdlock ();
    try {
      // extract coordinates
      Relatif x = p.getx (); Relatif y = p.gety ();
      // check for zero
      if ((x.iszero () == true) && (y.iszero () == true)) {
	unlock ();
	return true;
      }
      // compute rhs = (x^3 + a.x + b) (mod p)
      Relatif rhs = (Relatif::mme (x, 3, d_p) + (d_a * x) + d_b) % d_p;
      // compute lhs = y^2
      Relatif lhs = Relatif::mme (p.gety(), 2, d_p);
      // compare lhs with rhs 
      bool result = (rhs == lhs) ? true : false;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the curve prime

  Relatif Secp::getp (void) const {
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

  Relatif Secp::geta (void) const {
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

  Relatif Secp::getb (void) const {
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

  Ecp Secp::getg (void) const {
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

  Relatif Secp::getn (void) const {
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

  Relatif Secp::geth (void) const {
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

  // the secp supported quarks
  static const long QUARK_GETP = zone.intern ("get-prime");
  static const long QUARK_GETA = zone.intern ("get-coefficient-a");
  static const long QUARK_GETB = zone.intern ("get-coefficient-b");
  static const long QUARK_GETG = zone.intern ("get-generator");
  static const long QUARK_GETN = zone.intern ("get-order");
  static const long QUARK_GETH = zone.intern ("get-cofactor");

  // create a new object in a generic way
  
  Object* Secp::mknew (Vector* argv) {
    long argc = (argv == nullptr) ? 0 : argv->length ();
    // check for 0 argument
    if (argc == 0) return new Secp;
    // check for 6 arguments
    if (argc == 6) {
      Relatif p = ecc_to_relatif (argv->get(0));
      Relatif a = ecc_to_relatif (argv->get(1));
      Relatif b = ecc_to_relatif (argv->get(2));
      Ecp     g = ecc_to_ecp     (argv->get(3));
      Relatif n = ecc_to_relatif (argv->get(4));
      Relatif h = ecc_to_relatif (argv->get(5));
      return new Secp (p, a, b, g, n, h);
    }
    // invalid arguments
    throw Exception ("argument-error", "too many arguments with ecp");
  }
  
  // return true if the given quark is defined

  bool Secp::isquark (const long quark, const bool hflg) const {
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
  
  Object* Secp::apply (Evaluable* zobj, Nameset* nset, const long quark,
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
