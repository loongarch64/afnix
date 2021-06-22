// ---------------------------------------------------------------------------
// - Iform.cpp                                                               -
// - afnix cross debugger - instrumented form class implementation           -
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

#include "Iform.hpp"
#include "Breakable.hpp"
#include "Exception.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a null iform

  Iform::Iform (void) {
    d_brkp = false;
  }
  
  // create a form by form

  Iform::Iform (Form* form) {
    if (form != nullptr) Form::operator = (*form);
    d_brkp = false;
  }
  
  // return the class name
  
  String Iform::repr (void) const {
    return "Iform";
  }

  // set the form breakpoint

  void Iform::setbrkp (const bool brkp) {
    wrlock ();
    try {
      d_brkp = brkp;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // -------------------------------------------------------------------------
  // - object section                                                        -
  // -------------------------------------------------------------------------

  // evaluate this iform in the current nameset
  
  Object* Iform::eval (Evaluable* zobj, Nameset* nset) {
    // get the breakable
    auto bobj = dynamic_cast<Breakable*>(zobj);
    // check for breakpoint
    if ((bobj != nullptr) && (d_brkp == true)) bobj->bpoint (nset, this);
    // check for block form
    if ((bobj != nullptr) && (d_cctp == CCTP_BLOK)) {
      // synchronize the form
      if (p_mon != nullptr) p_mon->enter ();
      Object* result = nullptr;
      try {
	Cons* cons = this;
	while (cons != nullptr) {
	  Object::cref (result);
	  Object* car = cons->getcar ();
	  if (bobj->getnext () == true) {
	    bobj->setnext (false);
	    bobj->bpoint (nset, car);
	  }
	  result = (car == nullptr) ? nullptr : car->eval (zobj,nset);
	  cons   = cons->getcdr ();
	}
	if (p_mon != nullptr) p_mon->leave ();
	return result;
      } catch (...) {
	if (p_mon != nullptr) p_mon->leave ();
	throw;
      }
    }
    // default to form evaluation
    return Form::eval (zobj, nset);
  }
}
