// ---------------------------------------------------------------------------
// - XhtmlTbody.cpp                                                          -
// - afnix:wax module - xhtml tbody node class implementation                -
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

#include "QuarkZone.hpp"
#include "Exception.hpp"
#include "XhtmlTbody.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - private section                                                       -
  // -------------------------------------------------------------------------

  // the tbody node name
  static const String XML_TAG_NAME = "tbody";

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a default tbody node

  XhtmlTbody::XhtmlTbody (void) : XhtmlTelem (XML_TAG_NAME) {
  }

  // create a xhtml tbody node with a class

  XhtmlTbody::XhtmlTbody (const String& cls) : XhtmlTelem (XML_TAG_NAME, cls) {
  }

  // create a xhtml tbody node with a class with a default tr class

  XhtmlTbody::XhtmlTbody (const String& cls, 
			  const String& trc) : XhtmlTelem (XML_TAG_NAME, 
							   cls, trc) {
  }

  // create a xhtml tbody node with a class with a default tr and tx class

  XhtmlTbody::XhtmlTbody (const String& cls, const String& trc,
			  const String& txc) : XhtmlTelem (XML_TAG_NAME,
							   cls, trc, txc) {
  }

  // return the class name

  String XhtmlTbody::repr (void) const {
    return "XhtmlTbody";
  }

  // -------------------------------------------------------------------------
  // - object section                                                        -
  // -------------------------------------------------------------------------

  // create a new object in a generic way

  Object* XhtmlTbody::mknew (Vector* argv) {
    long argc = (argv == nullptr) ? 0 : argv->length ();
    // check for 0 argument
    if (argc == 0) return new XhtmlTbody;
    // check for 1 argument
    if (argc == 1) {
      String cls = argv->getstring (0);
      return new XhtmlTbody (cls);
    }
    // check for 2 arguments
    if (argc == 2) {
      String cls = argv->getstring (0);
      String trc = argv->getstring (1);
      return new XhtmlTbody (cls, trc);
    }
    // check for 3 arguments
    if (argc == 3) {
      String cls = argv->getstring (0);
      String trc = argv->getstring (1);
      String txc = argv->getstring (2);
      return new XhtmlTbody (cls, trc, txc);
    }
    // wrong arguments
    throw Exception ("argument-error", 
		     "too many arguments with xhtml tbody constructor");
  }
}
