// ---------------------------------------------------------------------------
// - Cgs.cpp                                                                 -
// - afnix:mth module - cgs iterative solver class implementation            -
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

#include "Cgs.hpp"
#include "Math.hpp"
#include "Vector.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"
 
namespace afnix {

  // -------------------------------------------------------------------------
  // - private section                                                       -
  // -------------------------------------------------------------------------

  // the solver log message
  static const String CGS_DBUG_SLV = "[afnix:mth:cgs] cgs solver iterate ";

  // solve a system with conjugate gradient squared method
  static bool krylov_cgs (Rvi& x, const Rmi& lhs, const Rvi& rhs, Logger* slg,
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
    Rvi*  q = nullptr;
    Rvi*  p = nullptr;
    Rvi*  u = nullptr;
    Rvi* qh = nullptr;
    Rvi* uh = nullptr;
    Rvi* vh = nullptr;
    // protected block
    try {
      // clear target and clone
      ri = dynamic_cast <Rvi*> (x.clone ()); ri->clear ();
      rn = dynamic_cast <Rvi*> (x.clone ()); rn->clear ();
      q  = dynamic_cast <Rvi*> (x.clone ()); q->clear  ();
      p  = dynamic_cast <Rvi*> (x.clone ()); p->clear  ();
      u  = dynamic_cast <Rvi*> (x.clone ()); u->clear  ();
      qh = dynamic_cast <Rvi*> (x.clone ()); qh->clear ();
      uh = dynamic_cast <Rvi*> (x.clone ()); qh->clear (); 
      vh = dynamic_cast <Rvi*> (x.clone ()); qh->clear ();
      // compute ri = b - Mx
      lhs.mul (*ri, x, -1.0); (*ri) += rhs;
      // check convergence - the initial x might be the solution
      t_real nr = ri->norm ();
      t_real er = rps * rnrm;
      if ((nr < aps) && (nr < er)) {
	delete ri; delete rn;
	delete  q; delete  p; delete  u; 
	delete uh; delete qh; delete vh;
	return true;
      }
      // copy ri into rn
      rn->cpy (*ri);
      // initialize factors
      t_real rhop = 1.0;
      // initialize status
      bool status = false;
      // main loop
      for (t_long k = 0LL; k < mni; k++) {
	// log a message
	if (slg != nullptr) {
	  String mesg = CGS_DBUG_SLV + k;
	  slg->add (Logger::MLVL_DBUG, mesg);
	}
	// compute next rho
	t_real rhon = (*ri) ^ (*rn);
	if (rhon == 0.0) {
	  status = false;
	  break;
	}
	if (k == 0LL) {
	  u->cpy (*rn);
	  p->cpy (*rn);
	} else {
	  // compute beta
	  t_real beta = rhon / rhop;
	  // compute u = rn + beta*q
	  u->add (*rn, *q, beta);
	  // compute p = u + beta (q + beta*p)
	  p->req (*q, beta); p->req (*u, beta);
	}
	// compute vh = M.p
	lhs.mul (*vh, *p, 1.0);
	// compute alfa = rhon / <ri, v>
	t_real alfa = rhon / ((*ri) ^ (*vh));
	if (Math::isinf (alfa) == true) {
	  status = false;
	  break;
	}
	// compute q = u -alfa*vh
	q->add (*u, *vh, -alfa);
	// solve uh = u + q
	uh->add (*u, *q);
	// compute x = x + alfa*uh
	x.aeq (*uh, alfa);
	// compute qh = M.uh
	lhs.mul (*qh, *uh, 1.0);
	// compute rn = rn - alfa*qh
	rn->aeq (*qh, -alfa);
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
      delete  q; delete  p; delete u; delete uh; delete qh; delete vh;
      return status;
    } catch (...) {
      delete ri; delete rn;
      delete  q; delete  p; delete u; delete uh; delete qh; delete vh;
      throw;
    }
  }

  // solve a system with preconditioned conjugate gradient squared method
  static bool krylov_cgs (Rvi& x, const Rmi& lhs, const Rvi& ovp,
			  const Rvi& rhs, Logger* slg, const long mni, 
			  const t_real aps, const t_real rps) {
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
    Rvi*  q = nullptr;
    Rvi*  p = nullptr;
    Rvi*  u = nullptr;
    Rvi* ph = nullptr;
    Rvi* qh = nullptr;
    Rvi* uh = nullptr;
    Rvi* vh = nullptr;
    // protected block
    try {
      // clear target and clone
      ri = dynamic_cast <Rvi*> (x.clone ()); ri->clear ();
      rn = dynamic_cast <Rvi*> (x.clone ()); rn->clear ();
      q  = dynamic_cast <Rvi*> (x.clone ()); q->clear  ();
      p  = dynamic_cast <Rvi*> (x.clone ()); p->clear  ();
      u  = dynamic_cast <Rvi*> (x.clone ()); u->clear  ();
      ph = dynamic_cast <Rvi*> (x.clone ()); ph->clear ();
      qh = dynamic_cast <Rvi*> (x.clone ()); qh->clear ();
      uh = dynamic_cast <Rvi*> (x.clone ()); uh->clear ();
      vh = dynamic_cast <Rvi*> (x.clone ()); vh->clear ();
      // compute ri = b - Mx
      lhs.mul (*ri, x, -1.0); (*ri) += rhs;
      // check convergence - the initial x might be the solution
      t_real nr = ri->norm ();
      t_real er = rps * rnrm;
      if ((nr < aps) && (nr < er)) {
	delete ri; delete rn;
	delete  q; delete  p; delete u; 
	delete ph; delete qh; delete uh; delete vh;
	return true;
      }
      // copy ri into rn
      rn->cpy (*ri);
      // initialize factors
      t_real rhop = 1.0;
      // initialize status
      bool status = false;
      // main loop
      for (long k = 0LL; k < mni; k++) {
	// log a message
	if (slg != nullptr) {
	  String mesg = CGS_DBUG_SLV + k;
	  slg->add (Logger::MLVL_DBUG, mesg);
	}
	// compute next rho
	t_real rhon = (*ri) ^ (*rn);
	if (rhon == 0.0) {
	  status = false;
	  break;
	}
	if (k == 0LL) {
	  u->cpy (*rn);
	  p->cpy (*rn);
	} else {
	  // compute beta
	  t_real beta = rhon / rhop;
	  // compute u = rn + beta*q
	  u->add (*rn, *q, beta);
	  // compute p = u + beta (q + beta*p)
	  p->req (*q, beta); p->req (*u, beta);
	}
	// solve P*ph = p
	ph->mul (ovp, *p);
	// compute vh = M.ph
	lhs.mul (*vh, *ph, 1.0);
	// compute alfa = rhon / <ri, v>
	t_real alfa = rhon / ((*ri) ^ (*vh));
	if (Math::isinf (alfa) == true) {
	  status = false;
	  break;
	}
	// compute q = u -alfa*vh
	q->add (*u, *vh, -alfa);
	// solve P*uh = u + q
	uh->add (*u, *q);
	uh->mul (ovp, *uh);
	// compute x = x + alfa*uh
	x.aeq (*uh, alfa);
	// compute qh = M.uh
	lhs.mul (*qh, *uh, 1.0);
	// compute rn = rn - alfa*qh
	rn->aeq (*qh, -alfa);
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
      delete  q; delete  p; delete u; 
      delete ph; delete uh; delete qh; delete vh;
      return status;
    } catch (...) {
      delete ri; delete rn;
      delete  q; delete  p; delete u; 
      delete ph; delete uh; delete qh; delete vh;
      throw;
    }
  }

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a default cgs solver

  Cgs::Cgs (void) {
    reset ();
  }

  // create a cgs solver by lhs

  Cgs::Cgs (Rmi* lhs) {
    setlhs (lhs);
  }

  // create a cgs solver by lhs and preconditionner

  Cgs::Cgs (Rmi* lhs, Rvi* ovp) {
    setlhs (lhs);
    setovp (ovp);
  }

  // return the class name
  
  String Cgs::repr (void) const {
    return "Cgs";
  }
  
  // solve a linear system

  Rvi* Cgs::solve (const Rvi& rhs) {
    wrlock ();
    Rvi* x = nullptr;
    try {
      // check for valid lhs
      if (p_lhs == nullptr) {
	throw Exception ("cgs-error", "nil lhs for solve");
      }
      // check consistency
      t_long size = rhs.getsize ();
      if ((p_lhs->getrsiz () != size) || (p_lhs->getcsiz () != size) || 
	  ((p_ovp != nullptr) && (p_ovp->getsize () != size))) {
	throw Exception ("cgs-error", 
			 "incompatible lhs/preconditionner in cgs solver");
      }
      // create a result vector
      x = dynamic_cast <Rvi*> (rhs.clone ());
      x->set (d_aps);
      // solver the system
      bool status = (p_ovp == nullptr) ? 
	krylov_cgs (*x, *p_lhs, rhs, p_slg, d_mni, d_aps, d_rps) :
	krylov_cgs (*x, *p_lhs, *p_ovp, rhs, p_slg, d_mni, d_aps, d_rps);
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
  
  Object* Cgs::mknew (Vector* argv) {
    long argc = (argv == nullptr) ? 0 : argv->length ();
    
    // check for 0 argument
    if (argc == 0) return new Cgs;    
    // check for 1 argument
    if (argc == 1) {
      Object* obj = argv->get (0);
      Rmi* lhs = dynamic_cast <Rmi*> (obj);
      if ((lhs == nullptr) && (obj != nullptr)) {
	throw Exception ("type-error", "invalid object for cgs constructor",
			 Object::repr (obj));
      }
      return new Cgs (lhs);
    }
    // check for 2 arguments
    if (argc == 2) {
      Object* obj = argv->get (0);
      // check for lhs
      Rmi* lhs = dynamic_cast <Rmi*> (obj);
      if ((lhs == nullptr) && (obj != nullptr)) {
	throw Exception ("type-error", "invalid object for cgs constructor",
			 Object::repr (obj));
      }
      // check for preconditionner
      obj = argv->get (1);
      Rvi* ovp = dynamic_cast <Rvi*> (obj);
      if ((ovp == nullptr) && (obj != nullptr)) {
	throw Exception ("type-error", "invalid object for cgs constructor",
			 Object::repr (obj));
      }
      return new Cgs (lhs, ovp);
    }
    // invalid arguments
    throw Exception ("argument-error", 
		     "invalid arguments with cgs constructor");
  }
}
