// ---------------------------------------------------------------------------
// - If.cpp                                                                  -
// - afnix engine builtin if function implementation                         -
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

  // evaluate the builtin if special form

  Object* Builtin::sfif (Evaluable* zobj, Nameset* nset, Cons* args) {
    // check for good arguments
    long argc = (args == nullptr) ? 0 : args->length ();
    if ((argc < 2) || (argc > 3))
      throw Exception ("argument-error", 
		       "missing or too many arguments with if form");
    // get the conditional
    Object* car   = args->getcar ();
    Object* pre   = (car == nullptr) ? nullptr : car->eval (zobj, nset);
    Boolean* bval = dynamic_cast <Boolean*> (pre);
    if (bval == nullptr) {
      throw Exception ("type-error", "expecting boolean object with if form");
    }
    bool flag = bval->tobool ();
    Object::cref (bval);
    // get the if and else form
    Object* form   = args->getcadr ();
    Object* result = nullptr;
    if (flag == true) {
      result = (form == nullptr) ? nullptr : form->eval (zobj, nset);
    } else {
      form   = (argc == 3)    ? args->getcaddr () : nullptr;
      result = (form == nullptr) ? nullptr : form->eval (zobj, nset);
    }
    return result;
  }
}
