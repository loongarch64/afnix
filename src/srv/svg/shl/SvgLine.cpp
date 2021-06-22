// ---------------------------------------------------------------------------
// - SvgLine.cpp                                                             -
// - afnix:svg service - svg line node class implementation                  -
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

#include "SvgLine.hpp"
#include "Utility.hpp"
#include "Integer.hpp"
#include "Evaluable.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - private section                                                       -
  // -------------------------------------------------------------------------

  // the svg node name
  static const String XML_TAG_NAME = "svg:line";

  // the svg line x position 1 attribute
  static const String XML_LX1_ATTR = "x1";
  // the svg line y position 1 attribute
  static const String XML_LY1_ATTR = "y1";
  // the svg line x position 2 attribute
  static const String XML_LX2_ATTR = "x2";
  // the svg line y position 2 attribute
  static const String XML_LY2_ATTR = "y2";

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a line by points

  SvgLine::SvgLine (const Ippd& p1, 
		    const Ippd& p2) : SvgStyling (XML_TAG_NAME) {
    // set end flag
    d_eflg = true;
    // set the first point
    setp1 (p1);
    // set the second point
    setp2 (p2);
  }

  // create a line by positions

  SvgLine::SvgLine (const long x1, const long y1, 
		    const long x2, const long y2) : SvgStyling (XML_TAG_NAME) {
    // set end flag
    d_eflg = true;
    // set the first point
    setp1 (x1, y1);
    // set the second point
    setp2 (x2, y2);
  }

  // return the class name

  String SvgLine::repr (void) const {
    return "SvgLine";
  }

  // set the line first point

  void SvgLine::setp1 (const Ippd& p) {
    // lock and set
    wrlock ();
    try {
      setattr (XML_LX1_ATTR, Utility::tostring (p.getx ()));
      setattr (XML_LY1_ATTR, Utility::tostring (p.gety ()));
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set the line first point

  void SvgLine::setp1 (const long x, const long y) {
    // lock and set
    wrlock ();
    try {
      setattr (XML_LX1_ATTR, Utility::tostring (x));
      setattr (XML_LY1_ATTR, Utility::tostring (y));
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set the line second point

  void SvgLine::setp2 (const Ippd& p) {
    // lock and set
    wrlock ();
    try {
      setattr (XML_LX2_ATTR, Utility::tostring (p.getx ()));
      setattr (XML_LY2_ATTR, Utility::tostring (p.gety ()));
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set the line second point

  void SvgLine::setp2 (const long x, const long y) {
    // lock and set
    wrlock ();
    try {
      setattr (XML_LX2_ATTR, Utility::tostring (x));
      setattr (XML_LY2_ATTR, Utility::tostring (y));
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
  static const long QUARK_ZONE_LENGTH = 2;
  static QuarkZone  zone (QUARK_ZONE_LENGTH);

  // the object supported quarks
  static const long QUARK_SETLP1 = zone.intern ("set-point-1");
  static const long QUARK_SETLP2 = zone.intern ("set-point-2");

  // create a new object in a generic way

  Object* SvgLine::mknew (Vector* argv) {
    long argc = (argv == nullptr) ? 0 : argv->length ();
    // check for 2 argument2
    if (argc == 2) {
      Object* obj = argv->get (0);
      // check for point 1
      Ippd* p1 = dynamic_cast <Ippd*> (obj);
      if (p1 == nullptr) {
	throw Exception ("type-error", "invalid integer point object in line",
			 Object::repr (obj));
      }
      // check for point 2
      obj = argv->get (1);
      Ippd* p2 = dynamic_cast <Ippd*> (obj);
      if (p2 == nullptr) {
	throw Exception ("type-error", "invalid integer point object in line",
			 Object::repr (obj));
      }
      return new SvgLine (*p1, *p2);
    }
    // check for 4 arguments
    if (argc == 4) {
      long x1 = argv->getlong (0);
      long y1 = argv->getlong (1);
      long x2 = argv->getlong (2);
      long y2 = argv->getlong (3);
      return new SvgLine (x1, y1, x2, y2);
    }
    // wrong arguments
    throw Exception ("argument-error", 
		     "too many arguments with svg line constructor");
  }
  
  // return true if the given quark is defined
  
  bool SvgLine::isquark (const long quark, const bool hflg) const {
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

  Object* SvgLine::apply (Evaluable* zobj, Nameset* nset, const long quark,
			  Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();
    
    // dispatch 1 argument
    if (argc == 1) {
      if (quark == QUARK_SETLP1) {
	Object* obj = argv->get (0);
	Ippd* p = dynamic_cast <Ippd*> (obj);
	if (p == nullptr) {
	  throw Exception ("type-error", "invalid integer point object in line",
			   Object::repr (obj));
	}
	setp1 (*p);
	return nullptr;
      }
      if (quark == QUARK_SETLP2) {
	Object* obj = argv->get (0);
	Ippd* p = dynamic_cast <Ippd*> (obj);
	if (p == nullptr) {
	  throw Exception ("type-error", "invalid integer point object in line",
			   Object::repr (obj));
	}
	setp2 (*p);
	return nullptr;
      }
    }

    // dispatch 2 arguments
    if (argc == 2) {
      if (quark == QUARK_SETLP1) {
	long x = argv->getlong (0);
	long y = argv->getlong (1);
	setp1 (x, y);
	return nullptr;
      }
      if (quark == QUARK_SETLP2) {
	long x = argv->getlong (0);
	long y = argv->getlong (1);
	setp2 (x, y);
	return nullptr;
      }
    }
    // call the svg styling method
    return SvgStyling::apply (zobj, nset, quark, argv);
  }
}
