// ---------------------------------------------------------------------------
// - XhtmlCol.cpp                                                             -
// - afnix:wax module - xhtml col node class implementation                   -
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

#include "XhtmlCol.hpp"
#include "Exception.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - private section                                                       -
  // -------------------------------------------------------------------------

  // the col node name
  static const String XML_TAG_NAME = "col";
  // the width attribute
  static const String XML_WTH_ATTR = "width";

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a default col node

  XhtmlCol::XhtmlCol (void) : XmlTag (XML_TAG_NAME) {
    d_eflg = true;
  }

  // create a col node with a width attribute

  XhtmlCol::XhtmlCol (const String& width) : XmlTag (XML_TAG_NAME) {
    d_eflg = true;
    setattr (XML_WTH_ATTR, width);
  }

  // return the class name

  String XhtmlCol::repr (void) const {
    return "XhtmlCol";
  }

  // -------------------------------------------------------------------------
  // - object section                                                        -
  // -------------------------------------------------------------------------

  // create a new object in a generic way

  Object* XhtmlCol::mknew (Vector* argv) {
    long argc = (argv == nullptr) ? 0 : argv->length ();
    // check for 0 argument
    if (argc == 0) return new XhtmlCol;
    // check for 1 argument
    if (argc == 1) {
      String width = argv->getstring (0);
      return new XhtmlCol(width);
    }
    // wrong arguments
    throw Exception ("argument-error", 
		     "too many arguments with xhtml col constructor");
  }
}
