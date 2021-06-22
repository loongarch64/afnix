// ---------------------------------------------------------------------------
// - XmlRef.cpp                                                              -
// - afnix:xml module - xml reference node class implementation              -
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

#include "XmlRef.hpp"
#include "Evaluable.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create an empty reference node

  XmlRef::XmlRef (void) {
    d_eflg = true;
  }

  // create a reference node by value

  XmlRef::XmlRef (const String& xref) {
    d_eflg = true;
    d_xref = xref;
  }

  // return the document class name

  String XmlRef::repr (void) const {
    return "XmlRef";
  }

  // set he xml reference value

  void XmlRef::setxref (const String& xref) {
    wrlock ();
    d_xref = xref;
    unlock ();
  }
    
  // get the reference node value

  String XmlRef::getxref (void) const {
    rdlock ();
    String result = d_xref;
    unlock ();
    return result;
  }

  // -------------------------------------------------------------------------
  // - object section                                                        -
  // -------------------------------------------------------------------------

  // the quark zone
  static const long QUARK_ZONE_LENGTH = 2;
  static QuarkZone  zone (QUARK_ZONE_LENGTH);

  // the object supported quarks
  static const long QUARK_SETXREF  = zone.intern ("set-xref");
  static const long QUARK_GETXREF  = zone.intern ("get-xref");

  // return true if the given quark is defined

  bool XmlRef::isquark (const long quark, const bool hflg) const {
    rdlock ();
    if (zone.exists (quark) == true) {
      unlock ();
      return true;
    }
    bool result = hflg ? XmlNode::isquark (quark, hflg) : false;
    unlock ();
    return result;
  }

  // apply this object with a set of arguments and a quark
  
  Object* XmlRef::apply (Evaluable* zobj, Nameset* nset, const long quark,
			 Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();

    // check for 0 argument
    if (argc == 0) {
      if (quark == QUARK_GETXREF) return new String (getxref ());
    }
    // check for 1 argument
    if (argc == 1) {
      if (quark == QUARK_SETXREF) {
	String xref = argv->getstring (0);
	setxref (xref);
	return nullptr;
      }
    }
    // call the xml node method
    return XmlNode::apply (zobj, nset, quark, argv);
  }
}
