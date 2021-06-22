// ---------------------------------------------------------------------------
// - TlsInfos.cpp                                                            -
// - afnix:tls service - tls info list base class implementation             -
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
#include "TlsInfos.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"

namespace afnix {
  
  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // copy move this tls infos

  TlsInfos::TlsInfos (TlsInfos&& that) noexcept :
    Object (static_cast<Object&&>(that)) {
  }
  
  // move this tls infos into this one

  TlsInfos& TlsInfos::operator = (TlsInfos&& that) noexcept {
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
  static const long QUARK_RESET   = zone.intern ("reset");
  static const long QUARK_GETINFO = zone.intern ("get-info");

  // return true if the given quark is defined

  bool TlsInfos::isquark (const long quark, const bool hflg) const {
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

  Object* TlsInfos::apply (Evaluable* zobj, Nameset* nset, const long quark,
			  Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();
    
    // check for 0 argument
    if (argc == 0) {
      if (quark == QUARK_GETINFO) return new Plist (getinfo ());
      if (quark == QUARK_RESET) {
	reset ();
	return nullptr;
      }
    }
    // call the object method
    return Object::apply (zobj, nset, quark, argv);
  }
}
