// ---------------------------------------------------------------------------
// - XhtmlEmph.cpp                                                           -
// - afnix:wax module - xhtml emphasize node class implementation            -
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

#include "Boolean.hpp"
#include "XhtmlEmph.hpp"
#include "Exception.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - private section                                                       -
  // -------------------------------------------------------------------------

  // the emph node name
  static const String XML_TAG_NAME = "em";

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a default emph node

  XhtmlEmph::XhtmlEmph (void) : XhtmlBase (XML_TAG_NAME) {
    d_eolf = false;
  }

  // create a xhtml emph node with a class

  XhtmlEmph::XhtmlEmph (const String& cls) : XhtmlBase (XML_TAG_NAME) {
    d_eolf = false;
    setclass (cls);
  }

  // return the class name

  String XhtmlEmph::repr (void) const {
    return "XhtmlEmph";
  }

  // -------------------------------------------------------------------------
  // - object section                                                        -
  // -------------------------------------------------------------------------

  // create a new object in a generic way

  Object* XhtmlEmph::mknew (Vector* argv) {
    long argc = (argv == nullptr) ? 0 : argv->length ();
    // check for 0 argument
    if (argc == 0) return new XhtmlEmph;
    // check for 1 argument
    if (argc == 1) {
      String cls = argv->getstring (0);
      return new XhtmlEmph (cls);
    }
    // wrong arguments
    throw Exception ("argument-error", 
		     "too many arguments with text emphasis constructor");
  }
}
