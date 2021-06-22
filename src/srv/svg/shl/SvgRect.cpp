// ---------------------------------------------------------------------------
// - SvgRect.cpp                                                              -
// - afnix:svg service - svg rectangle node class implementation              -
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
#include "SvgRect.hpp"
#include "Evaluable.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - private section                                                       -
  // -------------------------------------------------------------------------

  // the svg node name
  static const String XML_TAG_NAME = "svg:rect";

  // the svg rectangle x position attribute
  static const String XML_RXP_ATTR = "x";
  // the svg rectangle y position attribute
  static const String XML_RYP_ATTR = "y";
  // the svg width attribute
  static const String XML_WTH_ATTR = "width";
  // the svg height attribute
  static const String XML_HGT_ATTR = "height";
  // the svg x rounding attribute
  static const String XML_RRX_ATTR = "rx";
  // the svg y rounding attribute
  static const String XML_RRY_ATTR = "ry";

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a rectangle by width and height

  SvgRect::SvgRect (const long width, const long height) : 
    SvgStyling (XML_TAG_NAME) {
    // set end flag
    d_eflg = true;
    // set the width and height
    setwth (width);
    sethgt (height);
  }

  // create a rectangle by width, height and position

  SvgRect::SvgRect (const long width, const long height, const long x,
		    const long y) : SvgStyling (XML_TAG_NAME) {
    // set end flag
    d_eflg = true;
    // set the width and height
    setwth (width);
    sethgt (height);
    // set the position
    setx (x);
    sety (y);
  }

  // return the class name

  String SvgRect::repr (void) const {
    return "SvgRect";
  }

  // set the rectangle x position attribute

  void SvgRect::setx (const long x) {
    // lock and set
    wrlock ();
    try {
      setattr (XML_RXP_ATTR, Utility::tostring (x));
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set the rectangle y position attribute

  void SvgRect::sety (const long y) {
    // lock and set
    wrlock ();
    try {
      setattr (XML_RYP_ATTR, Utility::tostring (y));
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set the rectangle width attribute

  void SvgRect::setwth (const long width) {
    // check for null value
    if (width == 0LL) return;
    // check for negative value
    if (width < 0LL) {
      throw Exception ("svg-error", "invalid negative svg width");
    }
    // lock and set
    wrlock ();
    try {
      setattr (XML_WTH_ATTR, Utility::tostring (width));
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set the rectangle width attribute

  void SvgRect::setwth (const String& wth) {
    // lock and set
    wrlock ();
    try {
      setattr (XML_WTH_ATTR, wth);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set the rectangle height attribute

  void SvgRect::sethgt (const long heigth) {
    // check for null value
    if (heigth == 0LL) return;
    // check for negative value
    if (heigth < 0LL) {
      throw Exception ("svg-error", "invalid negative svg height");
    }
    // lock and set
    wrlock ();
    try {
      setattr (XML_HGT_ATTR, Utility::tostring (heigth));
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set the rectangle height attribute

  void SvgRect::sethgt (const String& hgt) {
    // lock and set
    wrlock ();
    try {
      setattr (XML_HGT_ATTR, hgt);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set the rectangle x rounding attribute

  void SvgRect::setrx (const long rx) {
    // check for null value
    if (rx == 0LL) return;
    // check for negative value
    if (rx < 0LL) {
      throw Exception ("svg-error", "invalid negative svg rounding");
    }
    // lock and set
    wrlock ();
    try {
      setattr (XML_RRX_ATTR, Utility::tostring (rx));
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set the rectangle y rounding attribute

  void SvgRect::setry (const long ry) {
    // check for null value
    if (ry == 0LL) return;
    // check for negative value
    if (ry < 0LL) {
      throw Exception ("svg-error", "invalid negative svg rounding");
    }
    // lock and set
    wrlock ();
    try {
      setattr (XML_RRY_ATTR, Utility::tostring (ry));
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
  static const long QUARK_SETRXP = zone.intern ("set-x");
  static const long QUARK_SETRYP = zone.intern ("set-y");
  static const long QUARK_SETWTH = zone.intern ("set-width");
  static const long QUARK_SETHGT = zone.intern ("set-height");
  static const long QUARK_SETRRX = zone.intern ("set-rounding-x");
  static const long QUARK_SETRRY = zone.intern ("set-rounding-y");

  // create a new object in a generic way

  Object* SvgRect::mknew (Vector* argv) {
    long argc = (argv == nullptr) ? 0 : argv->length ();
    // check for 2 arguments
    if (argc == 2) {
      long width  = argv->getlong (0);
      long height = argv->getlong (1);
      return new SvgRect (width, height);
    }
    // check for 4 arguments
    if (argc == 4) {
      long  width = argv->getlong (0);
      long height = argv->getlong (1);
      long      x = argv->getlong (2);
      long      y = argv->getlong (3);
      return new SvgRect (width, height, x, y);
    }
    // wrong arguments
    throw Exception ("argument-error", 
		     "too many arguments with svg rectangle constructor");
  }

  // return true if the given quark is defined

  bool SvgRect::isquark (const long quark, const bool hflg) const {
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

  Object* SvgRect::apply (Evaluable* zobj, Nameset* nset, const long quark,
			  Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();
    
    // dispatch 1 argument
    if (argc == 1) {
      if (quark == QUARK_SETRXP) {
	long x = argv->getlong (0);
	setx (x);
	return nullptr;
      }
      if (quark == QUARK_SETRYP) {
	long y = argv->getlong (0);
	sety (y);
	return nullptr;
      }
      if (quark == QUARK_SETWTH) {
	Object* obj = argv->get (0);
	// check for integer
	Integer* iobj = dynamic_cast <Integer*> (obj);
	if (iobj != nullptr) {
	  long width = iobj->tolong ();
	  setwth (width);
	  return nullptr;
	}
	// check for a string
	String* sobj = dynamic_cast <String*> (obj);
	if (sobj != nullptr) {
	  setwth (*sobj);
	  return nullptr;
	}
	throw Exception ("type-error", "invalid object with set-width",
			 Object::repr (obj));
      }
      if (quark == QUARK_SETHGT) {
	Object* obj = argv->get (0);
	// check for integer
	Integer* iobj = dynamic_cast <Integer*> (obj);
	if (iobj != nullptr) {
	  long height = iobj->tolong ();
	  sethgt (height);
	  return nullptr;
	}
	// check for a string
	String* sobj = dynamic_cast <String*> (obj);
	if (sobj != nullptr) {
	  sethgt (*sobj);
	  return nullptr;
	}
	throw Exception ("type-error", "invalid object with set-heigth",
			 Object::repr (obj));
      }
      if (quark == QUARK_SETRRX) {
	long rx = argv->getlong (0);
	setrx (rx);
	return nullptr;
      }
      if (quark == QUARK_SETRRY) {
	long ry = argv->getlong (0);
	setry (ry);
	return nullptr;
      }
    }
    // call the svg styling method
    return SvgStyling::apply (zobj, nset, quark, argv);
  }
}
