// ---------------------------------------------------------------------------
// - XhtmlThead.cpp                                                          -
// - afnix:wax module - xhtml thead node class implementation                -
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
#include "XhtmlThead.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - private section                                                       -
  // -------------------------------------------------------------------------

  // the thead node name
  static const String XML_TAG_NAME = "thead";

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a default thead node

  XhtmlThead::XhtmlThead (void) : XhtmlTelem (XML_TAG_NAME) {
  }

  // create a xhtml thead node with a class

  XhtmlThead::XhtmlThead (const String& cls) : XhtmlTelem (XML_TAG_NAME, cls) {
  }

  // create a xhtml thead node with a class with a default tr class

  XhtmlThead::XhtmlThead (const String& cls, 
			  const String& trc) : XhtmlTelem (XML_TAG_NAME, 
							   cls, trc) {
  }

  // create a xhtml thead node with a class with a default tr and tx class

  XhtmlThead::XhtmlThead (const String& cls, const String& trc,
			  const String& txc) : XhtmlTelem (XML_TAG_NAME,
							   cls, trc, txc) {
  }

  // return the class name

  String XhtmlThead::repr (void) const {
    return "XhtmlThead";
  }

  // -------------------------------------------------------------------------
  // - object section                                                        -
  // -------------------------------------------------------------------------

  // create a new object in a generic way

  Object* XhtmlThead::mknew (Vector* argv) {
    long argc = (argv == nullptr) ? 0 : argv->length ();
    // check for 0 argument
    if (argc == 0) return new XhtmlThead;
    // check for 1 argument
    if (argc == 1) {
      String cls = argv->getstring (0);
      return new XhtmlThead (cls);
    }
    // check for 2 arguments
    if (argc == 2) {
      String cls = argv->getstring (0);
      String trc = argv->getstring (1);
      return new XhtmlThead (cls, trc);
    }
    // check for 3 arguments
    if (argc == 3) {
      String cls = argv->getstring (0);
      String trc = argv->getstring (1);
      String txc = argv->getstring (2);
      return new XhtmlThead (cls, trc, txc);
    }
    // wrong arguments
    throw Exception ("argument-error", 
		     "too many arguments with xhtml thead constructor");
  }
}
