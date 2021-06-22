// ---------------------------------------------------------------------------
// - Collectable.cpp                                                         -
// - standard object library - collectable class implementation              -
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
#include "Central.hpp"
#include "QuarkZone.hpp"
#include "Collectable.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a collectable object

  Collectable::Collectable (void) {
    p_cptr = Central::confine (this);
  }

  // copy move this collectable

  Collectable::Collectable (Collectable&& that) noexcept {
    that.wrlock ();
    try {
      // move base object
      Object::operator = (static_cast<Object&&>(that));
      // copy move locally
      p_cptr = that.p_cptr; that.p_cptr = nullptr;
      that.unlock ();
    } catch (...) {
      p_cptr = nullptr;
      that.unlock ();
    }
  }
  
  // destroy this object

  Collectable::~Collectable (void) {
    Central::untangle (this, p_cptr);
  }
  
  // move this collectable into this one

  Collectable& Collectable::operator = (Collectable&& that) noexcept {
    // check for self-assignation
    if (this == &that) return *this;
    // lock and move
    wrlock ();
    that.wrlock ();
    try {
      // move base object
      Object::operator = (static_cast<Object&&>(that));
      // move locally
      p_cptr = that.p_cptr; that.p_cptr = nullptr;
      unlock ();
      that.unlock ();
    } catch (...) {
      p_cptr = nullptr;
      unlock ();
      that.unlock ();
    }      
    return *this;
  }

  // -------------------------------------------------------------------------
  // - object section                                                        -
  // -------------------------------------------------------------------------

  // the quark zone
  static const long QUARK_ZONE_LENGTH = 1;
  static QuarkZone  zone (QUARK_ZONE_LENGTH);

  // the object supported quarks
  static const long QUARK_RELEASE = zone.intern ("release");

  // return true if the given quark is defined

  bool Collectable::isquark (const long quark, const bool hflg) const {
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
  
  Object* Collectable::apply (Evaluable* zobj, Nameset* nset, const long quark,
			      Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();

    // dispatch 0 argument
    if (argc == 0) {
      if (quark == QUARK_RELEASE) {
	release ();
	return nullptr;
      }
    }

    // call the object method
    return Object::apply (zobj, nset, quark, argv);
  }
}
