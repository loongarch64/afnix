// ---------------------------------------------------------------------------
// - Rmi.cpp                                                                 -
// - afnix:mth module - real matrix interface implementation                 -
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

#include "Rmi.hpp"
#include "Real.hpp"
#include "Math.hpp"
#include "Item.hpp"
#include "Mthsid.hxx"
#include "Vector.hpp"
#include "Utility.hpp"
#include "Algebra.hpp"
#include "Boolean.hpp"
#include "Evaluable.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"
#include "OutputStream.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - private section                                                       -
  // -------------------------------------------------------------------------
  
  // the default row/column barrier
  static const t_long RMI_ROWB_DEF = 0LL;
  static const t_long RMI_COLB_DEF = 0LL;

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------


  // create a square matrix by size

  Rmi::Rmi (const t_long size) : Ami (size) {
  }

  // create a matrix by size

  Rmi::Rmi (const t_long rsiz, const t_long csiz) : Ami (rsiz, csiz) {
  }

  // copy construct this matrix

  Rmi::Rmi (const Rmi& that) {
    that.rdlock ();
    try {
      Ami::operator = (that);
      that.unlock ();
    } catch (...) {
      that.unlock ();
      throw;
    }
  }
  
  // copy move this matrix

  Rmi::Rmi (Rmi&& that) noexcept {
    that.wrlock ();
    try {
      Ami::operator = (static_cast<Ami&&>(that));
      that.unlock ();
    } catch (...) {
      d_rsiz = 0LL;
      d_csiz = 0LL;
      that.unlock ();
    } 
  }

  // assign a matrix to this one

  Rmi& Rmi::operator = (const Rmi& that) {
    // check for self-assignation
    if (this == &that) return *this;
    // lock and assign
    wrlock ();
    that.rdlock ();
    try {
      Ami::operator = (that);
      that.unlock ();
      unlock ();
      return *this;
    } catch (...) {
      that.unlock ();
      unlock ();
      throw;
    }
  }
  
  // move a matrix into this one

  Rmi& Rmi::operator = (Rmi&& that) noexcept {
    // check for self-move
    if (this == &that) return *this;
    // lock and move
    wrlock ();
    that.wrlock ();
    try {
      // assign base serial
      Ami::operator = (static_cast<Ami&&>(that));
      unlock ();
      that.unlock ();
    } catch (...) {
      d_rsiz = 0LL;
      d_csiz = 0LL;
      unlock ();
      that.unlock ();
    }
    return *this;
  }
  
  // serialize this object
  
  void Rmi::wrstream (OutputStream& os) const {
    rdlock ();
    try {
      // write the matric row/col size
      Serial::wrlong (d_rsiz, os);
      Serial::wrlong (d_csiz, os);
      // write the matrix data
      for (t_long row = 0; row < d_rsiz; row++) {
	for (t_long col = 0; col < d_csiz; col++) {
	  // get the matrix value
	  t_real val = nlget (row, col);
	  if (val == 0.0) continue;
	  // write by position
	  Serial::wrlong (row, os);
	  Serial::wrlong (col, os);
	  Serial::wrreal (val, os);
	}
      }
      // write marker for end
      Serial::wrlong (-1, os);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // deserialize this object

  void Rmi::rdstream (InputStream& is) {
    wrlock ();
    try {
      // get the matrix row/col size
      t_long rsiz = Serial::rdlong (is);
      t_long csiz = Serial::rdlong (is);
      // resize the matrix
      resize (rsiz, csiz);
      // get the matrix data by position
      for (t_long i = 0LL; i < rsiz; i++) {
	for (t_long j = 0; j < csiz; j++) {
	  t_long row = Serial::rdlong (is);
	  // check for marker
	  if (row == -1) {
	    unlock ();
	    return;
	  }
	  t_long col = Serial::rdlong (is);
	  t_real val = Serial::rdreal (is);
	  nlset (row, col, val);
	}
      }
      // get the remaining marker
      if (Serial::rdlong (is) != -1) {
	throw Exception ("rmi-error", "inconsistent serialized matrix");
      }
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // reset this matrix

  void Rmi::reset (void) {
    wrlock ();
    try {
      d_rsiz = 0LL;
      d_csiz = 0LL;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // clear this matrix

  void Rmi::clear (void) {
    wrlock ();
    try {
      nlclear (0LL, 0LL, d_rsiz, d_csiz);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // compare two matrices

  bool Rmi::operator == (const Rmi& x) const {
    rdlock ();
    x.rdlock ();
    try {
      bool result = Algebra::eql (*this, x);
      unlock ();
      x.unlock ();
      return result;
    } catch (...) {
      unlock ();
      x.unlock ();
      throw;
    }
  }

  // compare two matrices

  bool Rmi::operator != (const Rmi& x) const {
    return !(*this == x);
  }
  
  // get the matrix row barrier

  t_long Rmi::getrowb (void) const {
    rdlock ();
    try {
      t_long result = RMI_ROWB_DEF;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the matrix column barrier

  t_long Rmi::getcolb (void) const {
    rdlock ();
    try {
      t_long result = RMI_COLB_DEF;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return true if the matrix is null

  bool Rmi::isnil (void) const {
    rdlock ();
    try {
      bool result = false;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // clear a matrix zone

  void Rmi::clear (const t_long row, const t_long col,
		   const t_long rsz, const t_long csz) {
    wrlock ();
    try {
      nlclear (row, col, rsz, csz);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // clear the lower triangular part

  void Rmi::clt (void) {
    wrlock ();
    try {
      Algebra::clt (*this);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }	
  }

  // compress the matrix if possible

  bool Rmi::compress (void) {
    wrlock ();
    try {
      bool result = false;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set an identity matrix

  void Rmi::eye (void) {
    wrlock ();
    try {
      Algebra::eye (*this);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // copy a matrix row into a vector
  
  Rvi* Rmi::cpr (const t_long row) const {
    throw Exception ("rmi-error", "unimplemented matrix row copy");
  }

  // copy a matrix column into a vector
  
  Rvi* Rmi::cpc (const t_long col) const {
    throw Exception ("rmi-error", "unimplemented matrix column copy");
  }

  // compare two matrices by precision

  bool Rmi::cmp (const Rmi& mx) const {
    rdlock ();
    mx.rdlock ();
    try {
      bool result = Algebra::cmp (*this, mx);
      unlock ();
      mx.unlock ();
      return result;
    } catch (...) {
      unlock ();
      mx.unlock ();
      throw;
    }
  }

  // check if a matrix row is null

  bool Rmi::isnrow (const t_long row) const {
    rdlock ();
    try {
      bool result = Algebra::isnrow (*this, row);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // check if a matrix column is null

  bool Rmi::isncol (const t_long col) const {
    rdlock ();
    try {
      bool result = Algebra::isncol (*this, col);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // compute the matrix frobenius norm
  
  t_real Rmi::norm (void) const {
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

  // fill a matrix with another one - this is the opposite of set

  void Rmi::fill (Rmi& m) const {
    rdlock ();
    m.wrlock ();
    try {
      // reset and resize
      m.reset  ();
      m.resize (getrsiz (), getcsiz ());
      // do a direct copy
      Algebra::cpy (m, *this);
      // done
      unlock ();
      m.unlock ();
    } catch (...) {
      unlock ();
      m.unlock ();
      throw;
    }
  }

  // set a matrix with another one

  void Rmi::set (const Rmi& m) {
    wrlock ();
    m.rdlock ();
    try {
      // reset and resize
      reset  ();
      resize (m.getrsiz (), m.getcsiz ());
      // do a direct copy
      Algebra::cpy (*this, m);
      // done
      unlock ();
      m.unlock ();
    } catch (...) {
      unlock ();
      m.unlock ();
      throw;
    }
  }

  // set a matrix by position

  void Rmi::set (const t_long row, const t_long col, const t_real val) {
    wrlock ();
    try {
      // check for valid position
      if ((row < 0) || (row >= d_rsiz) ||
	  (col < 0) || (col >= d_csiz)) {
	throw Exception ("index-error", "invalid matrix position in set");
      }
      // set in unlocked mode
      nlset (row, col, val);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get a matrix value by position

  t_real Rmi::get (const t_long row, const t_long col) const {
    rdlock ();
    try {
      // check for valid position
      if ((row < 0) || (row >= d_rsiz) ||
	  (col < 0) || (col >= d_csiz)) {
	throw Exception ("index-error", "invalid matrix position in get");
      }
      // get in unlocked mode
      t_real result = nlget (row, col);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // multiply a matrix with a vector and a scaling factor

  Rvi& Rmi::mul (Rvi& r, const Rvi& x, const t_real s) const {
    rdlock ();
    try {
      Rvi& result = nlmul (r, x, s);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // multiply a transposed matrix with a vector and a scaling factor

  Rvi& Rmi::tmul (Rvi& r, const Rvi& x, const t_real s) const {
    rdlock ();
    try {
      Rvi& result = nltmul (r, x, s);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // fill this matrix with random values
  
  void Rmi::random (const t_real rmin, const t_real rmax) {
    wrlock ();
    try {
      Algebra::random (*this, rmin, rmax);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // fill this matrix with sparse random values
  
  void Rmi::sparse (const t_real rmin, const t_real rmax, const t_long nzsz) {
    wrlock ();
    try {
      Algebra::sparse (*this, rmin, rmax, nzsz);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // make this matrix diagonal dominant
  
  void Rmi::toddom (const t_real df) {
    wrlock ();
    try {
      Algebra::toddom (*this, df);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // perform an upper triangular multiplication

  Rvi& Rmi::utmul (Rvi& r, const Rvi& x) const {
    rdlock ();
    try {
      Algebra::utmul (r, *this, x);
      unlock ();
      return r;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // perform an upper triangular division

  Rvi& Rmi::utdiv (Rvi& r, const Rvi& x) const {
    rdlock ();
    try {
      Algebra::utdiv (r, *this, x);
      unlock ();
      return r;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // perform a lower triangular multiplication

  Rvi& Rmi::ltmul (Rvi& r, const Rvi& x) const {
    rdlock ();
    try {
      Algebra::ltmul (r, *this, x);
      unlock ();
      return r;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // perform a lower triangular division

  Rvi& Rmi::ltdiv (Rvi& r, const Rvi& x) const {
    rdlock ();
    try {
      Algebra::ltdiv (r, *this, x);
      unlock ();
      return r;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // permutate this matrix
  
  Rmi* Rmi::permutate (const Cpi& p) const {
    throw Exception ("rmi-error", "unimplemented matrix permutate");
  }

  // reverse permutate this matrix
  
  Rmi* Rmi::reverse (const Cpi& p) const {
    throw Exception ("rmi-error", "unimplemented matrix reverse permutate");
  }

  // perform a givens matrix update
  
  void Rmi::givens (const t_long i, const t_long j, const t_real c,
		    const t_real s, const bool pflg) {
    wrlock ();
    try {
      nlgivens (i, j, c, s, pflg);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // -------------------------------------------------------------------------
  // - no lock section                                                       -
  // -------------------------------------------------------------------------
  
  // no lock - clear a matrix zone

  void Rmi::nlclear (const t_long row, const t_long col,
		     const t_long rsz, const t_long csz) {
    // check valid zone
    if ((row < 0LL) || (col < 0LL)) return;
    t_long rsiz = row + rsz; if (rsiz > d_rsiz) rsiz = d_rsiz;
    t_long csiz = col + csz; if (csiz > d_csiz) csiz = d_csiz;
    // clear the zone - might be slow - very slow ...
    for (t_long i = 0LL; i < rsiz; i++) {
      for (t_long j = 0LL; j < csiz; j++) {
	nlset (i, j, 0.0);
      }
    }
  }

  // no lock - multiply a matrix with a vector and a scaling factor

  Rvi& Rmi::nlmul (Rvi& r, const Rvi& x, const t_real s) const {
    Algebra::mul (r, *this, x, s);
    return r;
  }

  // no lock - multiply a transposed matrix with a vector and a scaling factor

  Rvi& Rmi::nltmul (Rvi& r, const Rvi& x, const t_real s) const {
    Algebra::tmul (r, *this, x, s);
    return r;
  }

  // no lock - perform a givens matrix update
  
  void Rmi::nlgivens (const t_long i, const t_long j, const t_real c,
		      const t_real s, const bool pflg) {
    Algebra::givens (*this, i, j, c, s, pflg);
  }

  // -------------------------------------------------------------------------
  // - object section                                                        -
  // -------------------------------------------------------------------------

  // the quark zone
  static const long QUARK_ZONE_LENGTH = 24;
  static QuarkZone  zone (QUARK_ZONE_LENGTH);

  // the object supported quarks
  static const long QUARK_EQL       = zone.intern ("==");
  static const long QUARK_NEQ       = zone.intern ("!=");
  static const long QUARK_QEQ       = zone.intern ("?=");
  static const long QUARK_SET       = zone.intern ("set");
  static const long QUARK_GET       = zone.intern ("get");
  static const long QUARK_CPR       = zone.intern ("copy-row");
  static const long QUARK_CPC       = zone.intern ("copy-column");
  static const long QUARK_FILL      = zone.intern ("fill");
  static const long QUARK_NILP      = zone.intern ("nil-p");
  static const long QUARK_NORM      = zone.intern ("norm");
  static const long QUARK_CLLT      = zone.intern ("clear-lower-triangular");
  static const long QUARK_CLEAR     = zone.intern ("clear");
  static const long QUARK_ZEROS     = zone.intern ("zeros");
  static const long QUARK_UTMUL     = zone.intern ("upper-triangular-mul");
  static const long QUARK_UTDIV     = zone.intern ("upper-triangular-div");
  static const long QUARK_LTMUL     = zone.intern ("lower-triangular-mul");
  static const long QUARK_LTDIV     = zone.intern ("lower-triangular-div");
  static const long QUARK_TODDOM    = zone.intern ("to-diagonal-dominant");
  static const long QUARK_RANDOM    = zone.intern ("fill-random");
  static const long QUARK_SPARSE    = zone.intern ("fill-sparse");
  static const long QUARK_SQUAREP   = zone.intern ("square-p");
  static const long QUARK_COMPRESS  = zone.intern ("compress");
  static const long QUARK_REVERSE   = zone.intern ("reverse");
  static const long QUARK_PERMUTATE = zone.intern ("permutate");

  // return true if the given quark is defined

  bool Rmi::isquark (const long quark, const bool hflg) const {
    rdlock ();
    try {
      if (zone.exists (quark) == true){
	unlock ();
	return true;
      }
      bool result = hflg ? Iterable::isquark (quark, hflg) : false;
      if (result == false) {
	result = hflg ? Ami::isquark (quark, hflg) : false;
      }
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // operate this object with another object

  Object* Rmi::oper (t_oper type, Object* object) {
    Rmi* mobj = dynamic_cast <Rmi*> (object);
    switch (type) {
    case Object::OPER_EQL:
      if (mobj != nullptr) return new Boolean (*this == *mobj);
      break;
    case Object::OPER_NEQ:
      if (mobj != nullptr) return new Boolean (*this != *mobj);
      break;
    default:
      break;
    }
    throw Exception ("type-error", "invalid operand with real matrix",
		     Object::repr (object));
  }
  
  // apply this object with a set of arguments and a quark
  
  Object* Rmi::apply (Evaluable* zobj, Nameset* nset, const long quark,
                      Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();

    // dispatch 0 argument
    if (argc == 0) {
      if (quark == QUARK_NORM)     return new Real    (norm ());
      if (quark == QUARK_NILP)     return new Boolean (isnil ());
      if (quark == QUARK_COMPRESS) return new Boolean (compress ());
      if (quark == QUARK_ZEROS) {
	Rmi* result = zeros ();
	return result;
      }
      if (quark == QUARK_CLLT) {
	clt ();
	return nullptr;
      }
      if (quark == QUARK_TODDOM) {
	toddom (0.0);
	return nullptr;
      }
    }
    // dispatch 1 argument
    if (argc == 1) {
      if (quark == QUARK_EQL) return oper (Object::OPER_EQL, argv->get (0));
      if (quark == QUARK_NEQ) return oper (Object::OPER_NEQ, argv->get (0));
      if (quark == QUARK_CPR) {
	t_long row = argv->getlong (0);
	return cpr (row);
      }
      if (quark == QUARK_CPC) {
	t_long col = argv->getlong (0);
	return cpc (col);
      }
      if (quark == QUARK_TODDOM) {
	t_real df = argv->getreal (0);
	toddom (df);
	return nullptr;
      }
      if (quark == QUARK_RANDOM) {
	t_real rmax = argv->getreal (0);
	random (0.0, rmax);
	return nullptr;
      }
      if (quark == QUARK_QEQ) {
	Object* obj = argv->get (0);
	Rmi*    rmo = dynamic_cast <Rmi*> (obj);
	if (rmo == nullptr) {
	  throw Exception ("type-error", "invalid object for compare",
			   Object::repr (obj));
	}
	return new Boolean (cmp (*rmo));
      }
      if (quark == QUARK_UTMUL) {
	Object* obj = argv->get (0);
	Rvi* x = dynamic_cast <Rvi*> (obj);
	if (x == nullptr) {
	  throw Exception ("type-error",
			   "invalid object for upper-triangular-mul",
			   Object::repr (obj));
	}
	// clone and compute
	Rvi* result = nullptr;
	try {
	  // make a result clone
	  result = dynamic_cast <Rvi*> (x->clone ());
	  result->clear ();
	  utmul (*result, *x);
	  return result;
	} catch (...) {
	  unlock ();
	  throw;
	} 
      }
      if (quark == QUARK_UTDIV) {
	Object* obj = argv->get (0);
	Rvi* x = dynamic_cast <Rvi*> (obj);
	if (x == nullptr) {
	  throw Exception ("type-error",
			   "invalid object for upper-triangular-div",
			   Object::repr (obj));
	}
	// clone and compute
	Rvi* result = nullptr;
	try {
	  // make a result clone
	  result = dynamic_cast <Rvi*> (x->clone ());
	  result->clear ();
	  utdiv (*result, *x);
	  return result;
	} catch (...) {
	  unlock ();
	  throw;
	} 
      }
      if (quark == QUARK_LTMUL) {
	Object* obj = argv->get (0);
	Rvi* x = dynamic_cast <Rvi*> (obj);
	if (x == nullptr) {
	  throw Exception ("type-error",
			   "invalid object for lower-triangular-mul",
			   Object::repr (obj));
	}
	// clone and compute
	Rvi* result = nullptr;
	try {
	  // make a result clone
	  result = dynamic_cast <Rvi*> (x->clone ());
	  result->clear ();
	  ltmul (*result, *x);
	  return result;
	} catch (...) {
	  unlock ();
	  throw;
	} 
      }
      if (quark == QUARK_LTDIV) {
	Object* obj = argv->get (0);
	Rvi* x = dynamic_cast <Rvi*> (obj);
	if (x == nullptr) {
	  throw Exception ("type-error",
			   "invalid object for lower-triangular-div",
			   Object::repr (obj));
	}
	// clone and compute
	Rvi* result = nullptr;
	try {
	  // make a result clone
	  result = dynamic_cast <Rvi*> (x->clone ());
	  result->clear ();
	  ltdiv (*result, *x);
	  return result;
	} catch (...) {
	  unlock ();
	  throw;
	} 
      }
      if (quark == QUARK_SET) {
	Object* obj = argv->get (0);
	Rmi* m = dynamic_cast <Rmi*> (obj);
	if (m == nullptr) {
	  throw Exception ("type-error", "invalid object with set",
			   Object::repr (obj));
	}
	set (*m);
	return nullptr;
      }
      if (quark == QUARK_FILL) {
	Object* obj = argv->get (0);
	Rmi* m = dynamic_cast <Rmi*> (obj);
	if (m == nullptr) {
	  throw Exception ("type-error", "invalid object with fill",
			   Object::repr (obj));
	}
	fill (*m);
	return nullptr;
      }
      if (quark == QUARK_PERMUTATE) {
	Object* obj = argv->get (0);
	Cpi* p = dynamic_cast <Cpi*> (obj);
	if (p == nullptr) {
	  throw Exception ("type-error", "invalid object with permutate",
			   Object::repr (obj));
	}
	return permutate (*p);
      }
      if (quark == QUARK_REVERSE) {
	Object* obj = argv->get (0);
	Cpi* p = dynamic_cast <Cpi*> (obj);
	if (p == nullptr) {
	  throw Exception ("type-error", "invalid object with permutate",
			   Object::repr (obj));
	}
	return reverse (*p);
      }
    }
    // dispatch 2 argument
    if (argc == 2) {
      if (quark == QUARK_GET) {
        t_long row = argv->getlong (0);
        t_long col = argv->getlong (1);
        return new Real (get (row, col));
      }
      if (quark == QUARK_RANDOM) {
	t_real rmin = argv->getreal (0);
	t_real rmax = argv->getreal (1);
	random (rmin, rmax);
	return nullptr;
      }
      if (quark == QUARK_SPARSE) {
	t_real rmax = argv->getreal (0);
	t_long nzsz = argv->getlong (1);
	sparse (0.0, rmax, nzsz);
	return nullptr;
      }
    }
    // dispatch 3 arguments
    if (argc == 3) {
      if (quark == QUARK_SET) {
        t_long row = argv->getlong (0);
        t_long col = argv->getlong (1);
        t_real val = argv->getrint (2);
	set (row, col, val);
        return nullptr;
      }
      if (quark == QUARK_SPARSE) {
	t_real rmin = argv->getreal (0);
	t_real rmax = argv->getreal (1);
	t_long nzsz = argv->getlong (2);
	sparse (rmin, rmax, nzsz);
	return nullptr;
      }
      // dispatch 4 arguments
      if (argc == 4) {
	if (quark == QUARK_CLEAR) {
	  t_long row = argv->getlong (0);
	  t_long col = argv->getlong (1);
	  t_long rsz = argv->getlong (2);
	  t_long csz = argv->getlong (3);
	  clear (row, col, rsz, csz);
	  return nullptr;
	}
      }
    }
    // check the iterable method
    if (Iterable::isquark (quark, true) == true) {
      return Iterable::apply (zobj, nset, quark, argv);
    }
    // call the ami object
    return Ami::apply (zobj, nset, quark, argv);
  }

  // -------------------------------------------------------------------------
  // - iterator section                                                      -
  // -------------------------------------------------------------------------

  // move the iterator to the next position

  void Rmit::next (void) {
    wrlock ();
    try {
      nlnext ();
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // move the iterator to the previous position

  void Rmit::prev (void) {
    wrlock ();
    try {
      nlprev ();
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // check if the iterator is at the end

  bool Rmit::isend (void) const {
    rdlock ();
    try {
      bool result = nlend ();
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // move the iterator to a matrix point

  void Rmit::move (const t_long row, const t_long col) {
    wrlock ();
    try {
      nlmove (row, col);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the iterator row coordinate

  t_long Rmit::getrow (void) const {
    rdlock ();
    try {
      t_long result = nlgrow ();
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // get the iterator column coordinate
  
  t_long Rmit::getcol (void) const {
    rdlock ();
    try {
      t_long result = nlgcol ();
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // set the matrix at the iterator position

  void Rmit::setval (const t_real val) {
    wrlock ();
    try {
      nlsval (val);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the iterator matrix value

  t_real Rmit::getval (void) const {
    rdlock ();
    try {
      t_real result = nlgval ();
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set the iterator type

  void Rmit::settype (const t_rmit rmit) {
    wrlock ();
    try {
      // set the iterator type
      d_rmit = rmit;
      // reset to the beginning
      begin ();
      // done
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // -------------------------------------------------------------------------
  // - iterator object section                                               -
  // -------------------------------------------------------------------------

  // the object eval quarks
  static const long QUARK_RMIT_LENGTH = 2;
  static QuarkZone  rmit (QUARK_RMIT_LENGTH);

  static const long QUARK_RMITSEQ = rmit.intern ("SEQUENTIAL");
  static const long QUARK_RMITROW = rmit.intern ("ROW");
  static const long QUARK_RMITCOL = rmit.intern ("COL");
  static const long QUARK_RMIT    = String::intern ("Rmit");

  // map an enumeration item to a transcoding mode
  static inline Rmit::t_rmit item_to_rmit (const Item& item) {
    // check for a rmit item 
    if (item.gettid () != QUARK_RMIT)
      throw Exception ("item-error", "item is not a rmit item");
    // map the item to the enumeration
    long quark = item.getquark ();
    // map the quark as an iterator mode
    if (quark == QUARK_RMITSEQ) return Rmit::RMIT_SEQ;
    if (quark == QUARK_RMITROW) return Rmit::RMIT_ROW;
    if (quark == QUARK_RMITCOL) return Rmit::RMIT_COL;
    throw Exception ("item-error", "cannot map rmit mode");
  }

  // the quark zone
  static const long QUARK_ZNIT_LENGTH = 2;
  static QuarkZone  znit (QUARK_ZNIT_LENGTH);

  // the object supported quarks
  static const long QUARK_MOVE    = znit.intern ("move");
  static const long QUARK_SETRMIT = znit.intern ("set-type");

  // evaluate a quark statically
  
  Object* Rmit::meval (Evaluable* zobj, Nameset* nset, const long quark) {
    if (rmit.exists (quark) == true) {
      return new Item (QUARK_RMIT, quark);
    }
    throw Exception ("eval-error", "cannot evaluate member",
                     String::qmap (quark));
  }
  
  // return true if the given quark is defined
  
  bool Rmit::isquark (const long quark, const bool hflg) const {
    rdlock ();
    if (zone.exists (quark) == true) {
      unlock ();
      return true;
    }
    bool result = hflg ? Iterator::isquark (quark, hflg) : false;
    unlock ();
    return result;
  }

  // apply this object with a set of arguments and a quark
  
  Object* Rmit::apply (Evaluable* zobj, Nameset* nset, const long quark,
		       Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();
    
    // dispatch 1 argument
    if (argc == 1) {
      if (quark == QUARK_SETRMIT) {
	Object* iobj = argv->get (0);
        Item*   item = dynamic_cast <Item*> (iobj);
        if (item == nullptr) {       
          throw Exception ("argument-error", "invalid arguments with set-type");
        }
        Rmit::t_rmit rmit = item_to_rmit (*item);
        settype (rmit);
        return nullptr;
      }
    }
    // dispatch 2 arguments
    if (argc == 2) {
      if (quark == QUARK_MOVE) {
	t_long row = argv->getlong (0);
	t_long col = argv->getlong (1);
	move (row, col);
        return nullptr;
      }
    }
    // apply these arguments with the iterator
    return Iterator::apply (zobj, nset, quark, argv);
  }
}
