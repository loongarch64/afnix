// ---------------------------------------------------------------------------
// - XhtmlTelem.cpp                                                          -
// - afnix:wax module - xhtml table element node class implementation        -
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

#include "XhtmlRef.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"
#include "XhtmlTelem.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - private section                                                       -
  // -------------------------------------------------------------------------

  // this procedure format a column style into a td style
  static String xht_to_style (const Style& stl) {
    String result;
    // get the text weight
    switch (stl.getwght ()) {
    case Style::WGHT_N:
      result += "font-style: normal;";
      break;
    case Style::WGHT_B:
      result += "font-style: bold;";
      break;
    case Style::WGHT_I:
      result += "font-style: italic;";
      break;
    }
    // get the text alignment
    switch (stl.getalgn ()) {
    case Style::ALGN_L:
      result += "text-align: left;";
      break;
    case Style::ALGN_C:
      result += "text-align: center;";
      break;
    case Style::ALGN_R:
      result += "text-align: right;";
      break;
    }
    return result;
  }

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a table element node

  XhtmlTelem::XhtmlTelem (const String& tag) : XhtmlBase (tag) {
    d_trc = "";
    d_txc = "";
  }

  // create a table element node with a class

  XhtmlTelem::XhtmlTelem (const String& tag, 
			  const String& cls) : XhtmlBase (tag) {
    d_trc = "";
    d_txc = "";
    setclass (cls);
  }

  // create a table element node with a class with a default tr class

  XhtmlTelem::XhtmlTelem (const String& tag, const String& cls, 
			  const String& trc) : XhtmlBase (tag) {
    d_trc = trc;
    d_txc = "";
    setclass (cls);
  }

  // create a table element node with a class with a default tr and tx class

  XhtmlTelem::XhtmlTelem (const String& tag, const String& cls, 
			  const String& trc, 
			  const String& txc) : XhtmlBase (tag) {
    d_trc = trc;
    d_txc = txc;
    setclass (cls);
  }

  // return the class name

  String XhtmlTelem::repr (void) const {
    return "XhtmlTelem";
  }

  // set the default tr class

  void XhtmlTelem::settrc (const String& trc) {
    wrlock ();
    d_trc = trc;
    unlock ();
  }

  // set the default tx class

  void XhtmlTelem::settxc (const String& txc) {
    wrlock ();
    d_txc = txc;
    unlock ();
  }

  // get a new table row

  XhtmlTr* XhtmlTelem::newtr (void) {
    wrlock ();
    XhtmlTr* tr = d_trc.isnil () ? new XhtmlTr : new XhtmlTr (d_trc);
    if (d_txc.isnil () == false) tr->settxc (d_txc);
    try {
      addchild (tr);
      unlock ();
      return tr;
    } catch (...) {
      delete tr;
      unlock ();
      throw;
    }
  }

  // get a new table row by class

  XhtmlTr* XhtmlTelem::newtr (const String& cls) {
    wrlock ();
    XhtmlTr* tr  = new XhtmlTr (cls);
    if (d_txc.isnil () == false) tr->settxc (d_txc);
    try {
      addchild (tr);
      unlock ();
      return tr;
    } catch (...) {
      delete tr;
      unlock ();
      throw;
    }
  }

  // add a print table to this table

  void XhtmlTelem::addtbl (const PrintTable& tbl, const bool tflg) {
    // get the number of rows and columns
    long rows = tbl.getrows ();
    long cols = tbl.getcols ();
    if ((rows == 0) || (cols == 0)) return;
    wrlock ();
    try {
      // set the table head
      if (tbl.ishead () == true) {
	XhtmlTr* tr = newtr ();
        for (long j = 0; j < cols; j++) {
	  // get a new table head
	  XhtmlTh* th = tr->newth ();
	  // set the head data
	  String data = tbl.gethead (j);
	  th->parse (data);
	  // set the head style by column style
	  Style stl = tbl.getstyle (j);
	  th->setstyle (xht_to_style (stl));
	}	
      }
      // set the table body
      for (long i = 0; i < rows; i++) {
        XhtmlTr* tr = newtr ();
        for (long j = 0; j < cols; j++) {
          // get a new table data
          XhtmlTd* td = tr->newtd ();
	  if ((tflg == false) || (tbl.istag (i, j) == false)) {
	    // get the string data and parse it
	    String data = tbl.get (i, j);
	    td->parse (data);
	  } else {
	    // here we have a tag that is used as a test uri
	    String uri = tbl.gettag (i, j);
	    String txt = tbl.truncate (i, j);
	    // create a reference node
	    td->addchild (new XhtmlRef (uri, txt));
	  }
	  // set the table style by column style
	  Style stl = tbl.getstyle (j);
	  td->setstyle (xht_to_style (stl));
	}
      }
    } catch (...) {
      unlock ();
      throw;
    }
    unlock ();
  }


  // -------------------------------------------------------------------------
  // - object section                                                        -
  // -------------------------------------------------------------------------

  // the quark zone
  static const long QUARK_ZONE_LENGTH = 4;
  static QuarkZone  zone (QUARK_ZONE_LENGTH);

  // the object supported quarks
  static const long QUARK_NEWTR  = zone.intern ("new-row");
  static const long QUARK_ADDTBL = zone.intern ("add-table");
  static const long QUARK_SETTRC = zone.intern ("set-xrow-class");
  static const long QUARK_SETTXC = zone.intern ("set-xdef-class");

  // return true if the given quark is defined

  bool XhtmlTelem::isquark (const long quark, const bool hflg) const {
    rdlock ();
    if (zone.exists (quark) == true) {
      unlock ();
      return true;
    }
    bool result = hflg ? XhtmlBase::isquark (quark, hflg) : false;
    unlock ();
    return result;
  }

  // apply this object with a set of arguments and a quark

  Object* XhtmlTelem::apply (Evaluable* zobj, Nameset* nset, const long quark,
			    Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();
    
    // dispatch 0 argument
    if (argc == 0) {
      if (quark == QUARK_NEWTR) return newtr ();
    }
    // dispatch 1 argument
    if (argc == 1) {
      if (quark == QUARK_SETTRC) {
	String trc = argv->getstring (0);	
	settrc (trc);
	return nullptr;
      }
      if (quark == QUARK_SETTXC) {
	String txc = argv->getstring (0);	
	settxc (txc);
	return nullptr;
      }
      if (quark == QUARK_NEWTR) {
	String cls = argv->getstring (0);	
	return newtr (cls);
      }
      if (quark == QUARK_ADDTBL) {
        PrintTable* tbl = dynamic_cast <PrintTable*> (argv->get (0));
        if (tbl == nullptr) {
          throw Exception ("type-error", 
			   "print table expected with add-table");
	}
        addtbl (*tbl, false);
        return nullptr;
      }
    }
    // dispatch 2 arguments
    if (argc == 2) {
      if (quark == QUARK_ADDTBL) {
        PrintTable* tbl = dynamic_cast <PrintTable*> (argv->get (0));
        if (tbl == nullptr) {
          throw Exception ("type-error", 
			   "print table expected with add-table");
	}
	bool tflg = argv->getbool (1);
        addtbl (*tbl, tflg);
        return nullptr;
      }
    }
    // call the xhtml base method
    return XhtmlBase::apply (zobj, nset, quark, argv);
  }
}
