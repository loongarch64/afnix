// ---------------------------------------------------------------------------
// - XhtmlOption.cpp                                                         -
// - afnix:wax module - xhtml select option node class implementation        -
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
#include "XhtmlOption.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - private section                                                       -
  // -------------------------------------------------------------------------

  // the select option node name
  static const String XML_TAG_NAME = "option";
  // the select label attribute
  static const String XML_LBL_ATTR = "label";
  // the select value attribute
  static const String XML_VAL_ATTR = "value";

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a xhtml select option node by value

  XhtmlOption::XhtmlOption (const String& lval, 
			    const String& sval) : XmlTag (XML_TAG_NAME) {
    d_eflg = true;
    setattr (XML_LBL_ATTR, lval);
    setattr (XML_VAL_ATTR, sval);
  }

  // return the class name

  String XhtmlOption::repr (void) const {
    return "XhtmlOption";
  }

  // -------------------------------------------------------------------------
  // - object section                                                        -
  // -------------------------------------------------------------------------

  // create a new object in a generic way

  Object* XhtmlOption::mknew (Vector* argv) {
    long argc = (argv == nullptr) ? 0 : argv->length ();

    // check for 2 arguments
    if (argc == 2) {
      String lval = argv->getstring (0);
      String sval = argv->getstring (1);
      return new XhtmlOption (lval, sval);
    }
    // wrong arguments
    throw Exception ("argument-error", 
		     "too many arguments with xhtml select option constructor");
  }
}
