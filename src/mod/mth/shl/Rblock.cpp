// ---------------------------------------------------------------------------
// - Rblock.cpp                                                              -
// - afnix:mth module - real block matrix implementation                     -
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

#include "Rmd.hpp"
#include "Real.hpp"
#include "Math.hpp"
#include "Mthsid.hxx"
#include "Vector.hpp"
#include "Rblock.hpp"
#include "Algebra.hpp"
#include "Exception.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - private section                                                       -
  // -------------------------------------------------------------------------

  // this procedure computes the minimum of two integers
  static inline t_long min (const t_long x, const t_long y) {
    return (x < y) ? x : y;
  }

  // -------------------------------------------------------------------------
  // - public section                                                        -
  // -------------------------------------------------------------------------

  // generate a square random matrix by size

  Rblock* Rblock::random (const t_long size, const t_real rmin,
			  const t_real rmax) {
    // create a matrix by size
    Rblock* result = new Rblock (size);
    // fill the matrix
    Algebra::random (*result, rmin, rmax);
    // here it is
    return result;
  }

  // generate a square random matrix by size
  
  Rblock* Rblock::random (const t_long size, const t_real rmin,
			  const t_real rmax, const bool   ddom) {
    // create a matrix by size
    Rblock* result = new Rblock (size);
    // fill the matrix
    Algebra::random (*result, rmin, rmax);
    // force it dominant if requested
    if (ddom == true) Algebra::toddom (*result, (rmax + rmin) / 2.0);
    // here it is
    return result;
  }

  // generate a random matrix by size

  Rblock* Rblock::random (const t_long rsiz, const t_long csiz,
			  const t_real rmin, const t_real rmax,
			  const bool ddom) {
    // create a matrix by size
    Rblock* result = new Rblock (rsiz, csiz);
    // fill the matrix
    Algebra::random (*result, rmin, rmax);
    // force it dominant if requested
    if (ddom == true) Algebra::toddom (*result, (rmax + rmin) / 2.0);
    // here it is
    return result;
  }
  
  // generate a square sparse matrix by size

  Rblock* Rblock::sparse (const t_long size, const t_real rmin,
			  const t_real rmax, const t_long nzsz) {
    // create a matrix by size
    Rblock* result = new Rblock (size);
    // fill the matrix
    Algebra::sparse (*result, rmin, rmax, nzsz);
    // here it is
    return result;
  }

  // add a matrix with another one
  
  Rblock operator + (const Rblock& mx, const Rblock& my) {
    mx.rdlock ();
    my.rdlock ();
    try {
      // create a result matrix
      Rblock mr (mx.getrsiz (), my.getcsiz ());
      // add the matrices
      Algebra::add (mr, mx, my);
      // unlock and return
      mx.unlock ();
      my.unlock ();
      return mr;
    } catch (...) {
      mx.unlock ();
      my.unlock ();
      throw;
    }
  }

  // substract a matrix with another one

  Rblock operator - (const Rblock& mx, const Rblock& my) {
    mx.rdlock ();
    my.rdlock ();
    try {
      // create a result matrix
      Rblock mr (mx.getrsiz (), my.getcsiz ());
      // substract the matrices
      Algebra::sub (mr, mx, my);
      // unlock and return
      mx.unlock ();
      my.unlock ();
      return mr;
    } catch (...) {
      mx.unlock ();
      my.unlock ();
      throw;
    }
  }

  // multiply a matrix with a vector

  Rvector operator * (const Rblock& m, const Rvector& x) {
    m.rdlock ();
    x.rdlock ();
    try {
      // extract operating size
      t_long rows = m.getrsiz ();
      // create result vector
      Rvector r (rows);
      // perform the operation
      m.mul (r, x, 1.0);
      // unlock and return
      m.unlock ();
      x.unlock ();
      return r;
    } catch (...) {
      m.unlock ();
      x.unlock ();
      throw;
    }
  }

  // multiply two matrices

  Rblock operator * (const Rblock& mx, const Rblock& my) {
    mx.rdlock ();
    my.rdlock ();
    try {
      // create a result matrix
      Rblock mr (mx.getrsiz (), my.getcsiz ());
      // muliply the matrices
      Algebra::mul (mr, mx, my);
      // unlock and return
      mx.unlock ();
      my.unlock ();
      return mr;
    } catch (...) {
      mx.unlock ();
      my.unlock ();
      throw;
    }
  }

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a default matrix

  Rblock::Rblock (void) {
    p_blok = nullptr;
  }

  // create a square matrix by size

  Rblock::Rblock (const t_long size) : Rmi (size) {
    p_blok = nullptr;
    resize (size, size);
  }

  // create a matrix by size

  Rblock::Rblock (const t_long rsiz, const t_long csiz) : Rmi (rsiz, csiz) {
    p_blok = nullptr;
    resize (rsiz, csiz);    
  }

  // generate a matrix by vector multiply

  Rblock::Rblock (const Rvi& u, const Rvi& v) {
    // reset matrix
    p_blok = nullptr;
    // get the target size
    t_long rsiz = u.getsize ();
    t_long csiz = v.getsize ();
    // resize the matrix
    resize (rsiz, csiz);
    // update the matrix
    Algebra::mul (*this, u, v);
  }

  // copy construct this matrix

  Rblock::Rblock (const Rblock& that) {
    that.rdlock ();
    try {
      // allocate size
      p_blok = nullptr;
      resize (that.d_rsiz, that.d_csiz);
      // copy the original block
      t_long size = d_rsiz * d_csiz;
      for (t_long k = 0LL; k < size; k++) p_blok[k] = that.p_blok[k];
      that.unlock ();
    } catch (...) {
      that.unlock ();
      throw;
    }
  }
	
  // destroy this matrix

  Rblock::~Rblock (void) {
    delete [] p_blok;
  }

  // assign a matrix to this one

  Rblock& Rblock::operator = (const Rblock& that) {
    // check for self-assignation
    if (this == &that) return *this;
    // lock and assign
    wrlock ();
    that.rdlock ();
    try {
      // delete the old block
      delete [] p_blok;
      // allocate size
      p_blok = nullptr;
      resize (that.d_rsiz, that.d_csiz);
      // copy the original block
      t_long size = d_rsiz * d_csiz;
      for (t_long k = 0LL; k < size; k++) p_blok[k] = that.p_blok[k];
      // unlock and return
      unlock ();
      that.unlock ();
      return *this;
    } catch (...) {
      that.unlock ();
      throw;
    }
  }
  

  // return the class name

  String Rblock::repr (void) const {
    return "Rblock";
  }

  // return a clone of this object

  Object* Rblock::clone (void) const {
    return new Rblock (*this);
  }

  // return the serial did

  t_word Rblock::getdid (void) const {
    return SRL_DEOD_MTH;
  }

  // return the serial sid

  t_word Rblock::getsid (void) const {
    return SRL_RBLK_SID;
  }

  // return true if the matrix is null

  bool Rblock::isnil (void) const {
    wrlock ();
    try {
      bool result = true;
      t_long size = d_rsiz * d_csiz;
      for (t_long k = 0LL; k < size; k++) {
	if (p_blok[k] != 0.0) {
	  result = false;
	  break;
	}
      }
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // reset this matrix

  void Rblock::reset (void) {
    wrlock ();
    try {
      // reset base
      Rmi::reset ();
      // reset locally
      delete [] p_blok;
      p_blok = nullptr;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // clear this matrix

  void Rblock::clear (void) {
    wrlock ();
    try {
      t_long size = d_rsiz * d_csiz;
      for (t_long k = 0LL; k < size; k++) p_blok[k] = 0.0;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // resize this matrix

  void Rblock::resize (const t_long rsiz, const t_long csiz) {
    wrlock ();
    try {
      // check for valid size
      if ((rsiz < 0) || (csiz < 0)) {
	throw Exception ("rblock-error", "invalid resize parameters");
      }
      //  check for nil first
      if ((p_blok == nullptr) && (rsiz > 0LL) && (csiz > 0LL)) {
	// allocate the block
	t_long size = rsiz * csiz;
	p_blok = new t_real[size];
	// local clear before size update
	for (long k = 0L; k < size; k++) p_blok[k] = 0.0;
      } else {
	if ((d_rsiz != rsiz) || (d_csiz != csiz)) {
	  // allocate a new array
	  t_long  size = rsiz * csiz;
	  t_real* blok = new t_real[size];
	  // compute mininum sizes
	  t_long rmin = min (d_rsiz, rsiz);
	  t_long cmin = min (d_csiz, csiz);
	  // copy the array
	  for (t_long i = 0LL; i < rmin; i++) {
	    for (t_long j = 0LL; j < cmin; j++) {
	      blok[i*csiz+j] = p_blok[i*d_csiz+j];
	    }
	  }
	  // set null values
	  for (t_long i = rmin; i < rsiz; i++) {
	    for (t_long j = cmin; j < csiz; j++) {
	      blok[i*csiz+j] = 0.0;
	    }
	  }
	  delete [] p_blok;
	  p_blok = blok;
	}
      }
      // update size and unlock
      d_rsiz = rsiz;
      d_csiz = csiz;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // create a zero identical matrix

  Rmi* Rblock::zeros (void) const {
    rdlock ();
    try {
      Rmi* result = new Rblock (d_rsiz, d_csiz);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // copy a matrix row into a vector

  Rvi* Rblock::cpr (const t_long row) const {
    rdlock ();
    Rvi* result = nullptr;
    try {
      // prepare result vector
      result = (d_csiz == 0LL) ? nullptr : new Rvector (d_csiz);
      // copy if not nil
      if (result != nullptr) Algebra::cpr (*result, *this, row);
      unlock ();
      return result;
    } catch (...) {
      delete result;
      unlock ();
      throw;
    }
  }

  // copy a matrix column into a vector

  Rvi* Rblock::cpc (const t_long col) const {
    rdlock ();
    Rvi* result = nullptr;
    try {
      // prepare result vector
      result = (d_rsiz == 0LL) ? nullptr : new Rvector (d_rsiz);
      // copy if not nil
      if (result != nullptr) Algebra::cpc (*result, *this, col);
      unlock ();
      return result;
    } catch (...) {
      delete result;
      unlock ();
      throw;
    }
  }

  // return a new matrix iterator

  Iterator* Rblock::makeit (void) {
    rdlock ();
    try {
      Iterator* result = new Rblockit (this);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // permutate this matrix

  Rmi* Rblock::permutate (const Cpi& p) const {
    rdlock ();
    Rmi* result = nullptr;
    try {
      // create a result matrix
      result = new Rblock (d_rsiz, d_csiz);
      // permutate this matrix
      Algebra::permutate (*result, *this, p);
      unlock ();
      return result;
    } catch (...) {
      delete result;
      unlock ();
      throw;
    }
  }

  // reverse permutate this matrix

  Rmi* Rblock::reverse (const Cpi& p) const {
    rdlock ();
    Rmi* result = nullptr;
    try {
      // create a result matrix
      result = new Rblock (d_rsiz, d_csiz);
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

  long Rblock::tosize (void) const {
    rdlock ();
    try {
      long result = d_rsiz * d_csiz * sizeof(t_real);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the viewable data

  t_byte* Rblock::tobyte (void) {
    wrlock ();
    try {
      auto result = reinterpret_cast<t_byte*>(p_blok);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // get the viewable data

  const t_byte* Rblock::tobyte (void) const {
    rdlock ();
    try {
      auto result = reinterpret_cast<const t_byte*>(p_blok);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // -------------------------------------------------------------------------
  // - no lock section                                                       -
  // -------------------------------------------------------------------------

  // no lock - set a matrix by position

  void Rblock::nlset (const t_long row, const t_long col, const t_real val) {
    t_long idx = row * d_csiz + col;
    p_blok[idx] = val;
  }

  // set a matrix by position

  t_real Rblock::nlget (const t_long row, const t_long col) const {
    t_long idx = row * d_csiz + col;
    return p_blok[idx];
  }

  // -------------------------------------------------------------------------
  // - object section                                                        -
  // -------------------------------------------------------------------------

  // create a new object in a generic way

  Object* Rblock::mknew (Vector* argv) {
    long argc = (argv == nullptr) ? 0 : argv->length ();
    
    // check for 0 argument
    if (argc == 0) return new Rblock;
    // check for 1 argument
    if (argc == 1) {
      t_long size = argv->getlong (0);
      return new Rblock (size);
    }
    // check for 2 arguments
    if (argc == 2) {
      Object* zobj = argv->get (0);
      Object* cobj = argv->get (1);
      // check for integers
      Integer* ri = dynamic_cast <Integer*> (zobj);
      Integer* ci = dynamic_cast <Integer*> (cobj);
      if ((ri != nullptr) && (ci != nullptr)) {
	t_long rsiz = ri->tolong ();
	t_long csiz = ci->tolong ();
	return new Rblock (rsiz, csiz);
      }
      // check for vectors
      Rvi* u = dynamic_cast <Rvi*> (zobj);
      Rvi* v = dynamic_cast <Rvi*> (cobj);
      if ((u != nullptr) && (v != nullptr)) return new Rblock (*u, *v);
      // invalid object
      throw Exception ("type-error", "invalid objects as matrix arguments");
    }
    // invalid arguments
    throw Exception ("argument-error", 
		     "invalid arguments with rblock object");
  }

  // return true if the given quark is defined

  bool Rblock::isquark (const long quark, const bool hflg) const {
    rdlock ();
    bool result = hflg ? Iterable::isquark (quark, hflg) : false;
    if (result == false) {
      result = hflg ? Rmi::isquark (quark, hflg) : false;
    }
    unlock ();
    return result;
  }

  // operate this matrix with another object

  Object* Rblock::oper (t_oper type, Object* object) {
    Rvector* vobj = dynamic_cast <Rvector*> (object);
    Rblock* mobj = dynamic_cast <Rblock*> (object);
    switch (type) {
    case Object::OPER_ADD:
      if (mobj != nullptr) return new Rblock (*this + *mobj);
      break;
    case Object::OPER_SUB:
      if (mobj != nullptr) return new Rblock (*this - *mobj);
      break;
    case Object::OPER_MUL:
      if (vobj != nullptr) return new Rvector (*this * *vobj);
      if (mobj != nullptr) return new Rblock (*this * *mobj);
      break;
    default:
      throw Exception ("matrix-error", "invalid operator with rblock",
		       Object::repr (object));
      break;
    }
    throw Exception ("type-error", "invalid operand with rblock",
		     Object::repr (object));
  }

  // apply this object with a set of arguments and a quark
  
  Object* Rblock::apply (Evaluable* zobj, Nameset* nset, const long quark,
			 Vector* argv) {
    // check the iterable method
    if (Iterable::isquark (quark, true) == true) {
      return Iterable::apply (zobj, nset, quark, argv);
    }
    // call the rmi method
    return Rmi::apply (zobj, nset, quark, argv);
  }

  // -------------------------------------------------------------------------
  // - iterator section                                                      -
  // -------------------------------------------------------------------------

  // create a new matrix iterator
  
  Rblockit::Rblockit (Rblock* mobj) {
    d_rmit = RMIT_SEQ;
    Object::iref (p_mobj = mobj);
    begin ();
  }
  
  // create a new matrix iterator by type
  
  Rblockit::Rblockit (Rblock* mobj, const t_rmit rmit) {
    d_rmit = rmit;
    Object::iref (p_mobj = mobj);
    begin ();
  }

  // destroy this matrix iterator
  
  Rblockit::~Rblockit (void) {
    Object::dref (p_mobj);
  }

  // return the class name

  String Rblockit::repr (void) const {
    return "Rblockit";
  }

  // reset the iterator to the begining

  void Rblockit::begin (void) {
    wrlock ();
    try {
      // reset indexes
      nlmove (0LL, 0LL);
      // move to the next position if needed
      if (nlgval () == 0.0) next ();
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }
 
  // reset the iterator to the end

  void Rblockit::end (void) {
    wrlock ();
    try {
      // reset indexes
      if (p_mobj == nullptr) {
	unlock ();
	return;
      }
      nlmove (p_mobj->d_rsiz-1LL, p_mobj->d_csiz-1LL);
      // move the last end if needed
      if (nlgval () == 0.0) prev ();
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the object at the current position

  Object* Rblockit::getobj (void) const {
    rdlock ();
    try {
      if (p_mobj != nullptr) p_mobj->rdlock ();
      Object* result = new Rmd (nlgrow (), nlgcol (), nlgval ());
      if (p_mobj != nullptr) p_mobj->unlock ();
      unlock ();
      return result;
    } catch (...) {
      if (p_mobj != nullptr) p_mobj->unlock ();
      unlock ();
      throw;
    }
  }

  // -------------------------------------------------------------------------
  // - iterator no lock section                                              -
  // -------------------------------------------------------------------------

  // no lock - move the iterator to the next position
  
  void Rblockit::nlnext (void) {
    switch (d_rmit) {
    case RMIT_SEQ:
      mvnext ();
      break;
    case RMIT_ROW:
      mrnext ();
      break;
    case RMIT_COL:
      mcnext ();
      break;
    }
  }

  // no lock - move the iterator to the previous position

  void Rblockit::nlprev (void) {
    // move to the previous position
    switch (d_rmit) {
    case RMIT_SEQ:
      mvprev ();
      break;
    case RMIT_ROW:
      mrprev ();
      break;
    case RMIT_COL:
      mcprev ();
      break;
    }
  }

  // no lock - return true if the iterator is at the end

  bool Rblockit::nlend (void) const {
    // check for nil
    if (p_mobj == nullptr) return true;
    // check for bounds
    return (d_mrow >= p_mobj->d_rsiz) || (d_mcol >= p_mobj->d_csiz);
  }
  
  // no lock - move the iterator to a matrix point

  void Rblockit::nlmove (const t_long row, const t_long col) {
    // check for object valid coordinates
    if ((p_mobj == nullptr) || (row < 0LL) || (col < 0LL)) return;
    // update the coordinates
    d_mrow = (row >= p_mobj->d_rsiz) ? p_mobj->d_rsiz : row;
    d_mcol = (col >= p_mobj->d_csiz) ? p_mobj->d_csiz : col;
  }

  // no lock - move the iterator to the next position
  
  void Rblockit::mvnext (void) {
    // check for nil - double security
    if (p_mobj == nullptr) return;
    // check for upper bound
    if ((d_mrow >= p_mobj->d_rsiz) && (d_mcol >= p_mobj->d_csiz)) return;
    // move to the next column
    do {
      d_mcol++;
      if (d_mcol >= p_mobj->d_csiz) {
	d_mcol = 0LL;
	d_mrow++;
      }
    } while ((nlgval () == 0.0) && (d_mrow < p_mobj->d_rsiz));
  }

  // no lock - move the iterator to the next row position

  void Rblockit::mrnext (void) {
    // check for nil - double security
    if (p_mobj == nullptr) return;
    // check for upper bound
    if ((d_mrow >= p_mobj->d_rsiz) && (d_mcol >= p_mobj->d_csiz)) return;
    // move to the next position
    do {
      d_mrow++;
    } while ((nlgval () == 0.0) && (d_mrow < p_mobj->d_rsiz));
  }

  // no lock - move the iterator to the next column position

  void Rblockit::mcnext (void) {
    // check for nil - double security
    if (p_mobj == nullptr) return;
    // check for upper bound
    if ((d_mrow >= p_mobj->d_rsiz) && (d_mcol >= p_mobj->d_csiz)) return;
    // move to the next position
    do {
      d_mcol++;
    } while ((nlgval () == 0.0) && (d_mcol < p_mobj->d_csiz));
  }

  // no lock - move the iterator to the previous position
  
  void Rblockit::mvprev (void) {
    // check for nil - double security
    if (p_mobj == nullptr) return;
    // check null first
    if ((d_mrow == 0LL) && (d_mcol == 0LL)) return;
    // move to the previous position
    do {
      d_mcol--;
      if (d_mcol < 0LL) {
	d_mcol = p_mobj->d_csiz - 1LL;
	d_mrow--;
      }
    } while ((nlgval () == 0.0) && (d_mrow >= 0));
    if (d_mcol < 0LL) d_mcol = 0LL;
    if (d_mrow < 0LL) d_mrow = 0LL;
  }

  // move the iterator to the previous row position
  
  void Rblockit::mrprev (void) {
    // check for nil - double security
    if (p_mobj == nullptr) return;
    // check null first
    if ((d_mrow == 0LL) && (d_mcol == 0LL)) return;
    // more to the previous position
    do {
      d_mrow--;
    } while ((nlgval () == 0.0) && (d_mrow >= 0));
    if (d_mrow < 0LL) d_mrow = 0LL;
    if (d_mcol < 0LL) d_mcol = 0LL;
  }

  // no lock - move the iterator to the previous column position
  
  void Rblockit::mcprev (void) {
    // check for nil - double security
    if (p_mobj == nullptr) return;
    // check null first
    if ((d_mrow == 0LL) && (d_mcol == 0LL)) return;
    // more to the previous position
    do {
      d_mcol--;
    }  while ((nlgval () == 0.0) && (d_mcol >= 0));
    if (d_mrow < 0LL) d_mrow = 0LL;
    if (d_mcol < 0LL) d_mcol = 0LL;
  }

  // no lock - get the row coordinate at the iterator position (no lock)
  
  t_long Rblockit::nlgrow (void) const {
    // check for nil first
    if (p_mobj == nullptr) return 0LL;
    return d_mrow;
  }

  // no lock - get the column coordinate at the iterator position
  
  t_long Rblockit::nlgcol (void) const {
    // check for nil first
    if (p_mobj == nullptr) return 0LL;
    return d_mcol;
  }

  // no lock - set the value at the current position

  void Rblockit::nlsval (const t_real val)  {
    // check for nil first
    if (p_mobj == nullptr) return;
    // check for bound
    if ((d_mrow >= p_mobj->d_rsiz) || (d_mcol >= p_mobj->d_csiz)) return;
    p_mobj->nlset (d_mrow, d_mcol, val);
  }

  // no lock - get the value at the current position (no lock)

  t_real Rblockit::nlgval (void) const {
    // check for nil first
    if (p_mobj == nullptr) return 0LL;
    // check for bound
    if ((d_mrow >= p_mobj->d_rsiz) || (d_mcol >= p_mobj->d_csiz)) return 0.0;
    return p_mobj->nlget (d_mrow, d_mcol);
  }
}
