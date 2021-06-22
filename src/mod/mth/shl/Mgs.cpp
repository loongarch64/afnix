// ---------------------------------------------------------------------------
// - Mgs.cpp                                                                 -
// - afnix:mth module - modified Gram-Schmidt solver class implementation    -
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

#include "Mgs.hpp"
#include "Real.hpp"
#include "Math.hpp"
#include "Vector.hpp"
#include "Rblock.hpp"
#include "Rmatrix.hpp"
#include "Boolean.hpp"
#include "Algebra.hpp"
#include "Evaluable.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - private section                                                       -
  // -------------------------------------------------------------------------

  // the solver log message
  static const String MGS_DBUG_FCT = "[afnix::mth:mgs] mgs solver factorize ";

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a default mgs solver

  Mgs::Mgs (void) {
    p_q = nullptr;
    p_r = nullptr;
  }

  // create a mgs solver by lhs

  Mgs::Mgs (Rmi* lhs) {
    p_q = nullptr;
    p_r = nullptr;
    setlhs (lhs);
  }

  // destroy this mgs solver

  Mgs::~Mgs (void) {
    Object::dref (p_q);
    Object::dref (p_r);
  }
  
  // return the class name
  
  String Mgs::repr (void) const {
    return "Mgs";
  }

  // reset this solver

  void Mgs::reset (void) {
    wrlock ();
    try {
      // reset the base object
      Solver::reset ();
      // clear locally
      clear ();
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // clear this solver

  void Mgs::clear (void) {
    wrlock ();
    try {
      Object::dref (p_q); p_q = nullptr;
      Object::dref (p_r); p_r = nullptr;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set the solver lhs

  void Mgs::setlhs (Rmi* lhs) {
    wrlock ();
    try {
      // clear locally
      clear ();
      // set the base lhs
      Direct::setlhs (lhs);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }
    
  // solve a system by direct factorization

  Rvi* Mgs::solve (const Rvi& rhs) {
    wrlock ();
    Rvi*     tm = nullptr;
    Rvi* result = nullptr;
    try {
      // eventually factorize the system
      if ((p_q == nullptr) || (p_r == nullptr)) factorize ();
      if ((p_q == nullptr) || (p_r == nullptr)) {
	unlock ();
	return nullptr;
      }      
      // get the rhs size and check
      t_long size = rhs.getsize ();
      if ((p_q->getrsiz () != size) || (p_q->getcsiz () != size) ||
	  (p_r->getrsiz () != size) || (p_r->getcsiz () != size)) {
	throw Exception ("mgs-error", "inconsistent size in solver");
      }
      // allocate result vector
      result = new Rvector (size);
      // compute Q'*rhs
      if (p_cpi != nullptr) {
	tm = new Rvector (size);
	Algebra::permutate (*tm, rhs, *p_cpi);
	Algebra::tmul (*result, *p_q, *tm, 1.0);
	delete tm;
      } else {
	Algebra::tmul (*result, *p_q, rhs, 1.0);
      }
      // in place upper triangular division
      Algebra::utdiv (*result, *p_r, *result);
      // unlock and return
      unlock ();
      return result;
    } catch (...) {
      delete tm;
      delete result;
      unlock ();
      throw;
    }
  }

  // factorize a lhs

  void Mgs::factorize (void) {
    wrlock ();
    try {
      // check for valid lhs
      if (p_lhs == nullptr) {
	throw Exception ("direct-error", "nil lhs for factorize");
      }
      // clear the solver
      clear ();
      // check for a block matrix
      if (dynamic_cast <Rblock*> (p_lhs) != nullptr) {
	nlbm ();
	nlbf ();
      } else {
	// create a sparse matrix
	nlsm ();
	nlsf ();
      }
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the r matrix

  Rmi* Mgs::getr (void) const {
    rdlock ();
    try {
      Rmi* result = p_r;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // rebuild the qr product

  Rmi* Mgs::getqr (void) const {
    rdlock ();
    Rmi* tm     = nullptr;
    Rmi* result = nullptr;
    try {
      // check for nil
      if ((p_q == nullptr) || (p_r == nullptr)) {
	unlock ();
	return nullptr;
      }
      // get the matrix size
      t_long rsiz = p_q->getrsiz ();
      t_long csiz = p_q->getcsiz ();
      // allocate the result matrix
      tm = new Rblock (rsiz, csiz);
      // compute matrix product
      Algebra::mul (*tm, *p_q, *p_r);
      // check for a permutation
      if (p_cpi == nullptr) {
	unlock ();
	return tm;
      }
      // reverse permutate the matrix
      result = new Rblock (tm->getrsiz (), tm->getcsiz ());
      Algebra::reverse (*result, *tm, *p_cpi);
      delete tm;
      unlock ();
      return result;
    } catch (...) {
      delete tm;
      delete result;
      unlock ();
      throw;
    }
  }

  // check if the matrix has null diagonal elements

  bool Mgs::nulld (void) const {
    rdlock ();
    try {
      bool result = (p_r == nullptr) ? true : Algebra::nulld (*p_r);
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

  // create the initial qr block matrix

  void Mgs::nlbm (void) {
    // check for nil
    if (p_lhs == nullptr) return;
    // get the matrix sizes
    t_long csiz = p_lhs->getcsiz ();
    // clone the matrix
    p_q = p_lhs->zeros ();
    p_r = p_lhs->zeros (); p_r->resize (csiz, csiz);
    Object::iref (p_q);
    Object::iref (p_r);
    // copy the original matrix by permutation
    if (p_cpi != nullptr) {
      Algebra::permutate (*p_q, *p_lhs, *p_cpi);
    } else {
      p_q->set (*p_lhs);
    }
  }

  // create the initial qr sparse matrix

  void Mgs::nlsm (void) {
    // check for nil
    if (p_lhs == nullptr) return;
    // get the matrix sizes
    t_long csiz = p_lhs->getcsiz ();
    // clone the matrix
    Object::dref (p_q);
    Object::dref (p_r);
    p_q = p_lhs->zeros ();
    p_r = p_lhs->zeros (); p_r->resize (csiz, csiz);
    Object::iref (p_q);
    Object::iref (p_r);
    // create a matrix iterator
    Rmit* mit = dynamic_cast <Rmit*> (p_lhs->makeit ());
    try {
      if (mit->nlgval () == 0.0) mit->nlnext ();
      while (mit->nlend () == false) {
	// get position and value
	t_long row = mit->nlgrow ();
	t_long col = mit->nlgcol ();
	t_real val = mit->nlgval ();
	// permutate the row if any - need rget with mit
	if (p_cpi != nullptr) row = p_cpi->rget (row);
	// set target value
	p_q->nlset (row, col, val);
	// move to next position
	mit->nlnext ();
      }
      delete mit;
    } catch (...) {
      delete mit;
      throw;
    }
  }

  // no lock - factorize a block matrix

  void Mgs::nlbf (void) {
    // get the matrix sizes
    t_long rsiz = p_lhs->getrsiz ();
    t_long csiz = p_lhs->getcsiz ();
    // loop in the column
    for (t_long i = 0LL; i < csiz; i++) {
      // compute s = sum (Q(:,i) * Q(:,i))
      t_real s = 0.0;
      for (t_long k = 0LL; k < rsiz; k++) {
	t_real qki = p_q->nlget (k,i);
	s += qki * qki;
      }
      // R(i,i) = sqrt (s)
      s = Math::sqrt (s);
      p_r->nlset (i, i, s);
      if (s == 0.0) {
	throw Exception ("mgs-error", "invalid null diagonal factor");
      }
      // Q(:,i) = Q(:,i) / R(i,i)
      for (t_long k = 0LL; k < rsiz; k++) {
	t_real qki = p_q->nlget (k,i) / s;
	p_q->nlset (k, i, qki);
      }
      // update the next columns
      for (t_long j = i+1LL; j < csiz; j++) {
	// R(i,j) = sum (Q(:,i)Q(:,j))
	t_real rij = p_r->nlget (i,j);
	for (t_long k = 0LL; k < rsiz; k++) {
	  rij += p_q->nlget (k,i) * p_q->nlget (k,j);
	}
	p_r->nlset (i, j, rij);
	// Q(:,j) = Q(:,j) - Q(:,i)*R(i,j)
	for (t_long k = 0LL; k < rsiz; k++) {
	  t_real qkj = p_q->nlget (k, j);
	  t_real qki = p_q->nlget (k, i);
	  t_real val = qkj - qki * rij;
	  p_q->nlset (k, j, val);
	}
      }
      // log the factorization
      if (p_slg != nullptr) p_slg->add (Logger::MLVL_DBUG, MGS_DBUG_FCT + i);
    }
  }

  // no lock - factorize a sparse matrix

  void Mgs::nlsf (void) {
    Rmit* rit = nullptr;
    try {
      // get the matrix sizes
      t_long rsiz = p_lhs->getrsiz ();
      t_long csiz = p_lhs->getcsiz ();
      // get the row iterator
      rit = dynamic_cast <Rmit*> (p_q->makeit ());
      rit->settype (Rmit::RMIT_ROW);
      // loop in the column
      for (t_long i = 0LL; i < csiz; i++) {
	// compute s = sum (Q(:,i) * Q(:,i))
	t_real s = 0.0;
	rit->nlmove (0, i);
	if (rit->nlgval () == 0.0) rit->nlnext ();
	while (rit->nlend () == false) {
	  t_real qki = rit->nlgval ();
	  s += qki * qki;
	  rit->nlnext ();
	}
	// R(i,i) = sqrt (s)
	s = Math::sqrt (s);
	p_r->nlset (i, i, s);
	if (s == 0.0) {
	  throw Exception ("mgs-error", "invalid null diagonal factor");
	}
	// Q(:,i) = Q(:,i) / R(i,i)
	rit->nlmove (0, i);
	if (rit->nlgval () == 0.0) rit->nlnext ();
	while (rit->nlend () == false) {
	  t_real qki = rit->nlgval ();
	  rit->nlsval (qki / s);
	  rit->nlnext ();
	}
	// update the next columns
	for (t_long j = i+1LL; j < csiz; j++) {
	  // R(i,j) = sum (Q(:,i)Q(:,j))
	  t_real rij = p_r->nlget (i,j);
	  for (t_long k = 0LL; k < rsiz; k++) {
	    rij += p_q->nlget (k,i) * p_q->nlget (k,j);
	  }
	  p_r->nlset (i, j, rij);
	  // Q(:,j) = Q(:,j) - Q(:,i)*R(i,j)
	  for (t_long k = 0LL; k < rsiz; k++) {
	    t_real qki = p_q->nlget (k, i);
	    t_real qkj = p_q->nlget (k, j);
	    t_real val = qkj - qki * rij;
	    p_q->nlset (k, j, val);
	  }
	}
	// log the factorization
	if (p_slg != nullptr) p_slg->add (Logger::MLVL_DBUG, MGS_DBUG_FCT + i);
      }
      delete rit;
    } catch (...) {
      delete rit;
      throw;
    }
  }

  // -------------------------------------------------------------------------
  // - object section                                                        -
  // -------------------------------------------------------------------------

  // the quark zone
  static const long QUARK_ZONE_LENGTH = 3;
  static QuarkZone  zone (QUARK_ZONE_LENGTH);

  // the object supported quarks
  static const long QUARK_GETR      = zone.intern ("get-r");
  static const long QUARK_GETQR     = zone.intern ("get-qr");
  static const long QUARK_NULLD     = zone.intern ("null-r-diagonal-p");

  // create a new object in a generic way
  
  Object* Mgs::mknew (Vector* argv) {
    long argc = (argv == nullptr) ? 0 : argv->length ();
    
    // check for 0 argument
    if (argc == 0) return new Mgs;    
    // check for 1 argument
    if (argc == 1) {
      Object* obj = argv->get (0);
      Rmi* lhs = dynamic_cast <Rmi*> (obj);
      if ((lhs == nullptr) && (obj != nullptr)) {
	throw Exception ("type-error", "invalid object for mgs constructor",
			 Object::repr (obj));
      }
      return new Mgs (lhs);
    }
    // invalid arguments
    throw Exception ("argument-error", 
		     "invalid arguments with mgs constructor");
  }

  // return true if the given quark is defined

  bool Mgs::isquark (const long quark, const bool hflg) const {
    rdlock ();
    try {
      if (zone.exists (quark) == true){
	unlock ();
	return true;
      }
      bool result = hflg ? Direct::isquark (quark, hflg) : false;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // apply this object with a set of arguments and a quark
  
  Object* Mgs::apply (Evaluable* zobj, Nameset* nset, const long quark,
		     Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();
    
    // dispatch 0 argument
    if (argc == 0) {
      if (quark == QUARK_NULLD) return new Boolean (nulld ());
      if (quark == QUARK_GETQR) {
	Rmi* result = getqr ();
	return result;
      }
      if (quark == QUARK_GETR) {
	rdlock ();
	try {
	  Rmi* result = getr ();
	  zobj->post (result);
	  unlock ();
	  return result;
	} catch (...) {
	  unlock ();
	  throw;
	}
      }
    }
    // call the direct methods
    return Direct::apply (zobj, nset, quark, argv);
  }
}
