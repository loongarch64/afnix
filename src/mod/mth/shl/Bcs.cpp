// ---------------------------------------------------------------------------
// - Bcs.cpp                                                                 -
// - afnix:mth module - bcs iterative solver class implementation            -
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

#include "Bcs.hpp"
#include "Math.hpp"
#include "Vector.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"
 
namespace afnix {

  // -------------------------------------------------------------------------
  // - private section                                                       -
  // -------------------------------------------------------------------------

  // solve a system with bi-conjugate stabilized method
  static bool krylov_bcs (Rvi& x, const Rmi& lhs, const Rvi& rhs, 
			  const t_long mni,const t_real aps,const t_real rps) {
    // compute operating norms
    t_real rnrm = rhs.norm ();
    // check for null solution
    if (rnrm == 0.0) {
      x.clear ();
      return true;
    }
    // operating vector list
    Rvi* ri = nullptr;
    Rvi* rn = nullptr;
    Rvi*  p = nullptr;
    Rvi*  v = nullptr;
    Rvi*  s = nullptr;
    Rvi*  t = nullptr;
    // protected block
    try {
      // clear target and clone
      ri = dynamic_cast <Rvi*> (x.clone ()); ri->clear ();
      rn = dynamic_cast <Rvi*> (x.clone ()); rn->clear ();
      p  = dynamic_cast <Rvi*> (x.clone ()); p->clear  ();
      v  = dynamic_cast <Rvi*> (x.clone ()); v->clear  ();
      s  = dynamic_cast <Rvi*> (x.clone ()); s->clear  ();
      t  = dynamic_cast <Rvi*> (x.clone ()); t->clear  ();
      // compute ri = b - Mx
      lhs.mul (*ri, x, -1.0); (*ri) += rhs;
      // check convergence - the initial x might be the solution
      t_real nr = ri->norm ();
      t_real er = rps * rnrm;
      if ((nr < aps) && (nr < er)) {
	delete ri; delete rn;
	delete  p; delete  v; delete s; delete t;
	return true;
      }
      // copy ri into rn
      rn->cpy (*ri);
      // initialize factors
      t_real rhop = 1.0;
      t_real alfa = 1.0;
      t_real omga = 1.0;
      // initialize status
      bool status = false;
      // main loop
      for (long k = 0LL; k < mni; k++) {
	// compute next rho
	t_real rhon = (*ri) ^ (*rn);
	if (rhon == 0.0) {
	  status = false;
	  break;
	}
	if (k == 0LL) {
	  p->cpy (*rn);
	} else {
	  // compute beta
	  t_real beta = (rhon / rhop) * (alfa / omga);
	  // compute next p = rn + beta (p - omga*v)
	  p->aeq (*v, -omga); p->req (*rn, beta);
	}
	// compute v = M.p
	lhs.mul (*v, *p, 1.0);
	// compute alfa = rhon / <ri, v>
	alfa = rhon / ((*ri) ^ (*v));
	if (Math::isinf (alfa) == true) {
	  status = false;
	  break;
	}
	// compute s = rn - alfa*v
	s->add (*rn, *v, -alfa);
	// compute t = M.s
	lhs.mul (*t, *s, 1.0);
	// compute omga = <t, s> / <t, t>
	omga = ((*t) ^ (*s)) / ((*t) ^ (*t));
	if ((omga == 0.0) || (Math::isinf (omga) == true)) {
	  status = false;
	  break;
	}
	// compute x = x + alfa*p + omga*s
	x.aeq (*p, alfa); x.aeq (*s, omga);
	// compute rn = s - omga*t
	rn->add (*s, *t, -omga);
	// set previous rho
	rhop = rhon;
	// check convergence
	nr = rn->norm ();
	er = rps * rnrm;
	if ((nr < aps) && (nr < er)) {
	  status = true;
	  break;
	}
      }
      // clean and return
      delete ri; delete rn;
      delete  p; delete  v; delete s; delete t;
      return status;
    } catch (...) {
      delete ri; delete rn;
      delete  p; delete  v; delete s; delete t;
      throw;
    }
  }

  // solve a system with a preconditioned bi-conjugate stabilized method
  static bool krylov_bcs (Rvi& x, const Rmi& lhs, const Rvi& ovp, 
			  const Rvi& rhs, const t_long mni, const t_real aps,
			  const t_real rps) {
    // compute operating norms
    t_real rnrm = rhs.norm ();
    // check for null solution
    if (rnrm == 0.0) {
      x.clear ();
      return true;
    }
    // operating vector list
    Rvi* ri = nullptr;
    Rvi* rn = nullptr;
    Rvi*  p = nullptr;
    Rvi*  v = nullptr;
    Rvi*  s = nullptr;
    Rvi*  t = nullptr;
    Rvi* ph = nullptr;
    Rvi* sh = nullptr;
    // protected block
    try {
      // clear target and clone
      ri = dynamic_cast <Rvi*> (x.clone ()); ri->clear ();
      rn = dynamic_cast <Rvi*> (x.clone ()); rn->clear ();
      p  = dynamic_cast <Rvi*> (x.clone ()); p->clear  ();
      v  = dynamic_cast <Rvi*> (x.clone ()); v->clear  ();
      s  = dynamic_cast <Rvi*> (x.clone ()); s->clear  ();
      t  = dynamic_cast <Rvi*> (x.clone ()); t->clear  ();
      ph = dynamic_cast <Rvi*> (x.clone ()); ph->clear ();
      sh = dynamic_cast <Rvi*> (x.clone ()); sh->clear ();
      // compute ri = b - M.x
      lhs.mul (*ri, x, -1.0); (*ri) += rhs;
      // check convergence - the initial x might be the solution
      t_real nr = ri->norm ();
      t_real er = rps * rnrm;
      if ((nr < aps) && (nr < er)) {
	delete ri; delete rn;
	delete  p; delete  v; delete s; delete t;
	delete ph; delete sh;
	return true;
      }
      // copy ri into rn
      rn->cpy (*ri);
      // initialize factors
      t_real rhop = 1.0;
      t_real alfa = 1.0;
      t_real omga = 1.0;
      // initialize status
      bool status = false;
      // main loop
      for (long k = 0LL; k < mni; k++) {
	// compute next rho
	t_real rhon = (*ri) ^ (*rn);
	if (rhon == 0.0) {
	  status = false;
	  break;
	}
	if (k == 0LL) {
	  p->cpy (*rn);
	} else {
	  // compute beta
	  t_real beta = (rhon / rhop) * (alfa / omga);
	  // compute next p = rn + beta (p - omga*v)
	  p->aeq (*v, -omga); p->req (*rn, beta);
	}
	// solve P*ph = p
	ph->mul (ovp, *p);
	// compute v = M.ph
	lhs.mul (*v, *ph, 1.0);
	// compute alfa = rhon / <ri, v>
	alfa = rhon / ((*ri) ^ (*v));
	if (Math::isinf (alfa) == true) {
	  status = false;
	  break;
	}
	// compute s = rn - alfa*v
	s->add (*rn, *v, -alfa);
	// solve P*sh = s
	sh->mul (ovp, *s);
	// compute t = M.sh
	lhs.mul (*t, *sh, 1.0);
	// compute omga = <t, s> / <t, t>
	omga = ((*t) ^ (*s)) / ((*t) ^ (*t));
	// check omga
	if ((omga == 0.0) || (Math::isinf (omga) == true)){
	  status = false;
	  break;
	}
	// compute x = x + alfa*ph + omga*sh
	x.aeq (*ph, alfa); x.aeq (*sh, omga);
	// compute rn = s - omga*t
	rn->add (*s, *t, -omga);
	// set previous rho
	rhop = rhon;
	// check convergence
	nr = rn->norm ();
	er = rps * rnrm;
	if ((nr < aps) && (nr < er)) {
	  status = true;
	  break;
	}
      }
      // clean and return
      delete ri; delete rn;
      delete  p; delete  v; delete s; delete t;
      delete ph; delete sh;
      return status;
    } catch (...) {
      delete ri; delete rn;
      delete  p; delete  v; delete s; delete t;
      delete ph; delete sh;
      throw;
    }
  }

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a default bcs solver

  Bcs::Bcs (void) {
    reset ();
  }

  // create a bcs solver by lhs

  Bcs::Bcs (Rmi* lhs) {
    setlhs (lhs);
  }

  // create a bcs solver by lhs and preconditionner

  Bcs::Bcs (Rmi* lhs, Rvi* ovp) {
    setlhs (lhs);
    setovp (ovp);
  }

  // return the class name
  
  String Bcs::repr (void) const {
    return "Bcs";
  }
  
  // solve a linear system

  Rvi* Bcs::solve (const Rvi& rhs) {
    wrlock ();
    Rvi* x = nullptr;
    try {
      // check for valid lhs
      if (p_lhs == nullptr) {
	throw Exception ("bcs-error", "nil lhs for solve");
      }
      // check consistency
      t_long size = rhs.getsize ();
      if ((p_lhs->getrsiz () != size) || (p_lhs->getcsiz () != size) || 
	  ((p_ovp != nullptr) && (p_ovp->getsize () != size))) {
	throw Exception ("bcs-error", 
			 "incompatible lhs/preconditionner in bcs solver");
      }
      // create a result vector
      x = dynamic_cast <Rvi*> (rhs.clone ());
      x->set (d_aps);
      // solver the system
      bool status = (p_ovp == nullptr) ? 
	krylov_bcs (*x, *p_lhs, rhs, d_mni, d_aps, d_rps) :
	krylov_bcs (*x, *p_lhs, *p_ovp, rhs, d_mni, d_aps, d_rps);
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
  
  Object* Bcs::mknew (Vector* argv) {
    long argc = (argv == nullptr) ? 0 : argv->length ();
    
    // check for 0 argument
    if (argc == 0) return new Bcs;    
    // check for 1 argument
    if (argc == 1) {
      Object* obj = argv->get (0);
      Rmi* lhs = dynamic_cast <Rmi*> (obj);
      if ((lhs == nullptr) && (obj != nullptr)) {
	throw Exception ("type-error", "invalid object for bcs constructor",
			 Object::repr (obj));
      }
      return new Bcs (lhs);
    }
    // check for 2 arguments
    if (argc == 2) {
      Object* obj = argv->get (0);
      // check for lhs
      Rmi* lhs = dynamic_cast <Rmi*> (obj);
      if ((lhs == nullptr) && (obj != nullptr)) {
	throw Exception ("type-error", "invalid object for bcs constructor",
			 Object::repr (obj));
      }
      // check for preconditionner
      obj = argv->get (1);
      Rvi* ovp = dynamic_cast <Rvi*> (obj);
      if ((ovp == nullptr) && (obj != nullptr)) {
	throw Exception ("type-error", "invalid object for bcs constructor",
			 Object::repr (obj));
      }
      return new Bcs (lhs, ovp);
    }
    // invalid arguments
    throw Exception ("argument-error", 
		     "invalid arguments with bcs constructor");
  }
}
