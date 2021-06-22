// ---------------------------------------------------------------------------
// - SvgMime.cpp                                                             -
// - afnix:svg service - svg mime class implementation                       -
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

#include "SvgMime.hpp"
#include "Evaluable.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - private section                                                       -
  // -------------------------------------------------------------------------

  // the document content type
  static const String SVG_MIME_TYPE = "image/svg+xml"; 

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a default svg mime document

  SvgMime::SvgMime (void) {
    d_mime = SVG_MIME_TYPE;
  }

  // create a svg mime document by name

  SvgMime::SvgMime (const String& name) : XmlDocument (name) {
    d_mime = SVG_MIME_TYPE;
  }

  // create a svg mime document by name and stream

  SvgMime::SvgMime (const String& name, 
		    InputStream* is) : XmlDocument (name, is) {
    d_mime = SVG_MIME_TYPE;
  }
  
  // return the class name

  String SvgMime::repr (void) const {
    return "SvgMime";
  }

  // write a node to a buffer

  void SvgMime::write (Buffer& buf) const {
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

  void SvgMime::write (OutputStream& os) const {
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

  Object* SvgMime::mknew (Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();
    // check for 0 argument
    if (argc == 0) return new SvgMime;
    // check for 1 argument
    if (argc == 1) {
      String name = argv->getstring (0);
      return new SvgMime (name);
    }
    // check for 2 arguments
    if (argc == 2) {
      // get the document name
      String name = argv->getstring (0);
      // get the object and check
      Object* obj = argv->get (1);
      // check for an input stream
      InputStream* is = dynamic_cast <InputStream*> (obj);
      if (is != nullptr) return new SvgMime (name, is);
      // invalid object
      throw Exception ("type-error", 
		       "invalid object with svg mime constructor",
		       Object::repr (obj));	
    }
    // wrong arguments
    throw Exception ("argument-error", 
		     "too many arguments with svg mime constructor");
  }
  
  // return true if the given quark is defined
  
  bool SvgMime::isquark (const long quark, const bool hflg) const {
    rdlock ();
    bool result = hflg ? Mime::isquark (quark, hflg) : false;
    if (result == false) {
      result = hflg ? XmlDocument::isquark (quark, hflg) : false;
    }
    unlock ();
    return result;
  }

  // apply this object with a set of arguments and a quark
  
  Object* SvgMime::apply (Evaluable* zobj, Nameset* nset, const long quark,
			  Vector* argv) {
    // check the mime object
    if (Mime::isquark (quark, true) == true) {
      return Mime::apply (zobj, nset, quark, argv);
    }
    // default to xml document object
    return XmlDocument::apply (zobj, nset, quark, argv);
  }
}
