// ---------------------------------------------------------------------------
// - XhtmlSubmit.cpp                                                           -
// - afnix:wax module - xhtml input submit node class implementation           -
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
#include "QuarkZone.hpp"
#include "Exception.hpp"
#include "XhtmlSubmit.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - private section                                                       -
  // -------------------------------------------------------------------------

  // the input submit node name
  static const String XML_TAG_NAME = "input";
  // the input submit type attribute
  static const String XML_TYP_ATTR = "type";
  static const String XML_TYP_TVAL = "submit";
  // the input submit name attribute
  static const String XML_VAL_ATTR = "value";
  // the input submit size attribute
  static const String XML_SIZ_ATTR = "size";

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a xhtml input submit node by value

  XhtmlSubmit::XhtmlSubmit (const String& sval) : 
    XhtmlBase (XML_TAG_NAME, true) {
    setattr (XML_TYP_ATTR, XML_TYP_TVAL);
    setattr (XML_VAL_ATTR, sval);
  }

  // create a xhtml input submit by value and size

  XhtmlSubmit::XhtmlSubmit (const String& sval,
			    const String& size) : 
    XhtmlBase (XML_TAG_NAME, true) {
    setattr (XML_TYP_ATTR, XML_TYP_TVAL);
    setattr (XML_VAL_ATTR, sval);
    setsize (size);
  }

  // return the class name

  String XhtmlSubmit::repr (void) const {
    return "XhtmlSubmit";
  }

  // set the input submit size attribute

  void XhtmlSubmit::setsize (const String& size) {
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

  Object* XhtmlSubmit::mknew (Vector* argv) {
    long argc = (argv == nullptr) ? 0 : argv->length ();
    // check for 1 argument
    if (argc == 1) {
      String name = argv->getstring (0);
      return new XhtmlSubmit (name);
    }
    // check for 2 arguments
    if (argc == 2) {
      String name = argv->getstring (0);
      String size = argv->getstring (1);
      return new XhtmlSubmit (name, size);
    }
    // wrong arguments
    throw Exception ("argument-error", 
		     "too many arguments with xhtml input submit constructor");
  }

  // return true if the given quark is defined

  bool XhtmlSubmit::isquark (const long quark, const bool hflg) const {
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

  Object* XhtmlSubmit::apply (Evaluable* zobj, Nameset* nset, const long quark,
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
