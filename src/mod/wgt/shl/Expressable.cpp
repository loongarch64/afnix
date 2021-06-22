// ---------------------------------------------------------------------------
// - Expressable.cpp                                                         -
// - afnix:wgt module - expressable abstract class implementation            -
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
#include "Boolean.hpp"
#include "Evaluable.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"
#include "Expressable.hpp"

namespace afnix {
  
  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // copy move this expressable

  Expressable::Expressable (Expressable&& that) noexcept :
    Object (static_cast<Object&&>(that)) {
  }
  
  // move this expressable into this one

  Expressable& Expressable::operator = (Expressable&& that) noexcept {
    Object::operator = (static_cast<Object&&>(that));
    return *this;
  }

  // valuate a conditional
  
  bool Expressable::valuate (const Conditional& cond) const {
    rdlock ();
    try {
      // check for activate
      if (cond.getactv () == false) {
	unlock ();
	return false;
      }
      // collect conditional data
      String   oper = cond.getoper ();
      Literal* llhs = cond.getllhs ();
      Literal* lrhs = cond.getlrhs ();
      // check conistency
      if (llhs == nullptr) {
	throw Exception ("conditional-error", "invalid null lhs in valuate");
      }
      bool result = (lrhs == nullptr) ?
	valuate (oper, *llhs) : valuate (oper, *llhs, *lrhs);
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

  // the quark zone
  static const long QUARK_ZONE_LENGTH = 1;
  static QuarkZone zone (QUARK_ZONE_LENGTH);

  // the object supported quarks
  static const long QUARK_VALUATE = zone.intern ("valuate");

  // return true if the given quark is defined

  bool Expressable::isquark (const long quark, const bool hflg) const {
    rdlock ();
    try {
      if (zone.exists (quark) == true) {
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

  Object* Expressable::apply (Evaluable* zobj, Nameset* nset, const long quark,
			      Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();

    // check for 1 argument
    if (argc == 1) {
      if (quark == QUARK_VALUATE) {
	Object* obj = argv->get(0);
	auto   cond = dynamic_cast<Conditional*>(obj);
	if (cond == nullptr) {
	  throw Exception ("type-error", "invalid object as conditional",
			   Object::repr (obj));
	}
	return new Boolean (valuate (*cond));
      }
    }
    // check for 2 arguments
    if (argc == 2) {
      if (quark == QUARK_VALUATE) {
	String oper = argv->getstring (0);
	Object* obj = argv->get(1);
	auto   llhs = dynamic_cast<Literal*>(obj);
	if (llhs == nullptr) {
	  throw Exception ("type-error", "invalid object as lhs valuator",
			   Object::repr (obj));
	}
	return new Boolean (valuate (oper, *llhs));
      }
    }

    // check for 3 arguments
    if (argc == 3) {
      if (quark == QUARK_VALUATE) {
	String oper = argv->getstring (0);
	Object* obj = argv->get(1);
	auto   llhs = dynamic_cast<Literal*>(obj);
	if (llhs == nullptr) {
	  throw Exception ("type-error", "invalid object as lhs valuator",
			   Object::repr (obj));
	}
	obj = argv->get(2);
	auto lrhs = dynamic_cast<Literal*>(obj);
	if (lrhs == nullptr) {
	  throw Exception ("type-error", "invalid object as rhs valuator",
			   Object::repr (obj));
	}
	return new Boolean (valuate (oper, *llhs, *lrhs));
      }
    }
    // call the object method
    return Object::apply (zobj, nset, quark, argv);
  }
}
