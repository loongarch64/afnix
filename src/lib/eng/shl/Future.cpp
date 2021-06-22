// ---------------------------------------------------------------------------
// - Future.cpp                                                              -
// - afnix engine - future class implementation                              -
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

#include "Task.hpp"
#include "Vector.hpp"
#include "Thread.hpp"
#include "Future.hpp"
#include "Runnable.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - private section                                                       -
  // -------------------------------------------------------------------------

  // the future context
  struct s_fctx {
    // the future task
    Task* p_task;
    // the future thread
    Thread* p_thro;
    // evaluated object
    Object* p_eobj;
    // create a nil context
    s_fctx (void) {
      p_task = nullptr;
      p_thro = nullptr;
      p_eobj = nullptr;
    }
    // destroy this context
    ~s_fctx (void) {
      Object::dref (p_task);
      Object::dref (p_thro);
      Object::dref (p_eobj);
    }
  };
  
  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a default future

  Future::Future (void) {
    p_form = nullptr;
    p_fctx = nullptr;
  }

  // create a new future with a form

  Future::Future (Object* form) {
    p_form = Object::iref (form);
    p_fctx = nullptr;
  }

  // destroy this future

  Future::~Future (void) {
    delete p_fctx;
    Object::dref (p_form);
  }

  // return the class name

  String Future::repr (void) const {
    return "Future";
  }

  // wait for this future

  void Future::wait (void) {
    wrlock ();
    try {
      // check for nil
      if (p_fctx == nullptr) {
	unlock ();
	return;
      }
      // check for a task
      if (p_fctx->p_task != nullptr) p_fctx->p_task->wait ();
      // check for a thread
      if (p_fctx->p_thro != nullptr) p_fctx->p_thro->wait ();
      // no more to wait for
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // force the evaluation of this future

  Object* Future::force (Evaluable* zobj, Nameset* nset) {
    wrlock ();
    try {
      // check for a context
      if (p_fctx == nullptr) {
	// create a new context
	p_fctx = new s_fctx;
	// check for a task
	auto task = dynamic_cast<Taskable*>(p_form);
	if (task != nullptr) {
	  // schedule the task
	  Object::iref (p_fctx->p_task = new Task (task));
	  // collect result if any
	  Object* result = task->toresult ();
	  unlock ();
	  return result;
	}
	// check for a runnable
	auto robj = dynamic_cast<Runnable*>(zobj);
	if (robj != nullptr) {
	  // create the target thread
	  Object::iref (p_fctx->p_thro = new Thread);
	  // launch the thread
	  robj->launch(p_fctx->p_thro, p_form);
	  Object* result = p_fctx->p_thro->toresult();
	  unlock ();
	  return result;
	}
	// normal evaluation
	Object* result =
	  (p_form == nullptr) ? nullptr : p_form->eval (zobj, nset);
	Object::iref (p_fctx->p_eobj = result); zobj->post (result);
	unlock ();
	return result;
      }
      // run a normal evaluation
      Object* result = eval (zobj, nset);
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

  // evaluate this future

  Object* Future::eval (Evaluable* zobj, Nameset* nset) {
    rdlock ();
    try {
      // check for null context
      if (p_fctx == nullptr) {
	unlock ();
	return this;
      }
      // normal evaluation
      Object* result = p_fctx->p_eobj;
      // check for a task
      if (p_fctx->p_task != nullptr) {
	result = p_fctx->p_task->isend () ? p_fctx->p_task->toresult () : this;
      }
      // check for a thread
      if (p_fctx->p_thro != nullptr) {
	result = p_fctx->p_thro->isend () ? p_fctx->p_thro->toresult () : this;
      }
      // normal result
      zobj->post (result);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
}
