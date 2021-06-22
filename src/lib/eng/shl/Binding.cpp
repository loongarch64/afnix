// ---------------------------------------------------------------------------
// - Binding.cpp                                                             -
// - afnix engine - builtin binding functions implementation                 -
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
#include "Promise.hpp"
#include "Globalset.hpp"
#include "Exception.hpp"

namespace afnix {

  // const reserved function implementation

  Object* Builtin::sfconst (Evaluable* zobj, Nameset* nset, Cons* args) {
    long len = (args == nullptr) ? 0 : args->length ();
    if (len == 0) return nullptr;
    if (len == 2) {
      Object* car = args->getcar  ();
      Object* cdr = args->getcadr ();
      Object* obj = (cdr == nullptr) ? nullptr : cdr->eval (zobj, nset);
      return (car == nullptr) ? nullptr : car->cdef (zobj, nset, obj);
    }
    if ((len == 3) || (len == 4)) {
      Object* car = args->getcar  ();
      Object* obj = Builtin::sfgamma (zobj, nset, args->getcdr ());
      return (car == nullptr) ? nullptr : car->cdef (zobj, nset, obj);
    }

    // invalid number of arguments
    throw Exception ("argument-error", 
		     "invalid number of arguments with const");
  }

  // trans reserved function implementation

  Object* Builtin::sftrans (Evaluable* zobj, Nameset* nset, Cons* args) {
    long len = (args == nullptr) ? 0 : args->length ();
    if (len == 0) return nullptr;
    if (len == 2) {
      Object* car = args->getcar  ();
      Object* cdr = args->getcadr ();
      Object* obj = (cdr == nullptr) ? nullptr : cdr->eval (zobj, nset);
      return (car == nullptr) ? nullptr : car->vdef (zobj, nset, obj);
    }
    if ((len == 3) || (len == 4)) {
      Object* car = args->getcar  ();
      Object* obj = Builtin::sflambda (zobj, nset, args->getcdr ());
      return (car == nullptr) ? nullptr : car->vdef (zobj, nset, obj);
    }

    // invalid number of arguments
    throw Exception ("argument-error", 
		     "invalid number of arguments with trans");
  }

  // unref reserved function implementation

  Object* Builtin::sfunref (Evaluable* zobj, Nameset* nset, Cons* args) {
    long len = (args == nullptr) ? 0 : args->length ();
    if (len == 0) return nullptr;
    if (len == 1) {
      Object* car = args->getcar  ();
      return (car == nullptr) ? nullptr : car->udef (zobj, nset);
    }
    // invalid number of arguments
    throw Exception ("argument-error", 
		     "invalid number of arguments with unref");
  }

  // nameset reserved function implementation

  Object* Builtin::sfnameset (Evaluable* zobj, Nameset* nset, Cons* args) {
    long len = (args == nullptr) ? 0 : args->length ();
    if (len == 0) return new Globalset;
    if (len == 1) {
      Object* car = args->getcar ();
      Object* obj = (car == nullptr) ? nullptr : car->eval (zobj, nset);
      Nameset* nset = dynamic_cast <Nameset*> (obj);
      if (nset == nullptr) 
	throw Exception ("type-error", "invalid object with nameset",
			 Object::repr (obj));
      return new Globalset (nset);
    }
    throw Exception ("argument-error", "too many arguments with nameset");
  }

  // delay builtin function implementation
  
  Object* Builtin::sfdelay (Evaluable* zobj, Nameset* nset, Cons* args) {
    long len = (args == nullptr) ? 0 : args->length ();
    // check for nil
    if (len == 0) return nullptr;
    // check for single argument
    if (len == 1) return new Promise (args->getcar ());
    // prepare a cons cell with the car and loop
    Cons* pobj = new Cons (args->getcar ());
    Cons* parg = args->getcdr ();
    while (parg != nullptr) {
      // evaluate the car
      Object* pcar = parg->getcar ();
      Object* pval = (pcar == nullptr) ? nullptr : pcar->eval (zobj, nset);
      // add it in the cons cell
      pobj->add (pval);
      // move to next cdr
      parg = parg->getcdr ();
    }
    return new Promise (pobj);
  }

  // future builtin function implementation
  
  Object* Builtin::sffuture (Evaluable* zobj, Nameset* nset, Cons* args) {
    long len = (args == nullptr) ? 0 : args->length ();
    // check for nil
    if (len == 0) return nullptr;
    // check for single argument
    if (len == 1) return new Future (args->getcar ());
    // prepare a cons cell with the car and loop
    Cons* pobj = new Cons (args->getcar ());
    Cons* parg = args->getcdr ();
    while (parg != nullptr) {
      // evaluate the car
      Object* pcar = parg->getcar ();
      Object* pval = (pcar == nullptr) ? nullptr : pcar->eval (zobj, nset);
      // add it in the cons cell
      pobj->add (pval);
      // move to next cdr
      parg = parg->getcdr ();
    }
    return new Future (pobj);
  }

  // force the evaluation of a promise

  Object* Builtin::sfforce (Evaluable* zobj, Nameset* nset, Cons* args) {
    long len = (args == nullptr) ? 0 : args->length ();
    if (len != 1) throw Exception ("argument-error",
				   "invalid number of arguments with force");
    // get the car and evaluate it to an object
    Object* car = args->getcar ();
    Object* obj = (car == nullptr) ? nullptr : car->eval (zobj, nset);
    // check for a promise
    Promise* prm = dynamic_cast <Promise*> (obj);
    if (prm != nullptr) {
      // protect and evaluate
      Object::iref (prm);
      Object* result = prm->force (zobj, nset);
      // clean and return
      Object::dref (prm);
      return result;
    }
    // check for a future
    Future* fur = dynamic_cast <Future*> (obj);
    if (fur != nullptr) {
      // protect and evaluate
      Object::iref (fur);
      Object* result = fur->force (zobj, nset);
      // clean and return
      Object::dref (fur);
      return result;
    }
    // default to evaluated object
    return obj;
  }
}
