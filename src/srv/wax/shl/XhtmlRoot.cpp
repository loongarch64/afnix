// ---------------------------------------------------------------------------
// - XhtmlRoot.cpp                                                           -
// - afnix:wax service - xhtml root node class implementation                -
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
#include "Evaluable.hpp"
#include "XhtmlRoot.hpp"
#include "XhtmlHtml.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"
#include "XmlComment.hpp"
#include "XmlDoctype.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - private section                                                       -
  // -------------------------------------------------------------------------

  // the document content type
  static const String XML_MIME_TYPE = "application/xhtml+xml"; 
  // the default xhtml doctype root tag
  static const String XML_DOCT_ROOT = "html";  
  // the xml html tag name
  static const String XML_HTML_NAME = "html";
  // the xml html comment
  static const String XML_HTML_COMMENT = 
    " afnix web application extension (wax) service ";
  
  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------
  
  // create a xhtml root node with a head title

  XhtmlRoot::XhtmlRoot (const String& title) : Mime (XML_MIME_TYPE) {
    // create a default xml declaration
    addchild (new XmlDecl);
    // create a xml document type
    addchild (new XmlDoctype (XML_DOCT_ROOT));

    // add the html comment
    addchild (new XmlComment (XML_HTML_COMMENT));
    // add the html node
    addchild (new XhtmlHtml (title));
  }

  // return the class name

  String XhtmlRoot::repr (void) const {
    return "XhtmlRoot";
  }

  // get the xhtml html node

  XhtmlHtml* XhtmlRoot::gethtml (void) const {
    rdlock ();
    try {
      XmlNode* node = lookchild (XML_HTML_NAME);
      XhtmlHtml* result = dynamic_cast <XhtmlHtml*> (node);
      if (result == nullptr) {
	throw Exception ("lookup-error", "cannot find xhtml html node");
      }
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    } 
  }

  // get the xhtml head node

  XhtmlHead* XhtmlRoot::gethead (void) const {
    rdlock ();
    try {
      XhtmlHtml* html = gethtml ();
      if (html == nullptr) {
	throw Exception ("lookup-error", "cannot find xhtml html node");
      }
      XhtmlHead* result = html->gethead ();
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    } 
  }

  // get the xhtml body node

  XhtmlBody* XhtmlRoot::getbody (void) const {
    rdlock ();
    try {
      XhtmlHtml* html = gethtml ();
      if (html == nullptr) {
	throw Exception ("lookup-error", "cannot find xhtml html node");
      }
      XhtmlBody* result = html->getbody ();
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    } 
  }

  // write a node to a buffer

  void XhtmlRoot::write (Buffer& buf) const {
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

  void XhtmlRoot::write (OutputStream& os) const {
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
  static const long QUARK_ZONE_LENGTH = 3;
  static QuarkZone  zone (QUARK_ZONE_LENGTH);

  // the object supported quarks
  static const long QUARK_GETHTML = zone.intern ("get-html");
  static const long QUARK_GETHEAD = zone.intern ("get-head");
  static const long QUARK_GETBODY = zone.intern ("get-body");

  // create a new object in a generic way

  Object* XhtmlRoot::mknew (Vector* argv) {
    long argc = (argv == nullptr) ? 0 : argv->length ();
    // check for 1 argument
    if (argc == 1) {
      String title = argv->getstring (0);
      return new XhtmlRoot (title);
    }
    // wrong arguments
    throw Exception ("argument-error", 
		     "too many arguments with xhtml root constructor");
  }

  // return true if the given quark is defined

  bool XhtmlRoot::isquark (const long quark, const bool hflg) const {
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

  Object* XhtmlRoot::apply (Evaluable* zobj, Nameset* nset, const long quark,
			    Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();
    
    // dispatch 0 argument
    if (argc == 0) {
      if (quark == QUARK_GETHTML) {
	rdlock ();
	try {
	  Object* result = gethtml ();
	  zobj->post (result);
	  unlock ();
	  return result;
	} catch (...) {
	  unlock ();
	  throw;
	}
      }
      if (quark == QUARK_GETHEAD) {
	rdlock ();
	try {
	  Object* result = gethead ();
	  zobj->post (result);
	  unlock ();
	  return result;
	} catch (...) {
	  unlock ();
	  throw;
	}
      }
      if (quark == QUARK_GETBODY) {
	rdlock ();
	try {
	  Object* result = getbody ();
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
