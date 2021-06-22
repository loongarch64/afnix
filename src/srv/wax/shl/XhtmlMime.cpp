// ---------------------------------------------------------------------------
// - XhtmlMime.cpp                                                             -
// - afnix:wax module - xml mime class implementation                        -
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

#include "XhtmlMime.hpp"
#include "Evaluable.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - private section                                                       -
  // -------------------------------------------------------------------------

  // the xhtml mime definitions
  static const String PN_XHT_EXTN = "xht"; 
  static const String PN_XHT_MIME = Mime::tomime (PN_XHT_EXTN, false);

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a default xml mime document

  XhtmlMime::XhtmlMime (void) {
    d_mime = PN_XHT_MIME;
  }

  // create a xml mime document by name

  XhtmlMime::XhtmlMime (const String& name) : XmlMime (name) {
    d_mime = PN_XHT_MIME;
  }

  // create a xml mime document by name and stream

  XhtmlMime::XhtmlMime (const String& name, 
			InputStream* is) : XmlMime (name, is) {
    d_mime = PN_XHT_MIME;
  }

  // return the class name

  String XhtmlMime::repr (void) const {
    return "XhtmlMime";
  }

  // -------------------------------------------------------------------------
  // - object section                                                        -
  // -------------------------------------------------------------------------

  // create a new object in a generic way

  Object* XhtmlMime::mknew (Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();
    // check for o argument
    if (argc == 0) return new XhtmlMime;
    // check for 1 argument
    if (argc == 1) {
      String name = argv->getstring (0);
      return new XhtmlMime (name);
    }
    // check for 2 arguments
    if (argc == 2) {
      // get the document name
      String name = argv->getstring (0);
      // get the object and check
      Object* obj = argv->get (1);
      // check for an input stream
      InputStream* is = dynamic_cast <InputStream*> (obj);
      if (is != nullptr) return new XhtmlMime (name, is);
      throw Exception ("type-error", 
		       "invalid object with document constructor",
		       Object::repr (obj));	
    }
    // wrong arguments
    throw Exception ("argument-error", 
		     "too many arguments with xml mime constructor");
  }
}
