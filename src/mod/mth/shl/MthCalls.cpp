// ---------------------------------------------------------------------------
// - MthCalls.cpp                                                            -
// - afnix:mth module - math specific calls implementation                   -
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
#include "Infix.hpp"
#include "Vector.hpp"
#include "MthCalls.hpp"
#include "Exception.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - public section                                                        -
  // -------------------------------------------------------------------------

  // convert an infix string to a rvi

  Object* mth_torvi (Evaluable* zobj, Nameset* nset, Cons* args) {
    // evaluate the arguments
    Vector* argv = Vector::eval (zobj, nset, args);
    long    argc = (argv == nullptr) ? 0 : argv->length ();
    try {
      if (argc == 1) {
	String s = argv->getstring (0);
	delete argv; argv = nullptr;
	return Infix::torvi (s);
      }
      throw Exception ("argument-error", 
		       "too many arguments with to-rvi");
    } catch (...) {
      delete argv;
      throw;
    }
  }

  // convert an object to an infix string

  Object* mth_toinfix (Evaluable* zobj, Nameset* nset, Cons* args) {
    // evaluate the arguments
    Vector* argv = Vector::eval (zobj, nset, args);
    long    argc = (argv == nullptr) ? 0 : argv->length ();
    try {
      if (argc != 1) {
	throw Exception ("argument-error", 
			 "too many arguments with to-infix");
      }
      Object* obj = argv->get (0);
      // check for a rvi
      Rvi* rvi = dynamic_cast <Rvi*> (obj);
      if (rvi != nullptr) {
	return new String (Infix::tostring (*rvi));
	delete argv; argv = nullptr;
      }
      throw Exception ("type-error", "invalid object with to-infix",
		       Object::repr (obj));
    } catch (...) {
      delete argv;
      throw;
    }
  }
}
