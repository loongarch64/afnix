// ---------------------------------------------------------------------------
// - Thrset.cpp                                                              -
// - standard object library - thread set class implementation               -
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
#include "Thrset.hpp"
#include "Boolean.hpp"
#include "Integer.hpp"
#include "Evaluable.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"
#include "cthr.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - private section                                                       -
  // -------------------------------------------------------------------------

  // this function notify the thread set
  static void thrs_notify (void* args) {
    // check for valid argument
    if (args == nullptr) return;
    Object* tobj = reinterpret_cast <Object*> (args);
    // derive the thread set and notify
    Thrset* tset = dynamic_cast <Thrset*> (tobj);
    if (tset != nullptr) tset->notify ();
  }

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a default thread set

  Thrset::Thrset (void) {
    // initialize the thread set
    d_msiz = 0;
    d_tsiz = 0;
    d_pmod = false;
    p_tset = new Set;
    p_cvar = nullptr;
    // create a list of running thread
    s_thrl* thrl = c_thrgetl (false);
    s_thrl* elem = thrl;
    // loop in the list
    while (elem != nullptr) {
      // create a thread by id
      Thread* thr = new Thread (elem->p_thro);
      // add it in the set
      p_tset->add (thr);
      // next thread in list
      elem = elem->p_next;
    }
    // destroy the list
    delete thrl;
    // update the thread size
    d_tsiz = p_tset->length ();
  }

  // create a thread set by group name

  Thrset::Thrset (const String& tgn) {
    // initialize the thread set
    d_msiz = 0;
    d_tsiz = 0;
    d_pmod = false;
    p_tset = new Set;
    p_cvar = nullptr;
    // create a list of threads by group name
    s_thrl* thrl = c_thrgetl (tgn.toquark ());
    s_thrl* elem = thrl;
    // loop in the list
    while (elem != nullptr) {
      // create a thread by id
      Thread* thr = new Thread (elem->p_thro);
      // add it in the set
      p_tset->add (thr);
      // next thread in list
      elem = elem->p_next;
    }
    // destroy the list
    delete thrl;
    // update the thread size
    d_tsiz = p_tset->length ();
  }

  // create a new thread set by size

  Thrset::Thrset (const long msiz) {
    d_msiz = (msiz < 0) ? c_thrmax () : msiz;
    d_tsiz = 0;
    d_pmod = false;
    p_tset = new Set;
    p_cvar = nullptr;
  }

  // create a new thread set by mode

  Thrset::Thrset (const bool pmod) {
    d_msiz = c_thrmax ();
    d_tsiz = 0;
    d_pmod = pmod;
    p_tset = pmod ? nullptr : new Set;
    p_cvar = pmod ? new Condvar : nullptr;
  }

  // create a new thread set by size and mode

  Thrset::Thrset (const long msiz, const bool pmod) {
    d_msiz = (msiz < 0) ? c_thrmax () : msiz;
    d_tsiz = 0;
    d_pmod = pmod;
    p_tset = pmod ? nullptr : new Set;
    p_cvar = pmod ? new Condvar : nullptr;
  }

  // destroy this thread set

  Thrset::~Thrset (void) {
    delete p_tset;
    delete p_cvar;
  }

  // return the class name

  String Thrset::repr (void) const {
    return "Thrset";
  }

  // return true if the thread set is empty

  bool Thrset::empty (void) const {
    rdlock ();
    try {
      bool result = (d_tsiz == 0);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return true if the thread set is full

  bool Thrset::full (void) const {
    rdlock ();
    try {
      bool result = (d_msiz == 0) ? false : (d_tsiz >= d_msiz);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return the maximum number of threads

  long Thrset::getmsiz (void) const {
    rdlock ();
    try {
      long result = d_msiz;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return the thread set size

  long Thrset::length (void) const {
    rdlock ();
    try {
      long result = d_tsiz;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // request a new thread of control

  Thread* Thrset::request (void) {
    return request ("");
  }

  // request a new thread of control

  Thread* Thrset::request (const String& tgn) {
    wrlock ();
    try {
      // do not block in non pool mode when full
      if ((d_pmod == false) && (d_msiz != 0) && (d_tsiz >= d_msiz)) {
	unlock ();
	return nullptr;
      }
      // block in pool mode when full - but loop since notify
      // might be called by anybody
      while ((d_pmod == true) && (d_tsiz >= d_msiz)) {
	// lock the condition variable
	p_cvar->lock ();
	// unlock the object
	unlock ();
	// wait for room
	p_cvar->wait (false);
	// remove the condition variable lock
	p_cvar->rmlock ();
	// re-acquire the object write lock
	wrlock ();
	// eventually break the loop
	if (d_tsiz < d_msiz) break;
      }
      // create a new thread object
      Thread* result = nullptr;
      if (d_pmod == true) {
	Object::iref (this);
	result = new Thread ((void*) thrs_notify, this, tgn);
      } else {
	result = new Thread;
      }
      // add it if needed
      if (d_pmod == false) p_tset->add (result);
      // update the counter
      d_tsiz++;
      // check for consistency
      if ((d_pmod == false) && (d_tsiz != p_tset->length ())) {
	throw Exception ("internal-error", "inconsisten thread set size");
      }
      // unlock and return
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // wait for all thread to end

  void Thrset::wait (void) {
    wrlock ();
    try {
      // do nothing in pool mode
      if ((d_pmod == true) || (d_tsiz == 0)) {
	unlock ();
	return;
      }
      // check for consistency
      if ((d_pmod == false) && (d_tsiz != p_tset->length ())) {
	throw Exception ("internal-error", "inconsisten thread set size");
      }
      // loop and wait
      for (long i = 0; i < d_tsiz; i++) {
	Thread* thr = dynamic_cast <Thread*> (p_tset->get (i));
	if (thr != nullptr) thr->wait ();
      }
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // notify this thread set

  bool Thrset::notify (void) {
    wrlock ();
    try {
      // notify only in pool mode
      bool result = false;
      if ((d_pmod == true) && (p_cvar != nullptr)) {
	// fix pool size
	if (--d_tsiz < 0) d_tsiz = 0;
	// mark the condition
	p_cvar->mark ();
	// set the result
	result = (d_tsiz < d_msiz);
      }
      // unlock and return
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // -------------------------------------------------------------------------
  // - object section                                                        -
  // -------------------------------------------------------------------------

  // the quark zone
  static const long QUARK_ZONE_LENGTH = 7;
  static QuarkZone  zone (QUARK_ZONE_LENGTH);

  // the object supported quarks
  static const long QUARK_WAIT   = zone.intern ("wait");  
  static const long QUARK_RQST   = zone.intern ("request");  
  static const long QUARK_MSIZ   = zone.intern ("get-maximum-size");  
  static const long QUARK_FULLP  = zone.intern ("full-p");  
  static const long QUARK_EMPTYP = zone.intern ("empty-p");  
  static const long QUARK_LENGTH = zone.intern ("length");  
  static const long QUARK_NOTIFY = zone.intern ("notify");  

  // create a new object in a generic way
  
  Object* Thrset::mknew (Vector* argv) {
    long argc = (argv == nullptr) ? 0 : argv->length ();
    
    // check 0 argument
    if (argc == 0) return new Thrset;
    // check 1 argument
    if (argc == 1) {
      Object* obj = argv->get (0);
      // check for a string
      String* sobj = dynamic_cast <String*> (obj);
      if (sobj != nullptr) {
	return new Thrset (*sobj);
      }
      // check for an integer
      Integer* iobj = dynamic_cast <Integer*> (obj);
      if (iobj != nullptr) {
	long msiz = iobj->tolong ();
	return new Thrset (msiz);
      }
      // check for a boolean
      Boolean* bobj = dynamic_cast <Boolean*> (obj);
      if (bobj != nullptr) {
	bool pmod = bobj->tobool ();
	return new Thrset (pmod);
      }
      // type error
      throw Exception ("type-error", 
		       "invalid object with thread set constructor");
    }
    // check 2 arguments
    if (argc == 2) {
      long msiz = argv->getlong (0);
      bool pmod = argv->getbool (1);
      return new Thrset (msiz, pmod);
    }
    throw Exception ("argument-error", "too many argument for thrset");
  }

  // return true if the given quark is defined

  bool Thrset::isquark (const long quark, const bool hflg) const {
    rdlock ();
    if (zone.exists (quark) == true) {
      unlock ();
      return true;
    }
    bool result = hflg ? Object::isquark (quark, hflg) : false;
    unlock ();
    return result;
  }

  // apply this object with a quark and a set of arguments

  Object* Thrset::apply (Evaluable* zobj, Nameset* nset, const long quark,
			 Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();

    // dispatch 0 argument
    if (argc == 0) {
      if (quark == QUARK_MSIZ)   return new Integer (getmsiz ());
      if (quark == QUARK_FULLP)  return new Boolean (full    ());
      if (quark == QUARK_EMPTYP) return new Boolean (empty   ());
      if (quark == QUARK_LENGTH) return new Integer (length  ());
      if (quark == QUARK_NOTIFY) return new Boolean (notify  ());
      if (quark == QUARK_WAIT) {
	wait ();
	return nullptr;
      }	
      if (quark == QUARK_RQST) {
	Object* result = request ();
	zobj->post (result);
	return result;
      }
    }
    // dispatch 1 argument
    if (argc == 1) {
      if (quark == QUARK_RQST) {
	String tgn = argv->getstring (0);
	Object* result = request (tgn);
	zobj->post (result);
	return result;
      }
    }
    // call the object method
    return Object::apply (zobj, nset, quark, argv);
  }
}
