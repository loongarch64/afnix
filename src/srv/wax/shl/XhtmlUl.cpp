// ---------------------------------------------------------------------------
// - XhtmlUl.cpp                                                             -
// - afnix:wax module - xhtml ul node class implementation                   -
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

#include "Vector.hpp"
#include "XhtmlUl.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - private section                                                       -
  // -------------------------------------------------------------------------

  // the ul node name
  static const String XML_TAG_NAME = "ul";

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a default ul node

  XhtmlUl::XhtmlUl (void) : XhtmlBase (XML_TAG_NAME) {
    d_xcls = "";
  }

  // create a xhtml ul node with a class

  XhtmlUl::XhtmlUl (const String& cls) : XhtmlBase (XML_TAG_NAME) {
    setclass (cls);
  }

  // create a xhtml ul node with a class and a item class

  XhtmlUl::XhtmlUl (const String& cls, 
		    const String& xcls) : XhtmlBase (XML_TAG_NAME) {
    d_xcls = xcls;
    setclass (cls);
  }

  // return the class name

  String XhtmlUl::repr (void) const {
    return "XhtmlUl";
  }

  // add a default new item

  XhtmlLi* XhtmlUl::newitem (void) {
    wrlock ();
    try {
      XhtmlLi* result = 
	d_xcls.isnil () ? new XhtmlLi : new XhtmlLi (d_xcls);
      addchild (result);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // add a new item by class

  XhtmlLi* XhtmlUl::newitem (const String& xcls) {
    wrlock ();
    try {
      XhtmlLi* result = new XhtmlLi (xcls);
      addchild (result);
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
  static const long QUARK_ZONE_LENGTH = 1;
  static QuarkZone  zone (QUARK_ZONE_LENGTH);

  // the object supported quarks
  static const long QUARK_NEWITEM = zone.intern ("new-item");

  // create a new object in a generic way

  Object* XhtmlUl::mknew (Vector* argv) {
    long argc = (argv == nullptr) ? 0 : argv->length ();
    // check for 0 argument
    if (argc == 0) return new XhtmlUl;
    // check for 1 argument
    if (argc == 1) {
      String cls = argv->getstring (0);
      return new XhtmlUl (cls);
    }
    if (argc == 2) {
      String  cls = argv->getstring (0);
      String xcls = argv->getstring (1);
      return new XhtmlUl (cls, xcls);
    }
    // wrong arguments
    throw Exception ("argument-error", 
		     "too many arguments with xhtml ul constructor");
  }

  // return true if the given quark is defined

  bool XhtmlUl::isquark (const long quark, const bool hflg) const {
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

  Object* XhtmlUl::apply (Evaluable* zobj, Nameset* nset, const long quark,
			  Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();
    
    // dispatch 0 argument
    if (argc == 0) {
      if (quark == QUARK_NEWITEM) return newitem ();
    }
    // dispatch 1 argument
    if (argc == 1) {
      if (quark == QUARK_NEWITEM) {
	String xcls = argv->getstring (0);
	return newitem (xcls);
      }
    }
    // call the xhtml base method
    return XhtmlBase::apply (zobj, nset, quark, argv);
  }
}
