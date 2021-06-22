// ---------------------------------------------------------------------------
// - Rvector.cpp                                                             -
// - afnix:mth module - real vector implementation                           -
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

#include "Real.hpp"
#include "Math.hpp"
#include "Mthsid.hxx"
#include "Vector.hpp"
#include "Rvector.hpp"
#include "Algebra.hpp"
#include "Utility.hpp"
#include "Exception.hpp"
 
namespace afnix {

  // -------------------------------------------------------------------------
  // - public section                                                        -
  // -------------------------------------------------------------------------

  // generate a random vector by size

  Rvector* Rvector::random (const t_long size, 
			   const t_real rmin, const t_real rmax) {
    // create a vector by size
    Rvector* result = new Rvector(size);
    // fill the vector
    Algebra::random (*result, rmin, rmax);
    // done
    return  result;
  }
    
  // add a vector with a scalar

  Rvector operator + (const Rvector& x, const t_real s) {
    x.rdlock ();
    try {
      // create a result vector
      Rvector r (x.getsize ());
      // add the scalar
      r.add (x, s);
      // unlock and return
      x.unlock ();
      return r;
    } catch (...) {
      x.unlock ();
      throw;
    }
  }

  // add a vector with another one

  Rvector operator + (const Rvector& x, const Rvector& y) {
    x.rdlock ();
    y.rdlock ();
    try {
      // create a result vector
      Rvector r (x.getsize ());
      // add the scalar
      r.add (x, y);
      // unlock and return
      x.unlock ();
      y.unlock ();
      return r;
    } catch (...) {
      x.unlock ();
      y.unlock ();
      throw;
    }
  }

  // substract a vector with a scalar

  Rvector operator - (const Rvector& x, const t_real s) {
    x.rdlock ();
    try {
      // create a result vector
      Rvector r (x.getsize ());
      // add the scalar
      r.sub (x, s);
      // unlock and return
      x.unlock ();
      return r;
    } catch (...) {
      x.unlock ();
      throw;
    }
  }

  // substract a vector with another one

  Rvector operator - (const Rvector& x, const Rvector& y) {
    x.rdlock ();
    y.rdlock ();
    try {
      // create a result vector
      Rvector r (x.getsize ());
      // add the scalar
      r.sub (x, y);
      // unlock and return
      x.unlock ();
      y.unlock ();
      return r;
    } catch (...) {
      x.unlock ();
      y.unlock ();
      throw;
    }
  }

  // multiply a vector with a scalar

  Rvector operator * (const Rvector& x, const t_real s) {
    x.rdlock ();
    try {
      // create a result vector
      Rvector r (x.getsize ());
      // add the scalar
      r.mul (x, s);
      // unlock and return
      x.unlock ();
      return r;
    } catch (...) {
      x.unlock ();
      throw;
    }
  }

  // divide a vector with a scalar

  Rvector operator / (const Rvector& x, const t_real s) {
    x.rdlock ();
    try {
      // create a result vector
      Rvector r (x.getsize ());
      // add the scalar
      r.mul (x, (1.0 / s));
      // unlock and return
      x.unlock ();
      return r;
    } catch (...) {
      x.unlock ();
      throw;
    }
  }

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a default vector

  Rvector::Rvector (void) {
    p_vtab = nullptr;
  }

  // create a vector by size

  Rvector::Rvector (const t_long size) : Rvi (size) {
    p_vtab = nullptr;
    preset ();
  }
  
  // copy construct this vector

  Rvector::Rvector (const Rvector& that) {
    that.rdlock ();
    try {
      // copy base class
      Rvi::operator = (that);
      // copy locally
      p_vtab = nullptr;
      if (d_size > 0) {
	p_vtab = new t_real[d_size];
	for (t_long i = 0; i < d_size; i++) p_vtab[i] = that.p_vtab[i];
      }
      that.unlock ();
    } catch (...) {
      that.unlock ();
      throw;
    }
  }
	
  // destroy this vector

  Rvector::~Rvector (void) {
    delete [] p_vtab;
  }

  // assign a vector to this one

  Rvector& Rvector::operator = (const Rvector& that) {
    // check for self-assignation
    if (this == &that) return *this;
    // lock and assign
    wrlock ();
    that.rdlock ();
    try {
      // clean locally
      delete [] p_vtab; p_vtab = nullptr;
      // assign base class
      Rvi::operator = (that);
      // assign locally
      if (d_size > 0) {
	p_vtab = new t_real[d_size];
	for (t_long i = 0; i < d_size; i++) p_vtab[i] = that.p_vtab[i];
      }
      // unlock and return
      unlock ();
      that.unlock ();
      return *this;
    } catch (...) {
      that.unlock ();
      throw;
    }
  }

  // compare two vectors

  bool Rvector::operator == (const Rvi& x) const {
    rdlock ();
    x.rdlock ();
    try {
      // check size first
      if (d_size != x.getsize ()) {
	throw Exception ("vector-error", 
			 "incompatible size with operator ==");
      }
      // initialize result
      bool result = true;
      // loop in locked mode
      for (t_long i = 0; i < d_size; i++) {
	t_real ti = nlget (i);
	t_real xi = x.nlget (i);
	if (ti != xi) {
	  result = false;
	  break;
	}
      }
      // unlock and return
      unlock ();
      x.unlock ();
      return result;
    } catch (...) {
      unlock ();
      x.unlock ();
      throw;
    }
  }

  // compute the vector dot product

  t_real Rvector::operator ^ (const Rvi& x) const {
    rdlock ();
    x.rdlock ();
    try {
      // check size compatibility
      if (d_size != x.getsize ()) {
	throw Exception ("vector-error", 
			 "incompatible size with dot product");
      }
      // initialize result
      t_real result = 0.0;
      // loop in locked mode
      for (t_long i = 0; i < d_size; i++) {
	result += nlget (i) * x.nlget (i);
      }
      // unlock and return
      unlock ();
      x.unlock ();
      return result;
    } catch (...) {
      unlock ();
      x.unlock ();
      throw;
    }
  }

  // return the class name

  String Rvector::repr (void) const {
    return "Rvector";
  }

  // return a clone of this object

  Object* Rvector::clone (void) const {
    return new Rvector (*this);
  }
  
  // return the serial did

  t_word Rvector::getdid (void) const {
    return SRL_DEOD_MTH;
  }

  // return the serial sid

  t_word Rvector::getsid (void) const {
    return SRL_RVEC_SID;
  }
  
  // resize this vector

  void Rvector::resize (const t_long size) {
    wrlock ();
    try {
      if (size < 0) {
	throw Exception ("rvector-error", "invalid negatize size in resize");
      }
      if (size == 0)  {
	delete [] p_vtab;
	d_size = 0;
	p_vtab = nullptr;
	unlock ();
	return;
      }
      // do nothing if equal
      if (size == d_size) {
	unlock ();
	return;
      }
      // create a new array by size
      t_real* vtab = new t_real[size];
      // check for smaller size
      if (size < d_size) {
	for (long k = 0; k < size; k++) {
	  vtab[k] = (p_vtab == nullptr) ? 0.0 : p_vtab[k];
	}
      } else {
	for (long k = 0; k < d_size; k++) {
	  vtab[k] = (p_vtab == nullptr) ? 0.0 : p_vtab[k];
	}
	for (long k = d_size; k < size; k++) vtab[k] = 0.0;
      }
      d_size = size;
      delete [] p_vtab;
      p_vtab = vtab;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // reset this vector

  void Rvector::reset (void) {
    wrlock ();
    try {
      delete [] p_vtab;
      d_size = 0;
      p_vtab = nullptr;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // clear this vector

  void Rvector::clear (void) {
    wrlock ();
    try {
      for (t_long k = 0; k < d_size; k++) {
	p_vtab[k] = 0.0;
      }
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // preset this vector

  void Rvector::preset (void) {
    wrlock ();
    try {
      delete [] p_vtab;
      p_vtab = (d_size == 0) ? nullptr : new t_real[d_size];
      clear ();
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // compute the vector norm
  
  t_real Rvector::norm (void) const {
    rdlock ();
    try {
      t_real result = Algebra::norm (*this);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // copy a vector into this one

  Rvi& Rvector::cpy (const Rvi& x) {
    wrlock ();
    x.rdlock ();
    try {
      // check target size
      if (d_size != x.getsize ()) {
	throw Exception ("vector-error", "incompatible size in vector copy");
      }
      // loop in locked mode
      for (t_long i = 0; i < d_size; i++) nlset (i, x.nlget (i));
      unlock ();
      x.unlock ();
      return *this;
    } catch (...) {
      unlock ();
      x.unlock ();
      throw;
    }
  }

  // add a vector with a scalar

  Rvi& Rvector::add (const Rvi& x, const t_real s) {
    wrlock ();
    x.rdlock ();
    try {
      // check target size
      if (d_size != x.getsize ()) {
	throw Exception ("vector-error", "incompatible size in vector add");
      }
      // loop in locked mode
      for (t_long i = 0; i < d_size; i++) nlset (i, x.nlget (i) + s);
      unlock ();
      x.unlock ();
      return *this;
    } catch (...) {
      unlock ();
      x.unlock ();
      throw;
    }
  }

  // add a vector with another one

  Rvi& Rvector::add (const Rvi& x, const Rvi& y) {
    wrlock ();
    x.rdlock ();
    y.rdlock ();
    try {
      // check target size
      if ((d_size != x.getsize ()) || (d_size != y.getsize ())) {
	throw Exception ("vector-error", "incompatible size in vector add");
      }
      for (t_long i = 0; i < d_size; i++) {
	nlset (i, x.nlget (i) + y.nlget (i));
      }
      unlock ();
      x.unlock ();
      y.unlock ();
      return *this;
    } catch (...) {
      unlock ();
      x.unlock ();
      y.unlock ();
      throw;
    }
  }

  // add a vector with another scaled one

  Rvi& Rvector::add (const Rvi& x, const Rvi& y, const t_real s) {
    wrlock ();
    x.rdlock ();
    y.rdlock ();
    try {
      // check target size
      if ((d_size != x.getsize ()) || (d_size != y.getsize ())) {
	throw Exception ("vector-error", "incompatible size in vector add");
      }
      // loop in locked mode
      for (t_long i = 0; i < d_size; i++) {
	nlset (i,  x.nlget (i) + (s * y.nlget (i)));
      }
      unlock ();
      x.unlock ();
      y.unlock ();
      return *this;
    } catch (...) {
      unlock ();
      x.unlock ();
      y.unlock ();
      throw;
    }
  }

  // substract a vector with a scalar

  Rvi& Rvector::sub (const Rvi& x, const t_real s) {
    wrlock ();
    x.rdlock ();
    try {
      // check target size
      if (d_size != x.getsize ()) {
	throw Exception ("vector-error", "incompatible size in vector sub");
      }
      for (t_long i = 0; i < d_size; i++) nlset (i, x.nlget (i) - s);
      unlock ();
      x.unlock ();
      return *this;
    } catch (...) {
      unlock ();
      x.unlock ();
      throw;
    }
  }

  // substract a vector with another one

  Rvi& Rvector::sub (const Rvi& x, const Rvi& y) {
    wrlock ();
    x.rdlock ();
    y.rdlock ();
    try {
      // check target size
      if ((d_size != x.getsize ()) || (d_size != y.getsize ())) {
	throw Exception ("vector-error", "incompatible size in vector sub");
      }
      // loop in locked mode
      for (t_long i = 0; i < d_size; i++) {
	nlset (i, x.nlget (i) - y.nlget (i));
      }
      unlock ();
      x.unlock ();
      y.unlock ();
      return *this;
    } catch (...) {
      unlock ();
      x.unlock ();
      y.unlock ();
      throw;
    }
  }

  // multiply a vector with a scalar

  Rvi& Rvector::mul (const Rvi& x, const t_real s) {
    wrlock ();
    x.rdlock ();
    try {
      // check target size
      if (d_size != x.getsize ()) {
	throw Exception ("vector-error", "incompatible size in vector mul");
      }
      // loop in locked mode
      for (t_long i = 0; i < d_size; i++) nlset (i, s * x.nlget (i));
      unlock ();
      x.unlock ();
      return *this;
    } catch (...) {
      unlock ();
      x.unlock ();
      throw;
    }
  }

  // multiply a vector with another one

  Rvi& Rvector::mul (const Rvi& x, const Rvi& y) {
    wrlock ();
    x.rdlock ();
    y.rdlock ();
    try {
      // check target size
      if ((d_size != x.getsize ()) || (d_size != y.getsize ())) {
	throw Exception ("vector-error", "incompatible size in vector mul");
      }
      // loop in locked mode
      for (t_long i = 0; i < d_size; i++) {
	nlset (i , x.nlget (i) * y.nlget (i));
      }
      unlock ();
      x.unlock ();
      y.unlock ();
      return *this;
    } catch (...) {
      unlock ();
      x.unlock ();
      y.unlock ();
      throw;
    }
  }

  // divide a vector with another one

  Rvi& Rvector::div (const Rvi& x, const Rvi& y) {
    wrlock ();
    x.rdlock ();
    y.rdlock ();
    try {
      // check target size
      if ((d_size != x.getsize ()) || (d_size != y.getsize ())) {
	throw Exception ("vector-error", "incompatible size in vector div");
      }
      // loop in locked mode
      for (t_long i = 0; i < d_size; i++) {
	nlset (i, x.nlget (i) / y.nlget (i));
      }
      unlock ();
      x.unlock ();
      y.unlock ();
      return *this;
    } catch (...) {
      unlock ();
      x.unlock ();
      y.unlock ();
      throw;
    }
  }

  // add a vector with another one

  Rvi& Rvector::aeq (const Rvi& x) {
    wrlock ();
    x.rdlock ();
    try {
      // check target size
      if (d_size != x.getsize ()) {
	throw Exception ("vector-error", "incompatible size in vector aeq");
      }
      // loop in locked mode
      for (t_long i = 0; i < d_size; i++) {
	nlset (i, nlget (i) + x.nlget (i));
      }
      unlock ();
      x.unlock ();
      return *this;
    } catch (...) {
      unlock ();
      x.unlock ();
      throw;
    }
  }

  // add a vector with a scaled vector

  Rvi& Rvector::aeq (const Rvi& x, const t_real s) {
    wrlock ();
    x.rdlock ();
    try {
      // check target size
      if (d_size != x.getsize ()) {
	throw Exception ("vector-error", "incompatible size in vector aeq");
      }
      // loop in locked mode
      for (t_long i = 0; i < d_size; i++) {
	nlset (i, nlget (i) + (s * x.nlget (i)));
      }
      unlock ();
      x.unlock ();
      return *this;
    } catch (...) {
      unlock ();
      x.unlock ();
      throw;
    }
  }

  // rescale equal with a vector

  Rvi& Rvector::req (const Rvi& x, const t_real s) {
    wrlock ();
    x.rdlock ();
    try {
      // check target size
      if (d_size != x.getsize ()) {
	throw Exception ("vector-error", "incompatible size in vector req");
      }
      // loop in locked mode
      for (t_long i = 0; i < d_size; i++) {
	nlset (i, (s * nlget (i)) + x.nlget (i));
      }
      unlock ();
      x.unlock ();
      return *this;
    } catch (...) {
      unlock ();
      x.unlock ();
      throw;
    }
  }

  // permutate this vector

  Rvi* Rvector::permutate (const Cpi& p) const {
    rdlock ();
    Rvi* result = nullptr;
    try {
      // create a result vector
      result = new Rvector (d_size);
      // permutate this vector
      Algebra::permutate (*result, *this, p);
      unlock ();
      return result;
    } catch (...) {
      delete result;
      unlock ();
      throw;
    }
  }

  // reverse permutate this vector

  Rvi* Rvector::reverse (const Cpi& p) const {
    rdlock ();
    Rvi* result = nullptr;
    try {
      // create a result vector
      result = new Rvector (d_size);
      // permutate this vector
      Algebra::reverse (*result, *this, p);
      unlock ();
      return result;
    } catch (...) {
      delete result;
      unlock ();
      throw;
    }
  }

  // get the viewable size

  long Rvector::tosize (void) const {
    rdlock ();
    try {
      long result = d_size * sizeof(t_real);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the viewable data

  t_byte* Rvector::tobyte (void) {
    wrlock ();
    try {
      auto result = reinterpret_cast<t_byte*>(p_vtab);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // get the viewable data

  const t_byte* Rvector::tobyte (void) const {
    rdlock ();
    try {
      auto result = reinterpret_cast<const t_byte*>(p_vtab);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // -------------------------------------------------------------------------
  // - no lock section                                                        -
  // -------------------------------------------------------------------------

  // no lock - set a vector by position

  void Rvector::nlset (const t_long pos, const t_real val) {
    if (p_vtab != nullptr) p_vtab[pos] = val;
  }

  // no lock - get a vector by position
  
  t_real Rvector::nlget (const t_long pos) const {
    return (p_vtab == nullptr) ? 0.0 : p_vtab[pos];
  }

  // -------------------------------------------------------------------------
  // - object section                                                        -
  // -------------------------------------------------------------------------

  // create a new object in a generic way

  Object* Rvector::mknew (Vector* argv) {
    long argc = (argv == nullptr) ? 0 : argv->length ();
    
    // check for 0 argument
    if (argc == 0) return new Rvector;
    // check for 1 argument
    if (argc == 1) {
      t_long size = argv->getlong (0);
      return new Rvector (size);
    }
    // invalid arguments
    throw Exception ("argument-error", 
		     "invalid arguments with real vector object");
  }

  // operate this vector with another object

  Object* Rvector::oper (t_oper type, Object* object) {
    Real*    dobj = dynamic_cast <Real*>    (object);
    Rvector* vobj = dynamic_cast <Rvector*> (object);
    switch (type) {
    case Object::OPER_ADD:
      if (vobj != nullptr) return new Rvector (*this + *vobj);
      if (dobj != nullptr) return new Rvector (*this + dobj->toreal ());
      break;
    case Object::OPER_SUB:
      if (vobj != nullptr) return new Rvector (*this - *vobj);
      if (dobj != nullptr) return new Rvector (*this - dobj->toreal ());
      break;
    case Object::OPER_MUL:
      if (dobj != nullptr) return new Rvector (*this * dobj->toreal ());
      break;
    case Object::OPER_DIV:
      if (dobj != nullptr) return new Rvector (*this / dobj->toreal ());
      break;
    default:
      break;
    }
    // call the rvi operator
    return Rvi::oper (type, object);
  }
}

