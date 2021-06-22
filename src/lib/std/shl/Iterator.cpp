// ---------------------------------------------------------------------------
// - Iterator.cpp                                                            -
// - standard object library - iterator abstract class implementation        -
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
#include "Iterator.hpp"
#include "Evaluable.hpp"
#include "QuarkZone.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // copy move this iterator - no lock since abstract

  Iterator::Iterator (Iterator&& that) noexcept :
    Object (static_cast<Object&&>(that)) {
  }
  
  // move this iterator into this one - no lock since abstract

  Iterator& Iterator::operator = (Iterator&& that) noexcept {
    Object::operator = (static_cast<Object&&>(that));
    return *this;
  }
  
  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  bool Iterator::valid (void) const {
    rdlock ();
    bool result = (isend () == true) ? false : true;
    unlock ();
    return result;
  }

  // -------------------------------------------------------------------------
  // - object section                                                        -
  // -------------------------------------------------------------------------

  // the quark zone
  static const long QUARK_ZONE_LENGTH = 7;
  static QuarkZone  zone (QUARK_ZONE_LENGTH);

  // the object supported quarks
  static const long QUARK_END      = zone.intern ("end");
  static const long QUARK_ENDP     = zone.intern ("end-p");
  static const long QUARK_NEXT     = zone.intern ("next");
  static const long QUARK_BEGIN    = zone.intern ("begin");
  static const long QUARK_VALIDP   = zone.intern ("valid-p");
  static const long QUARK_GETOBJ   = zone.intern ("get-object");
  static const long QUARK_PREVIOUS = zone.intern ("previous");

  // return true if the given quark is defined

  bool Iterator::isquark (const long quark, const bool hflg) const {
    rdlock ();
    if (zone.exists (quark) == true) {
      unlock ();
      return true;
    }
    bool result = hflg ? Object::isquark (quark, hflg) : false;
    unlock ();
    return result;
  }

  // apply this object with a set of arguments and a quark

  Object* Iterator::apply (Evaluable* zobj, Nameset* nset, const long quark,
			   Vector* argv) {
    
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();
    
    // dispatch 0 argument
    if (argc == 0) {
      if (quark == QUARK_VALIDP) return new Boolean (valid ());
      if (quark == QUARK_ENDP)   return new Boolean (isend ());
      if (quark == QUARK_GETOBJ) {
	rdlock ();
	Object* result = getobj ();
	zobj->post (result);
	unlock ();
	return result;
      }
      if (quark == QUARK_BEGIN) {
	begin ();
	return nullptr;
      }
      if (quark == QUARK_END) {
	end ();
	return nullptr;
      }
      if (quark == QUARK_NEXT) {
	next ();
	return nullptr;
      }
      if (quark == QUARK_PREVIOUS) {
	prev ();
	return nullptr;
      }
    }
    // call the object method
    return Object::apply (zobj, nset, quark, argv);
  }
}
