// ---------------------------------------------------------------------------
// - Number.cpp                                                              -
// - standard object library - number class implementation                   -
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
#include "Unicode.hpp"
#include "Boolean.hpp"
#include "Integer.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - public section                                                        -
  // -------------------------------------------------------------------------

  // check if a string is a decimal number

  bool Number::isdnum (const String& s) {
    long slen = s.length ();
    for (long k = 0L; k < slen; k++) {
      if (Unicode::isdigit (s[k]) == false) return false;
    }
    return true;
  }

  // check if a string is a hexadecimal number

  bool Number::ishnum (const String& s) {
    long slen = s.length ();
    for (long k = 0L; k < slen; k++) {
      if (Unicode::ishexa (s[k]) == false) return false;
    }
    return true;
  }
  
  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------
  
  // copy move this number - no lock since abstract

  Number::Number (Number&& that) noexcept :
    Literal (static_cast<Literal&&>(that)) {
  }
  
  // move this number into this one - no lock since abstract

  Number& Number::operator = (Number&& that) noexcept {
    Literal::operator = (static_cast<Literal&&>(that));
    return *this;
  }

  // return a hexadecimal representation of this number

  String Number::tohexa (void) const {
    throw Exception ("number-error", 
		     "cannot generate a hexadecimal representation", repr ());
  }

  // return a hexadecimal string of this number

  String Number::tohstr (void) const {
    throw Exception ("number-error", 
		     "cannot generate a hexadecimal string", repr ());
  }

  // return a formatted string in scientific notation

  String Number::format (const bool sflg) const {
    rdlock ();
    try {
      // create a style with a flag
      Style lstl; lstl.setsflg (sflg);
      // format the number
      String result = Literal::format (lstl);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return a formatted string of this number with a precision

  String Number::format (const long psiz) const {
    rdlock ();
    try {
      // create a style with a precision
      Style lstl; lstl.setpsiz (psiz);
      // format the number
      String result = Literal::format (lstl);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return a formatted string in scientifc notation with a precision

  String Number::format (const long psiz, const bool sflg) const {
    rdlock ();
    try {
      // create a style with a precision
      Style lstl; lstl.setsflg (sflg); lstl.setpsiz (psiz);
      // format the number
      String result = Literal::format (lstl);
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
  static const long QUARK_FORMAT = zone.intern ("format");
  static const long QUARK_TOHEXA = zone.intern ("to-hexa");
  static const long QUARK_TOHSTR = zone.intern ("to-hexa-string");

  // return true if the given quark is defined

  bool Number::isquark (const long quark, const bool hflg) const {
    rdlock ();
    try {
      if (zone.exists (quark) == true) {
	unlock ();
	return true;
      }
      bool result = hflg ? Literal::isquark (quark, hflg) : false;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // apply this object with a set of arguments and a quark

  Object* Number::apply (Evaluable* zobj, Nameset* nset, const long quark,
			 Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();
    
    // dispatch 0 argument
    if (argc == 0) {
      if (quark == QUARK_FORMAT) return new String (format (0L));
      if (quark == QUARK_TOHEXA) return new String (tohexa ());
      if (quark == QUARK_TOHSTR) return new String (tohstr ());
    }
    
    // dispatch 1 argument
    if (argc == 1) {
      if (quark == QUARK_FORMAT) {
	Object* obj = argv->get (0);
	// check for a boolean
	Boolean* bobj = dynamic_cast <Boolean*> (obj);
	if (bobj != nullptr) return new String (format (bobj->tobool()));
	// check for an integer
	Integer* iobj = dynamic_cast <Integer*> (obj);
	if (iobj != nullptr) return new String (format ((long) iobj->tolong ()));
	// invalid type
	throw Exception ("type-error", "invalid object with number format",
			 Object::repr (obj));
      }
    }
    // dispatch 2 arguments
    if (argc == 2) {
      if (quark == QUARK_FORMAT) {
	long psiz = argv->getlong (0);
	bool sflg = argv->getbool (1);
	return new String (format (psiz, sflg));
      }
    }
    // call the literal method
    return Literal::apply (zobj, nset, quark, argv);
  }
}
