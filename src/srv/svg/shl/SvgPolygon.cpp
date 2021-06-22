// ---------------------------------------------------------------------------
// - SvgPolygon.cpp                                                          -
// - afnix:svg service - svg polygon node class implementation               -
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

#include "Ippd.hpp"
#include "Utility.hpp"
#include "Integer.hpp"
#include "Evaluable.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"
#include "SvgPolygon.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - private section                                                       -
  // -------------------------------------------------------------------------

  // the svg node name
  static const String XML_TAG_NAME = "svg:polygon";

  // the svg polygon points
  static const String XML_PTS_ATTR = "points";

  // this function converts an integer plane point into a string
  static String svg_ipptos (const Ippd& p) {
    String result = Utility::tostring (p.getx ());
    result += ',';
    result += Utility::tostring (p.gety ());
    return result;
  }

  // this function converts a point vector into a string
  static String svg_ipvtos (const Vector& pv) {
    String result;
    long   length = pv.length ();
    if (length == 0) return result;
    // loop in the vector
    for (long i = 0; i < length; i++) {
      Object*  obj = pv.get (i);
      Ippd*   p = dynamic_cast <Ippd*> (obj);
      if (p == nullptr) {
	throw Exception ("type-error", "invalid object as integer point",
			 Object::repr (obj));
      }
      if (result.isnil () == false) result += ' ';
      result += svg_ipptos (*p);
    }
    return result;
  }

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a polygon by vector by points

  SvgPolygon::SvgPolygon (const Vector& pv) : SvgStyling (XML_TAG_NAME) {
    // set end flag
    d_eflg = true;
    // set the vector points
    setpv (pv);
  }

  // return the class name

  String SvgPolygon::repr (void) const {
    return "SvgPolygon";
  }

  // set the polygon vector

  void SvgPolygon::setpv (const Vector& pv) {
    // lock and set
    wrlock ();
    try {
      // get the polygon points string
      String pts = svg_ipvtos (pv);
      // set the points attribute
      if (pts.isnil () == false) setattr (XML_PTS_ATTR, pts);
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
  static const long QUARK_ZONE_LENGTH = 1;
  static QuarkZone  zone (QUARK_ZONE_LENGTH);
  
  // the object supported quarks
  static const long QUARK_SETPV = zone.intern ("set-point-vector");
  
  // create a new object in a generic way
  
  Object* SvgPolygon::mknew (Vector* argv) {
    long argc = (argv == nullptr) ? 0 : argv->length ();
    // check for 1 argument
    if (argc == 1) {
      Object* obj = argv->get (0);
      // check for point vector
      Vector* pv = dynamic_cast <Vector*> (obj);
      if (pv == nullptr) {
	throw Exception ("type-error", "invalid object in polygon",
			 Object::repr (obj));
      }
      return new SvgPolygon (*pv);
    }
    // wrong arguments
    throw Exception ("argument-error", 
		     "too many arguments with svg polygon constructor");
  }
  
  // return true if the given quark is defined
  
  bool SvgPolygon::isquark (const long quark, const bool hflg) const {
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

  Object* SvgPolygon::apply (Evaluable* zobj, Nameset* nset, const long quark,
			     Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();
    
    // dispatch 1 argument
    if (argc == 1) {
      if (quark == QUARK_SETPV) {
	Object* obj = argv->get (0);
	// check for point vector
	Vector* pv = dynamic_cast <Vector*> (obj);
	if (pv == nullptr) {
	  throw Exception ("type-error", "invalid object in polygon set",
			   Object::repr (obj));
	}
	setpv (*pv);
	return nullptr;
      }
    }
    // call the svg styling method
    return SvgStyling::apply (zobj, nset, quark, argv);
  }
}
