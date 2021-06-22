// ---------------------------------------------------------------------------
// - XhtmlLink.cpp                                                           -
// - afnix:wax module - xhtml link node class implementation                 -
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

#include "XhtmlLink.hpp"
#include "Exception.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - private section                                                       -
  // -------------------------------------------------------------------------

  // the link node name
  static const String XML_TAG_NAME  = "link";
  // the link href attribute
  static const String XML_HREF_ATTR = "href";
  // the link type attribute
  static const String XML_TYPE_ATTR = "type";
  // the link rel attribute
  static const String XML_LREL_ATTR = "rel";

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a xhtml link node by reference

  XhtmlLink::XhtmlLink (const String& href) : XmlTag (XML_TAG_NAME) {
    d_eflg = true;
    setattr (XML_HREF_ATTR, href);
  }

  // create a xhtml link node by reference and type

  XhtmlLink::XhtmlLink (const String& href, 
			const String& type) : XmlTag (XML_TAG_NAME) {
    d_eflg = true;
    setattr (XML_HREF_ATTR, href);
    setattr (XML_TYPE_ATTR, type);
  }

  // create a xhtml link node by reference, type and relation

  XhtmlLink::XhtmlLink (const String& href, const String& type, 
			const String& lrel) : XmlTag (XML_TAG_NAME) {
    d_eflg = true;
    setattr (XML_HREF_ATTR, href);
    if (type.isnil() == false) setattr (XML_TYPE_ATTR, type);
    setattr (XML_LREL_ATTR, lrel);
  }

  // return the class name

  String XhtmlLink::repr (void) const {
    return "XhtmlLink";
  }

  // -------------------------------------------------------------------------
  // - object section                                                        -
  // -------------------------------------------------------------------------

  // create a new object in a generic way

  Object* XhtmlLink::mknew (Vector* argv) {
    long argc = (argv == nullptr) ? 0 : argv->length ();
    // check for 1 argument
    if (argc == 1) {
      String href = argv->getstring (0);
      return new XhtmlLink (href);
    }
    // check for 2 arguments
    if (argc == 2) {
      String href = argv->getstring (0);
      String type = argv->getstring (1);
      return new XhtmlLink (href, type);
    }
    // check for 3 arguments
    if (argc == 3) {
      String href = argv->getstring (0);
      String type = argv->getstring (1);
      String lrel = argv->getstring (2);
      return new XhtmlLink (href, type, lrel);
    }
    // wrong arguments
    throw Exception ("argument-error", 
		     "too many arguments with xhtml link constructor");
  }
}
