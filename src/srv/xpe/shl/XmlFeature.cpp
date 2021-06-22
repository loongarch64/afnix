// ---------------------------------------------------------------------------
// - XmlFeature.cpp                                                          -
// - afnix:xpe module - xml processor feature class implementation           -
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

#include "Boolean.hpp"
#include "Integer.hpp"
#include "Evaluable.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"
#include "XmlFeature.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a xml feature by name

  XmlFeature::XmlFeature (const String& name) {
    d_name = name;
    d_plvl = 0;
  }

  // create a xml feature by name and info

  XmlFeature::XmlFeature (const String& name, const String& info) {
    d_name = name;
    d_info = info;
    d_plvl = 0;
  }

  // create a xml feature by name, info and level

  XmlFeature::XmlFeature (const String& name, const String& info,
			  const long plvl) {
    d_name = name;
    d_info = info;
    d_plvl = plvl;
  }

  // get the feature name

  String XmlFeature::getname(void) const {
    rdlock ();
    String result = d_name;
    unlock ();
    return result;
  }

  // get the feature info

  String XmlFeature::getinfo(void) const {
    rdlock ();
    String result = d_info;
    unlock ();
    return result;
  }

  // return true if the feature is at the processing level

  bool XmlFeature::isplvl (const long plvl) const {
    rdlock ();
    bool result = (d_plvl == plvl);
    unlock ();
    return result;
  }

  // set the feature processing level

  void XmlFeature::setplvl (const long plvl) {
    wrlock ();
    d_plvl = plvl;
    unlock ();
  }

  // get the feature processing level

  long XmlFeature::getplvl(void) const {
    rdlock ();
    long result = d_plvl;
    unlock ();
    return result;
  }

  // -------------------------------------------------------------------------
  // - object section                                                        -
  // -------------------------------------------------------------------------

  // the quark zone
  static const long QUARK_ZONE_LENGTH = 5;
  static QuarkZone  zone (QUARK_ZONE_LENGTH);

  // the object supported quarks
  static const long QUARK_PLVLP   = zone.intern ("processing-level-p");
  static const long QUARK_PROCESS = zone.intern ("process");
  static const long QUARK_GETINFO = zone.intern ("get-info");
  static const long QUARK_SETPLVL = zone.intern ("set-processing-level");
  static const long QUARK_GETPLVL = zone.intern ("get-processing-level");

  // return true if the given quark is defined

  bool XmlFeature::isquark (const long quark, const bool hflg) const {
    rdlock ();
    if (zone.exists (quark) == true) {
      unlock ();
      return true;
    }
    bool result = hflg ? Nameable::isquark (quark, hflg) : false;
    unlock ();
    return result;
  }

  // apply this object with a set of arguments and a quark

  Object* XmlFeature::apply (Evaluable* zobj, Nameset* nset, const long quark,
			     Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();
    
    // check for 0 argument
    if (argc == 0) {
      if (quark == QUARK_GETINFO) return new String  (getinfo ());
      if (quark == QUARK_GETPLVL) return new Integer (getplvl ());
    }

    // check for 1 argument
    if(argc == 1) {
      if (quark == QUARK_PLVLP) {
	long plvl = argv->getlong (0);
	return new Boolean (isplvl (plvl));
      }
      if (quark == QUARK_SETPLVL) {
	long plvl = argv->getlong (0);
	setplvl (plvl);
	return nullptr;
      }
      if (quark == QUARK_PROCESS) {
	Object* obj = argv->get (0);
	XmlContent* xcnt = dynamic_cast <XmlContent*> (obj);
	if (xcnt == nullptr) {
	  throw Exception ("type-error", "invalid object to process",
			   Object::repr (obj));
	}
	return process (xcnt);
      }
    }
    // call the nameable method
    return Nameable::apply (zobj, nset, quark, argv);
  }
}
