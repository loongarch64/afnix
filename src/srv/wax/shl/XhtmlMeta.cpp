// ---------------------------------------------------------------------------
// - XhtmlMeta.cpp                                                           -
// - afnix:wax module - xhtml meta node class implementation                 -
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

#include "XhtmlMeta.hpp"
#include "Exception.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - private section                                                       -
  // -------------------------------------------------------------------------

  // the meta node name
  static const String XML_TAG_NAME  = "meta";
  // the meta data name attribute
  static const String XML_NAME_ATTR = "name";
  // the meta data content attribute
  static const String XML_CNTS_ATTR = "content";

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a xhtml meta node by name and content

  XhtmlMeta::XhtmlMeta (const String& name, 
			const String& cnts) : XmlTag (XML_TAG_NAME) {
    d_eflg = true;
    setattr (XML_NAME_ATTR, name);
    setattr (XML_CNTS_ATTR, cnts);
  }

  // return the class name

  String XhtmlMeta::repr (void) const {
    return "XhtmlMeta";
  }

  // -------------------------------------------------------------------------
  // - object section                                                        -
  // -------------------------------------------------------------------------

  // create a new object in a generic way

  Object* XhtmlMeta::mknew (Vector* argv) {
    long argc = (argv == nullptr) ? 0 : argv->length ();
    // check for 2 arguments
    if (argc == 2) {
      String name = argv->getstring (0);
      String cnts = argv->getstring (1);
      return new XhtmlMeta (name, cnts);
    }
    // wrong arguments
    throw Exception ("argument-error", 
		     "too many arguments with xhtml meta constructor");
  }
}
