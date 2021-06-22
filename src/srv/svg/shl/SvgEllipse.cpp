// ---------------------------------------------------------------------------
// - SvgEllipse.cpp                                                          -
// - afnix:svg service - svg ellipse node class implementation               -
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
#include "QuarkZone.hpp"
#include "Exception.hpp"
#include "SvgEllipse.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - private section                                                       -
  // -------------------------------------------------------------------------

  // the svg node name
  static const String XML_TAG_NAME = "svg:ellipse";

  // the svg ellipse x position attribute
  static const String XML_CXP_ATTR = "cx";
  // the svg ellipse y position attribute
  static const String XML_CYP_ATTR = "cy";
  // the svg ellipse x radius attribute
  static const String XML_CRX_ATTR = "rx";
  // the svg ellipse y radius attribute
  static const String XML_CRY_ATTR = "ry";

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a ellipse by radius

  SvgEllipse::SvgEllipse (const long rx, const long ry) : 
    SvgStyling (XML_TAG_NAME) {
    // set end flag
    d_eflg = true;
    // set the radius
    seter (rx, ry);
  }

  // create a ellipse by radius and position

  SvgEllipse::SvgEllipse (const long rx, const long ry, const long cx,
			  const long cy) : SvgStyling (XML_TAG_NAME) {
    // set end flag
    d_eflg = true;
    // set the radius
    seter (rx, ry);
    // set the position
    setec (cx, cy);
  }

  // return the class name

  String SvgEllipse::repr (void) const {
    return "SvgEllipse";
  }

  // set the ellipse x position attribute

  void SvgEllipse::setcx (const long cx) {
    // lock and set
    wrlock ();
    try {
      setattr (XML_CXP_ATTR, Utility::tostring (cx));
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set the ellipse cy position attribute

  void SvgEllipse::setcy (const long cy) {
    // lock and set
    wrlock ();
    try {
      setattr (XML_CYP_ATTR, Utility::tostring (cy));
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set the ellipse center at once

  void SvgEllipse::setec (const long cx, const long cy) {
    // lock and set
    wrlock ();
    try {
      setcx (cx);
      setcy (cy);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set the ellipse radius x attribute

  void SvgEllipse::setrx (const long rx) {
    // check for negative value
    if (rx < 0LL) {
      throw Exception ("svg-error", "invalid negative svg radius");
    }
    // lock and set
    wrlock ();
    try {
      setattr (XML_CRX_ATTR, Utility::tostring (rx));
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set the ellipse radius y attribute

  void SvgEllipse::setry (const long ry) {
    // check for negative value
    if (ry < 0LL) {
      throw Exception ("svg-error", "invalid negative svg radius");
    }
    // lock and set
    wrlock ();
    try {
      setattr (XML_CRY_ATTR, Utility::tostring (ry));
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set the ellipse radius attribute at once

  void SvgEllipse::seter (const long rx, const long ry) {
    // lock and set
    wrlock ();
    try {
      setrx (rx);
      setry (ry);
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
  static const long QUARK_ZONE_LENGTH = 6;
  static QuarkZone  zone (QUARK_ZONE_LENGTH);

  // the object supported quarks
  static const long QUARK_SETCXP = zone.intern ("set-x-center");
  static const long QUARK_SETCYP = zone.intern ("set-y-center");
  static const long QUARK_SETECP = zone.intern ("set-center");
  static const long QUARK_SETCRX = zone.intern ("set-radius-x");
  static const long QUARK_SETCRY = zone.intern ("set-radius-x");
  static const long QUARK_SETCRD = zone.intern ("set-radius");

  // create a new object in a generic way

  Object* SvgEllipse::mknew (Vector* argv) {
    long argc = (argv == nullptr) ? 0 : argv->length ();
    // check for 2 arguments
    if (argc == 2) {
      long rx = argv->getlong (0);
      long ry = argv->getlong (1);
      return new SvgEllipse (rx, ry);
    }
    // check for 4 arguments
    if (argc == 4) {
      long rx = argv->getlong (0);
      long ry = argv->getlong (1);
      long cx = argv->getlong (2);
      long cy = argv->getlong (3);
      return new SvgEllipse (rx, ry, cx, cy);
    }
    // wrong arguments
    throw Exception ("argument-error", 
		     "too many arguments with svg ellipse constructor");
  }

  // return true if the given quark is defined

  bool SvgEllipse::isquark (const long quark, const bool hflg) const {
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

  Object* SvgEllipse::apply (Evaluable* zobj, Nameset* nset, const long quark,
			     Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();
    
    // dispatch 1 argument
    if (argc == 1) {
      if (quark == QUARK_SETCXP) {
	long cx = argv->getlong (0);
	setcx (cx);
	return nullptr;
      }
      if (quark == QUARK_SETCYP) {
	long cy = argv->getlong (0);
	setcy (cy);
	return nullptr;
      }
      if (quark == QUARK_SETCRX) {
	long rx = argv->getlong (0);
	setrx (rx);
	return nullptr;
      }
      if (quark == QUARK_SETCRY) {
	long ry = argv->getlong (0);
	setry (ry);
	return nullptr;
      }
    }
    // dispatch 2 arguments
    if (argc == 2) {
      if (quark == QUARK_SETECP) {
	long cx = argv->getlong (0);
	long cy = argv->getlong (1);
	setec (cx, cy);
	return nullptr;
      }
      if (quark == QUARK_SETCRD) {
	long rx = argv->getlong (0);
	long ry = argv->getlong (1);
	seter (rx, ry);
	return nullptr;
      }
    }
    // call the svg styling method
    return SvgStyling::apply (zobj, nset, quark, argv);
  }
}
