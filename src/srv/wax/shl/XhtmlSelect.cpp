// ---------------------------------------------------------------------------
// - XhtmlSelect.cpp                                                         -
// - afnix:wax module - xhtml select node class implementation               -
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

#include "Vector.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"
#include "XhtmlSelect.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - private section                                                       -
  // -------------------------------------------------------------------------

  // the select option node name
  static const String XML_TAG_NAME = "select";
  // the select name attribute
  static const String XML_NAM_ATTR = "name";

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a xhtml select option node by value

  XhtmlSelect::XhtmlSelect (const String& name) : XhtmlBase (XML_TAG_NAME) {
    setattr (XML_NAM_ATTR, name);
  }

  // return the class name

  String XhtmlSelect::repr (void) const {
    return "XhtmlSelect";
  }

  // add an option by label and value

  XhtmlOption* XhtmlSelect::addoption (const String& lval, const String& sval) {
    wrlock ();
    try {
      // create a new option
      XhtmlOption* result = new XhtmlOption (lval, sval);
      // add it to the select tag
      addchild (result);
      unlock ();
      return result;
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
  static const long QUARK_ADDOPTN = zone.intern ("add-option");

  // create a new object in a generic way

  Object* XhtmlSelect::mknew (Vector* argv) {
    long argc = (argv == nullptr) ? 0 : argv->length ();

    // check for 1 argument
    if (argc == 1) {
      String name = argv->getstring (0);
      return new XhtmlSelect (name);
    }
    // wrong arguments
    throw Exception ("argument-error", 
		     "too many arguments with xhtml select constructor");
  }

  // return true if the given quark is defined

  bool XhtmlSelect::isquark (const long quark, const bool hflg) const {
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

  Object* XhtmlSelect::apply (Evaluable* zobj, Nameset* nset, const long quark,
			      Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();
    
    // dispatch 2 arguments
    if (argc == 2) {
      if (quark == QUARK_ADDOPTN) {
	String lval = argv->getstring (0);
	String sval = argv->getstring (1);
	return addoption (lval, sval);
      }
    }
    // call the xhtml base method
    return XhtmlBase::apply (zobj, nset, quark, argv);
  }
}
