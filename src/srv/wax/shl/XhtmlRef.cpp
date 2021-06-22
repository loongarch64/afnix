// ---------------------------------------------------------------------------
// - XhtmlRef.cpp                                                            -
// - afnix:wax module - xhtml ref (a) node class implementation              -
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

#include "XhtmlRef.hpp"
#include "Exception.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - private section                                                       -
  // -------------------------------------------------------------------------

  // the ref node name
  static const String XML_TAG_NAME = "a";
  // the href attribute
  static const String XML_REF_NAME = "href";

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a default ref node

  XhtmlRef::XhtmlRef (void) : XhtmlBase (XML_TAG_NAME) {
  }

  // create a xhtml ref node with a uri

  XhtmlRef::XhtmlRef (const String& uri) : XhtmlBase (XML_TAG_NAME) {
    setattr (XML_REF_NAME, uri);
  }

  // create a xhtml ref node with a uri and text

  XhtmlRef::XhtmlRef (const String& uri, const String& txt) : 
    XhtmlBase (XML_TAG_NAME) {
    setattr (XML_REF_NAME, uri);
    parse (txt);
  }

  // return the class name

  String XhtmlRef::repr (void) const {
    return "XhtmlRef";
  }

  // -------------------------------------------------------------------------
  // - object section                                                        -
  // -------------------------------------------------------------------------

  // create a new object in a generic way

  Object* XhtmlRef::mknew (Vector* argv) {
    long argc = (argv == nullptr) ? 0 : argv->length ();
    // check for 0 argument
    if (argc == 0) return new XhtmlRef;
    // check for 1 argument
    if (argc == 1) {
      String uri = argv->getstring (0);
      return new XhtmlRef (uri);
    }
    if (argc == 2) {
      String uri = argv->getstring (0);
      String txt = argv->getstring (1);
      return new XhtmlRef (uri, txt);
    }
    // wrong arguments
    throw Exception ("argument-error", 
		     "too many arguments with xhtml ref constructor");
  }
}
