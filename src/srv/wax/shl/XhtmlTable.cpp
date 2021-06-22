// ---------------------------------------------------------------------------
// - XhtmlTable.cpp                                                          -
// - afnix:wax module - xhtml table node class implementation                -
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

#include "QuarkZone.hpp"
#include "Exception.hpp"
#include "XhtmlTable.hpp"
#include "XhtmlCaption.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - private section                                                       -
  // -------------------------------------------------------------------------

  // the table node name
  static const String XML_TAG_NAME = "table";

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a default table node

  XhtmlTable::XhtmlTable (void) : XhtmlTelem (XML_TAG_NAME) {
  }

  // create a xhtml table node with a class

  XhtmlTable::XhtmlTable (const String& cls) : XhtmlTelem (XML_TAG_NAME, cls) {
  }

  // create a xhtml table node with a class with a default tr class

  XhtmlTable::XhtmlTable (const String& cls, 
			  const String& trc) : XhtmlTelem (XML_TAG_NAME, 
							   cls, trc) {
  }

  // create a xhtml table node with a class with a default tr and tx class

  XhtmlTable::XhtmlTable (const String& cls, const String& trc,
			  const String& txc) : XhtmlTelem (XML_TAG_NAME,
							   cls, trc, txc) {
  }

  // return the class name

  String XhtmlTable::repr (void) const {
    return "XhtmlTable";
  }

  // set the table caption text

  void XhtmlTable::setcaption (const String& text) {
    wrlock ();
    try {
      XmlNode* node = new XhtmlCaption (text);
      addchild (node);
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
  static const long QUARK_SETCAPT = zone.intern ("set-caption");

  // create a new object in a generic way

  Object* XhtmlTable::mknew (Vector* argv) {
    long argc = (argv == nullptr) ? 0 : argv->length ();
    // check for 0 argument
    if (argc == 0) return new XhtmlTable;
    // check for 1 argument
    if (argc == 1) {
      String cls = argv->getstring (0);
      return new XhtmlTable (cls);
    }
    // check for 2 arguments
    if (argc == 2) {
      String cls = argv->getstring (0);
      String trc = argv->getstring (1);
      return new XhtmlTable (cls, trc);
    }
    // check for 3 arguments
    if (argc == 3) {
      String cls = argv->getstring (0);
      String trc = argv->getstring (1);
      String txc = argv->getstring (2);
      return new XhtmlTable (cls, trc, txc);
    }
    // wrong arguments
    throw Exception ("argument-error", 
		     "too many arguments with xhtml table constructor");
  }

  // return true if the given quark is defined

  bool XhtmlTable::isquark (const long quark, const bool hflg) const {
    rdlock ();
    if (zone.exists (quark) == true) {
      unlock ();
      return true;
    }
    bool result = hflg ? XhtmlTelem::isquark (quark, hflg) : false;
    unlock ();
    return result;
  }

  // apply this object with a set of arguments and a quark

  Object* XhtmlTable::apply (Evaluable* zobj, Nameset* nset, const long quark,
			    Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();
    
    // dispatch 1 argument
    if (argc == 1) {
      if (quark == QUARK_SETCAPT) {
	String text = argv->getstring (0);	
	setcaption (text);
	return nullptr;
      }
    }
    // call the table element method
    return XhtmlTelem::apply (zobj, nset, quark, argv);
  }
}
