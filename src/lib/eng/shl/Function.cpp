// ---------------------------------------------------------------------------
// - Function.cpp                                                            -
// - afnix engine - function class implementation                            -
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

#include "Evaluable.hpp"
#include "Function.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a default function

  Function::Function (void) {
    p_func = nullptr;
  }

  // create a new function

  Function::Function (t_func func) {
    p_func = func;
  }

  // return the class name

  String Function::repr (void) const {
    return "Function";
  }

  // -------------------------------------------------------------------------
  // - object section                                                        -
  // -------------------------------------------------------------------------

  // apply this function

  Object* Function::apply (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* result = (p_func == nullptr) ? nullptr : p_func (zobj, nset, args);
    zobj->post (result);
    return result;
  }
}
