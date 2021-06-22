// ---------------------------------------------------------------------------
// - XhtmlBase.cpp                                                           -
// - afnix:wax module - xhtml base class implementation                      -
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

#include "XhtmlBase.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - private section                                                       -
  // -------------------------------------------------------------------------

  // the class name attribute
  static const String XML_CLS_ATTR = "class";
  // the style name attribute
  static const String XML_STL_ATTR = "style";

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a xhtml base element

  XhtmlBase::XhtmlBase (const String& name) : XmlTag (name) {
  }

  // create a xhtml base element with a end flag

  XhtmlBase::XhtmlBase (const String& name, const bool eflg) : XmlTag (name) {
    d_eflg = eflg;
  }

  // set the class attribute

  void XhtmlBase::setclass (const String& sval) {
    wrlock ();
    try {
      setattr (XML_CLS_ATTR, sval);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }
  // set the style attribute

  void XhtmlBase::setstyle (const String& sval) {
    wrlock ();
    try {
      setattr (XML_STL_ATTR, sval);
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
  static const long QUARK_SETCLASS = zone.intern ("set-class");
  static const long QUARK_SETSTYLE = zone.intern ("set-style");

  // return true if the given quark is defined

  bool XhtmlBase::isquark (const long quark, const bool hflg) const {
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

  Object* XhtmlBase::apply (Evaluable* zobj, Nameset* nset, const long quark,
			    Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();
    
    // dispatch 1 argument
    if (argc == 1) {
      if (quark == QUARK_SETCLASS) {
	String sval = argv->getstring (0);
	setclass (sval);
	return nullptr;
      }
      if (quark == QUARK_SETSTYLE) {
	String sval = argv->getstring (0);
	setstyle (sval);
	return nullptr;
      }
    }
    // call the xml tag method
    return XmlTag::apply (zobj, nset, quark, argv);
  }
}
