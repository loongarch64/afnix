// ---------------------------------------------------------------------------
// - XhtmlHead.cpp                                                           -
// - afnix:wax module - xhtml head node class implementation                 -
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
#include "XhtmlHead.hpp"
#include "XhtmlMeta.hpp"
#include "XhtmlLink.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"
#include "XhtmlTitle.hpp"
#include "XhtmlScript.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - private section                                                       -
  // -------------------------------------------------------------------------

  // the head node name
  static const String XML_TAG_NAME  = "head";
  // the xhtml title name
  static const String XML_TITL_NAME = "title";
  // the charset meta
  static const String XML_CSET_NAME = "charset";
  // the charset code
  static const String XML_CSET_CODE = "utf-8";
  // the style link type mime value
  static const String XML_LINK_MIME = "";
  // the style link relation value
  static const String XML_LINK_LREL = "stylesheet";

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a xhtml head node by title

  XhtmlHead::XhtmlHead (const String& title) : XmlTag (XML_TAG_NAME) {
    addchild (new XhtmlTitle (title));
  }

  // return the class name

  String XhtmlHead::repr (void) const {
    return "XhtmlHead";
  }

  // set the head title node

  void XhtmlHead::settitle (const String& title) {
    wrlock ();
    try {
      XmlNode* node = lookchild (XML_TITL_NAME);
      auto   result = dynamic_cast <XhtmlTitle*> (node);
      if (result == nullptr) {
	throw Exception ("lookup-error", "cannot find xhtml title node");
      }
      result->settitle (title);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    } 
  }

  // add a default charset meta

  void XhtmlHead::addcset (void) {
    wrlock();
    try {
      addmeta (XML_CSET_NAME, XML_CSET_CODE);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // add a meta data node by name and content

  void XhtmlHead::addmeta (const String& name, const String& cnts) {
    wrlock ();
    try {
      addchild (new XhtmlMeta (name, cnts));
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // add a style node with a url

  void XhtmlHead::addstyle (const String& url) {
    wrlock ();
    try {
      addchild (new XhtmlLink (url, XML_LINK_MIME, XML_LINK_LREL));
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // add a script node by type and url

  void XhtmlHead::addscript (const String& type, const String& url) {
    wrlock ();
    try {
      addchild (new XhtmlScript (type, url));
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
  static const long QUARK_ZONE_LENGTH = 5;
  static QuarkZone  zone (QUARK_ZONE_LENGTH);

  // the object supported quarks
  static const long QUARK_ADDMETA   = zone.intern ("add-meta");
  static const long QUARK_ADDCSET   = zone.intern ("add-charset");
  static const long QUARK_SETTITLE  = zone.intern ("set-title");
  static const long QUARK_ADDSTYLE  = zone.intern ("add-style");
  static const long QUARK_ADDSCRIPT = zone.intern ("add-script");

  // create a new object in a generic way

  Object* XhtmlHead::mknew (Vector* argv) {
    long argc = (argv == nullptr) ? 0 : argv->length ();
    // check for 1 argument
    if (argc == 1) {
      String title = argv->getstring (0);
      return new XhtmlHead (title);
    }
    // wrong arguments
    throw Exception ("argument-error", 
		     "too many arguments with xhtml head constructor");
  }

  // return true if the given quark is defined

  bool XhtmlHead::isquark (const long quark, const bool hflg) const {
    rdlock ();
    try {
      if (zone.exists (quark) == true) {
	unlock ();
	return true;
      }
      bool result = hflg ? XmlTag::isquark (quark, hflg) : false;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // apply this object with a set of arguments and a quark

  Object* XhtmlHead::apply (Evaluable* zobj, Nameset* nset, const long quark,
			    Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();
    // dispatch 0 argument
    if (argc == 0) {
      if (quark == QUARK_ADDCSET) {
	addcset ();
	return nullptr;
      }
    } 
    // dispatch 1 argument
    if (argc == 1) {
      if (quark == QUARK_SETTITLE) {
	String title = argv->getstring (0);
	settitle (title);
	return nullptr;
      }
      if (quark == QUARK_ADDSTYLE) {
	String url = argv->getstring (0);
	addstyle (url);
	return nullptr;
      }
    }
    // dispatch 2 arguments
    if (argc == 2) {
      if (quark == QUARK_ADDMETA) {
	String name = argv->getstring (0);
	String cnts = argv->getstring (1);
	addmeta (name, cnts);
	return nullptr;
      }
      if (quark == QUARK_ADDSCRIPT) {
	String type = argv->getstring (0);
	String url  = argv->getstring (1);
	addscript (type, url);
	return nullptr;
      }
    }
    // call the xml tag method
    return XmlTag::apply (zobj, nset, quark, argv);
  }
}
