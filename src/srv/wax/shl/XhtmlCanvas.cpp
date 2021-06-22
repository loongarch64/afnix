// ---------------------------------------------------------------------------
// - XhtmlCanvas.cpp                                                         -
// - afnix:wax module - xhtml canvas node class implementation               -
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
#include "XhtmlCanvas.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - private section                                                       -
  // -------------------------------------------------------------------------

  // the canvas node name
  static const String XML_TAG_NAME = "canvas";
  // the canvas width attribute
  static const String XML_WTH_ATTR = "width";
  // the canvas height attribute
  static const String XML_HGT_ATTR = "height";

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a default xhtml canvas

  XhtmlCanvas::XhtmlCanvas (void) : XmlTag (XML_TAG_NAME) {
    d_eflg = false;
  }

  // return the class name

  String XhtmlCanvas::repr (void) const {
    return "XhtmlCanvas";
  }

  // set the canvas width attribute

  void XhtmlCanvas::setwth (const String& wth) {
    wrlock ();
    try {
      setattr (XML_WTH_ATTR, wth);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set the canvas height attribute

  void XhtmlCanvas::sethgt (const String& hgt) {
    wrlock ();
    try {
      setattr (XML_HGT_ATTR, hgt);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set the canvas width and height attributes

  void XhtmlCanvas::setgeo (const String& wth, const String& hgt) {
    wrlock ();
    try {
      setattr (XML_WTH_ATTR, wth);
      setattr (XML_HGT_ATTR, hgt);
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
  static const long QUARK_SETWTH = zone.intern ("set-width");
  static const long QUARK_SETHGT = zone.intern ("set-height");
  static const long QUARK_SETGEO = zone.intern ("set-geometry");

  // create a new object in a generic way

  Object* XhtmlCanvas::mknew (Vector* argv) {
    long argc = (argv == nullptr) ? 0 : argv->length ();
    // check for 0 argument
    if (argc == 0) return new XhtmlCanvas;
    // wrong arguments
    throw Exception ("argument-error", 
		     "too many arguments with xhtml canvas constructor");
  }
  
  // return true if the given quark is defined
  
  bool XhtmlCanvas::isquark (const long quark, const bool hflg) const {
    rdlock ();
    try {
      if (zone.exists (quark) == true) {
	unlock ();
	return true;
      }
      bool result = hflg ? XmlTag::isquark (quark, hflg) : false;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // apply this object with a set of arguments and a quark

  Object* XhtmlCanvas::apply (Evaluable* zobj, Nameset* nset, const long quark,
			    Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();
    
    // dispatch 1 argument
    if (argc == 1) {
      if (quark == QUARK_SETWTH) {
	String wth = argv->getstring (0);
	setwth (wth);
	return nullptr;
      }
      if (quark == QUARK_SETHGT) {
	String hgt = argv->getstring (0);
	sethgt (hgt);
	return nullptr;
      }
    }
    // dispatch 2 arguments
    if (argc == 2) {
      if (quark == QUARK_SETGEO) {
	String wth = argv->getstring (0);
	String hgt = argv->getstring (1);
	setgeo (wth, hgt);
	return nullptr;
      }
    }
    // call the xml tag method
    return XmlTag::apply (zobj, nset, quark, argv);
  }
}
