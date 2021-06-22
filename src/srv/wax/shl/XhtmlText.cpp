// ---------------------------------------------------------------------------
// - XhtmlText.cpp                                                           -
// - afnix:wax module - xhtml input text node class implementation           -
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
#include "XhtmlText.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - private section                                                       -
  // -------------------------------------------------------------------------

  // the input text node name
  static const String XML_TAG_NAME = "input";
  // the input text type attribute
  static const String XML_TYP_ATTR = "type";
  static const String XML_TYP_TVAL = "text";
  // the input text name attribute
  static const String XML_NAM_ATTR = "name";
  // the input text size attribute
  static const String XML_SIZ_ATTR = "size";

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a xhtml input text node by name

  XhtmlText::XhtmlText (const String& name) : XhtmlBase (XML_TAG_NAME, true) {
    setattr (XML_TYP_ATTR, XML_TYP_TVAL);
    setattr (XML_NAM_ATTR, name);
  }

  // create a xhtml input text by name and size

  XhtmlText::XhtmlText (const String& name,
			const String& size) : XhtmlBase (XML_TAG_NAME, true) {
    setattr (XML_TYP_ATTR, XML_TYP_TVAL);
    setattr (XML_NAM_ATTR, name);
    setsize (size);
  }

  // return the class name

  String XhtmlText::repr (void) const {
    return "XhtmlText";
  }

  // set the input text size attribute

  void XhtmlText::setsize (const String& size) {
    wrlock ();
    try {
      setattr (XML_SIZ_ATTR, size);
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
  static const long QUARK_SETSIZE = zone.intern ("set-size");

  // create a new object in a generic way

  Object* XhtmlText::mknew (Vector* argv) {
    long argc = (argv == nullptr) ? 0 : argv->length ();
    // check for 1 argument
    if (argc == 1) {
      String name = argv->getstring (0);
      return new XhtmlText (name);
    }
    // check for 2 arguments
    if (argc == 2) {
      String name = argv->getstring (0);
      String size = argv->getstring (1);
      return new XhtmlText (name, size);
    }
    // wrong arguments
    throw Exception ("argument-error", 
		     "too many arguments with xhtml input text constructor");
  }

  // return true if the given quark is defined

  bool XhtmlText::isquark (const long quark, const bool hflg) const {
    rdlock ();
    if (zone.exists (quark) == true) {
      unlock ();
      return true;
    }
    bool result = hflg ? XhtmlBase::isquark (quark, hflg) : false;
    unlock ();
    return result;
  }

  // apply this object with a set of arguments and a quark

  Object* XhtmlText::apply (Evaluable* zobj, Nameset* nset, const long quark,
			    Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();
    
    // dispatch 1 argument
    if (argc == 1) {
      if (quark == QUARK_SETSIZE) {
	String size = argv->getstring (0);
	setsize (size);
	return nullptr;
      }
    }
    // call the xhtml base method
    return XhtmlBase::apply (zobj, nset, quark, argv);
  }
}
