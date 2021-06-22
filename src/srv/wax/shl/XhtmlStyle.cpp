// ---------------------------------------------------------------------------
// - XhtmlStyle.cpp                                                          -
// - afnix:wax module - xhtml style node class implementation                -
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

#include "XmlText.hpp"
#include "Exception.hpp"
#include "XhtmlStyle.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - private section                                                       -
  // -------------------------------------------------------------------------

  // the style node name
  static const String XML_TAG_NAME  = "style";
  // the style type attribute
  static const String XML_TYPE_ATTR = "type";
  // the style type value
  static const String XML_TYPE_XVAL = "text/css";

  // this procedure formats a style string with a url
  static String format_style_url (const String& url) {
    String result = "@import url(";
    result += url;
    result += ");";
    return result;
  }

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a xhtml style node with a url

  XhtmlStyle::XhtmlStyle (const String& url) : XmlTag (XML_TAG_NAME) {
    setattr  (XML_TYPE_ATTR, XML_TYPE_XVAL);
    addchild (new XmlText (format_style_url (url)));
  }

  // return the class name

  String XhtmlStyle::repr (void) const {
    return "XhtmlStyle";
  }

  // -------------------------------------------------------------------------
  // - object section                                                        -
  // -------------------------------------------------------------------------

  // create a new object in a generic way

  Object* XhtmlStyle::mknew (Vector* argv) {
    long argc = (argv == nullptr) ? 0 : argv->length ();
    // check for 1 argument
    if (argc == 1) {
      String url = argv->getstring (0);
      return new XhtmlStyle (url);
    }
    // wrong arguments
    throw Exception ("argument-error", 
		     "too many arguments with xhtml style constructor");
  }
}
