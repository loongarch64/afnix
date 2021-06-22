// ---------------------------------------------------------------------------
// - Launch.cpp                                                              -
// - afnix engine - thread builtin functions implementation                  -
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
#include "Future.hpp"
#include "Builtin.hpp"
#include "Runnable.hpp"
#include "Exception.hpp"

namespace afnix {

  // launch reserved function implementation

  Object* Builtin::sflaunch (Evaluable* zobj, Nameset* nset, Cons* args) {
    // extract argument count and check
    long argc = (args == nullptr) ? 0 : args->length ();
    // check for null
    if (argc == 0) return nullptr;
    // check for a runnable
    auto robj = dynamic_cast<Runnable*>(zobj);
    if (robj == nullptr) {
      throw Exception ("thread-error", "invalid object as runnable",
		       Object::repr (zobj));
    }
    // process one argument
    if (argc == 1) {
      Object* form = args->getcar ();
      if (form == nullptr) return nullptr;
      Object* cobj = Cons::mkobj (zobj, nset, form);
      return robj->launch (cobj);
    }
    // process two arguments
    if (argc == 2) {
      // get the thread object
      Object* tobj = args->getcar ();
      Object* thro = (tobj == nullptr) ? nullptr : tobj->eval (zobj, nset);
      Object* cobj = nullptr;
      // protect the thread object
      Object::iref (thro);
      // try to run the thread
      try {
	// get the form
	Object* form = args->getcadr ();
	if (form == nullptr) {
	  Object::dref (thro);
	  return nullptr;
	}
	// evaluate the form
	Object::iref (cobj = Cons::mkobj (zobj, nset, form));
	// launch the thread
	Object* result = robj->launch (thro, cobj);
	// clean and return
	Object::dref (cobj);
	Object::dref (thro);
	return result;
      } catch (...) {
	Object::dref (cobj);
	Object::dref (thro);
	throw;
      }
    }
    throw Exception ("argument-error", "too many arguments with launch");
  }

  // synchronize reserved function implementation

  Object* Builtin::sfsync (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* form = (args == nullptr) ? nullptr : args->getcar ();
    if (form == nullptr) return nullptr;
    // check for a form
    auto cons = dynamic_cast <Cons*> (form);
    if (cons != nullptr) {
      cons->mksync ();
      return cons->eval (zobj, nset);
    }
    // evaluate the object
    Object* result = form->eval (zobj, nset);
    // check for a future
    auto futr = dynamic_cast<Future*>(result);
    if (futr != nullptr) {
      futr->wait ();
      return futr->eval (zobj, nset);
    }
    return result;
  }
}
