// ---------------------------------------------------------------------------
// - XmlProcessor.cpp                                                        -
// - afnix:xpe module - xml processor class implementation                   -
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

#include "Integer.hpp"
#include "Evaluable.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"
#include "XmlProcessor.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a default xml processor

  XmlProcessor::XmlProcessor (void) {
    d_xftr.reset ();
    d_mlvl = 0;
  }

  // return the class name

  String XmlProcessor::repr (void) const {
    return "XmlProcessor";
  }

  // get the number of features

  long XmlProcessor::lenxftr (void) const {
    rdlock ();
    try {
      long result = d_xftr.length ();
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get a feature by index

  XmlFeature* XmlProcessor::getxftr (const long index) const {
    rdlock ();
    try {
      XmlFeature* result = dynamic_cast <XmlFeature*> (d_xftr.get(index));
      if (result == nullptr) {
	unlock ();
	throw Exception ("internal-error", "invald nil feature");
      }
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // add a new feature to the processor
  
  void XmlProcessor::addxftr (XmlFeature* xftr) {
    // check for nil
    if (xftr == nullptr) return;
    // lock and add
    wrlock();
    try {
      d_xftr.add (xftr);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // process a xml content
  
  XmlContent* XmlProcessor::process (const XmlContent* xcnt) const {
    // check for nil content first
    if (xcnt == nullptr) return nullptr;
    // get a fresh copy of the content
    XmlContent* result = new XmlContent (*xcnt);
    // lock and process
    rdlock ();
    try {
      // get the number of features
      long xlen = d_xftr.length ();
      // find the maximum level
      long minl = 0;
      long maxl = 0;
      for (long i = 0; i < xlen; i++) {
	// get the feature
	XmlFeature* xftr = getxftr (i);
	if (xftr == nullptr) continue;
	// get the processing level and compare
	long plvl = xftr->getplvl ();
	if (plvl < minl) minl = plvl;
	if (plvl > maxl) maxl = plvl;
      }
      // loop in the features
      for (long i = minl; i <= maxl; i++) {
	for (long j = 0; j < xlen; j++) {
	  // get the feature
	  XmlFeature* xftr = getxftr (j);
	  if (xftr == nullptr) continue;
	  if (xftr->isplvl (i) == false) continue;
	  // process the content
	  XmlContent* scnt = result;
	  result = xftr->process (scnt);
	  delete scnt;
	}
      }
      unlock ();
      return result;
    } catch (...) {
      delete result;
      unlock ();
      throw;
    }
  }

  // -------------------------------------------------------------------------
  // - object section                                                        -
  // -------------------------------------------------------------------------

  // the quark zone
  static const long QUARK_ZONE_LENGTH = 4;
  static QuarkZone  zone (QUARK_ZONE_LENGTH);

  // the object supported quarks
  static const long QUARK_PROCESS = zone.intern ("process");
  static const long QUARK_LENXFTR = zone.intern ("feature-length");
  static const long QUARK_GETXFTR = zone.intern ("get-feature");
  static const long QUARK_ADDXFTR = zone.intern ("add-feature");

  // create a new object in a generic way

  Object* XmlProcessor::mknew (Vector* argv) {
    long argc = (argv == nullptr) ? 0 : argv->length ();
    // check for 0 argument
    if (argc == 0) return new XmlProcessor;
    // wrong arguments
    throw Exception ("argument-error", 
		     "too many arguments with xml processor constructor");
  }

  // return true if the given quark is defined

  bool XmlProcessor::isquark (const long quark, const bool hflg) const {
    rdlock ();
    if (zone.exists (quark) == true) {
      unlock ();
      return true;
    }
    bool result = hflg ? Object::isquark (quark, hflg) : false;
    unlock ();
    return result;
  }

  // apply this object with a set of arguments and a quark

  Object* XmlProcessor::apply (Evaluable* zobj, Nameset* nset, const long quark,
			       Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();
    
    // check for 0 argument
    if (argc == 0) {
      if (quark == QUARK_LENXFTR) return new Integer (lenxftr ());
    }

    // check for 1 argument
    if (argc == 1) {
      if (quark == QUARK_GETXFTR) {
	long index = argv->getlong (0);
	rdlock ();
	try {
	  Object* result = getxftr (index);
	  zobj->post (result);
	  unlock ();
	  return result;
	} catch (...) {
	  unlock ();
	  throw;
	}
      }
      if (quark == QUARK_ADDXFTR) {
	Object* obj = argv->get (0);
	XmlFeature* xftr = dynamic_cast <XmlFeature*> (obj);
	if (xftr == nullptr) {
	  throw Exception ("type-error", "invalid object with add-feature",
			   Object::repr (obj));
	}
	addxftr (xftr);
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

    // call the object method
    return Object::apply (zobj, nset, quark, argv);
  }
}
