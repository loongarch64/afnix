// ---------------------------------------------------------------------------
// - XhtmlHtml.cpp                                                           -
// - afnix:wax module - xhtml html node class implementation                 -
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

#include "Evaluable.hpp"
#include "XhtmlHtml.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"
#include "XmlComment.hpp"
#include "XhtmlTitle.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - private section                                                       -
  // -------------------------------------------------------------------------

  // the html node name
  static const String XML_TAG_NAME  = "html";
  // the html head name
  static const String XML_HEAD_NAME = "head";
  // the html body name
  static const String XML_BODY_NAME = "body";
  // the xml namespace attribute name
  static const String XML_NSPC_NAME = "xmlns";
  // the xml namespace url
  static const String XML_NSPC_PATH = "http://www.w3.org/1999/xhtml";
  // the xml language attribute
  static const String XML_LANG_NAME = "xml:lang";
  // the default xml language
  static const String XML_LANG_XVAL = "en";

  // the html head comment
  static const String XML_HEAD_COMMENT = " page head ";
  // the html body comment
  static const String XML_BODY_COMMENT = " page body ";

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a xhtml html node by title

  XhtmlHtml::XhtmlHtml (const String& title) : XmlTag (XML_TAG_NAME) {
    // add the xml namespace attribute
    setattr (XML_NSPC_NAME, XML_NSPC_PATH);
    // add the xml default language
    setattr (XML_LANG_NAME, XML_LANG_XVAL);

    // add the head comment node
    addchild (new XmlComment (XML_HEAD_COMMENT));
    // add the head node with a title
    addchild (new XhtmlHead (title));

    // add the body comment node
    addchild (new XmlComment (XML_BODY_COMMENT));
    // add an empty body
    addchild (new XhtmlBody);
  }

  // return the class name

  String XhtmlHtml::repr (void) const {
    return "XhtmlHtml";
  }

  // get the xhtml head node

  XhtmlHead* XhtmlHtml::gethead (void) const {
    rdlock ();
    try {
      XmlNode* node = lookchild (XML_HEAD_NAME);
      XhtmlHead* result = dynamic_cast <XhtmlHead*> (node);
      if (result == nullptr) {
	throw Exception ("lookup-error", "cannot find xhtml head node");
      }
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    } 
  }

  // get the xhtml body node

  XhtmlBody* XhtmlHtml::getbody (void) const {
    rdlock ();
    try {
      XmlNode* node = lookchild (XML_BODY_NAME);
      XhtmlBody* result = dynamic_cast <XhtmlBody*> (node);
      if (result == nullptr) {
	throw Exception ("lookup-error", "cannot find xhtml body node");
      }
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    } 
  }

  // -------------------------------------------------------------------------
  // - object section                                                        -
  // -------------------------------------------------------------------------

  // the quark zone
  static const long QUARK_ZONE_LENGTH = 2;
  static QuarkZone  zone (QUARK_ZONE_LENGTH);

  // the object supported quarks
  static const long QUARK_GETHEAD = zone.intern ("get-head");
  static const long QUARK_GETBODY = zone.intern ("get-body");

  // create a new object in a generic way

  Object* XhtmlHtml::mknew (Vector* argv) {
    long argc = (argv == nullptr) ? 0 : argv->length ();
    // check for 1 argument
    if (argc == 1) {
      String title = argv->getstring (0);
      return new XhtmlHtml (title);
    }
    // wrong arguments
    throw Exception ("argument-error", 
		     "too many arguments with xhtml html constructor");
  }

  // return true if the given quark is defined

  bool XhtmlHtml::isquark (const long quark, const bool hflg) const {
    rdlock ();
    if (zone.exists (quark) == true) {
      unlock ();
      return true;
    }
    bool result = hflg ? XmlTag::isquark (quark, hflg) : false;
    unlock ();
    return result;
  }

  // apply this object with a set of arguments and a quark

  Object* XhtmlHtml::apply (Evaluable* zobj, Nameset* nset, const long quark,
			    Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();
    
    // dispatch 0 argument
    if (argc == 0) {
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
    // call the xml tag method
    return XmlTag::apply (zobj, nset, quark, argv);
  }
}
