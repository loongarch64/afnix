// ---------------------------------------------------------------------------
// - Literal.cpp                                                             -
// - standard object library - literal class implementation                  -
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
#include "Exception.hpp"
#include "QuarkZone.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // copy move this literal - no lock since abstract

  Literal::Literal (Literal&& that) noexcept :
    Serial (static_cast<Serial&&>(that)) {
  }
  
  // move this literal into this one - no lock since abstract

  Literal& Literal::operator = (Literal&& that) noexcept {
    Serial::operator = (static_cast<Serial&&>(that));
    return *this;
  }
  
  // get a style printable representation of the literal

  String Literal::format (const Style& lstl) const {
    rdlock ();
    try {
      String result = tostring ();
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // check if both literals are equal

  bool Literal::operator == (const Literal& lobj) const {
    rdlock ();
    try {
      String tstr = tostring ();
      String lstr = lobj.tostring ();
      bool result = (tstr == lstr);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // check if both literals are not equal

  bool Literal::operator != (const Literal& lobj) const {
    rdlock ();
    try {
      bool result = ! (*this == lobj);
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
  static const long QUARK_ZONE_LENGTH = 3;
  static QuarkZone  zone (QUARK_ZONE_LENGTH);

  // the object supported quarks
  static const long QUARK_CLEAR     = zone.intern ("clear");
  static const long QUARK_FORMAT    = zone.intern ("format");
  static const long QUARK_TOSTRING  = zone.intern ("to-string");
  static const long QUARK_TOLITERAL = zone.intern ("to-literal");

  // return true if the given quark is defined

  bool Literal::isquark (const long quark, const bool hflg) const {
    rdlock ();
    try {
      if (zone.exists (quark) == true) {
	unlock ();
	return true;
      }
      bool result = hflg ? Serial::isquark (quark, hflg) : false;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // apply this object with a set of arguments and a quark
  
  Object* Literal::apply (Evaluable* zobj, Nameset* nset, const long quark,
			  Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();
    
    // dispatch 0 argument
    if (argc == 0) {
      if (quark == QUARK_TOSTRING)  return new String (tostring  ());
      if (quark == QUARK_TOLITERAL) return new String (toliteral ());
      if (quark == QUARK_CLEAR) {
	clear ();
	return nullptr;
      }
    }
    // dispatch 1 argument
    if (argc == 1) {
      if (quark == QUARK_FORMAT) {
	Object* sobj = argv->get (0);
	Style*  lstl = dynamic_cast <Style*> (sobj);
	if (lstl == nullptr) {
	  throw Exception ("type-error", "invalid object for literal format",
			   Object::repr (sobj));
	}
	return new String (format (*lstl));
      }
    }
    // call the serial method
    return Serial::apply (zobj, nset, quark, argv);
  }
}
