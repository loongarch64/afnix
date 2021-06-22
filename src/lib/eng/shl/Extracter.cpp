// ---------------------------------------------------------------------------
// - Extracter.cpp                                                           -
// - afnix engine - form extracter class implementation                      -
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

#include "Extracter.hpp"
#include "Exception.hpp"
#include "InputStream.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a default extracter

  Extracter::Extracter (void) {
    p_is = nullptr;
  }

  // create a new extracter

  Extracter::Extracter (InputStream* is) {
    Object::iref (p_is = is);
  }

  // destroy this extracter

  Extracter::~Extracter (void) {
    Object::dref (p_is);
  }

  // return a string representation

  String Extracter::repr (void) const {
    return "Extracter";
  }

  // extract a new cons cell

  Form* Extracter::parse (void) {
    rdlock ();
    try {
      // check for eos condition
      if (p_is == nullptr) return nullptr;
      if (p_is->iseos () == true) return nullptr;
      // get a cons cell
      Object* sobj = Serial::deserialize (*p_is);
      Form*   form = dynamic_cast <Form*> (sobj);
      if (form == nullptr) {
	String what = Object::repr (sobj);
	Object::dref (sobj);
	throw Exception ("parse-error", "cannot read cons cell but got", what);
      }
      return form;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return a default line number

  long Extracter::getlnum (void) const {
    return 0;
  }
}
