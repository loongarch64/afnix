// ---------------------------------------------------------------------------
// - XhtmlHidden.cpp                                                         -
// - afnix:wax module - xhtml input hidden node class implementation         -
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
#include "XhtmlHidden.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - private section                                                       -
  // -------------------------------------------------------------------------

  // the input hidden node name
  static const String XML_TAG_NAME = "input";
  // the input submit type attribute
  static const String XML_TYP_ATTR = "type";
  static const String XML_TYP_TVAL = "hidden";
  // the input name attribute
  static const String XML_NAM_ATTR = "name";
  // the input value attribute
  static const String XML_VAL_ATTR = "value";

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a xhtml input hidden node by value

  XhtmlHidden::XhtmlHidden (const String& name, 
			    const String& sval) : XmlTag (XML_TAG_NAME) {
    d_eflg = true;
    setattr (XML_TYP_ATTR, XML_TYP_TVAL);
    setattr (XML_NAM_ATTR, name);
    setattr (XML_VAL_ATTR, sval);
  }

  // return the class name

  String XhtmlHidden::repr (void) const {
    return "XhtmlHidden";
  }

  // -------------------------------------------------------------------------
  // - object section                                                        -
  // -------------------------------------------------------------------------

  // create a new object in a generic way

  Object* XhtmlHidden::mknew (Vector* argv) {
    long argc = (argv == nullptr) ? 0 : argv->length ();

    // check for 2 arguments
    if (argc == 2) {
      String name = argv->getstring (0);
      String sval = argv->getstring (1);
      return new XhtmlHidden (name, sval);
    }
    // wrong arguments
    throw Exception ("argument-error", 
		     "too many arguments with xhtml input hidden constructor");
  }
}
