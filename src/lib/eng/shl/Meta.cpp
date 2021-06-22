// ---------------------------------------------------------------------------
// - Meta.cpp                                                                -
// - afnix engine - meta class implementation                                -
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

#include "Meta.hpp"
#include "Evaluable.hpp"
#include "Exception.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create an empty meta class

  Meta::Meta (void) {
    p_eval = nullptr;
    p_func = nullptr;
  }

  // create a new meta class

  Meta::Meta (t_eval func) {
    p_eval = func;
    p_func = nullptr;
  }

  // create a new meta class

  Meta::Meta (t_meta func) {
    p_eval = nullptr;
    p_func = func;
  }

  // create a new meta class with an evaluator and a constructor

  Meta::Meta (t_eval feval, t_meta fmeta) {
    p_eval = feval;
    p_func = fmeta;
  }

  // return the class name

  String Meta::repr (void) const {
    return "Meta";
  }

  // -------------------------------------------------------------------------
  // - object section                                                        -
  // -------------------------------------------------------------------------

  // evaluate a quark for this meta object

  Object* Meta::eval (Evaluable* zobj, Nameset* nset, const long quark) {
    rdlock ();
    try {
      if (p_eval == nullptr) {
	Object* result = Object::eval (zobj, nset, quark);
	zobj->post (result);
	unlock ();
	return result;
      }
      Object* result = p_eval (zobj, nset, quark);
      zobj->post (result);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // generate a new object
  
  Object* Meta::apply (Evaluable* zobj, Nameset* nset, Cons* args) {
    rdlock ();
    // check for a function to apply
    if (p_func == nullptr) {
      unlock ();
      throw Exception ("apply-error", "trying to apply a nil meta class");
    }
    // initialize agument vector
    Vector* argv = nullptr;
    try {
      argv = Vector::eval (zobj,nset,args);
    } catch (...) {
      unlock ();
      throw;
    }
    try {      
      Object::iref (argv);
      Object* result = p_func (argv);
      Object::dref (argv);
      zobj->post (result);
      unlock ();
      return result;
    } catch (...) {
      Object::dref (argv);
      unlock ();
      throw;
    }
  }
}
