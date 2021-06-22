// ---------------------------------------------------------------------------
// - XhtmlDiv.cpp                                                            -
// - afnix:wax module - xhtml div node class implementation                  -
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
#include "XhtmlDiv.hpp"
#include "Exception.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - private section                                                       -
  // -------------------------------------------------------------------------

  // the div node name
  static const String XML_TAG_NAME = "div";

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a default div node

  XhtmlDiv::XhtmlDiv (void) : XhtmlBase (XML_TAG_NAME) {}

  // create an empty div node by empty flag

  XhtmlDiv::XhtmlDiv (const bool eflg) : XhtmlBase (XML_TAG_NAME, eflg) {}
  
  // create a xhtml div node with a class

  XhtmlDiv::XhtmlDiv (const String& cls) : XhtmlBase (XML_TAG_NAME) {
    setclass (cls);
  }

  // return the class name

  String XhtmlDiv::repr (void) const {
    return "XhtmlDiv";
  }

  // -------------------------------------------------------------------------
  // - object section                                                        -
  // -------------------------------------------------------------------------

  // create a new object in a generic way

  Object* XhtmlDiv::mknew (Vector* argv) {
    long argc = (argv == nullptr) ? 0 : argv->length ();
    // check for 0 argument
    if (argc == 0) return new XhtmlDiv;
    // check for 1 argument
    if (argc == 1) {
      Object* obj = argv->get (0);
      // check for a string
      auto sobj = dynamic_cast<String*>(obj);
      if (sobj != nullptr) return new XhtmlDiv (*sobj);
      // check for a boolean
      auto bobj = dynamic_cast<Boolean*>(obj);
      if (bobj != nullptr) return new XhtmlDiv (bobj->tobool());
      // invalid object
      throw Exception ("type-error", "invalid object for div constructor",
		       Object::repr (obj));
    }
    // wrong arguments
    throw Exception ("argument-error", 
		     "too many arguments with xhtml div constructor");
  }
}
