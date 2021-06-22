// ---------------------------------------------------------------------------
// - Qr.cpp                                                                  -
// - afnix:mth module - qr based solver class implementation                 -
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

#include "Qr.hpp"
#include "Real.hpp"
#include "Math.hpp"
#include "Vector.hpp"
#include "Boolean.hpp"
#include "Integer.hpp"
#include "Rmatrix.hpp"
#include "Algebra.hpp"
#include "Evaluable.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - private section                                                       -
  // -------------------------------------------------------------------------

  // the solver log message
  static const String QR_DBUG_FCT = "[afnix:mth:qr] qr solver row factorize ";
  static const String QR_DBUG_UPD = "[afnix:mth:qr] qr solver update ";

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a default qr solver

  Qr::Qr (void) {
    p_q = new Rgivens;
    p_r = nullptr;
  }

  // create a qr solver by lhs

  Qr::Qr (Rmi* lhs) {
    p_q = new Rgivens;
    p_r = nullptr;
    setlhs (lhs);
  }

  // destroy this qr solver

  Qr::~Qr (void) {
    delete p_q;
    Object::dref (p_r);
  }
  
  // return the class name
  
  String Qr::repr (void) const {
    return "Qr";
  }

  // reset this solver

  void Qr::reset (void) {
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

  void Qr::clear (void) {
    wrlock ();
    try {
      delete p_q; p_q = new Rgivens;
      Object::dref (p_r); p_r = nullptr;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set the solver lhs

  void Qr::setlhs (Rmi* lhs) {
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

  Rvi* Qr::solve (const Rvi& rhs) {
    wrlock ();
    Rvi* result = nullptr;
    try {
      // eventually factorize the system
      if ((p_q == nullptr) || (p_r == nullptr)) factorize ();
      // reverse givens update
      result = rgivens (rhs);
      // in place upper triangular division
      Algebra::utdiv (*result, *p_r, *result);
      // unlock and return
      unlock ();
      return result;
    } catch (...) {
      delete result;
      unlock ();
      throw;
    }
  }

  // factorize a lhs

  void Qr::factorize (void) {
    wrlock ();
    try {
      // check for valid lhs
      if (p_lhs == nullptr) {
	throw Exception ("direct-error", "nil lhs for factorize");
      }
      // get the matrix row size
      t_long rsiz = p_lhs->getrsiz ();
      // factorize the full range
      factorize (0LL, rsiz);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // factorize by lhs range
  
  void Qr::factorize (const t_long row, const t_long rng) {
    wrlock ();
    Rmit* mit = nullptr;
    try {
      // check for valid lhs
      if (p_lhs == nullptr) {
	throw Exception ("qr-error", "nil lhs for factorize");
      }
      // clear the solver
      clear ();
      // get the matrix size
      t_long rsiz = p_lhs->getrsiz ();
      t_long csiz = p_lhs->getcsiz ();
      // check for valid range
      t_long rrng = row + rng;
      if ((row < 0LL) || (row >= rsiz) || (rng < 0LL) || (rrng > rsiz)) {
	throw Exception ("qr-error", "invalid row range for factorize");
      }
      // create the r matrix
      Object::iref (p_r = new Rmatrix (rsiz, csiz));
      t_long rowb = p_r->getrowb ();
      // create a column iterator for the lhs
      mit = dynamic_cast <Rmit*> (p_lhs->makeit ());
      mit->settype (Rmit::RMIT_COL);
      // create a column iterator for r
      Rmatrixit rit (dynamic_cast <Rmatrix*> (p_r), Rmit::RMIT_COL);
      for (t_long k = row; k < rrng; k++) {
	// get the permutated row and reset minimum column
        t_long pr = (p_cpi == nullptr) ? k : p_cpi->get (k);
	t_long mc = csiz;
	// start by updating the r matrix
	mit->nlmove (pr, 0);
	if (mit->nlgval () == 0.0) mit->nlnext ();
	while (mit->isend () == false) {
	  // get position and value
	  t_long col = mit->nlgcol ();
	  if (col < mc) mc = col;
	  t_real val = mit->nlgval ();
	  // set the matrix
	  if (val != 0.0) p_r->nlset (k, col, val);
	  // move to next position
	  mit->nlnext ();
	}
	// move to the first non null column
	rit.nlmove (k, mc);
	if (rit.nlgval () == 0.0) rit.nlnext ();
	while (rit.nlend () == false) {
	  // get position
	  t_long col = rit.nlgcol ();
	  // check for lower triangular
	  if (col >= k) break;
	  // nullify/update matrix
	  nlnm (k, col);
	  // move to next position
	  rit.nlnext ();
	}
	// eventually force lower triangular cleanup
	if ((rowb != 0LL) && ((k % rowb) == 0LL)) p_r->clt ();
	// log the factorization
	if (p_slg != nullptr) p_slg->add (Logger::MLVL_DBUG, QR_DBUG_FCT + k);
      }
      delete mit;
      // done
      unlock ();
    } catch (...) {
      delete mit;
      unlock ();
      throw;
    }	
  }

  // update a qr factorization by lhs range

  void Qr::update (const t_long row, const t_long rng) {
    wrlock ();
    Rvi*    u = nullptr;
    Rvi*    w = nullptr;
    Rmit* mit = nullptr;
    try {
      // check for valid lhs
      if (p_lhs == nullptr) {
	throw Exception ("qr-error", "nil lhs for update by range");
      }
      // eventually factorize the system instead of updating
      if ((p_q == nullptr) || (p_r == nullptr)) {
	factorize (row, rng);
	unlock ();
	return;
      }
      // get the matrix size
      t_long rsiz = p_lhs->getrsiz ();
      // check for valid range
      t_long rrng = row + rng;
      if ((row < 0LL) || (row >= rsiz) || (rng < 0LL) || (rrng > rsiz)) {
	throw Exception ("qr-error", "invalid row range for update");
      }
      // create an operating vector
      u = new Rvector (rsiz);
      // create a column iterator for the lhs
      mit = dynamic_cast <Rmit*> (p_lhs->makeit ());
      mit->settype (Rmit::RMIT_COL);
      // loop by row index
      for (t_long k = row; k < rrng; k++) {
	// set the row value in u
	u->set (k, 1.0);
	// compute w = Q'.u - the row is permuted inside
	w = rgivens (*u);
	// nullify the w vector - only w[0] is valid
	nlnv (*w);
	t_real s = w->nlget (0);
	// update the first row of the r matrix
	mit->nlmove (k, 0);
	if (mit->nlgval () == 0.0) mit->nlnext ();
	while (mit->isend () == false) {
	  // get position and value
	  t_long col = mit->nlgcol ();
	  t_real val = mit->nlgval ();
	  // set the matrix
	  if (val != 0.0) p_r->nlset (0, col, p_r->nlget (0, col) + s * val);
	  // move to next position
	  mit->nlnext ();
	}
	// factorize the upper-hessenberg r matrix
	nlnh ();
	// reset the row index in u and clean w
	u->set (k, 0.0);
	delete w;
	// log the factorization
	if (p_slg != nullptr) p_slg->add (Logger::MLVL_DBUG, QR_DBUG_UPD + k);
      }
      // clean and unlock
      delete u;
      delete mit;
      unlock ();
    } catch (...) {
      delete u;
      delete w;
      delete mit;
      unlock ();
      throw;
    }
  }

  // update a qr factorization by vectors

  void Qr::update (const Rvi& u, const Rvi& v) {
    wrlock ();
    Rvi* w = nullptr;
    try {
      // check for an existsing lhs
      if (p_lhs == nullptr) {
	setlhs (new Rmatrix (u,v));
	factorize ();
	unlock ();
	return;
      }
      // get target size
      t_long rsiz = p_lhs->getrsiz ();
      t_long csiz = p_lhs->getcsiz ();
      // check for consistency
      if ((u.getsize () != rsiz) ||(v.getsize () != csiz)) {
	throw Exception ("qr-error", "inconsistent vector size in update");
      }
      // eventually factorize the system
      if ((p_q == nullptr) || (p_r == nullptr)) factorize ();
      // paranoid verification
      if ((p_r == nullptr) || (p_r->getrsiz () != rsiz) || 
	  (p_r->getcsiz () != csiz)) {
	throw Exception ("internal-error", "inconsistent qr state in update");
      }
      // compute w = Q'.u - the permutation is done inside
      w = rgivens (u);
      // nullify the w vector - only w[0] is valid
      nlnv (*w);
      t_real s = w->nlget (0);
      // add equal the r first row with v
      Algebra::rva (*p_r, v, s, 0LL);
      // factorize the upper-hessenberg r matrix
      nlnh ();
      // log the factorization
      if (p_slg != nullptr) p_slg->add (Logger::MLVL_DBUG, QR_DBUG_UPD);
      delete w;
      unlock ();
    } catch (...) {
      delete w;
      unlock ();
      throw;
    }
  }

  // get the r matrix

  Rmi* Qr::getr (void) const {
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

  Rmi* Qr::getqr (void) const {
    rdlock ();
    Rmi* tm = nullptr;
    Rmi* result = nullptr;
    try {
      // check for nil
      if ((p_q == nullptr) || (p_r == nullptr)) {
	unlock ();
	return nullptr;
      }
      // create a temporary matrix from r
      Rmi* tm = new Rmatrix;
      tm->set (*p_r);
      // get the number of givens elements
      long glen = p_q->length ();
      for (long k = glen-1; k >=0; k--) nltg (*tm, k);
      // check for a permutation
      if (p_cpi == nullptr) {
	unlock ();
	return tm;
      }
      // reverse permutate the matrix
      result = new Rmatrix (tm->getrsiz (), tm->getcsiz ());
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

  // perform a reverse givens matrix-vector product

  Rvi* Qr::rgivens (const Rvi& x) const {
    rdlock ();
    Rvi* result = nullptr;
    try {
      // clone the result
      result = dynamic_cast <Rvi*> (x.clone ());
      // eventually permutate the vector
      if (p_cpi != nullptr) Algebra::permutate (*result, x, *p_cpi);
      // perform the reverse givens update
      long glen = p_q->length ();
      for (long k = 0; k < glen; k++) nlrg (*result, k);
      unlock ();
      return result;
    } catch (...) {
      delete result;
      unlock ();
      throw;
    }
  }

  // check if the matrix has null diagonal elements

  bool Qr::nulld (void) const {
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

  // no lock - perform a reverse givens vector product

  void Qr::nlrg (Rvi& x, const long k) const {
    // get the givens rotation
    Rgivens::s_givens gr = p_q->nlget (k);
    // get the givens coordinates
    t_long row = gr.d_row;
    t_long col = gr.d_col;
    // get the givens coefficients
    t_real c = gr.d_c;
    t_real s = gr.d_s;
    // perform a givens vector update
    Algebra::givens (x, row, col, c, s);
  }

  // no lock - perform a reverse givens matrix product

  void Qr::nlrg (Rmi& m, const long k) const {
    // get the givens rotation
    Rgivens::s_givens gr = p_q->nlget (k);
    // get the givens coordinates
    t_long row = gr.d_row;
    t_long col = gr.d_col;
    // get the givens coefficients
    t_real c = gr.d_c;
    t_real s = gr.d_s;
    // perform a givens matrix update
    Algebra::givens (m, row, col, c, s, true);
  }

  // no lock - perform a transposed givens matrix product

  void Qr::nltg (Rmi& m, const long k) const {
    // get the givens rotation
    Rgivens::s_givens gr = p_q->nlget (k);
    // get the givens coordinates
    t_long row = gr.d_row;
    t_long col = gr.d_col;
    // get the givens coefficients
    t_real c = gr.d_c;
    t_real s = gr.d_s;
    // perform a givens matrix update by transposition (-s)
    Algebra::givens (m, row, col, c, -s, false);
  }

  // no lock - nullify/update by vector

  void Qr::nlnv (Rvi& w) {
    // check for valid vector
    t_long size = w.getsize ();
    if (size == 0LL) return;
    // loop in reverse order
    for (t_long k = size - 1LL; k > 0LL; k--) {
      // set row and col
      t_long row = k;
      t_long col = k - 1LL;
      // get vector values and check
      t_real rcv = w.nlget (row);
      if (rcv == 0.0) continue;
      t_real ccv = w.nlget (col);
      // compute a givens rotation
      long gidx = p_q->add (row, col, rcv, ccv);
      // get the givens coefficients
      t_real c = p_q->getc (gidx);
      t_real s = p_q->gets (gidx);
      // perform a givens vector update
      Algebra::givens (w, row, col, c, s);
      // perform a givens matrix update
      p_r->nlgivens (row, col, c, s, true);
    }
  } 

  // no lock - nullify/update by matrix coordinates

  void Qr::nlnm (const t_long i, const t_long j) {
    // get the matrix values
    t_real rcv = p_r->nlget (i,j);
    t_real ccv = p_r->nlget (j,j);
    // compute a givens rotation
    long gidx = p_q->add (i, j, rcv, ccv);
    // get the givens coefficients
    t_real c = p_q->getc (gidx);
    t_real s = p_q->gets (gidx);
    // perform a givens matrix update
    p_r->nlgivens (i, j, c, s, true);
  }

  // no lock - nullify/update hessenberg r matrix
  
  void Qr::nlnh (void) {
    // get the r matrix size
    t_long rsiz = (p_r == nullptr) ? 0LL : p_r->getrsiz ();
    if (rsiz == 0LL) return;
    // factorize the upper-hessenberg r matrix
    Rmatrixit rit (dynamic_cast <Rmatrix*> (p_r), Rmit::RMIT_COL);
    for (t_long k = 1LL; k < rsiz; k++) {
      // move to the lower diagonal
      rit.nlmove (k, k-1LL);
      if (rit.nlgval () == 0.0) rit.nlnext ();
      while (rit.nlend () == false) {
	// get position
	t_long row = rit.nlgrow ();
	t_long col = rit.nlgcol ();
	// check for lower triangular
	if (col >= row) break;
	// nullify/update matrix
	nlnm (row, col);
	// move to next position
	rit.nlnext ();
      }
    }
  }

  // -------------------------------------------------------------------------
  // - object section                                                        -
  // -------------------------------------------------------------------------

  // the quark zone
  static const long QUARK_ZONE_LENGTH = 5;
  static QuarkZone  zone (QUARK_ZONE_LENGTH);

  // the object supported quarks
  static const long QUARK_GETR      = zone.intern ("get-r");
  static const long QUARK_GETQR     = zone.intern ("get-qr");
  static const long QUARK_NULLD     = zone.intern ("null-r-diagonal-p");
  static const long QUARK_UPDATE    = zone.intern ("update");
  static const long QUARK_FACTORIZE = zone.intern ("factorize");

  // create a new object in a generic way
  
  Object* Qr::mknew (Vector* argv) {
    long argc = (argv == nullptr) ? 0 : argv->length ();
    
    // check for 0 argument
    if (argc == 0) return new Qr;    
    // check for 1 argument
    if (argc == 1) {
      Object* obj = argv->get (0);
      Rmi* lhs = dynamic_cast <Rmi*> (obj);
      if ((lhs == nullptr) && (obj != nullptr)) {
	throw Exception ("type-error", "invalid object for qr constructor",
			 Object::repr (obj));
      }
      return new Qr (lhs);
    }
    // invalid arguments
    throw Exception ("argument-error", "invalid arguments with qr constructor");
  }

  // return true if the given quark is defined

  bool Qr::isquark (const long quark, const bool hflg) const {
    rdlock ();
    if (zone.exists (quark) == true){
      unlock ();
      return true;
    }
    bool result = hflg ? Direct::isquark (quark, hflg) : false;
    unlock ();
    return result;
  }

  // apply this object with a set of arguments and a quark
  
  Object* Qr::apply (Evaluable* zobj, Nameset* nset, const long quark,
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
    // dispatch 2 arguments
    if (argc == 2) {
      if (quark == QUARK_FACTORIZE) {
	t_long row = argv->getlong (0);
	t_long rng = argv->getlong (1);
	factorize (row, rng);
	return nullptr;
      }
      if (quark == QUARK_UPDATE) {
	Object* uobj = argv->get (0);
	Object* vobj = argv->get (1);
	// update by range
	Integer* ri = dynamic_cast <Integer*> (uobj);
	Integer* ni = dynamic_cast <Integer*> (vobj);
	if ((ri != nullptr) && (ni != nullptr)) {
	  t_long row = ri->tolong ();
	  t_long rng = ni->tolong ();
	  update (row, rng);
	  return nullptr;
	}
	// update by vectors
	Rvi* u = dynamic_cast <Rvi*> (uobj);
	Rvi* v = dynamic_cast <Rvi*> (vobj);
	if ((u != nullptr) && (v != nullptr)) {
	  update (*u, *v);
	  return nullptr;
	}
	// invalid objects
	throw Exception ("type-error", "invalid object for direct update");
      }
    }
    // call the direct methods
    return Direct::apply (zobj, nset, quark, argv);
  }
}
