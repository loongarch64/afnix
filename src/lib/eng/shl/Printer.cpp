// ---------------------------------------------------------------------------
// - Printer.cpp                                                             -
// - afnix engine - printer class implementation                             -
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
#include "Printer.hpp"
#include "Loopable.hpp"
#include "Exception.hpp"
#include "OutputStream.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a default printer class

  Printer::Printer (void) {
    d_type = OUTPUTLN;
  }

  // create a new printer class

  Printer::Printer (const t_type type) {
    d_type = type;
  }

  // return the class name

  String Printer::repr (void) const {
    return "Printer";
  }

  // -------------------------------------------------------------------------
  // - object section                                                        -
  // -------------------------------------------------------------------------

  // apply this object on the evaluable streams

  Object* Printer::apply (Evaluable* zobj, Nameset* nset, Cons* args) {
    rdlock ();
    try {
      // get the loopable object
      auto lobj = dynamic_cast<Loopable*>(zobj);
      // get the evaluable stream based on the printer type
      OutputStream* os = nullptr;
      if ((d_type == Printer::OUTPUT) || (d_type == Printer::OUTPUTLN))
	os  = (lobj == nullptr) ? nullptr : lobj->getos ();
      if ((d_type == Printer::ERROR) || (d_type == Printer::ERRORLN)) 
	os  = (lobj == nullptr) ? nullptr : lobj->getes ();
      // loop for each argument
      String result;
      while (args != nullptr) {
	Object* car = args->getcar ();
	try {
	  Object* obj = (car == nullptr) ? nullptr : car->eval (zobj, nset);
	  if (obj == nullptr)
	    result = result + "nil";
	  else {
	    Literal* lit = dynamic_cast <Literal*> (obj);
	    if (lit == nullptr) {
	      String repr = Object::repr (obj);
	      Object::cref (obj);
	      throw Exception ("type-error", "non literal object for printer",
			       repr);
	    }
	    result += lit->tostring ();
	    Object::cref (obj);
	  }
	} catch (Exception& e) {
	  e.setnlf (true);
	  throw e;
	}
	args = args->getcdr ();
      }
      // check for newline
      if ((d_type == Printer::OUTPUTLN) || (d_type == Printer::ERRORLN)) {
	result = result + eolc;
      }
      if (os != nullptr) os->write (result);
      unlock ();
      return nullptr;
    } catch (...) {
      unlock ();
      throw;
    }
  }
}
