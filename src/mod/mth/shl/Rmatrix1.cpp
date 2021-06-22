// ---------------------------------------------------------------------------
// - Rmatrix1.cpp                                                            -
// - afnix:mth module - real matrix 1 implementation                         -
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
#include "Rmatrix1.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"

namespace afnix {
  
  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create an identity matrix

  Rmatrix1::Rmatrix1 (void) {
    resize (1, 1);
    eye ();
  }
  
  // copy construct this matrix

  Rmatrix1::Rmatrix1 (const Rmatrix1& that) {
    that.rdlock ();
    try {
      Rblock::operator = (that);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }  
  }

  // assign a matrix to this one

  Rmatrix1& Rmatrix1::operator = (const Rmatrix1& that) {
    // check for self-assignation
    if (this == &that) return *this;
    // lock and assign
    wrlock ();
    that.rdlock ();
    try {
      Rblock::operator = (that);
      unlock ();
      that.unlock ();
      return *this;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return the class name

  String Rmatrix1::repr (void) const {
    return "Rmatrix1";
  }

  // return a clone of this object

  Object* Rmatrix1::clone (void) const {
    return new Rmatrix1 (*this);
  }

  // multiply a matrix with a vector

  Rvector1 Rmatrix1::operator * (const Rvector1& v) const {
    rdlock ();
    v.rdlock ();
    try {
      Rvector1 rv;
      for (long i = 0L; i < d_rsiz; i++) {
	t_real val = 0.0;
	for (long j = 0L; j < d_csiz; j++) {
	  val += nlget(i, j) * v.nlget(j);
	}
	rv.nlset (i, val);
      }
      unlock ();
      v.unlock ();
      return rv;
    } catch (...) {
      unlock ();
      v.unlock ();
      throw;
    }
  }

  // multiply a matrix with a point

  Rpoint1 Rmatrix1::operator * (const Rpoint1& p) const {
    rdlock ();
    p.rdlock ();
    try {
      Rpoint1 rp;
      for (long i = 0L; i < d_rsiz; i++) {
	t_real val = 0.0;
	for (long j = 0L; j < d_csiz; j++) {
	  val += nlget(i, j) * p.nlget(j);
	}
	rp.nlset (i, val);
      }
      unlock ();
      p.unlock ();
      return rp;
    } catch (...) {
      unlock ();
      p.unlock ();
      throw;
    }
  }

  // multiply a matrix with a matrix

  Rmatrix1 Rmatrix1::operator * (const Rmatrix1& m) const {
    rdlock ();
    m.rdlock ();
    try {
      Rmatrix1 rm;
      for (long k = 0L; k < d_csiz; k++) {
	for (long i = 0L; i < d_rsiz; i++) {
	  t_real val = 0.0;
	  for (long j = 0L; j < d_csiz; j++) {
	    val += nlget(i, j) * m.nlget(j, k);
	  }
	  rm.nlset (i, k, val);
	}
      }
      unlock ();
      m.unlock ();
      return rm;
    } catch (...) {
      unlock ();
      m.unlock ();
      throw;
    }
  }
  
  // -------------------------------------------------------------------------
  // - object section                                                        -
  // -------------------------------------------------------------------------

  // the quark zone
  static const long QUARK_ZONE_LENGTH = 1;
  static QuarkZone  zone (QUARK_ZONE_LENGTH);

  // the object supported quarks
  static const long QUARK_MUL = zone.intern ("mul");

  // create a new object in a generic way

  Object* Rmatrix1::mknew (Vector* argv) {
    long argc = (argv == nullptr) ? 0 : argv->length ();
    // check for 0 argument
    if (argc == 0) return new Rmatrix1;
    // invalid arguments
    throw Exception ("argument-error", "invalid arguments with matrix 1"); 
  }
  
  // return true if the given quark is defined
  
  bool Rmatrix1::isquark (const long quark, const bool hflg) const {
    rdlock ();
    try {
      if (zone.exists (quark) == true) {
	unlock ();
	return true;
      }
      bool result = hflg ? Rblock::isquark (quark, hflg) : false;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // apply this object with a set of arguments and a quark

  Object* Rmatrix1::apply (Evaluable* zobj, Nameset* nset, const long quark,
			  Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();
    
    // dispatch 1 argument
    if (argc == 1) {
      if (quark == QUARK_MUL) {
	Object* obj = argv->get (0);
	// check for a vector
	auto v = dynamic_cast<Rvector1*>(obj);
	if (v != nullptr) return new Rvector1 (*this * *v);
	// check for a point
	auto p = dynamic_cast<Rpoint1*>(obj);
	if (p != nullptr) return new Rpoint1 (*this * *p);
	// check for a matrix
	auto m = dynamic_cast<Rmatrix1*>(obj);
	if (m != nullptr) return new Rmatrix1 (*this * *m);
	// invalid argument
	throw Exception ("type-error", "invalid object as matrix mul argument",
			 Object::repr (obj));
      }
    }
    // call the rblock method
    return Rblock::apply (zobj, nset, quark, argv);
  }
}
