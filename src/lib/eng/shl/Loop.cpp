// ---------------------------------------------------------------------------
// - Loop.cpp                                                                -
// - afnix engine - builtin loop functions implementation                    -
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

#include "Cons.hpp"
#include "Builtin.hpp"
#include "Boolean.hpp"
#include "Evaluable.hpp"
#include "Globalset.hpp"
#include "Exception.hpp"

namespace afnix {

  // this procedure evaluate an object and check that we have a boolean. It
  // returns the boolean value
  static inline bool check_cond (Evaluable* zobj, Nameset* nset, Object* obj) {
    // evaluate the boolean object
    Object*  bobj = (obj == nullptr) ? nullptr : obj->eval (zobj, nset);
    Boolean* bval = dynamic_cast <Boolean*> (bobj);
    if (bval == nullptr) {
      throw Exception ("type-error", "illegal object in loop condition",
		       Object::repr (bobj));
    }
    // convert and clean
    bool result = bval->tobool ();
    Object::cref (bval);
    return result;
  }

  // run the while loop

  Object* Builtin::sfwhile (Evaluable* zobj, Nameset* nset, Cons* args) {
    // check for nil
    if (args == nullptr) return nullptr;
    // extract argument count and check
    long argc = args->length ();
    if (argc > 3) 
      throw Exception ("argument-error",
		       "missing or too many arguments with while loop");
    // the simple case is with 2 arguments
    if (argc == 2) {
      // extract condition and form
      Object* cond = args->getcar  ();
      Object* form = args->getcadr ();    
      // loop until false condition
      Object* result = nullptr;
      while (check_cond (zobj, nset, cond) == true) {
	Object::dref (result);
	result = (form == nullptr) ? nullptr : form->eval (zobj, nset);
	Object::iref (result);
      }
      zobj->post (result);
      Object::tref (result);
      return result;
    }
    // here we have 3 arguments - this indicates the existence of a
    // starting condition that is executed only once
    Object* sobj = args->getcar   ();
    Object* cond = args->getcadr  ();
    Object* form = args->getcaddr ();
    // create a new nameset and link it with parent
    Nameset* lset = new Globalset (nset);
    Object::iref (lset);
    try {
      // call the initial condition
      if (sobj != nullptr) Object::cref (sobj->eval (zobj, lset));
      // loop until false condition
      Object* result = nullptr;
      while (check_cond (zobj, lset, cond) == true) {
	Object::dref (result);
	result = (form == nullptr) ? nullptr : form->eval (zobj, lset);
	Object::iref (result);
      }
      zobj->post (result);
      lset->reset ();
      Object::dref (lset);
      Object::tref (result);
      return result;
    } catch (...) {
      lset->reset ();
      Object::dref (lset);
      throw;
    }
  }

  // run the do loop

  Object* Builtin::sfdo (Evaluable* zobj, Nameset* nset, Cons* args) {
    // check for nil
    if (args == nullptr) return nullptr;
    // extract argument count and check
    long argc = args->length ();
    if (argc > 3) 
      throw Exception ("argument-error",
		       "missing or too many arguments with while loop");
    // the simple case is with 2 arguments
    if (argc == 2) {    
      // extract form and condition
      Object* form = args->getcar  ();
      Object* cond = args->getcadr ();
      // loop until false condition
      Object* result = nullptr;
      do {
	Object::dref (result);
	result = (form == nullptr) ? nullptr : form->eval (zobj, nset);
	Object::iref (result);
      } while (check_cond (zobj, nset, cond) == true);
      zobj->post (result);
      Object::tref (result);
      return result;
    }
    // here we have 3 arguments - this indicates the existence of a
    // starting condition that is executed only once
    Object* sobj = args->getcar   ();
    Object* form = args->getcadr  ();
    Object* cond = args->getcaddr ();
    // create a new nameset and link it with parent
    Nameset* lset = new Globalset (nset);
    Object::iref (lset);
    try {
      // call the initial condition
      if (sobj != nullptr) Object::cref (sobj->eval (zobj, lset));
      // loop until false condition
      Object* result = nullptr;
      do {
	Object::dref (result);
	result = (form == nullptr) ? nullptr : form->eval (zobj, nset);
	Object::iref (result);
      } while (check_cond (zobj, nset, cond) == true);
      zobj->post (result);
      lset->reset ();
      Object::dref (lset);
      Object::tref (result);
      return result;
    } catch (...) {
      lset->reset ();
      Object::dref (lset);
      throw;
    }
  }
  
  // run the 'loop' loop

  Object* Builtin::sfloop (Evaluable* zobj, Nameset* nset, Cons* args) {
    // trivial check first
    long argc = (args == nullptr) ? 0 : args->length ();
    if ((argc != 2) && (argc != 4)) {
      throw Exception ("argument-error",
		       "missing or too many arguments with loop");
    }
    // extract start, end step and form
    Object* sobj = (argc == 4) ? args->getcar () : nullptr;
    Object* eobj = (argc == 4) ? args->getcadr () : args->getcar ();
    Object* step = (argc == 4) ? args->getcaddr () : nullptr;    
    Object* form = (argc == 4) ? args->getcadddr () : args->getcadr ();
    // create a new nameset and link it with parent
    Nameset* lset = new Globalset (nset);
    Object::iref (lset);
    try {
      // call the initial condition
      if (sobj != nullptr) Object::cref (sobj->eval (zobj, lset));
      // loop until false condition
      Object* result = nullptr;
      while (check_cond (zobj, lset, eobj) == true) {
	Object::dref (result);
	result = (form == nullptr) ? nullptr : form->eval (zobj, lset);
	Object::iref (result);
	if (step != nullptr) Object::cref (step->eval (zobj, lset));
      }
      zobj->post (result);
      lset->reset ();
      Object::dref (lset);
      Object::tref (result);
      return result;
    } catch (...) {
      lset->reset ();
      Object::dref (lset);
      throw;
    }
  }
}
