// ---------------------------------------------------------------------------
// - Ivector.cpp                                                             -
// - afnix:mth module - integer vector implementation                        -
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

#include "Math.hpp"
#include "Mthsid.hxx"
#include "Vector.hpp"
#include "Ivector.hpp"
#include "Integer.hpp"
#include "Algebra.hpp"
#include "Utility.hpp"
#include "Exception.hpp"
 
namespace afnix {

  // -------------------------------------------------------------------------
  // - public section                                                        -
  // -------------------------------------------------------------------------

  // generate a random vector by size

  Ivector* Ivector::random (const t_long size,
			   const long rmin, const long rmax) {
    // create a vector by size
    Ivector* result = new Ivector(size);
    // fill the vector
    Algebra::random (*result, rmin, rmax);
    // done
    return  result;
  }
    
  // add a vector with a scalar

  Ivector operator + (const Ivector& x, const long s) {
    x.rdlock ();
    try {
      // create a result vector
      Ivector r (x.getsize ());
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

  Ivector operator + (const Ivector& x, const Ivector& y) {
    x.rdlock ();
    y.rdlock ();
    try {
      // create a result vector
      Ivector r (x.getsize ());
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

  Ivector operator - (const Ivector& x, const long s) {
    x.rdlock ();
    try {
      // create a result vector
      Ivector r (x.getsize ());
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

  Ivector operator - (const Ivector& x, const Ivector& y) {
    x.rdlock ();
    y.rdlock ();
    try {
      // create a result vector
      Ivector r (x.getsize ());
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

  Ivector operator * (const Ivector& x, const long s) {
    x.rdlock ();
    try {
      // create a result vector
      Ivector r (x.getsize ());
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

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a default vector

  Ivector::Ivector (void) {
    p_vtab = nullptr;
  }

  // create a vector by size

  Ivector::Ivector (const t_long size) : Ivi (size) {
    p_vtab = nullptr;
    preset ();
  }
  
  // copy construct this vector

  Ivector::Ivector (const Ivector& that) {
    that.rdlock ();
    try {
      // copy base class
      Ivi::operator = (that);
      // copy locally
      p_vtab = nullptr;
      if (d_size > 0) {
	p_vtab = new long[d_size];
	for (t_long i = 0; i < d_size; i++) p_vtab[i] = that.p_vtab[i];
      }
      that.unlock ();
    } catch (...) {
      that.unlock ();
      throw;
    }
  }
	
  // destroy this vector

  Ivector::~Ivector (void) {
    delete [] p_vtab;
  }

  // assign a vector to this one

  Ivector& Ivector::operator = (const Ivector& that) {
    // check for self-assignation
    if (this == &that) return *this;
    // lock and assign
    wrlock ();
    that.rdlock ();
    try {
      // clean locally
      delete [] p_vtab; p_vtab = nullptr;
      // assign base class
      Ivi::operator = (that);
      // assign locally
      if (d_size > 0) {
	p_vtab = new long[d_size];
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

  bool Ivector::operator == (const Ivi& x) const {
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
	long ti = nlget (i);
	long xi = x.nlget (i);
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

  // return the class name

  String Ivector::repr (void) const {
    return "Ivector";
  }

  // return a clone of this object

  Object* Ivector::clone (void) const {
    return new Ivector (*this);
  }
  
  // return the serial did

  t_word Ivector::getdid (void) const {
    return SRL_DEOD_MTH;
  }

  // return the serial sid

  t_word Ivector::getsid (void) const {
    return SRL_IVEC_SID;
  }
  
  // resize this vector

  void Ivector::resize (const t_long size) {
    wrlock ();
    try {
      if (size < 0) {
	throw Exception ("ivector-error", "invalid negatize size in resize");
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
      long* vtab = new long[size];
      // check for smaller size
      if (size < d_size) {
	for (long k = 0; k < size; k++) {
	  vtab[k] = (p_vtab == nullptr) ? 0L : p_vtab[k];
	}
      } else {
	for (long k = 0; k < d_size; k++) {
	  vtab[k] = (p_vtab == nullptr) ? 0L : p_vtab[k];
	}
	for (long k = d_size; k < size; k++) vtab[k] = 0L;
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

  void Ivector::reset (void) {
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

  void Ivector::clear (void) {
    wrlock ();
    try {
      for (t_long k = 0; k < d_size; k++) {
	p_vtab[k] = 0L;
      }
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // preset this vector

  void Ivector::preset (void) {
    wrlock ();
    try {
      delete [] p_vtab;
      p_vtab = (d_size == 0) ? nullptr : new long[d_size];
      clear ();
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // copy a vector into this one

  Ivi& Ivector::cpy (const Ivi& x) {
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

  Ivi& Ivector::add (const Ivi& x, const long s) {
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

  Ivi& Ivector::add (const Ivi& x, const Ivi& y) {
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

  Ivi& Ivector::add (const Ivi& x, const Ivi& y, const long s) {
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

  Ivi& Ivector::sub (const Ivi& x, const long s) {
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

  Ivi& Ivector::sub (const Ivi& x, const Ivi& y) {
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

  Ivi& Ivector::mul (const Ivi& x, const long s) {
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

  Ivi& Ivector::mul (const Ivi& x, const Ivi& y) {
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

  // add a vector with another one

  Ivi& Ivector::aeq (const Ivi& x) {
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

  Ivi& Ivector::aeq (const Ivi& x, const long s) {
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

  Ivi& Ivector::req (const Ivi& x, const long s) {
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

  Ivi* Ivector::permutate (const Cpi& p) const {
    rdlock ();
    Ivi* result = nullptr;
    try {
      // create a result vector
      result = new Ivector (d_size);
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

  Ivi* Ivector::reverse (const Cpi& p) const {
    rdlock ();
    Ivi* result = nullptr;
    try {
      // create a result vector
      result = new Ivector (d_size);
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

  long Ivector::tosize (void) const {
    rdlock ();
    try {
      long result = d_size * sizeof(long);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the viewable data

  t_byte* Ivector::tobyte (void) {
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

  const t_byte* Ivector::tobyte (void) const {
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

  void Ivector::nlset (const t_long pos, const long val) {
    if (p_vtab != nullptr) p_vtab[pos] = val;
  }

  // no lock - get a vector by position
  
  long Ivector::nlget (const t_long pos) const {
    return (p_vtab == nullptr) ? 0L : p_vtab[pos];
  }

  // -------------------------------------------------------------------------
  // - object section                                                        -
  // -------------------------------------------------------------------------

  // create a new object in a generic way

  Object* Ivector::mknew (Vector* argv) {
    long argc = (argv == nullptr) ? 0 : argv->length ();
    
    // check for 0 argument
    if (argc == 0) return new Ivector;
    // check for 1 argument
    if (argc == 1) {
      t_long size = argv->getlong (0);
      return new Ivector (size);
    }
    // invalid arguments
    throw Exception ("argument-error", 
		     "invalid arguments with integer vector object");
  }

  // operate this vector with another object

  Object* Ivector::oper (t_oper type, Object* object) {
    auto dobj = dynamic_cast <Integer*> (object);
    auto vobj = dynamic_cast <Ivector*> (object);
    switch (type) {
    case Object::OPER_ADD:
      if (vobj != nullptr) return new Ivector (*this + *vobj);
      if (dobj != nullptr) return new Ivector (*this + dobj->tolong ());
      break;
    case Object::OPER_SUB:
      if (vobj != nullptr) return new Ivector (*this - *vobj);
      if (dobj != nullptr) return new Ivector (*this - dobj->tolong ());
      break;
    case Object::OPER_MUL:
      if (dobj != nullptr) return new Ivector (*this * dobj->tolong ());
      break;
    default:
      break;
    }
    // call the ivi operator
    return Ivi::oper (type, object);
  }
}

