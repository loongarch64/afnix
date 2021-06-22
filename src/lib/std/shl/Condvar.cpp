// ---------------------------------------------------------------------------
// - Condvar.cpp                                                             -
// - standard object library - condition variable class implementation       -
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
#include "Condvar.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"
#include "cthr.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a new condition variable

  Condvar::Condvar (void) {
    d_cond = false;
    p_mtx  = c_mtxcreate ();
    if (p_mtx == nullptr) 
      throw Exception ("condition-error", "cannot create condition variable");
    p_tcv = c_tcvcreate ();
    if (p_tcv == nullptr) {
      c_mtxdestroy (p_mtx);
      throw Exception ("condition-error", "cannot create condition variable");
    }
  }

  // destroy this condition variable

  Condvar::~Condvar (void) {
    c_tcvdestroy (p_tcv);
    c_mtxdestroy (p_mtx);
  }

  // return the class name

  String Condvar::repr (void) const {
    return "Condvar";
  }

  // lock this condition variable

  void Condvar::lock (void) const {
    c_mtxlock (p_mtx);
  }

  // remove this condition variable lock

  void Condvar::rmlock (void) const {
    c_mtxunlock (p_mtx);
  }

  // wait on this condition variable

  void Condvar::wait (const bool lflg) {
    // take the lock if requested
    if (lflg == true) c_mtxlock (p_mtx);
    // now loop until the condition is true
    while (d_cond == false) c_tcvwait (p_tcv, p_mtx);
    // we have the lock again mark as false now
    d_cond = false;
    // eventually unlock if requested
    if (lflg == true) c_mtxunlock (p_mtx);
  }

  // mark this condition variable and broadcast

  void Condvar::mark (void) {
    c_mtxlock   (p_mtx);
    if (d_cond == false) {
      d_cond = true;
      c_tcvbdcast (p_tcv);
    }
    c_mtxunlock (p_mtx);
  }
  
  // reset this condition variable

  void Condvar::reset (void) {
    c_mtxlock (p_mtx);
    d_cond = false;
    c_mtxunlock (p_mtx);
  }

  // -------------------------------------------------------------------------
  // - object section                                                        -
  // -------------------------------------------------------------------------

  // the quark zone
  static const long QUARK_ZONE_LENGTH = 5;
  static QuarkZone  zone (QUARK_ZONE_LENGTH);

  // the object supported quarks
  static const long QUARK_LOCK     = zone.intern ("lock");
  static const long QUARK_WAIT     = zone.intern ("wait");
  static const long QUARK_MARK     = zone.intern ("mark");
  static const long QUARK_RESET    = zone.intern ("reset");
  static const long QUARK_RMLOCK   = zone.intern ("rmlock");

  // create a new object in a generic way

  Object* Condvar::mknew (Vector* argv) {
    long argc = (argv == nullptr) ? 0 : argv->length ();
    if (argc == 0) return new Condvar;
    throw Exception ("argument-error", "too many argument with condvar");
  }

  // return true if the given quark is defined

  bool Condvar::isquark (const long quark, const bool hflg) const {
    rdlock ();
    try {
      if (zone.exists (quark) == true) {
	unlock ();
	return true;
      }
      bool result = hflg ? Object::isquark (quark, hflg) : false;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }


  // apply this object with a set of arguments and a quark

  Object* Condvar::apply (Evaluable* zobj, Nameset* nset, const long quark,
			  Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();

    // dispatch 0 argument
    if (argc == 0) {
      if (quark == QUARK_LOCK) {
	lock ();
	return nullptr;
      }
      if (quark == QUARK_RMLOCK) {
	rmlock ();
	return nullptr;
      }
      if (quark == QUARK_WAIT) {
	wait (true);
	return nullptr;
      }
      if (quark == QUARK_MARK) {
	mark ();
	return nullptr;
      }
      if (quark == QUARK_RESET) {
	reset ();
	return nullptr;
      }
    }
    // dispatch 1 argument
    if (argc == 1) {
      if (quark == QUARK_WAIT) {
	bool lflg = argv->getbool (0);
	wait (lflg);
	return nullptr;
      }
    }
    // call the object method
    return Object::apply (zobj, nset, quark, argv);
  }

}
