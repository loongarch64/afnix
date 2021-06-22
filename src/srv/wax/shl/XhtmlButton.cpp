// ---------------------------------------------------------------------------
// - XhtmlButton.cpp                                                         -
// - afnix:wax module - xhtml button node class implementation               -
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

#include "Exception.hpp"
#include "XhtmlButton.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - private section                                                       -
  // -------------------------------------------------------------------------

  // the button node name
  static const String XML_TAG_NAME = "button";

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a default button node

  XhtmlButton::XhtmlButton (void) : XhtmlBase (XML_TAG_NAME) {
  }

  // create a xhtml button node with a name

  XhtmlButton::XhtmlButton (const String& name) : XhtmlBase (XML_TAG_NAME) {
    parse (name);
  }

  // return the class name

  String XhtmlButton::repr (void) const {
    return "XhtmlButton";
  }

  // -------------------------------------------------------------------------
  // - object section                                                        -
  // -------------------------------------------------------------------------

  // create a new object in a generic way

  Object* XhtmlButton::mknew (Vector* argv) {
    long argc = (argv == nullptr) ? 0 : argv->length ();
    // check for 0 argument
    if (argc == 0) return new XhtmlButton;
    // check for 1 argument
    if (argc == 1) {
      String name = argv->getstring (0);
      return new XhtmlButton (name);
    }
    // wrong arguments
    throw Exception ("argument-error", 
		     "too many arguments with xhtml button constructor");
  }
}
