// ---------------------------------------------------------------------------
// - XhtmlTd.cpp                                                             -
// - afnix:wax module - xhtml td node class implementation                   -
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

#include "XhtmlTd.hpp"
#include "Exception.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - private section                                                       -
  // -------------------------------------------------------------------------

  // the td node name
  static const String XML_TAG_NAME = "td";

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a default td node

  XhtmlTd::XhtmlTd (void) : XhtmlBase (XML_TAG_NAME) {}

  // create a xhtml td node with a class

  XhtmlTd::XhtmlTd (const String& cls) : XhtmlBase (XML_TAG_NAME) {
    setclass (cls);
  }
  
  // return the class name

  String XhtmlTd::repr (void) const {
    return "XhtmlTd";
  }

  // -------------------------------------------------------------------------
  // - object section                                                        -
  // -------------------------------------------------------------------------

  // create a new object in a generic way

  Object* XhtmlTd::mknew (Vector* argv) {
    long argc = (argv == nullptr) ? 0 : argv->length ();
    // check for 0 argument
    if (argc == 0) return new XhtmlTd;
    // check for 1 argument
    if (argc == 1) {
      String cls = argv->getstring (0);
      return new XhtmlTd (cls);
    }
    // wrong arguments
    throw Exception ("argument-error", 
		     "too many arguments with xhtml td constructor");
  }
}
