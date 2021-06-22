// ---------------------------------------------------------------------------
// - SvgForming.cpp                                                          -
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

#include "QuarkZone.hpp"
#include "Exception.hpp"
#include "SvgForming.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - private section                                                       -
  // -------------------------------------------------------------------------

  // the svg transform attribute
  static const String XML_TRF_ATTR = "transform";

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a svg graphic by name

  SvgForming::SvgForming (const String& name) : XmlTag (name) {
  }

  // set the svg transform attribute

  void SvgForming::settrf (const String& ts) {
    // lock and set
    wrlock ();
    try {
      setattr (XML_TRF_ATTR, ts);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set the svg transform attribute

  void SvgForming::settrf (const Transform& to) {
    // lock and set
    wrlock ();
    try {
      if (to.isnil () == false) {
	setattr (XML_TRF_ATTR, to.tostring ());
      }
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
  static const long QUARK_SETTRF = zone.intern ("set-transform");

  // return true if the given quark is defined

  bool SvgForming::isquark (const long quark, const bool hflg) const {
    rdlock ();
    if (zone.exists (quark) == true) {
      unlock ();
      return true;
    }
    bool result = hflg ? XmlTag::isquark (quark, hflg) : false;
    unlock ();
    return result;
  }

  // apply this object with a set of arguments and a quark

  Object* SvgForming::apply (Evaluable* zobj, Nameset* nset, const long quark,
			     Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();
    
    // dispatch 1 argument
    if (argc == 1) {
      if (quark == QUARK_SETTRF) {
	Object* obj = argv->get (0);
	// check for a string
	String* sobj = dynamic_cast <String*> (obj);
	if (sobj != nullptr) {
	  settrf (*sobj);
	  return nullptr;
	}
	// check for a transformation
	Transform* tobj = dynamic_cast <Transform*> (obj);
	if (tobj != nullptr) {
	  settrf (*tobj);
	  return nullptr;
	}
	// invalid object
	throw Exception ("type-error", "invalid transformation object",
			 Object::repr (obj));
      }
    }
    // call the xml tag method
    return XmlTag::apply (zobj, nset, quark, argv);
  }
}
