// ---------------------------------------------------------------------------
// - XhtmlH2.cpp                                                             -
// - afnix:wax module - xhtml heading 2 node class implementation            -
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

#include "XhtmlH2.hpp"
#include "Exception.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - private section                                                       -
  // -------------------------------------------------------------------------

  // the para node name
  static const String XML_TAG_NAME = "h2";

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a default para node

  XhtmlH2::XhtmlH2 (void) : XhtmlBase (XML_TAG_NAME) {
  }

  // create a xhtml para node with a class

  XhtmlH2::XhtmlH2 (const String& cls) : XhtmlBase (XML_TAG_NAME) {
    setclass (cls);
  }

  // return the class name

  String XhtmlH2::repr (void) const {
    return "XhtmlH2";
  }

  // -------------------------------------------------------------------------
  // - object section                                                        -
  // -------------------------------------------------------------------------

  // create a new object in a generic way

  Object* XhtmlH2::mknew (Vector* argv) {
    long argc = (argv == nullptr) ? 0 : argv->length ();
    // check for 0 argument
    if (argc == 0) return new XhtmlH2;
    // check for 1 argument
    if (argc == 1) {
      String cls = argv->getstring (0);
      return new XhtmlH2 (cls);
    }
    // wrong arguments
    throw Exception ("argument-error", 
		     "too many arguments with xhtml h2 constructor");
  }
}
