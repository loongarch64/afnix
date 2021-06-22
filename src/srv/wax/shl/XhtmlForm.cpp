// ---------------------------------------------------------------------------
// - XhtmlForm.cpp                                                           -
// - afnix:wax module - xhtml form node class implementation                 -
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

#include "XhtmlForm.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - private section                                                       -
  // -------------------------------------------------------------------------

  // the form node name
  static const String XML_TAG_NAME = "form";
  // the form action attribute
  static const String XML_ACT_ATTR = "action";
  // the form method attribute
  static const String XML_MTH_ATTR = "method";

  // validate the method and return a string
  static String xhtml_form_method (const String& meth) {
    String result = meth.tolower ();
    if ((result == "get") || (result == "post")) return result;
    throw Exception ("xhtml-form", "invalid form method", meth);
  }

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a xhtml form node by action and method

  XhtmlForm::XhtmlForm (const String& href, 
			const String& meth) : XmlTag (XML_TAG_NAME) {
    setattr (XML_ACT_ATTR, href);
    setattr (XML_MTH_ATTR, xhtml_form_method (meth));
  }

  // return the class name

  String XhtmlForm::repr (void) const {
    return "XhtmlForm";
  }

  // -------------------------------------------------------------------------
  // - object section                                                        -
  // -------------------------------------------------------------------------

  // create a new object in a generic way

  Object* XhtmlForm::mknew (Vector* argv) {
    long argc = (argv == nullptr) ? 0 : argv->length ();
    // check for 2 arguments
    if (argc == 2) {
      String href = argv->getstring (0);
      String meth = argv->getstring (1);
      return new XhtmlForm (href, meth);
    }
    // wrong arguments
    throw Exception ("argument-error", 
		     "too many arguments with xhtml form constructor");
  }
}
