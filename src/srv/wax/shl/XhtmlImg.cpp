// ---------------------------------------------------------------------------
// - XhtmlImg.cpp                                                            -
// - afnix:wax module - xhtml image node class implementation                -
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

#include "XhtmlImg.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - private section                                                       -
  // -------------------------------------------------------------------------

  // the image node name
  static const String XML_TAG_NAME = "img";
  // the image src attribute
  static const String XML_SRC_ATTR = "src";
  // the image alt attribute
  static const String XML_ALT_ATTR = "alt";
  // the image width attribute
  static const String XML_WTH_ATTR = "width";
  // the image height attribute
  static const String XML_HGT_ATTR = "height";

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a xhtml image node by source and alternate

  XhtmlImg::XhtmlImg (const String& src, 
		      const String& alt) : XmlTag (XML_TAG_NAME) {
    d_eflg = true;
    setattr (XML_SRC_ATTR, src);
    setattr (XML_ALT_ATTR, alt);
  }

  // return the class name

  String XhtmlImg::repr (void) const {
    return "XhtmlImg";
  }

  // set the image width attribute

  void XhtmlImg::setwth (const String& wth) {
    wrlock ();
    try {
      setattr (XML_WTH_ATTR, wth);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set the image height attribute

  void XhtmlImg::sethgt (const String& hgt) {
    wrlock ();
    try {
      setattr (XML_HGT_ATTR, hgt);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set the image width and height attributes

  void XhtmlImg::setgeo (const String& wth, const String& hgt) {
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

  Object* XhtmlImg::mknew (Vector* argv) {
    long argc = (argv == nullptr) ? 0 : argv->length ();
    // check for 2 arguments
    if (argc == 2) {
      String src = argv->getstring (0);
      String alt = argv->getstring (1);
      return new XhtmlImg (src, alt);
    }
    // wrong arguments
    throw Exception ("argument-error", 
		     "too many arguments with xhtml image constructor");
  }

  // return true if the given quark is defined

  bool XhtmlImg::isquark (const long quark, const bool hflg) const {
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

  Object* XhtmlImg::apply (Evaluable* zobj, Nameset* nset, const long quark,
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
