// ---------------------------------------------------------------------------
// - PhyCalls.cpp                                                            -
// - afnix:phy service - physics specific calls implementation               -
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
#include "Real.hpp"
#include "Vector.hpp"
#include "Physics.hpp"
#include "Periodic.hpp"
#include "PhyCalls.hpp"
#include "Exception.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - public section                                                        -
  // -------------------------------------------------------------------------

  // create a unit by name

  Object* phy_tounit (Evaluable* zobj, Nameset* nset, Cons* args) {
    // evaluate the arguments
    Vector* argv = Vector::eval (zobj, nset, args);
    long    argc = (argv == nullptr) ? 0 : argv->length ();
    try {
      if (argc == 1) {
	String name = argv->getstring (0);
	Object* result = new Unit (Unit::create (name));
	delete argv;
	return result;
      }
      throw Exception ("argument-error", "too many arguments with to-unit");
    } catch (...) {
      delete argv;
      throw;
    }
  }

  // create a physical constant by name

  Object* phy_tophys (Evaluable* zobj, Nameset* nset, Cons* args) {
    // evaluate the arguments
    Vector* argv = Vector::eval (zobj, nset, args);
    long    argc = (argv == nullptr) ? 0 : argv->length ();
    try {
      if (argc == 1) {
	String name = argv->getstring (0);
	Object* result = new Physics (Physics::create (name));
	delete argv;
	return result;
      }
      throw Exception ("argument-error", "too many arguments with to-physics");
    } catch (...) {
      delete argv;
      throw;
    }
  }
}
