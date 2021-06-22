// ---------------------------------------------------------------------------
// - XhtmlTxti.cpp                                                           -
// - afnix:wax module - xhtml text italic node class implementation          -
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
#include "XhtmlTxti.hpp"
#include "Exception.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - private section                                                       -
  // -------------------------------------------------------------------------

  // the text italic node name
  static const String XML_TAG_NAME = "i";

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a default text italic node

  XhtmlTxti::XhtmlTxti (void) : XhtmlBase (XML_TAG_NAME) {
    d_eolf = false;
  }

  // create a xhtml text italic node with a class

  XhtmlTxti::XhtmlTxti (const String& cls) : XhtmlBase (XML_TAG_NAME) {
    d_eolf = false;
    setclass (cls);
  }

  // return the class name

  String XhtmlTxti::repr (void) const {
    return "XhtmlTxti";
  }

  // -------------------------------------------------------------------------
  // - object section                                                        -
  // -------------------------------------------------------------------------

  // create a new object in a generic way

  Object* XhtmlTxti::mknew (Vector* argv) {
    long argc = (argv == nullptr) ? 0 : argv->length ();
    // check for 0 argument
    if (argc == 0) return new XhtmlTxti;
    // check for 1 argument
    if (argc == 1) {
      String cls = argv->getstring (0);
      return new XhtmlTxti (cls);
    }
    // wrong arguments
    throw Exception ("argument-error", 
		     "too many arguments with text italic constructor");
  }
}
