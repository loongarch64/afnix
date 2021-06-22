// ---------------------------------------------------------------------------
// - XhtmlCaption.cpp                                                        -
// - afnix:wax module - xhtml caption node class implementation              -
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
#include "XhtmlCaption.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - private section                                                       -
  // -------------------------------------------------------------------------

  // the caption node name
  static const String XML_TAG_NAME = "caption";

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a xhtml caption node  by text

  XhtmlCaption::XhtmlCaption (const String& text) : XmlTag (XML_TAG_NAME) {
    parse (text);
  }

  // return the class name

  String XhtmlCaption::repr (void) const {
    return "XhtmlCaption";
  }

  // -------------------------------------------------------------------------
  // - object section                                                        -
  // -------------------------------------------------------------------------

  // create a new object in a generic way

  Object* XhtmlCaption::mknew (Vector* argv) {
    long argc = (argv == nullptr) ? 0 : argv->length ();
    // check for 1 argument
    if (argc == 1) {
      String text = argv->getstring (0);
      return new XhtmlCaption (text);
    }
    // wrong arguments
    throw Exception ("argument-error", 
		     "too many arguments with xhtml caption constructor");
  }
}
