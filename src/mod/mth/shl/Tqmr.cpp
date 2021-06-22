// ---------------------------------------------------------------------------
// - Tqmr.cpp                                                                -
// - afnix:mth module - tqmr iterative solver class implementation           -
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

#include "Tqmr.hpp"
#include "Math.hpp"
#include "Vector.hpp"
#include "Algebra.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"
 
namespace afnix {

  // -------------------------------------------------------------------------
  // - private section                                                       -
  // -------------------------------------------------------------------------

  // solve a system with the transpose-free qmr method
  static bool krylov_tqmr (Rvi& x, const Rmi& lhs, const Rvi& rhs, 
			   const t_long mni) {
    // compute operating norms
    t_real rn = rhs.norm ();
    // check for null solution
    if (rn == 0.0) {
      x.clear ();
      return true;
    }
    // operating vector list
    Rvi* d = nullptr;
    Rvi* r = nullptr;
    Rvi* v = nullptr;
    Rvi* w = nullptr;
    Rvi* u[2] = {nullptr, nullptr};
    Rvi* y[2] = {nullptr, nullptr};
    // protected block
    try {
      // clear target and clone
      d = dynamic_cast <Rvi*> (x.clone ());       d->clear ();
      r = dynamic_cast <Rvi*> (x.clone ());       r->clear ();
      v = dynamic_cast <Rvi*> (x.clone ());       v->clear ();
      w = dynamic_cast <Rvi*> (x.clone ());       w->clear ();
      u[0] = dynamic_cast <Rvi*> (x.clone ()); u[0]->clear ();
      u[1] = dynamic_cast <Rvi*> (x.clone ()); u[1]->clear ();
      y[0] = dynamic_cast <Rvi*> (x.clone ()); y[0]->clear ();
      y[1] = dynamic_cast <Rvi*> (x.clone ()); y[1]->clear ();
      // compute ri = b - M.x
      lhs.mul (*r, x, -1.0); (*r) += rhs;
      // check convergence - the initial x might be the solution
      t_real nr = r->norm ();
      t_real er = Math::d_reps * rn;
      if ((nr < Math::d_aeps) && (nr < er)) {
	delete d;    delete r;    delete v;    delete w; 
	delete u[0]; delete u[1]; delete y[0]; delete y[1]; 
	return true;
      }
      // copy r into w/y
      w->cpy (*r); y[0]->cpy (*r);
      // initalize v = M.y[0]
      lhs.mul (*v, *y[0], 1.0); u[0]->cpy (*v);
      // initialize factors
      t_real   eta = 0.0;
      t_real theta = 0.0;
      t_real   tau = r->norm ();
      t_real   rho = tau * tau;
      // initialize status
      bool status = false;
      // main loop
      for (long k = 0LL; k < mni; k++) {
	// sigma = (ri, v)
	t_real sigma = Algebra::kdot (*r, *v);
	// alpha = rho / sigma
	t_real alpha = rho / sigma;
	if (Math::isinf (alpha) == true) {
	  status = false;
	  break;
	}
	// y[1] = y[0] - alpha*v
	y[1]->add (*y[0], *v, -alpha);
	// u[1] = M.y[1]
	lhs.mul (*u[1], *y[1], 1.0);
	for (long j = 0; j < 2; j++) {
	  // w  = w - alpha*u[j]
	  w->aeq (*u[j], -alpha);
	  // d = y[j] + (theta^2.eta/alpha)*d
	  d->req (*y[j], theta*theta*eta/alpha);
	  // theta = ||w|| / tau
	  theta = w->norm () / tau;
	  // c = 1 / (1 + theta^2)^0.5
	  t_real c = 1.0 / Math::sqrt (1.0 + theta*theta);
	  // tau = tau * theta * c
	  tau *= theta * c;
	  // eta = c^2 * alpha
	  eta = c * c * alpha;
	  // x = x + eta * d
	  x.aeq (*d, eta);
	  // exit condition
	  long n = 2 * k + j;
	  if (tau * Math::sqrt ((t_real) (n + 1)) < Math::d_reps * rn) {
	    status = true;
	    break;
	  }
	}
	// rhon = (r,w)
	t_real rhon = Algebra::kdot (*r, *w);
	t_real beta = rhon / rho; rho = rhon;
	if (rho == 0.0) {
	  status = false;
	  break;
	}
	// y[0] = w + beta * y[1]
	y[0]->add (*w, *y[1], beta);
	// u[0] = M.y[0]
	lhs.mul (*u[0], *y[0], 1.0);
	// v = u[0] + beta (u[1] + beta.v)
	v->req (*u[1], beta); v->req (*u[0], beta);
      }
      // clean and return
      delete d;    delete r;    delete v;    delete w; 
      delete u[0]; delete u[1]; delete y[0]; delete y[1]; 
      return status;
    } catch (...) {
      delete d;    delete r;    delete v;    delete w; 
      delete u[0]; delete u[1]; delete y[0]; delete y[1]; 
      throw;
    }
  }

  // solve a system with the preconditioned transpose-free qmr method
  static bool krylov_tqmr (Rvi& x, const Rmi& lhs, const Rvi& ovp, 
			   const Rvi& rhs, const t_long mni) {
    // compute operating norms
    t_real rn = rhs.norm ();
    // check for null solution
    if (rn == 0.0) {
      x.clear ();
      return true;
    }
    // operating vector list
    Rvi* d = nullptr;
    Rvi* v = nullptr;
    Rvi* r = nullptr;
    Rvi* t = nullptr;
    Rvi* w = nullptr;
    Rvi* u[2] = {nullptr, nullptr};
    Rvi* y[2] = {nullptr, nullptr};
    // protected block
    try {
      // clear target and clone
      d = dynamic_cast <Rvi*> (x.clone ());       d->clear ();
      r = dynamic_cast <Rvi*> (x.clone ());       r->clear ();
      t = dynamic_cast <Rvi*> (x.clone ());       t->clear ();
      v = dynamic_cast <Rvi*> (x.clone ());       v->clear ();
      w = dynamic_cast <Rvi*> (x.clone ());       w->clear ();
      u[0] = dynamic_cast <Rvi*> (x.clone ()); u[0]->clear ();
      u[1] = dynamic_cast <Rvi*> (x.clone ()); u[1]->clear ();
      y[0] = dynamic_cast <Rvi*> (x.clone ()); y[0]->clear ();
      y[1] = dynamic_cast <Rvi*> (x.clone ()); y[1]->clear ();
      // compute r = b - M.x
      lhs.mul (*r, x, -1.0); (*r) += rhs;
      // check convergence - the initial x might be the solution
      t_real nr = r->norm ();
      t_real er = Math::d_reps * rn;
      if ((nr < Math::d_aeps) && (nr < er)) {
	delete d;    delete v;    delete r;    delete t;  delete w;
	delete u[0]; delete u[1]; delete y[0]; delete y[1]; 
	return true;
      }
      // copy r into w/y
      w->cpy (*r); y[0]->cpy (*r);
      // initalize v = M.y[0]
      t->mul (ovp, *y[0]); lhs.mul (*v, *t, 1.0); u[0]->cpy (*v);
      // initialize factors
      t_real   eta = 0.0;
      t_real theta = 0.0;
      t_real   tau = r->norm ();
      t_real   rho = tau * tau;
      // initialize status
      bool status = false;
      // main loop
      for (long k = 0LL; k < mni; k++) {
	// sigma = (ri, v)
	t_real sigma = Algebra::kdot (*r, *v);
	// alpha = rho / sigma
	t_real alpha = rho / sigma;
	if (Math::isinf (alpha) == true) {
	  status = false;
	  break;
	}
	// y[1] = y[0] - alpha*v
	y[1]->add (*y[0], *v, -alpha);
	// u[1] = M.y[1]
	t->mul (ovp, *y[1]); lhs.mul (*u[1], *t, 1.0);
	for (long j = 0; j < 2; j++) {
	  // w  = w - alpha*u[j]
	  w->aeq (*u[j], -alpha);
	  // d = y[j] + (theta^2.eta/alpha)*d
	  d->req (*y[j], theta*theta*eta/alpha);
	  // theta = ||w|| / tau
	  theta = w->norm () / tau;
	  // c = 1 / (1 + theta^2)^0.5
	  t_real c = 1.0 / Math::sqrt (1.0 + theta*theta);
	  // tau = tau * theta * c
	  tau *= theta * c;
	  // eta = c^2 * alpha
	  eta = c * c * alpha;
	  // x = x + eta * d
	  t->mul (ovp, *d); x.aeq (*t, eta);
	  // exit condition
	  long n = 2 * k + j;
	  if (tau * Math::sqrt ((t_real) (n + 1)) < Math::d_reps * rn) {
	    status = true;
	    break;
	  }
	}
	// rhon = (r,w)
	t_real rhon = Algebra::kdot (*r, *w);
	t_real beta = rhon / rho; rho = rhon;
	if (rho == 0.0) {
	  status = false;
	  break;
	}
	// y[0] = w + beta * y[1]
	y[0]->add (*w, *y[1], beta);
	// u[0] = M.y[0]
	t->mul (ovp, *y[0]); lhs.mul (*u[0], *t, 1.0);
	// v = u[0] + beta (u[1] + beta.v)
	v->req (*u[1], beta); v->req (*u[0], beta);
      }
      // clean and return
      delete d;    delete v;    delete r;    delete t;  delete w;
      delete u[0]; delete u[1]; delete y[0]; delete y[1]; 
      return status;
    } catch (...) {
      delete d;    delete v;    delete r;    delete t;  delete w;
      delete u[0]; delete u[1]; delete y[0]; delete y[1]; 
      throw;
    }
  }

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a default tqmr solver

  Tqmr::Tqmr (void) {
    reset ();
  }

  // create a tqmr solver by lhs

  Tqmr::Tqmr (Rmi* lhs) {
    setlhs (lhs);
  }

  // create a tqmr solver by lhs and preconditionner

  Tqmr::Tqmr (Rmi* lhs, Rvi* ovp) {
    setlhs (lhs);
    setovp (ovp);
  }

  // return the class name
  
  String Tqmr::repr (void) const {
    return "Tqmr";
  }
  
  // solve a linear system

  Rvi* Tqmr::solve (const Rvi& b) {
    wrlock ();
    Rvi* x = nullptr;
    try {
      // check for valid lhs
      if (p_lhs == nullptr) {
	throw Exception ("tqmr-error", "nil lhs for solve");
      }
      // check consistency
      t_long size = b.getsize ();
      if ((p_lhs->getrsiz () != size) || (p_lhs->getcsiz () != size) || 
	  ((p_ovp != nullptr) && (p_ovp->getsize () != size))) {
	throw Exception ("tqmr-error", 
			 "incompatible lhs/preconditionner in tqmr solver");
      }
      // create a result vector
      x = dynamic_cast <Rvi*> (b.clone ());
      x->set (Math::d_aeps);
      // solver the system
      bool status = (p_ovp == nullptr) ? krylov_tqmr (*x, *p_lhs, b, d_mni) :
	krylov_tqmr (*x, *p_lhs, *p_ovp, b, d_mni);
      if (status == false) {
	delete x; x = nullptr;
      }
      unlock ();
      return x;
    } catch (...) {
      delete x;
      unlock ();
      throw;
    }
  }

  // -------------------------------------------------------------------------
  // - object section                                                        -
  // -------------------------------------------------------------------------

  // create a new object in a generic way
  
  Object* Tqmr::mknew (Vector* argv) {
    long argc = (argv == nullptr) ? 0 : argv->length ();
    
    // check for 0 argument
    if (argc == 0) return new Tqmr;    
    // check for 1 argument
    if (argc == 1) {
      Object* obj = argv->get (0);
      Rmi* lhs = dynamic_cast <Rmi*> (obj);
      if ((lhs == nullptr) && (obj != nullptr)) {
	throw Exception ("type-error", "invalid object for tqmr constructor",
			 Object::repr (obj));
      }
      return new Tqmr (lhs);
    }
    // check for 2 arguments
    if (argc == 2) {
      Object* obj = argv->get (0);
      // check for lhs
      Rmi* lhs = dynamic_cast <Rmi*> (obj);
      if ((lhs == nullptr) && (obj != nullptr)) {
	throw Exception ("type-error", "invalid object for tqmr constructor",
			 Object::repr (obj));
      }
      // check for preconditionner
      obj = argv->get (1);
      Rvi* ovp = dynamic_cast <Rvi*> (obj);
      if ((ovp == nullptr) && (obj != nullptr)) {
	throw Exception ("type-error", "invalid object for tqmr constructor",
			 Object::repr (obj));
      }
      return new Tqmr (lhs, ovp);
    }
    // invalid arguments
    throw Exception ("argument-error", 
		     "invalid arguments with tqmr constructor");
  }
}
