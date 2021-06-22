// ---------------------------------------------------------------------------
// - Former.cpp                                                              -
// - afnix engine - former class implementation                              -
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
#include "Former.hpp"
#include "Integer.hpp"
#include "QuarkZone.hpp"

namespace afnix {
  
  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a default former

  Former::Former (void) {
  }
  
  // copy move this former

  Former::Former (Former&& that) noexcept :
    Object (static_cast<Object&&>(that)) {
  }
  
  // move this former into this one

  Former& Former::operator = (Former&& that) noexcept {
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
  static const long QUARK_PARSE   = zone.intern ("parse");
  static const long QUARK_GETLNUM = zone.intern ("get-line-number");

  // return true if the given quark is defined

  bool Former::isquark (const long quark, const bool hflg) const {
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

  Object* Former::apply (Evaluable* zobj, Nameset* nset, const long quark,
			 Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();
    
    // dispatch 0 argument
    if (argc == 0) {
      if (quark == QUARK_PARSE)   return parse ();
      if (quark == QUARK_GETLNUM) return new Integer (getlnum ());
    }
    // call the object method
    return Object::apply (zobj, nset, quark, argv);
  }
}
