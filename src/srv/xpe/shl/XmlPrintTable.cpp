// ---------------------------------------------------------------------------
// - XmlPrintTable.cpp                                                       -
// - afnix:xml module - xml print table class implementation                 -
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

#include "XmlTag.hpp"
#include "Evaluable.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"
#include "XmlPrintTable.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - private section                                                       -
  // -------------------------------------------------------------------------

  // the default table tag name
  static const String XML_TTGN_NAME = "table";
  // the default head tag name
  static const String XML_HTGN_NAME = "head";
  // the default body tag name
  static const String XML_BTGN_NAME = "body";
  // the default row tag name
  static const String XML_RTGN_NAME = "tr";
  // the default data tag name
  static const String XML_DTGN_NAME = "td";
  
  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a default xml print table

  XmlPrintTable::XmlPrintTable (void) {
    d_ttgn = XML_TTGN_NAME;
    d_htgn = XML_HTGN_NAME;
    d_btgn = XML_BTGN_NAME;
    d_rtgn = XML_RTGN_NAME;
    d_dtgn = XML_DTGN_NAME;
  }

  // create a xml print table by table

  XmlPrintTable::XmlPrintTable (const PrintTable& ptbl) {
    setptbl (ptbl);
  }

  // create a xml print table by name and table

  XmlPrintTable::XmlPrintTable (const String& name, const PrintTable& ptbl) {
    d_name = name;
    d_ttgn = XML_TTGN_NAME;
    d_htgn = XML_HTGN_NAME;
    d_btgn = XML_BTGN_NAME;
    d_rtgn = XML_RTGN_NAME;
    d_dtgn = XML_DTGN_NAME;
    setptbl (ptbl);
  }

  // copy construct this xml print table

  XmlPrintTable::XmlPrintTable (const XmlPrintTable& that) {
    that.rdlock ();
    try {
      XmlDocument::operator = (that);
      d_ttgn = that.d_ttgn;
      d_htgn = that.d_htgn;
      d_btgn = that.d_btgn;
      d_rtgn = that.d_rtgn;
      d_dtgn = that.d_dtgn;
      that.unlock ();
    } catch (...) {
      that.unlock ();
      throw;
    }
  }

  // return the xml print table class name

  String XmlPrintTable::repr (void) const {
    return "XmlPrintTable";
  }

  // get a clone of this object

  Object* XmlPrintTable::clone (void) const {
    return new XmlPrintTable (*this);
  }

  // get the table tag name

  String XmlPrintTable::getttgn (void) const {
    rdlock ();
    try {
      String result = d_ttgn;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set the table tag name

  void XmlPrintTable::setttgn (const String& ttgn) {
    wrlock ();
    try {
      d_ttgn = ttgn;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the head tag name

  String XmlPrintTable::gethtgn (void) const {
    rdlock ();
    try {
      String result = d_htgn;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set the head tag name

  void XmlPrintTable::sethtgn (const String& htgn) {
    wrlock ();
    try {
      d_htgn = htgn;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the body tag name

  String XmlPrintTable::getbtgn (void) const {
    rdlock ();
    try {
      String result = d_btgn;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set the body tag name

  void XmlPrintTable::setbtgn (const String& btgn) {
    wrlock ();
    try {
      d_btgn = btgn;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the row tag name

  String XmlPrintTable::getrtgn (void) const {
    rdlock ();
    try {
      String result = d_rtgn;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set the row tag name

  void XmlPrintTable::setrtgn (const String& rtgn) {
    wrlock ();
    try {
      d_rtgn = rtgn;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

 // get the data tag name

  String XmlPrintTable::getdtgn (void) const {
    rdlock ();
    try {
      String result = d_dtgn;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set the data tag name

  void XmlPrintTable::setdtgn (const String& dtgn) {
    wrlock ();
    try {
      d_dtgn = dtgn;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set the xml print table

  void XmlPrintTable::setptbl (const PrintTable& ptbl) {
    wrlock ();
    Object::dref (p_root);
    p_root = nullptr;
    try {
      // create a new root node
      Object::iref (p_root = new XmlRoot);
      // add a declaration node
      p_root->addchild (new XmlDecl);
      // add a table tag
      XmlTag* ttag = new XmlTag (d_ttgn);
      p_root->addchild (ttag);
      // get the number of columnsm
      long cols = ptbl.getcols ();
      // check if the table head is defined
      if (ptbl.ishead () == true) {
	// add a head tag
	XmlTag* htag = new XmlTag (d_htgn);
	ttag->addchild (htag);
	// loop in the head
	for (long col = 0; col < cols; col++) {
	  // get the head value
	  String data = ptbl.gethead (col);
	  // create a new data tag
	  XmlTag* dtag = new XmlTag (d_dtgn);
	  // parse the data value
	  dtag->parse (data);
	  // add the data node to the head
	  htag->addchild (dtag);
	}
      }
      // create a body node
      XmlTag* btag = new XmlTag (d_btgn);
      ttag->addchild (btag);
      // loop in the table
      long rows = ptbl.getrows ();
      for (long row = 0; row < rows; row++) {
	// create a new row
	XmlTag* tr = new XmlTag (d_rtgn);
	btag->addchild (tr);
	// loop in the row
	for (long col = 0; col < cols; col++) {
	  // get the row value
	  String data = ptbl.get (row, col);
	  // create a new data tag
	  XmlTag* dtag = new XmlTag (d_dtgn);
	  // parse the data value
	  dtag->parse (data);
	  // add the data node to the row
	  tr->addchild (dtag);
	}
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

  // the quark zone
  static const long QUARK_ZONE_LENGTH = 11;
  static QuarkZone  zone (QUARK_ZONE_LENGTH);

  // the object supported quarks
  static const long QUARK_SETPTBL = zone.intern ("set-print-table");
  static const long QUARK_SETTTGN = zone.intern ("set-table-tag-name");
  static const long QUARK_GETTTGN = zone.intern ("get-table-tag-name");
  static const long QUARK_SETHTGN = zone.intern ("set-head-tag-name");
  static const long QUARK_GETHTGN = zone.intern ("get-head-tag-name");
  static const long QUARK_SETBTGN = zone.intern ("set-body-tag-name");
  static const long QUARK_GETBTGN = zone.intern ("get-body-tag-name");
  static const long QUARK_SETRTGN = zone.intern ("set-row-tag-name");
  static const long QUARK_GETRTGN = zone.intern ("get-row-tag-name");
  static const long QUARK_SETDTGN = zone.intern ("set-data-tag-name");
  static const long QUARK_GETDTGN = zone.intern ("get-data-tag-name");

  // create a new object in a generic way

  Object* XmlPrintTable::mknew (Vector* argv) {
    long argc = (argv == nullptr) ? 0 : argv->length ();
    // create a default document object
    if (argc == 0) return new XmlPrintTable;
    // check for 1 argument
    if (argc == 1) {
      Object*      obj = argv->get (0);
      PrintTable* ptbl = dynamic_cast <PrintTable*> (obj);
      if (ptbl == nullptr) {
	throw Exception ("type-error", 
			 "invalid object with xml print table constructor",
			 Object::repr (obj));
      }
      return new XmlPrintTable (*ptbl);
    }
    // check for 2 arguments
    if (argc == 2) {
      // get the xml print table name
      String name = argv->getstring (0);
      // get the print table object
      Object*      obj = argv->get (0);
      PrintTable* ptbl = dynamic_cast <PrintTable*> (obj);
      if (ptbl == nullptr) {
	throw Exception ("type-error", 
			 "invalid object with xml print table constructor",
			 Object::repr (obj));
      }
      return new XmlPrintTable (name, *ptbl);
    }
    throw Exception ("argument-error",
                     "too many argument with xml print table constructor");
  }

  // return true if the given quark is defined

  bool XmlPrintTable::isquark (const long quark, const bool hflg) const {
    rdlock ();
    if (zone.exists (quark) == true) {
      unlock ();
      return true;
    }
    bool result = hflg ? XmlDocument::isquark (quark, hflg) : false;
    unlock ();
    return result;
  }

  // apply this object with a set of arguments and a quark

  Object* XmlPrintTable::apply (Evaluable* zobj, Nameset* nset, 
				const long quark, Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();

    // check for 0 argument
    if (argc == 0) {
      if (quark == QUARK_GETTTGN) return new String (getttgn ());
      if (quark == QUARK_GETHTGN) return new String (gethtgn ());
      if (quark == QUARK_GETBTGN) return new String (getbtgn ());
      if (quark == QUARK_GETRTGN) return new String (getrtgn ());
      if (quark == QUARK_GETDTGN) return new String (getdtgn ());
    }
    // check for 1 argument
    if (argc == 1) {
      if (quark == QUARK_SETTTGN) {
	String ttgn = argv->getstring (0);
	setttgn (ttgn);
	return nullptr;
      }
      if (quark == QUARK_SETHTGN) {
	String htgn = argv->getstring (0);
	sethtgn (htgn);
	return nullptr;
      }
      if (quark == QUARK_SETBTGN) {
	String btgn = argv->getstring (0);
	setbtgn (btgn);
	return nullptr;
      }
      if (quark == QUARK_SETRTGN) {
	String rtgn = argv->getstring (0);
	setrtgn (rtgn);
	return nullptr;
      }
      if (quark == QUARK_SETDTGN) {
	String dtgn = argv->getstring (0);
	setdtgn (dtgn);
	return nullptr;
      }
      if (quark == QUARK_SETPTBL) {
	// get the print table object
	Object*      obj = argv->get (0);
	PrintTable* ptbl = dynamic_cast <PrintTable*> (obj);
	if (ptbl == nullptr) {
	  throw Exception ("type-error", "invalid object with set-print-table",
			   Object::repr (obj));
	}
	setptbl (*ptbl);
	return nullptr;
      }
    }
    // call the xml document method
    return XmlDocument::apply (zobj, nset, quark, argv);
  }
}
