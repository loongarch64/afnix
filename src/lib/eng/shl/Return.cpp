// ---------------------------------------------------------------------------
// - Return.cpp                                                              -
// - afnix engine - return exception class implementation                    -
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

#include "Return.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a default return object

  Return::Return (void) {
    p_object = nullptr;
  }

  // create a return object with an object

  Return::Return (Object* object) {
    p_object = Object::iref (object);
  }

  // copy constructor for this returned object
  
  Return::Return (const Return& that) {
    that.rdlock ();
    try {
      p_object = Object::iref (that.p_object);
      that.unlock ();
    } catch (...) {
      that.unlock ();
      throw;
    }
  }

  // destroy this return object

  Return::~Return (void) {
    Object::dref (p_object);
  }

  // return the class name

  String Return::repr (void) const {
    return "Return";
  }

  // return the returned object
    
  Object* Return::getobj (void) const {
    rdlock ();
    try {
      Object* result = p_object;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
}
