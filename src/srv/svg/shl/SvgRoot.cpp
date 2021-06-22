// ---------------------------------------------------------------------------
// - SvgRoot.cpp                                                             -
// - afnix:svg service - svg root node class implementation                  -
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

#include "XmlDecl.hpp"
#include "SvgRoot.hpp"
#include "Evaluable.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"
#include "XmlComment.hpp"
#include "XmlDoctype.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - private section                                                       -
  // -------------------------------------------------------------------------

  // the document content type
  static const String XML_MIME_TYPE = "image/svg+xml";
  // the default svg doctype root tag
  static const String XML_DOCT_ROOT = "svg";
  // the default svg public identifier
  static const String XML_DOCT_PUBL = "-//W3C//DTD SVG 1.1//EN";
  // the default svg system identifier
  static const String XML_DOCT_SYSL =
    "http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd";
  
  // the xml svg tag nam1e
  static const String XML_SVG_NAME = "svg:svg";
  
  // the xml svg comment
  static const String XML_HTML_COMMENT = 
    " afnix scalable vector graphic (svg) service ";
  
  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a svg root node

  SvgRoot::SvgRoot (void) : Mime (XML_MIME_TYPE) {
    // create a default xml declaration
    addchild (new XmlDecl);
    // create a xml document type
    addchild (new XmlDoctype (XML_DOCT_ROOT, XML_DOCT_PUBL, XML_DOCT_SYSL));
    // add the svg comment
    addchild (new XmlComment (XML_HTML_COMMENT));
  }

  // create a svg root node with a fragment

  SvgRoot::SvgRoot (SvgFragment* frag) :  Mime (XML_MIME_TYPE) {
    // create a default xml declaration
    addchild (new XmlDecl);
    // create a xml document type
    addchild (new XmlDoctype (XML_DOCT_ROOT, XML_DOCT_PUBL, XML_DOCT_SYSL));
    // add the svg comment
    addchild (new XmlComment (XML_HTML_COMMENT));
    // add the svg fragment
    if (frag != nullptr) addchild (frag);
  }

  // create a svg root node by width and height

  SvgRoot::SvgRoot (const long  width, const long height) : 
    Mime (XML_MIME_TYPE) {
    // create a default xml declaration
    addchild (new XmlDecl);
    // create a xml document type
    addchild (new XmlDoctype (XML_DOCT_ROOT, XML_DOCT_PUBL, XML_DOCT_SYSL));
    // add the svg comment
    addchild (new XmlComment (XML_HTML_COMMENT));
    // add the svg node
    addchild (new SvgFragment (width, height));
  }

  // return the class name

  String SvgRoot::repr (void) const {
    return "SvgRoot";
  }

  // get the svg node

  SvgFragment* SvgRoot::getsvg (void) const {
    rdlock ();
    try {
      XmlNode* node = lookchild (XML_SVG_NAME);
      SvgFragment* result = dynamic_cast <SvgFragment*> (node);
      if (result == nullptr) {
	throw Exception ("lookup-error", "cannot find svg fragment node");
      }
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    } 
  }

  // write a node to a buffer

  void SvgRoot::write (Buffer& buf) const {
    rdlock ();
    try {
      XmlRoot::write (buf);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // write a node to an output stream

  void SvgRoot::write (OutputStream& os) const {
    rdlock ();
    try {
      XmlRoot::write (os);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // -------------------------------------------------------------------------
  // - object section                                                        -
  // -------------------------------------------------------------------------

  // the quark zone
  static const long QUARK_ZONE_LENGTH = 1;
  static QuarkZone  zone (QUARK_ZONE_LENGTH);

  // the object supported quarks
  static const long QUARK_GETSVG = zone.intern ("get-svg");

  // create a new object in a generic way

  Object* SvgRoot::mknew (Vector* argv) {
    long argc = (argv == nullptr) ? 0 : argv->length ();
    // check for 0 argument
    if (argc == 0) return new SvgRoot;
    // check for 1 argument
    if (argc == 1) {
      Object* obj = argv->get (0);
      SvgFragment* frag = dynamic_cast <SvgFragment*> (obj);
      if (frag == nullptr) {
	throw Exception ("type-error", "invalid fragment object for svg root",
			 Object::repr (obj));
      }
      return new SvgRoot (frag);
    }
    // check for 2 arguments
    if (argc == 2) {
      long width  = argv->getlong (0);
      long height = argv->getlong (1);
      return new SvgRoot (width, height);
    }
    // wrong arguments
    throw Exception ("argument-error", 
		     "too many arguments with svg root constructor");
  }

  // return true if the given quark is defined

  bool SvgRoot::isquark (const long quark, const bool hflg) const {
    rdlock ();
    if (zone.exists (quark) == true) {
      unlock ();
      return true;
    }
    bool result = hflg ? Mime::isquark (quark, hflg) : false;
    if (result == false) {
      result = hflg ? XmlRoot::isquark (quark, hflg) : false;
    }
    unlock ();
    return result;
  }

  // apply this object with a set of arguments and a quark

  Object* SvgRoot::apply (Evaluable* zobj, Nameset* nset, const long quark,
			    Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();
    
    // dispatch 0 argument
    if (argc == 0) {
      if (quark == QUARK_GETSVG) {
	rdlock ();
	try {
	  Object* result = getsvg ();
	  zobj->post (result);
	  unlock ();
	  return result;
	} catch (...) {
	  unlock ();
	  throw;
	}
      }
    }
    // check the mime object
    if (Mime::isquark (quark, true) == true)
      return Mime::apply (zobj, nset, quark, argv);
    // default to xml root object
    return XmlRoot::apply (zobj, nset, quark, argv);
  }
}
