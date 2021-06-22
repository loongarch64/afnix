// ---------------------------------------------------------------------------
// - Add.cpp                                                                 -
// - afnix api example - add function example                                -
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

#include "Vector.hpp"
#include "Integer.hpp"
#include "Apicalls.hpp"

namespace example {
  // use the afnix namespace
  using namespace afnix;
  
  // add function implementation
  Object* api_add (Evaluable* zobj, Nameset* nset, Cons* args) {
    // evaluate the arguments in a vector
    Vector* argv = Vector::eval (zobj, nset, args);
    try {
      // compute the result
      long result = argv->getlong (0) + argv->getlong (1);
      // clean the vector
      delete argv;
      // generate result
      return new Integer (result);
    } catch (...) {
      delete argv;
      throw;
    }
  }
}

