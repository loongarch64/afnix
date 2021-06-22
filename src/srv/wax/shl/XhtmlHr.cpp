// ---------------------------------------------------------------------------
// - XhtmlHr.cpp                                                            -
// - afnix:wax module - xhtml hr node class implementation                  -
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

#include "XhtmlHr.hpp"
#include "Exception.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - private section                                                       -
  // -------------------------------------------------------------------------

  // the hr node name
  static const String XML_TAG_NAME = "hr";

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a default hr node

  XhtmlHr::XhtmlHr (void) : XhtmlBase (XML_TAG_NAME, true) {}

  // create a xhtml hr node with a class

  XhtmlHr::XhtmlHr (const String& cls) : XhtmlBase (XML_TAG_NAME, true) {
    setclass (cls);
  }

  // return the class name

  String XhtmlHr::repr (void) const {
    return "XhtmlHr";
  }

  // -------------------------------------------------------------------------
  // - object section                                                        -
  // -------------------------------------------------------------------------

  // create a new object in a generic way

  Object* XhtmlHr::mknew (Vector* argv) {
    long argc = (argv == nullptr) ? 0 : argv->length ();
    // check for 0 argument
    if (argc == 0) return new XhtmlHr;
    // check for 1 argument
    if (argc == 1) {
      String cls = argv->getstring (0);
      return new XhtmlHr (cls);
    }
    // wrong arguments
    throw Exception ("argument-error", 
		     "too many arguments with xhtml hr constructor");
  }
}
