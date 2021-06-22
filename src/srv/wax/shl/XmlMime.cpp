// ---------------------------------------------------------------------------
// - XmlMime.cpp                                                             -
// - afnix:wax service - xml mime class implementation                       -
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

#include "XmlMime.hpp"
#include "Evaluable.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - private section                                                       -
  // -------------------------------------------------------------------------

  // the xml mime definitions
  static const String PN_XML_EXTN = "xml"; 
  static const String PN_XML_MIME = Mime::tomime (PN_XML_EXTN, false);

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a default xml mime document

  XmlMime::XmlMime (void) {
    d_mime = PN_XML_MIME;
  }

  // create a xml mime document by name

  XmlMime::XmlMime (const String& name) : XmlDocument (name) {
    d_mime = PN_XML_MIME;
  }

  // create a xml mime by document

  XmlMime::XmlMime (const XmlDocument& xdoc) : XmlDocument (xdoc) {
    d_mime = PN_XML_MIME;
  }

  // create a xml mime document by name and stream

  XmlMime::XmlMime (const String& name, 
		    InputStream* is) : XmlDocument (name, is) {
    d_mime = PN_XML_MIME;
  }

  // create a xml mime document by name and xml root node

  XmlMime::XmlMime (const String& name, XmlRoot* root) : 
    XmlDocument (name, root) {
    d_mime = PN_XML_MIME;
  }

  // return the class name

  String XmlMime::repr (void) const {
    return "XmlMime";
  }

  // write a node to a buffer

  void XmlMime::write (Buffer& buf) const {
    rdlock ();
    try {
      // get the xml root node
      XmlRoot* node = getroot ();
      // write into the buffer
      if (node != nullptr) node->write (buf);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // write a node to an output stream

  void XmlMime::write (OutputStream& os) const {
    rdlock ();
    try {
      // get the xml root node
      XmlRoot* node = getroot ();
      // write into the stream
      if (node != nullptr) node->write (os);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // -------------------------------------------------------------------------
  // - object section                                                        -
  // -------------------------------------------------------------------------

  // create a new object in a generic way

  Object* XmlMime::mknew (Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();
    // check for 0 argument
    if (argc == 0) return new XmlMime;
    // check for 1 argument
    if (argc == 1) {
      // get the object and check
      Object* obj = argv->get (0);
      // check for a string
      String* sobj = dynamic_cast <String*> (obj);
      if (sobj != nullptr) return new XmlMime (*sobj);
      // check for a xml document
      XmlDocument* xdoc = dynamic_cast <XmlDocument*> (obj);
      if (xdoc != nullptr) return new XmlMime (*xdoc);
      // invalid type
      throw Exception ("type-error", "invalid object with XmlMime constructor",
		       Object::repr (obj));
    }
    // check for 2 arguments
    if (argc == 2) {
      // get the document name
      String name = argv->getstring (0);
      // get the object and check
      Object* obj = argv->get (1);
      // check for an input stream
      InputStream* is = dynamic_cast <InputStream*> (obj);
      if (is != nullptr) return new XmlMime (name, is);
      // check for xml root node
      XmlRoot* root = dynamic_cast <XmlRoot*> (obj);
      if (root != nullptr) return new XmlMime (name, root);
      // invalid object
      throw Exception ("type-error", 
		       "invalid object with xml mime constructor",
		       Object::repr (obj));	
    }
    // wrong arguments
    throw Exception ("argument-error", 
		     "too many arguments with xml mime constructor");
  }

  // return true if the given quark is defined

  bool XmlMime::isquark (const long quark, const bool hflg) const {
    rdlock ();
    try {
      bool result = hflg ? Mime::isquark (quark, hflg) : false;
      if (result == false) {
	result = hflg ? XmlDocument::isquark (quark, hflg) : false;
      }
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // apply this object with a set of arguments and a quark

  Object* XmlMime::apply (Evaluable* zobj, Nameset* nset, const long quark,
			  Vector* argv) {
    // check the mime object
    if (Mime::isquark (quark, true) == true) {
      return Mime::apply (zobj, nset, quark, argv);
    }
    // default to xml document object
    return XmlDocument::apply (zobj, nset, quark, argv);
  }
}
