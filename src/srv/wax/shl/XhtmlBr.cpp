// ---------------------------------------------------------------------------
// - XhtmlBr.cpp                                                            -
// - afnix:wax module - xhtml br node class implementation                  -
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

#include "XhtmlBr.hpp"
#include "Exception.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - private section                                                       -
  // -------------------------------------------------------------------------

  // the br node name
  static const String XML_TAG_NAME = "br";

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a default br node

  XhtmlBr::XhtmlBr (void) : XhtmlBase (XML_TAG_NAME, true) {}

  // create a xhtml br node with a class

  XhtmlBr::XhtmlBr (const String& cls) : XhtmlBase (XML_TAG_NAME, true) {
    setclass (cls);
  }

  // return the class name

  String XhtmlBr::repr (void) const {
    return "XhtmlBr";
  }

  // -------------------------------------------------------------------------
  // - object section                                                        -
  // -------------------------------------------------------------------------

  // create a new object in a generic way

  Object* XhtmlBr::mknew (Vector* argv) {
    long argc = (argv == nullptr) ? 0 : argv->length ();
    // check for 0 argument
    if (argc == 0) return new XhtmlBr;
    // check for 1 argument
    if (argc == 1) {
      String cls = argv->getstring (0);
      return new XhtmlBr (cls);
    }
    // wrong arguments
    throw Exception ("argument-error", 
		     "too many arguments with xhtml br constructor");
  }
}
