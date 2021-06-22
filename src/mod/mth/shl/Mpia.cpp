// ---------------------------------------------------------------------------
// - Mpia.cpp                                                                -
// - afnix:mth module - multi-precision integer arithmetic implementation    -
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
#include "Vector.hpp"
#include "Utility.hpp"
#include "Integer.hpp"
#include "Relatif.hpp"
#include "Exception.hpp"
 
namespace afnix {

  // compute the gcd of two relatifs

  Object* mth_gcd (Evaluable* zobj, Nameset* nset, Cons* args) {
    // get the arguments
    Vector* argv = Vector::eval (zobj, nset, args);
    long    argc = (argv == nullptr) ? 0 : argv->length ();
    try {
      // check for 2 arguments
      if (argc != 2) {
	throw Exception ("argument-error", "too many arguments with gcd");
      }
      // get x argument
      Object*  xobj = argv->get (0);
      Relatif* xrel = dynamic_cast <Relatif*> (xobj);
      if (xrel == nullptr) {
	throw Exception ("type-error", "invalid object with gcd",
			 Object::repr (xobj));
      }
      // get y argument
      Object*  yobj = argv->get (1);
      Relatif* yrel = dynamic_cast <Relatif*> (yobj);
      if (yrel == nullptr) {
	throw Exception ("type-error", "invalid object with gcd",
			 Object::repr (yobj));
      }
      // compute result
      Relatif* result = new Relatif (Relatif::gcd (*xrel, *yrel));
      delete argv;
      return result;
    } catch (...) {
      delete argv;
      throw;
    }
  }

  // compute the lcm of two relatifs

  Object* mth_lcm (Evaluable* zobj, Nameset* nset, Cons* args) {
    // get the arguments
    Vector* argv = Vector::eval (zobj, nset, args);
    long    argc = (argv == nullptr) ? 0 : argv->length ();
    try {
      // check for 2 arguments
      if (argc != 2) {
	throw Exception ("argument-error", "too many arguments with lcm");
      }
      // get x argument
      Object*  xobj = argv->get (0);
      Relatif* xrel = dynamic_cast <Relatif*> (xobj);
      if (xrel == nullptr) {
	throw Exception ("type-error", "invalid object with lcm",
			 Object::repr (xobj));
      }
      // get y argument
      Object*  yobj = argv->get (1);
      Relatif* yrel = dynamic_cast <Relatif*> (yobj);
      if (yrel == nullptr) {
	throw Exception ("type-error", "invalid object with lcm",
			 Object::repr (yobj));
      }
      // compute result
      Relatif* result = new Relatif (Relatif::lcm (*xrel, *yrel));
      delete argv;
      return result;
    } catch (...) {
      delete argv;
      throw;
    }
  }

  // compute the mmi of a relatif

  Object* mth_mmi (Evaluable* zobj, Nameset* nset, Cons* args) {
    // get the arguments
    Vector* argv = Vector::eval (zobj, nset, args);
    long    argc = (argv == nullptr) ? 0 : argv->length ();
    try {
      // check for 2 arguments
      if (argc != 2) {
	throw Exception ("argument-error", "too many arguments with mmi");
      }
      // get x argument
      Object*  xobj = argv->get (0);
      Relatif* xrel = dynamic_cast <Relatif*> (xobj);
      if (xrel == nullptr) {
	throw Exception ("type-error", "invalid object with mmi",
			 Object::repr (xobj));
      }
      // get y argument
      Object*  yobj = argv->get (1);
      Relatif* yrel = dynamic_cast <Relatif*> (yobj);
      if (yrel == nullptr) {
	throw Exception ("type-error", "invalid object with mmi",
			 Object::repr (yobj));
      }
      // compute result
      Relatif* result = new Relatif (Relatif::mmi (*xrel, *yrel));
      delete argv;
      return result;
    } catch (...) {
      delete argv;
      throw;
    }
  }
}
