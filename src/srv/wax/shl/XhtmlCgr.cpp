// ---------------------------------------------------------------------------
// - XhtmlCgr.cpp                                                            -
// - afnix:wax module - xhtml column group node class implementation         -
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

#include "XhtmlCgr.hpp"
#include "Exception.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - private section                                                       -
  // -------------------------------------------------------------------------

  // the cgr node name
  static const String XML_TAG_NAME = "colgroup";

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a default cgr node

  XhtmlCgr::XhtmlCgr (void) : XmlTag (XML_TAG_NAME) {
  }

  // return the class name

  String XhtmlCgr::repr (void) const {
    return "XhtmlCgr";
  }

  // -------------------------------------------------------------------------
  // - object section                                                        -
  // -------------------------------------------------------------------------

  // create a new object in a generic way

  Object* XhtmlCgr::mknew (Vector* argv) {
    long argc = (argv == nullptr) ? 0 : argv->length ();
    // check for 0 argument
    if (argc == 0) return new XhtmlCgr;
    // wrong arguments
    throw Exception ("argument-error", 
		     "too many arguments with xhtml cgr constructor");
  }
}
