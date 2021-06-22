// ---------------------------------------------------------------------------
// - XhtmlTr.cpp                                                             -
// - afnix:wax module - xhtml tr node class implementation                   -
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

#include "XhtmlTr.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - private section                                                       -
  // -------------------------------------------------------------------------

  // the tr node name
  static const String XML_TAG_NAME = "tr";

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a default tr node

  XhtmlTr::XhtmlTr (void) : XhtmlBase (XML_TAG_NAME) {
    d_thc = "";
    d_tdc = "";
  }

  // create a xhtml tr node with a class

  XhtmlTr::XhtmlTr (const String& cls) : XhtmlBase (XML_TAG_NAME) {
    d_thc = "";
    d_tdc = "";
    setclass (cls);
  }

  // create a xhtml tr node with a class and a default tx class

  XhtmlTr::XhtmlTr (const String& cls, 
		    const String& txc) : XhtmlBase (XML_TAG_NAME) {
    d_thc = txc;
    d_tdc = txc;
    setclass (cls);
  }

  // return the class name

  String XhtmlTr::repr (void) const {
    return "XhtmlTr";
  }

  // set the default th class

  void XhtmlTr::setthc (const String& thc) {
    wrlock ();
    d_thc = thc;
    unlock ();
  }

  // set the default td class

  void XhtmlTr::settdc (const String& tdc) {
    wrlock ();
    d_tdc = tdc;
    unlock ();
  }

  // set the default tx class

  void XhtmlTr::settxc (const String& txc) {
    wrlock ();
    d_thc = txc;
    d_tdc = txc;
    unlock ();
  }

  // get a new table head

  XhtmlTh* XhtmlTr::newth (void) {
    wrlock ();
    XhtmlTh* th = d_thc.isnil () ? new XhtmlTh : new XhtmlTh (d_thc);
    try {
      addchild (th);
      unlock ();
      return th;
    } catch (...) {
      delete th;
      unlock ();
      throw;
    }
  }

  // get a new table head by class

  XhtmlTh* XhtmlTr::newth (const String& cls) {
    wrlock ();
    XhtmlTh* th = new XhtmlTh (cls);
    try {
      addchild (th);
      unlock ();
      return th;
    } catch (...) {
      delete th;
      unlock ();
      throw;
    }
  }

  // get a new table data

  XhtmlTd* XhtmlTr::newtd (void) {
    wrlock ();
    XhtmlTd* td = d_tdc.isnil () ? new XhtmlTd : new XhtmlTd (d_tdc);
    try {
      addchild (td);
      unlock ();
      return td;
    } catch (...) {
      delete td;
      unlock ();
      throw;
    }
  }

  // get a new table data by class

  XhtmlTd* XhtmlTr::newtd (const String& cls) {
    wrlock ();
    XhtmlTd* td = new XhtmlTd (cls);
    try {
      addchild (td);
      unlock ();
      return td;
    } catch (...) {
      delete td;
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
  static const long QUARK_NEWTH  = zone.intern ("new-head");
  static const long QUARK_NEWTD  = zone.intern ("new-data");
  static const long QUARK_SETTHC = zone.intern ("set-head-class");
  static const long QUARK_SETTDC = zone.intern ("set-data-class");
  static const long QUARK_SETTXC = zone.intern ("set-xdef-class");

  // create a new object in a generic way

  Object* XhtmlTr::mknew (Vector* argv) {
    long argc = (argv == nullptr) ? 0 : argv->length ();
    // check for 0 argument
    if (argc == 0) return new XhtmlTr;
    // check for 1 argument
    if (argc == 1) {
      String cls = argv->getstring (0);
      return new XhtmlTr (cls);
    }
    // check for 2 arguments
    if (argc == 1) {
      String cls = argv->getstring (0);
      String txc = argv->getstring (1);
      return new XhtmlTr (cls, txc);
    }
    // wrong arguments
    throw Exception ("argument-error", 
		     "too many arguments with xhtml tr constructor");
  }

  // return true if the given quark is defined

  bool XhtmlTr::isquark (const long quark, const bool hflg) const {
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

  Object* XhtmlTr::apply (Evaluable* zobj, Nameset* nset, const long quark,
			    Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();
    
    // dispatch 0 argument
    if (argc == 0) {
      if (quark == QUARK_NEWTH) return newth ();
      if (quark == QUARK_NEWTD) return newtd ();
    }
    // dispatch 1 argument
    if (argc == 1) {
      if (quark == QUARK_NEWTH) {
	String cls = argv->getstring (0);	
	return newth (cls);
      }
      if (quark == QUARK_NEWTD) {
	String cls = argv->getstring (0);	
	return newtd (cls);
      }
      if (quark == QUARK_SETTHC) {
	String thc = argv->getstring (0);	
	setthc (thc);
	return nullptr;
      }
      if (quark == QUARK_SETTDC) {
	String tdc = argv->getstring (0);	
	settdc (tdc);
	return nullptr;
      }
      if (quark == QUARK_SETTXC) {
	String txc = argv->getstring (0);	
	settxc (txc);
	return nullptr;
      }
    }
    // call the xml tag method
    return XmlTag::apply (zobj, nset, quark, argv);
  }
}
