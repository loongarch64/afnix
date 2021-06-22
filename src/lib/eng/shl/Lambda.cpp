// ---------------------------------------------------------------------------
// - Lambda.cpp                                                              -
// - afnix engine - builtin lambda and gamma function implementation         -
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
#include "Lexical.hpp"
#include "Closure.hpp"
#include "Builtin.hpp"
#include "Nameset.hpp"
#include "Evaluable.hpp"
#include "Exception.hpp"

namespace afnix {

  // create a new closure 
  static Object* get_closure (Evaluable* zobj, Nameset* nset, Cons* args,
    			      const bool type) {
    // get number of arguments
    long len = (args == nullptr) ? 0 : args->length ();
    if ((len != 2) && (len != 3))
      throw Exception ("argument-error", 
                       "invalid number of argument with closure");
    // extract the argument list
    Cons*    argl = nullptr;
    Object*  car  = args->getcar ();
    Lexical* lex  = dynamic_cast <Lexical*> (car);
    if (lex != nullptr) {
      if (lex->isnil () == false) 
	throw Exception ("argument-error", "only nil is a valid lexical");
    } else {
      argl = dynamic_cast <Cons*> (car);
      if (argl == nullptr)
        throw Exception ("argument-error", "invalid object as argument list",
                         Object::repr (car));
    }
    // extract the closed variable and the execution form
    Cons*   clvl = nullptr;
    Object* form = nullptr;
    if (len == 3) {
      clvl = dynamic_cast <Cons*> (args->getcadr ());
      form = args->getcaddr ();
      if (clvl == nullptr) 
        throw Exception ("argument-error", 
                         "invalid object as closed variable list");
    } else {
      form = args->getcadr ();
    }
    // create the closure
    Closure* result = new Closure (type, argl, form);
    // bind the closed variables
    try {
      while (clvl != nullptr) {
        Object*  car = clvl->getcar ();
        Lexical* lex = dynamic_cast <Lexical*> (car);
        if (lex == nullptr)
          throw Exception ("argument-error", 
			   "invalid object as closed variable",
                           Object::repr (car));
        Object* obj = car->eval (zobj, nset);
        result->addclv (lex->toquark (), false, obj);
	clvl = clvl->getcdr ();
      }
    } catch (...) {
      delete result;
      throw;
    }
    zobj->post (result);
    return result;
  }

  // create a new lambda expression

  Object* Builtin::sflambda (Evaluable* zobj, Nameset* nset, Cons* args) {
    return get_closure (zobj, nset, args, true);
  }

  // create a new gamma expression

  Object* Builtin::sfgamma (Evaluable* zobj, Nameset* nset, Cons* args) {
    return get_closure (zobj, nset, args, false);
  }
}
