// ---------------------------------------------------------------------------
// - SvgStyling.cpp                                                          -
// - afnix:svg service - svg styling node class implementation               -
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

#include "Utility.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"
#include "SvgStyling.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - private section                                                       -
  // -------------------------------------------------------------------------

  // the svg fill attribute
  static const String XML_FIL_ATTR = "fill";
  // the svg stroke attribute
  static const String XML_STK_ATTR = "stroke";
  // the svg stroke width attribute
  static const String XML_SWT_ATTR = "stroke-width";

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a svg graphic by name

  SvgStyling::SvgStyling (const String& name) : SvgForming (name) {
  }

  // set the svg fill attribute

  void SvgStyling::setfill (const String& color) {
    // lock and set
    wrlock ();
    try {
      setattr (XML_FIL_ATTR, color);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set the svg stroke attribute
  
  void SvgStyling::setstrk (const String& color) {
    // lock and set
    wrlock ();
    try {
      setattr (XML_STK_ATTR, color);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set the svg stroke width attribute
  
  void SvgStyling::setswth (const long width) {
    // lock and set
    wrlock ();
    try {
      setattr (XML_SWT_ATTR, Utility::tostring (width));
      unlock ();
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
  static const long QUARK_SETFILL = zone.intern ("set-fill");
  static const long QUARK_SETSTRK = zone.intern ("set-stroke");
  static const long QUARK_SETSWTH = zone.intern ("set-stroke-width");

  // return true if the given quark is defined

  bool SvgStyling::isquark (const long quark, const bool hflg) const {
    rdlock ();
    if (zone.exists (quark) == true) {
      unlock ();
      return true;
    }
    bool result = hflg ? SvgForming::isquark (quark, hflg) : false;
    unlock ();
    return result;
  }

  // apply this object with a set of arguments and a quark

  Object* SvgStyling::apply (Evaluable* zobj, Nameset* nset, const long quark,
			     Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();
    
    // dispatch 1 argument
    if (argc == 1) {
      if (quark == QUARK_SETFILL) {
	String color = argv->getstring (0);
	setfill (color);
	return nullptr;
      }
      if (quark == QUARK_SETSTRK) {
	String color = argv->getstring (0);
	setstrk (color);
	return nullptr;
      }
      if (quark == QUARK_SETSWTH) {
	long width = argv->getlong (0);
	setswth (width);
	return nullptr;
      }
    }
    // call the svg forming method
    return SvgForming::apply (zobj, nset, quark, argv);
  }
}
