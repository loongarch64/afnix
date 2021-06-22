// ---------------------------------------------------------------------------
// - Signable.cpp                                                            -
// - afnix:sec module - abstract signable class implementation               -
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
#include "Signable.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // copy move this signable

  Signable::Signable (Signable&& that) noexcept :
    Object (static_cast<Object&&>(that)) {
  }
  
  // move this signable into this one

  Signable& Signable::operator = (Signable&& that) noexcept {
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
  static const long QUARK_SIGN   = zone.intern ("sign");
  static const long QUARK_VERIFY = zone.intern ("verify");

  // return true if the given quark is defined

  bool Signable::isquark (const long quark, const bool hflg) const {
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

  Object* Signable::apply (Evaluable* zobj, Nameset* nset, const long quark,
                           Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();
    
    // dispatch 0 argument
    if (argc == 0) {
      if (quark == QUARK_VERIFY) return new Boolean (verify ());
    }

    // dispatch 1 argument
    if (argc == 1) {
      if (quark == QUARK_SIGN) {
	Object* obj = argv->get (0);
	auto skey = dynamic_cast<Key*>(obj);
	if (skey == nullptr) {
	  throw Exception ("type-error", "invalid object as signing key",
			   Object::repr (obj));
	}
	return new Boolean (sign (*skey));
      }
    }
    // call the object method
    return Object::apply (zobj, nset, quark, argv);
  }
}