// ---------------------------------------------------------------------------
// - Rni.cpp                                                                 -
// - afnix:mth module - real non linear interface implementation             -
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

#include "Rni.hpp"
#include "Vector.hpp"
#include "Evaluable.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"
 
namespace afnix {
  
  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a default rni

  Rni::Rni (void) {
  }
  
  // copy move this rni

  Rni::Rni (Rni&& that) noexcept :
    Object (static_cast<Object&&>(that)) {
  }
  
  // move this rni into this one

  Rni& Rni::operator = (Rni&& that) noexcept {
    Object::operator = (static_cast<Object&&>(that));
    return *this;
  }

  // -------------------------------------------------------------------------
  // - object section                                                        -
  // -------------------------------------------------------------------------

  // the quark zone
  static const long QUARK_ZONE_LENGTH = 2;
  static QuarkZone  zone (QUARK_ZONE_LENGTH);

  // the object supported quarks
  static const long QUARK_GETRHS = zone.intern ("getrhs");
  static const long QUARK_GETLHS = zone.intern ("getlhs");

  // return true if the given quark is defined

  bool Rni::isquark (const long quark, const bool hflg) const {
    rdlock ();
    try {
      if (zone.exists (quark) == true){
	unlock ();
	return true;
      }
      bool result = hflg ? Object::isquark (quark, hflg) : false;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // apply this object with a set of arguments and a quark
  
  Object* Rni::apply (Evaluable* zobj, Nameset* nset, const long quark,
                      Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();

    // dispatch 1 argument
    if (argc == 0) {
      if (quark == QUARK_GETLHS) {
	rdlock ();
	try {
	  // get the vector operand
	  Object* obj = argv->get (0);
	  Rvi*    vop = dynamic_cast <Rvi*> (obj);
	  if (vop == nullptr) {
	    throw Exception ("type-error", "invalid object for getlhs",
			     Object::repr (obj));
	  }
	  // get the lhs by vector operand
	  Rmi* result = getlhs (vop);
	  zobj->post (result);
	  unlock ();
	  return result;
	} catch (...) {
	  unlock ();
	  throw;
	}
      }
      if (quark == QUARK_GETRHS) {
	rdlock ();
	try {
	  // get the vector operand
	  Object* obj = argv->get (0);
	  Rvi*    vop = dynamic_cast <Rvi*> (obj);
	  if (vop == nullptr) {
	    throw Exception ("type-error", "invalid object for getlhs",
			     Object::repr (obj));
	  }
	  // get the rhs by vector operand
	  Rvi* result = getrhs (vop);
	  zobj->post (result);
	  unlock ();
	  return result;
	} catch (...) {
	  unlock ();
	  throw;
	}
      }
    }
    // call the object
    return Object::apply (zobj, nset, quark, argv);
  }
}

