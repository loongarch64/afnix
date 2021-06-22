// ---------------------------------------------------------------------------
// - Block.cpp                                                               -
// - afnix engine - builtin block function implementation                    -
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
#include "Evaluable.hpp"
#include "Localset.hpp"
#include "Exception.hpp"

namespace afnix {

  // block reserved function implementation

  Object* Builtin::sfblock (Evaluable* zobj, Nameset* nset, Cons* args) {
    long len = (args == nullptr) ? 0 : args->length ();
    if (len == 0) return nullptr;
    if (len != 1) throw Exception ("argument-error",
				   "missing or too many argument with block");
    // extract the form
    Object* form = args->getcar ();
    if (form == nullptr) return nullptr;

    // create a nameset - link it with the parent and execute the form
    Nameset*  lset = new Localset (nset);
    Object* result = nullptr;
    try {
      result = form->eval (zobj, lset);
      Object::iref (result);
      zobj->post (result);
      lset->reset ();
      delete lset;
      Object::tref (result);
      return result;
    } catch (...) {
      lset->reset ();
      delete lset;
      throw;
    }
  }
}
