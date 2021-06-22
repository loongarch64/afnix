// ---------------------------------------------------------------------------
// - Galois.cpp                                                              -
// - afnix:sec module - galois field arithmetic class implementation         -
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
#include "Galois.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a default galois field

  Galois::Galois (void) {
    d_poly = 0L;
  }

  // create a galois field by polynome

  Galois::Galois (const Relatif& poly) {
    d_poly = poly;
  }

  // copy construct this object

  Galois::Galois (const Galois& that) {
    that.rdlock ();
    try {
      d_poly = that.d_poly;
      that.unlock ();
    } catch (...) {
      that.unlock ();
      throw;
    }
  }

  // assign an object to this one

  Galois& Galois::operator = (const Galois& that) {
    // check for self-assignation
    if (this == &that) return *this;
    // lock and assign
    wrlock ();
    that.rdlock ();
    try {
      d_poly = that.d_poly;
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

  String Galois::repr (void) const {
    return "Galois";
  }

  // return a clone of this object

  Object* Galois::clone (void) const {
    return new Galois (*this);
  }

  // compute the modulus of a relatif

  Relatif Galois::mod (const Relatif& x) const {
    rdlock ();
    try {
      // check for null polynome
      if (d_poly.iszero () == true) {
	unlock ();
	return x;
      }
      // get the msbs
      long xmsb = x.getmsb ();
      long pmsb = d_poly.getmsb ();
      // remove normal case
      if (xmsb < pmsb) {
	unlock ();
	return x;
      }
      // perform a long division
      Relatif r = x; long rmsb = xmsb;
      while (rmsb >= pmsb) {
	r = r ^ (d_poly << (rmsb - pmsb));
	rmsb = r.getmsb ();
      }
      unlock ();
      return r;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // add two relatifs in this field

  Relatif Galois::add (const Relatif& x, const Relatif& y) const {
    rdlock ();
    try {
      // check for null polynome
      if (d_poly.iszero () == true) {
	Relatif z = x + y;
	unlock ();
	return z;
      }
      // evalute result
      Relatif r = mod (x ^ y);
      unlock ();
      return r;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // multiply two relatifs in this field

  Relatif Galois::mul (const Relatif& x, const Relatif& y) const {
    rdlock ();
    try {
      // check for null polynomial
      if (d_poly.iszero () == true) {
	Relatif z = x * y;
	unlock ();
	return z;
      }
      // normalize arguments
      Relatif xx = mod (x);
      Relatif yy = mod (y);
      // get the polynomial mask
      Relatif mask = Relatif(1) << d_poly.getmsb() - 2L;
      // preset result and loop
      Relatif r = 0;
      while ((xx.iszero() == false) && (yy.iszero () == false)) {
	// add a to the result if y is odd
	if (yy.isodd () == true) r = add (r, xx);
	if ((xx & mask) != 0) {
	  xx = (xx << 1) ^ d_poly;
	} else {
	  xx = xx << 1;
	}
	yy = yy >> 1;
      }
      unlock ();
      return r;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // galois gcm multiplication

  Relatif Galois::gmm (const Relatif& x, const Relatif& y) const {
    rdlock ();
    try {
      // check for null polynomial
      if (d_poly.iszero () == true) {
	throw Exception ("gcm-error", "invalid null polynomial with gmul");
      }
      // rebuild reverse polynomial
      Relatif poly = d_poly.reverse (d_poly.getmsb ()-1L);
      // check valid operands
      long pmsb = poly.getmsb ();
      if ((x.getmsb () > pmsb) || (y.getmsb () > pmsb)) {
	throw Exception ("gcm-error", "out of range operands with gmul");
      }
      // nist fips 800-38D multiplication
      Relatif v = y;
      Relatif z = 0;
      for (long k = 0L; k < pmsb; k++) {
	// check bit at position
	if (x.getbit (pmsb-1-k) == true) z = z ^ v;
	// adjust operand
	if (v.isodd () == true) {
	  v = (v >> 1) ^ poly;
	} else {
	  v = v >> 1;
	}
      }
      if (z.getmsb () > pmsb) {
	throw Exception ("galois-error", "out of range result with gmm");
      }
      unlock ();
      return z;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // inverse a relatif in this field

  Relatif Galois::inv (const Relatif& x) const {
    rdlock ();
    try {
      // check for null polynomial
      if (d_poly.iszero () == true) {
	throw Exception ("galois-error", "invalid null polynomial with inv");
      }
      // extended euclidian algorithm
      Relatif a = x; Relatif b = d_poly;
      Relatif u = 1; Relatif v = 0;
      while (a != 1) {
	long j = (a.getmsb() - 1L) - (b.getmsb () - 1L);
	if (j < 0L) {
	  Relatif t = a; a = b; b = t; t = u; u = v; v = t; j = -j;
	}
	a = add (a, (b << j));
	u = add (u, (v << j));
      }
      unlock ();
      return u;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // divide two relatifs in this field

  Relatif Galois::div (const Relatif& x, const Relatif& y) const {
    rdlock ();
    try {
      // check for null polynomial
      if (d_poly.iszero () == true) {
	Relatif z = x / y;
	unlock ();
	return z;
      }
      // divide by inversion
      Relatif result = mul (x, inv (y));
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

  // the signature supported quarks
  static const long QUARK_MOD = zone.intern ("mod");
  static const long QUARK_ADD = zone.intern ("add");
  static const long QUARK_MUL = zone.intern ("mul");
  static const long QUARK_GMM = zone.intern ("gmm");
  static const long QUARK_INV = zone.intern ("inv");
  static const long QUARK_DIV = zone.intern ("div");

  // create a new object in a generic way
  
  Object* Galois::mknew (Vector* argv) {
    long argc = (argv == nullptr) ? 0 : argv->length ();
    // check for 0 argument
    if (argc == 0) return new Galois;
    // check for 1 argument
    if (argc == 1) {
      // get the polynom
      Object* obj = argv->get (0);
      // check for a relatif
      auto poly = dynamic_cast <Relatif*> (obj);
      if (poly == nullptr) {
	throw Exception ("type-error",
			 "invalid object with galois field constructor",
			 Object::repr (obj));
      }
      return new Galois (*poly);
    }
    // invalid arguments
    throw Exception ("argument-error", "too many arguments with galois field");
  }

  // return true if the given quark is defined

  bool Galois::isquark (const long quark, const bool hflg) const {
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
  
  Object* Galois::apply (Evaluable* zobj, Nameset* nset, const long quark,
			 Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();
    
    // check for 1 argument
    if (argc == 1) {
      if (quark == QUARK_MOD) {
	Object* obj = argv->get (0);
	auto x = dynamic_cast<Relatif*>(obj);
	if (x == nullptr) {
	  throw Exception ("type-error", "invalid object with galois mod",
                           Object::repr (obj));
        }
	return new Relatif (mod (*x));
      }
      if (quark == QUARK_INV) {
	Object* obj = argv->get (0);
	auto x = dynamic_cast<Relatif*>(obj);
	if (x == nullptr) {
	  throw Exception ("type-error", "invalid object with galois inv",
                           Object::repr (obj));
        }
	return new Relatif (inv (*x));
      }
    }
    // check for 2 arguments
    if (argc == 2) {
      if (quark == QUARK_ADD) {
	Object* obj = argv->get (0);
	auto x = dynamic_cast<Relatif*>(obj);
	if (x == nullptr) {
	  throw Exception ("type-error", "invalid object with galois add",
                           Object::repr (obj));
        }
	obj = argv->get (1);
	auto y = dynamic_cast<Relatif*>(obj);
	if (y != nullptr) return new Relatif (add (*x, *y));
	// invald object
	throw Exception ("type-error", "invalid object with galois add",
			 Object::repr (obj));
      }
      if (quark == QUARK_MUL) {
	Object* obj = argv->get (0);
	auto x = dynamic_cast<Relatif*>(obj);
	if (x == nullptr) {
	  throw Exception ("type-error", "invalid object with galois mul",
                           Object::repr (obj));
        }
	obj = argv->get (1);
	auto y = dynamic_cast<Relatif*>(obj);
	if (y != nullptr) return new Relatif (mul (*x, *y));
	// invald object
	throw Exception ("type-error", "invalid object with galois mul",
			 Object::repr (obj));
      }
      if (quark == QUARK_GMM) {
	Object* obj = argv->get (0);
	auto x = dynamic_cast<Relatif*>(obj);
	if (x == nullptr) {
	  throw Exception ("type-error", "invalid object with galois gmm",
                           Object::repr (obj));
        }
	obj = argv->get (1);
	auto y = dynamic_cast<Relatif*>(obj);
	if (y != nullptr) return new Relatif (gmm (*x, *y));
	// invald object
	throw Exception ("type-error", "invalid object with galois gmm",
			 Object::repr (obj));
      }
      if (quark == QUARK_DIV) {
	Object* obj = argv->get (0);
	auto x = dynamic_cast<Relatif*>(obj);
	if (x == nullptr) {
	  throw Exception ("type-error", "invalid object with galois div",
                           Object::repr (obj));
        }
	obj = argv->get (1);
	auto y = dynamic_cast<Relatif*>(obj);
	if (y != nullptr) return new Relatif (div (*x, *y));
	// invald object
	throw Exception ("type-error", "invalid object with galois div",
			 Object::repr (obj));
      }
    }
    // call the object method
    return Object::apply (zobj, nset, quark, argv);
  }
}
