// ---------------------------------------------------------------------------
// - SvgCircle.cpp                                                           -
// - afnix:svg service - svg circle node class implementation                -
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
#include "Integer.hpp"
#include "Evaluable.hpp"
#include "SvgCircle.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - private section                                                       -
  // -------------------------------------------------------------------------

  // the svg node name
  static const String XML_TAG_NAME = "svg:circle";

  // the svg circle x position attribute
  static const String XML_CXP_ATTR = "cx";
  // the svg circle y position attribute
  static const String XML_CYP_ATTR = "cy";
  // the svg circle radius attribute
  static const String XML_CRD_ATTR = "r";

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a circle by radius

  SvgCircle::SvgCircle (const long radius) : SvgStyling (XML_TAG_NAME) {
    // set end flag
    d_eflg = true;
    // set the radius
    setcr (radius);
  }

  // create a circle by radius and position

  SvgCircle::SvgCircle (const long radius, const long x,
			const long y) : SvgStyling (XML_TAG_NAME) {
    // set end flag
    d_eflg = true;
    // set the radius
    setcr (radius);
    // set the position
    setx (x);
    sety (y);
  }

  // return the class name

  String SvgCircle::repr (void) const {
    return "SvgCircle";
  }

  // set the circle x position attribute

  void SvgCircle::setx (const long x) {
    // lock and set
    wrlock ();
    try {
      setattr (XML_CXP_ATTR, Utility::tostring (x));
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set the circle y position attribute

  void SvgCircle::sety (const long y) {
    // lock and set
    wrlock ();
    try {
      setattr (XML_CYP_ATTR, Utility::tostring (y));
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set the circle radius attribute

  void SvgCircle::setcr (const long radius) {
    // check for negative value
    if (radius < 0LL) {
      throw Exception ("svg-error", "invalid negative svg radius");
    }
    // lock and set
    wrlock ();
    try {
      setattr (XML_CRD_ATTR, Utility::tostring (radius));
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
  static const long QUARK_SETCXP = zone.intern ("set-center-x");
  static const long QUARK_SETCYP = zone.intern ("set-center-y");
  static const long QUARK_SETCRD = zone.intern ("set-radius");

  // create a new object in a generic way

  Object* SvgCircle::mknew (Vector* argv) {
    long argc = (argv == nullptr) ? 0 : argv->length ();
    // check for 1 argument
    if (argc == 1) {
      long radius = argv->getlong (0);
      return new SvgCircle (radius);
    }
    // check for 3 arguments
    if (argc == 3) {
      long radius = argv->getlong (0);
      long      x = argv->getlong (1);
      long      y = argv->getlong (2);
      return new SvgCircle (radius, x, y);
    }
    // wrong arguments
    throw Exception ("argument-error", 
		     "too many arguments with svg circle constructor");
  }

  // return true if the given quark is defined

  bool SvgCircle::isquark (const long quark, const bool hflg) const {
    rdlock ();
    if (zone.exists (quark) == true) {
      unlock ();
      return true;
    }
    bool result = hflg ? SvgStyling::isquark (quark, hflg) : false;
    unlock ();
    return result;
  }

  // apply this object with a set of arguments and a quark

  Object* SvgCircle::apply (Evaluable* zobj, Nameset* nset, const long quark,
			  Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();
    
    // dispatch 1 argument
    if (argc == 1) {
      if (quark == QUARK_SETCXP) {
	long x = argv->getlong (0);
	setx (x);
	return nullptr;
      }
      if (quark == QUARK_SETCYP) {
	long y = argv->getlong (0);
	sety (y);
	return nullptr;
      }
      if (quark == QUARK_SETCRD) {
	long radius = argv->getlong (0);
	setcr (radius);
	return nullptr;
      }
    }
    // call the svg styling method
    return SvgStyling::apply (zobj, nset, quark, argv);
  }
}
