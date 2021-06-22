// ---------------------------------------------------------------------------
// - XhtmlPara.cpp                                                           -
// - afnix:wax module - xhtml paragraph node class implementation            -
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
#include "XhtmlPara.hpp"
#include "Exception.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - private section                                                       -
  // -------------------------------------------------------------------------

  // the para node name
  static const String XML_TAG_NAME = "p";

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a default para node

  XhtmlPara::XhtmlPara (void) : XhtmlBase (XML_TAG_NAME) {
    d_eolf = false;
  }

  // create a xhtml para node with a class

  XhtmlPara::XhtmlPara (const String& cls) : XhtmlBase (XML_TAG_NAME) {
    d_eolf = false;    
    setclass (cls);
  }

  // create an empty xhtml para node

  XhtmlPara::XhtmlPara (const bool eflg) : XhtmlBase (XML_TAG_NAME, eflg) {
    d_eolf = false;    
  }

  // return the class name

  String XhtmlPara::repr (void) const {
    return "XhtmlPara";
  }

  // -------------------------------------------------------------------------
  // - object section                                                        -
  // -------------------------------------------------------------------------

  // create a new object in a generic way

  Object* XhtmlPara::mknew (Vector* argv) {
    long argc = (argv == nullptr) ? 0 : argv->length ();
    // check for 0 argument
    if (argc == 0) return new XhtmlPara;
    // check for 1 argument
    if (argc == 1) {
      Object* obj = argv->get (0);
      // check for a string
      String* sobj = dynamic_cast <String*> (obj);
      if (sobj != nullptr) return new XhtmlPara (*sobj);
      // check for a boolean
      Boolean* bobj = dynamic_cast <Boolean*> (obj);
      if (bobj != nullptr) return new XhtmlPara (bobj->tobool ());
      // type error
      throw Exception ("type-error", 
		       "invalid object with xhtml para constructor",
		       Object::repr (obj));
    }
    // wrong arguments
    throw Exception ("argument-error", 
		     "too many arguments with xhtml para constructor");
  }
}
