// ---------------------------------------------------------------------------
// - XhtmlLi.cpp                                                             -
// - afnix:wax module - xhtml list item node class implementation            -
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

#include "XhtmlLi.hpp"
#include "Exception.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - private section                                                       -
  // -------------------------------------------------------------------------

  // the item node name
  static const String XML_TAG_NAME = "li";

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a default list item node

  XhtmlLi::XhtmlLi (void) : XhtmlBase (XML_TAG_NAME) {}

  // create a xhtml list item node with a class

  XhtmlLi::XhtmlLi (const String& cls) : XhtmlBase (XML_TAG_NAME) {
    setclass (cls);
  }

  // return the class name

  String XhtmlLi::repr (void) const {
    return "XhtmlLi";
  }

  // -------------------------------------------------------------------------
  // - object section                                                        -
  // -------------------------------------------------------------------------

  // create a new object in a generic way

  Object* XhtmlLi::mknew (Vector* argv) {
    long argc = (argv == nullptr) ? 0 : argv->length ();
    // check for 0 argument
    if (argc == 0) return new XhtmlLi;
    // check for 1 argument
    if (argc == 1) {
      String cls = argv->getstring (0);
      return new XhtmlLi (cls);
    }
    // wrong arguments
    throw Exception ("argument-error", 
		     "too many arguments with xhtml item constructor");
  }
}
