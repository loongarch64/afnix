// ---------------------------------------------------------------------------
// - Logical.cpp                                                             -
// - afnix engine - logical builtin functions implementation                 -
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
#include "Exception.hpp"

namespace afnix {

  // not: compute the negation of the argument

  Object* Builtin::sfnot (Evaluable* zobj, Nameset* nset, Cons* args) {
    // trivial case first
    if ((args == nullptr) || (args->length () != 1)) {
      throw Exception ("argument-error", 
		       "missing or too many arguments with not");
    }
    // get the boolean and compute
    Object*  car  = args->getcar ();
    Object*  obj  = (car == nullptr) ? nullptr : car->eval (zobj, nset);
    Boolean* bval = dynamic_cast <Boolean*> (obj);
    if (bval == nullptr) {
      throw Exception ("type-error", "boolean expected with not",
		       Object::repr (obj));
    }
    // compute result
    Object* result = (*bval == true) ? new Boolean (false) : 
                                       new Boolean (true);
    Object::cref (bval);
    return result;
  }

  // and: compute the and of several arguments

  Object* Builtin::sfand (Evaluable* zobj, Nameset* nset, Cons* args) {
    // trivial case first
    if ((args == nullptr) || (args->length () < 2)) {
      throw Exception ("argument-error", 
		       "missing or too many arguments with and");
    }
    // loop for the result
    bool result = true;
    while (args != nullptr) {
      Object*  car  = args->getcar ();
      Object*  obj  = (car == nullptr) ? nullptr : car->eval (zobj, nset);
      Boolean* bval = dynamic_cast <Boolean*> (obj);
      if (bval == nullptr) {
	throw Exception ("type-error", "boolean expected with and",
			 Object::repr (obj));
      }
      // compute result
      result &= (*bval == true);
      Object::cref (bval);
      // shortcut rule
      if (result == false) break;
      // move to the next argument
      args = args->getcdr ();
    }
    return new Boolean (result);
  }

  // or: compute the or of several arguments

  Object* Builtin::sfor (Evaluable* zobj, Nameset* nset, Cons* args) {
    // trivial case first
    if ((args == nullptr) || (args->length () < 2)) {
      throw Exception ("argument-error", 
		       "missing or too many arguments with or");
    }
    // loop for the result
    bool result = false;
    while (args != nullptr) {
      Object*  car  = args->getcar ();
      Object*  obj  = (car == nullptr) ? nullptr : car->eval (zobj, nset);
      Boolean* bval = dynamic_cast <Boolean*> (obj);
      if (bval == nullptr) {
	throw Exception ("type-error", "boolean expected with or",
			 Object::repr (obj));
      }
      // compute result
      result |= (*bval == true);
      Object::cref (bval);
      // shortcut rule
      if (result == true) break;
      // move to the next argument
      args = args->getcdr ();
    }
    return new Boolean (result);
  }
}
