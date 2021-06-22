// ---------------------------------------------------------------------------
// - XhtmlTxtt.cpp                                                           -
// - afnix:wax module - xhtml text teletype node class implementation        -
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
#include "XhtmlTxtt.hpp"
#include "Exception.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - private section                                                       -
  // -------------------------------------------------------------------------

  // the text teletype node name
  static const String XML_TAG_NAME = "tt";

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a default text teletype node

  XhtmlTxtt::XhtmlTxtt (void) : XhtmlBase (XML_TAG_NAME) {
    d_eolf = false;
  }

  // create a xhtml text teletype node with a class

  XhtmlTxtt::XhtmlTxtt (const String& cls) : XhtmlBase (XML_TAG_NAME) {
    d_eolf = false;
    setclass (cls);
  }

  // return the class name

  String XhtmlTxtt::repr (void) const {
    return "XhtmlTxtt";
  }

  // -------------------------------------------------------------------------
  // - object section                                                        -
  // -------------------------------------------------------------------------

  // create a new object in a generic way

  Object* XhtmlTxtt::mknew (Vector* argv) {
    long argc = (argv == nullptr) ? 0 : argv->length ();
    // check for 0 argument
    if (argc == 0) return new XhtmlTxtt;
    // check for 1 argument
    if (argc == 1) {
      String cls = argv->getstring (0);
      return new XhtmlTxtt (cls);
    }
    // wrong arguments
    throw Exception ("argument-error", 
		     "too many arguments with text teletype constructor");
  }
}
