// ---------------------------------------------------------------------------
// - Eval.cpp                                                                -
// - afnix engine - builtin eval/protect/return functions implementation     -
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
#include "Return.hpp"
#include "Builtin.hpp"
#include "Evaluable.hpp"
#include "Exception.hpp"

namespace afnix {

  // evaluate an object directly

  Object* Builtin::sfeval (Evaluable* zobj, Nameset* nset, Cons* args) {
    if ((args == nullptr) || (args->length () != 1)) 
      throw Exception ("argument-error", 
		       "missing or too many argument with eval");
    Object* car = args->getcar ();
    Object* obj = (car == nullptr) ? nullptr : car->eval (zobj, nset);
    while (obj != nullptr) {
      Object::iref (obj);
      Object* result = obj->eval (zobj, nset);
      Object::dref (obj);
      if (result == obj) return result;
      obj = result;
    }
    return nullptr;
  }

  // protect an object against evaluation

  Object* Builtin::sfprotect (Evaluable* zobj, Nameset* nset, Cons* args) {
    if ((args == nullptr) || (args->length () != 1)) 
      throw Exception ("argument-error", 
		       "missing or too many argument with protect");
    Object* result = args->getcar ();
    if (zobj != nullptr) zobj->post (result);
    return result;
  }
  
  // return an object by throwing an exception

  Object* Builtin::sfreturn (Evaluable* zobj, Nameset* nset, Cons* args) {
    // check for 0 argument
    if ((args == nullptr) || (args->length () == 0)) throw Return ();

    // check for one argument only
    if (args->length () != 1) 
      throw Exception ("argument-error", 
		       "too many arguments with return form");

    // evaluate the argument and throw the return object
    Object* car = args->getcar ();
    Object* obj = (car == nullptr) ? nullptr : car->eval (zobj, nset);
    throw Return (obj);
  }
}
