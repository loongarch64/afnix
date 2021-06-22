// ---------------------------------------------------------------------------
// - Rfunction.cpp                                                           -
// - afnix:mth module - real function class implementation                   -
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

#include "Real.hpp"
#include "Vector.hpp"
#include "Rfunction.hpp"
#include "Evaluable.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"
 
namespace afnix {

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a default real function

  Rfunction::Rfunction (void) {
    p_fptr = nullptr;
    p_dptr = nullptr;
  }

  // create a real function by pointer

  Rfunction::Rfunction (t_rfi fptr) {
    p_fptr = fptr;
    p_dptr = nullptr;
  }

  // create a real function by pointers

  Rfunction::Rfunction (t_rfi fptr, t_rfi dptr) {
    p_fptr = fptr;
    p_dptr = dptr;
  }

  // copy construct this real function

  Rfunction::Rfunction (const Rfunction& that) {
    that.rdlock ();
    try {
      p_fptr = that.p_fptr;
      p_dptr = that.p_dptr;
      that.unlock ();
    } catch (...) {
      that.unlock ();
      throw;
    }
  }
  
  // return the class name
  
  String Rfunction::repr (void) const {
    return "Rfunction";
  }
  
  // return a clone of this object
  
  Object* Rfunction::clone (void) const {
    return new Rfunction (*this);
  }
  
  // assign a real function to this one

  Rfunction& Rfunction::operator = (const Rfunction& that) {
    // check for self assignation
    if (this == &that) return *this;
    // lock and assign
    wrlock ();
    that.rdlock ();
    try {
      p_fptr = that.p_fptr;
      p_dptr = that.p_dptr;
      unlock ();
      that.unlock ();
      return *this;
    } catch (...) {
      unlock ();
      that.unlock ();
      throw;
    }
  }

  // compute the function at a point

  t_real Rfunction::compute (const t_real x) const {
    rdlock ();
    try {
      t_real result = (p_fptr == nullptr) ? 0.0 : p_fptr (x);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // compute the derivate at a point

  t_real Rfunction::derivate (const t_real x) const {
    rdlock ();
    try {
      t_real result = (p_dptr == nullptr) ? Rfi::derivate (x) : p_dptr (x);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // -------------------------------------------------------------------------
  // - object section                                                        -
  // -------------------------------------------------------------------------

  // create a new object in a generic way
  
  Object* Rfunction::mknew (Vector* argv) {
    long argc = (argv == nullptr) ? 0 : argv->length ();
    
    // check for 0 argument
    if (argc == 0) return new Rfunction;
    // invalid arguments
    throw Exception ("argument-error", 
		     "invalid arguments with real function object");
  }
}
