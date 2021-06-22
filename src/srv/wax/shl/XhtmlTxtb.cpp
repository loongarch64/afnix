// ---------------------------------------------------------------------------
// - XhtmlTxtb.cpp                                                           -
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
#include "XhtmlTxtb.hpp"
#include "Exception.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - private section                                                       -
  // -------------------------------------------------------------------------

  // the text bold node name
  static const String XML_TAG_NAME = "b";

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a default text bold node

  XhtmlTxtb::XhtmlTxtb (void) : XhtmlBase (XML_TAG_NAME) {
    d_eolf = false;
  }

  // create a xhtml text bold node with a class

  XhtmlTxtb::XhtmlTxtb (const String& cls) : XhtmlBase (XML_TAG_NAME) {
    d_eolf = false;
    setclass (cls);
  }

  // return the class name

  String XhtmlTxtb::repr (void) const {
    return "XhtmlTxtb";
  }

  // -------------------------------------------------------------------------
  // - object section                                                        -
  // -------------------------------------------------------------------------

  // create a new object in a generic way

  Object* XhtmlTxtb::mknew (Vector* argv) {
    long argc = (argv == nullptr) ? 0 : argv->length ();
    // check for 0 argument
    if (argc == 0) return new XhtmlTxtb;
    // check for 1 argument
    if (argc == 1) {
      String cls = argv->getstring (0);
      return new XhtmlTxtb (cls);
    }
    // wrong arguments
    throw Exception ("argument-error", 
		     "too many arguments with text italic constructor");
  }
}
