// ---------------------------------------------------------------------------
// - XhtmlScript.cpp                                                         -
// - afnix:wax module - xhtml script node class implementation               -
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

#include "Boolean.hpp"
#include "XmlData.hpp"
#include "Exception.hpp"
#include "XhtmlScript.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - private section                                                       -
  // -------------------------------------------------------------------------

  // the script node name
  static const String XML_TAG_NAME  = "script";
  // the script type attribute
  static const String XML_TYPE_ATTR = "type";
  // the script src attribute
  static const String XML_XSRC_ATTR = "src";
  /// the default cdata flag
  static const bool   XML_CDAT_FLAG = false;

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a xhtml script node with a type

  XhtmlScript::XhtmlScript (const String& type) : XmlTag (XML_TAG_NAME) {
    // set the type attribute
    setattr  (XML_TYPE_ATTR, type);
    // fix the data node
    d_cflg = XML_CDAT_FLAG;
    p_data = nullptr;
  }

  // create a xhtml script node with a type and a cdata flag

  XhtmlScript::XhtmlScript (const String& type, 
			    const bool cflg) : XmlTag (XML_TAG_NAME) {
    // set the type attribute
    setattr  (XML_TYPE_ATTR, type);
    // fix the data node
    d_cflg = cflg;
    p_data = nullptr;
  }


  // create a xhtml script node with a type and url

  XhtmlScript::XhtmlScript (const String& type,
			    const String& url) : XmlTag (XML_TAG_NAME) {
    // set the node attributes
    setattr  (XML_TYPE_ATTR, type);
    setattr  (XML_XSRC_ATTR, url);
    // fix the data node
    d_cflg = XML_CDAT_FLAG;
    p_data = nullptr;
  }

  // destroy this script node

  XhtmlScript::~XhtmlScript (void) {
    Object::dref (p_data);
  }

  // return the class name

  String XhtmlScript::repr (void) const {
    return "XhtmlScript";
  }

  // parse a string and add it to the cdata node

  void XhtmlScript::parse (const String& s) {
    wrlock ();
    try {
      // check if the cdata node exists
      if ((p_data == nullptr) && (d_cflg == true)) {
	// add the cdata node
	Object::iref (p_data = new XmlData);
	addchild (p_data);
	// start with a new line
	p_data->parse (eolq);	
      }
      if (d_cflg == true) {
	p_data->parse (s);
      } else {
	XmlNode::parse (s);
      }
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

  Object* XhtmlScript::mknew (Vector* argv) {
    long argc = (argv == nullptr) ? 0 : argv->length ();
    // check for 1 argument
    if (argc == 1) {
      String type = argv->getstring (0);
      return new XhtmlScript (type);
    }
    // check for 2 arguments
    if (argc == 2) {
      // get the type
      String type = argv->getstring (0);
      // get the obejct and check
      Object* obj = argv->get (1);
      // check for a string
      String* sobj = dynamic_cast <String*> (obj);
      if (sobj != nullptr) return new XhtmlScript (type, *sobj);
      // check for a boolean
      Boolean* bobj = dynamic_cast <Boolean*> (obj);
      if (bobj != nullptr) return new XhtmlScript (type, bobj->tobool ());
      // type error
      throw Exception ("type-error", 
		       "invalid object with xhtml script constructor",
                      Object::repr (obj));
    }
    // wrong arguments
    throw Exception ("argument-error", 
		     "too many arguments with xhtml script constructor");
  }
}
