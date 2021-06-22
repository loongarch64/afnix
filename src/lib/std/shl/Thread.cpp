// ---------------------------------------------------------------------------
// - Thread.cpp                                                              -
// - standard object library - thread class implementation                   -
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
#include "Thread.hpp"
#include "Boolean.hpp"
#include "Runnable.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"
#include "cthr.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - private section                                                       -
  // -------------------------------------------------------------------------

  // this function is the object destructor as void pointer - extreme caution
  // should be exercise here since the argument can be a evaluable which
  // support multiple inheritance...
  static void thr_clean (void* args, const bool rarg) {
    auto zobj = rarg
      ? reinterpret_cast <Runnable*> (args)
      : reinterpret_cast <Object*>   (args);
    Object::dref (zobj);
  }

  // this function starts the thread
  static void* thr_start (void* args) {
    auto robj = reinterpret_cast <Runnable*> (args);
    return Object::iref (robj->run ());
  }

  // create a thread object by id

  Thread::Thread (void* tid) {
    p_tid  = tid;
    d_tgn  = String::qmap (c_thrgetgid (tid));
    p_thrn = nullptr;
    p_thrs = nullptr;
  }

  // create a thread by notifier and set - the thread set object reference
  // count is already adjusted by the thread set and reset by the low-level
  // object destructor callback

  Thread::Thread (void* thrn, Object* thrs, const String& tgn) {
    p_tid  = nullptr;
    d_tgn  = tgn;
    p_thrn = thrn;
    p_thrs = thrs;
  }

  // -------------------------------------------------------------------------
  // - public section                                                        -
  // -------------------------------------------------------------------------

  // return true if we are the master thread

  bool Thread::ismaster (void) {
    return c_thrmaster ();
  }

  // return true if the thread list is nil

  bool Thread::isnil (void) {
    // do nothing if not the master thread
    if (c_thrmaster () == false) return false;
    // check for nil list
    return c_thrnullptr ();
  }

  // wait for the thread list to be nil

  void Thread::wnil (void) {
    // do nothing if not the master thread
    if (c_thrmaster () == false) return;
    // wait for a nil list
    c_thrwnil ();
  }

  // wait for all threads to terminate

  void Thread::wall (void) {
    // do nothing if not the master thread
    if (c_thrmaster () == false) return;
    // loop for all threads
    s_thrl* thrl = c_thrgetl (true);
    s_thrl* elem = thrl;
    while (elem != nullptr) {
      // wait for the thread
      c_thrwait (elem->p_thro);
      // detroy the element reference
      c_thrdel (elem->p_thro);
      // move to the next element
      elem = elem->p_next;
      // check for nil in case new threads have been created
      if (elem == nullptr) {
    	// destroy the old list
    	delete thrl;
    	// update the list
    	thrl = c_thrgetl (true);
    	// restart at the beginning
    	elem = thrl;
      }
    }
  }

  // wait for all threads to terminate by group name

  void Thread::wall (const String& tgn) {
    // map the thread group name
    long tgid = tgn.toquark ();
    // loop for all threads
    s_thrl* thrl = c_thrgetl (tgid, true);
    s_thrl* elem = thrl;
    while (elem != nullptr) {
      // do not wait for ourself
      if (c_threqual (elem->p_thro) == false) {
	// wait for the thread
	c_thrwait (elem->p_thro);
	// detroy the element reference
	c_thrdel (elem->p_thro);
      }
      // move to the next element
      elem = elem->p_next;
      // check for nil in case new threads have been created
      if (elem == nullptr) {
    	// destroy the old list
    	delete thrl;
    	// update the list
    	thrl = c_thrgetl (tgid, true);
    	// restart at the beginning
    	elem = thrl;
      }
    }
  }

  // make the current thread terminate itself

  void Thread::exit (void) {
    c_threxit ();
  }

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a default thread

  Thread::Thread (void) {
    p_tid  = nullptr;
    p_thrn = nullptr;
    p_thrs = nullptr;

  }

  // create a thread by group name

  Thread::Thread (const String& tgn) {
    p_tid  = nullptr;
    d_tgn  = tgn;
    p_thrn = nullptr;
    p_thrs = nullptr;
  }

  // destroy this thread

  Thread::~Thread (void) {
    Object::dref (p_thrs);
    if (p_tid != nullptr) c_thrdel (p_tid);
  }

  // return the class name

  String Thread::repr (void) const {
    return "Thread";
  }

  // start a thread if possible

  void Thread::start (Evaluable* zobj) {
    wrlock ();
    try {
      // check for a valid evaluable
      if (zobj == nullptr) {
	throw Exception ("thread-error", "invalid nil evaluable object");
      }
      // check for a valid descriptor
      if (p_tid != nullptr) {
	throw Exception ("thread-error", "thread already started in start");
      }
      // let the ball rolling - mark the evaluable object as used so it is
      // unreferenced at the exit of the thread via thrclean
      Object::iref (zobj);
      // create a thread argument structure
      s_targ targ;
      targ.d_tgid = d_tgn.toquark ();
      targ.p_func = thr_start;
      targ.p_args = zobj;
      targ.p_dtor = thr_clean;
      targ.p_thrn = (t_thrn) p_thrn;
      targ.p_thrs = (void*) p_thrs;
      // create a new thread
      p_tid = c_thrnew (targ);
      // reset internal element
      p_thrs = nullptr;
      p_thrn = nullptr;
      if (p_tid == nullptr) {
	throw Exception ("thread-error", "cannot start thread");
      }
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return true if the thread has ended

  bool Thread::isend (void) const {
    rdlock ();
    try {
      bool result = (p_tid == nullptr) ? false : c_thrend (p_tid);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // wait for this thread to terminate

  void Thread::wait (void) {
    wrlock ();
    try {
      c_thrwait (p_tid);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the thread group name

  String Thread::gettgn (void) const {
    rdlock ();
    try {
      String result = d_tgn;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the thread result

  Object* Thread::toresult (void) const {
    rdlock ();
    try {
      Object* result = (Object*) c_thrgetres (p_tid);
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
  static const long QUARK_ZONE_LENGTH = 3;
  static QuarkZone  zone (QUARK_ZONE_LENGTH);

  // the object supported quarks
  static const long QUARK_WAIT     = zone.intern ("wait");  
  static const long QUARK_ENDP     = zone.intern ("end-p");  
  static const long QUARK_GETTGN   = zone.intern ("group");  
  static const long QUARK_TORESULT = zone.intern ("result");  

  // create a new object in a generic way
  
  Object* Thread::mknew (Vector* argv) {
    long argc = (argv == nullptr) ? 0 : argv->length ();
    
    // check 0 argument
    if (argc == 0) return new Thread;
    // too many arguments
    throw Exception ("argument-error", "too many argument for thread");
  }

  // return true if the given quark is defined

  bool Thread::isquark (const long quark, const bool hflg) const {
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

  Object* Thread::apply (Evaluable* zobj, Nameset* nset, const long quark,
			 Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();

    // dispatch 0 argument
    if (argc == 0) {
      if (quark == QUARK_ENDP)   return new Boolean (isend  ());
      if (quark == QUARK_GETTGN) return new String  (gettgn ());
      if (quark == QUARK_WAIT)   {
	wait ();
	return nullptr;
      }
      if (quark == QUARK_TORESULT) {
	rdlock ();
	try {
	  Object* result = toresult ();
	  zobj->post (result);
	  unlock ();
	  return result;
	} catch (...) {
	  unlock ();
	  throw;
	}
      }
    }
    // call the object method
    return Object::apply (zobj, nset, quark, argv);
  }
}
