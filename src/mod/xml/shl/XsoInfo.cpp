// ---------------------------------------------------------------------------
// - XsoInfo.cpp                                                             -
// - afnix:xml module - xml/xsm node info class implementation               -
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
#include "Integer.hpp"
#include "Boolean.hpp"
#include "XsoInfo.hpp"
#include "Evaluable.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a default info object

  XsoInfo::XsoInfo (void) {}

  // create an info object by name

  XsoInfo::XsoInfo (const String& name) {
    d_name = name;
  }

  // create an info object by name and text

  XsoInfo::XsoInfo (const String& name, const String& xval) {
    d_name = name;
    d_xval = xval;
  }

  // create an info object by name, attribute list and text

  XsoInfo::XsoInfo (const String& name,const Plist& alst,const String& xval) {
    d_name = name;
    d_alst = alst;
    d_xval = xval;
  }

  // copy construct this node

  XsoInfo::XsoInfo (const XsoInfo& that) {
    that.rdlock ();
    try {
      d_name = that.d_name;
      d_alst = that.d_alst;
      d_xval = that.d_xval;
      that.unlock ();
    } catch (...) {
      that.unlock ();
      throw;
    }
  }

  // return the class name

  String XsoInfo::repr (void) const {
    return "XsoInfo";
  }

  // get a clone of this node

  Object* XsoInfo::clone (void) const {
    return new XsoInfo (*this);
  }

  // assign a node to this one

  XsoInfo& XsoInfo::operator = (const XsoInfo& that) {
    // check for self-assignation
    if (this == &that) return *this;
    // lock and assign
    wrlock ();
    that.rdlock ();
    try {
      d_name = that.d_name;
      d_alst = that.d_alst;
      d_xval = that.d_xval;
      unlock ();
      that.unlock ();
      return *this;
    } catch (...) {
      unlock ();
      that.unlock ();
      throw;
    }
  }

  // set the info object name

  void XsoInfo::setname (const String& name) {
    wrlock ();
    try {
      d_name = name;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the info object name

  String XsoInfo::getname (void) const {
    rdlock ();
    try {
      String result = d_name;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return true if an attribute exists

  bool XsoInfo::exists (const String& name) const {
    rdlock ();
    try {
      bool result = d_alst.exists (name);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set a info attribute by name and value

  void XsoInfo::setattr (const String& name, const Literal& pval) {
    wrlock ();
    try {
      d_alst.set (name, pval);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the info attribute list

  Plist XsoInfo::getattr (void) const {
        rdlock ();
    try {
      Plist result = d_alst;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the info attribute value by name

  String XsoInfo::getpval (const String& name) const {
        rdlock ();
    try {
      String result = d_alst.getpval (name);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set the info object text

  void XsoInfo::setxval (const String& xval) {
    wrlock ();
    try {
      d_xval = xval;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the info object text

  String XsoInfo::getxval (void) const {
    rdlock ();
    try {
      String result = d_xval;
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
  static const long QUARK_ZONE_LENGTH = 7;
  static QuarkZone  zone (QUARK_ZONE_LENGTH);

  // the object supported quarks
  static const long QUARK_EXISTSP = zone.intern ("exists-p");
  static const long QUARK_SETNAME = zone.intern ("set-name");
  static const long QUARK_SETXVAL = zone.intern ("set-text");
  static const long QUARK_GETXVAL = zone.intern ("get-text");
  static const long QUARK_SETATTR = zone.intern ("set-attribute");
  static const long QUARK_GETPVAL = zone.intern ("get-attribute-value");
  static const long QUARK_GETALST = zone.intern ("get-attribute-list");

  // create a new object in a generic way

  Object* XsoInfo::mknew (Vector* argv) {
    long argc = (argv == nullptr) ? 0 : argv->length ();
    // check for 0 argument
    if (argc == 0) return new XsoInfo;
    // check for 1 argument
    if (argc == 1) {
      String name = argv->getstring (0);
      return new XsoInfo (name);
    }
    // check for 2 arguments
    if (argc == 2) {
      String name = argv->getstring (0);
      String xval = argv->getstring (1);
      return new XsoInfo (name, xval);
    }
    throw Exception ("argument-error",
                     "too many argument with info node constructor");
  }

  // return true if the given quark is defined

  bool XsoInfo::isquark (const long quark, const bool hflg) const {
    rdlock ();
    if (zone.exists (quark) == true) {
      unlock ();
      return true;
    }
    bool result = hflg ? Nameable::isquark (quark, hflg) : false;
    unlock ();
    return result;
  }

  // apply this object with a set of arguments and a quark

  Object* XsoInfo::apply (Evaluable* zobj, Nameset* nset, const long quark,
			  Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();

    // check for 0 argument
    if (argc == 0) {
      if (quark == QUARK_GETXVAL)  return new String  (getxval ());
      if (quark == QUARK_GETALST)  return new Plist   (getattr ());
    }
    // check for 1 argument
    if (argc == 1) {
      if (quark == QUARK_EXISTSP) {
	String name = argv->getstring (0);
	return new Boolean (exists (name));
      }
      if (quark == QUARK_SETNAME) {
	String name = argv->getstring (0);
	setname (name);
	return nullptr;
      }
      if (quark == QUARK_SETXVAL) {
	String xval = argv->getstring (0);
	setxval (xval);
	return nullptr;
      }
      if (quark == QUARK_GETPVAL) {
	String name = argv->getstring (0);
	return new String (getpval (name));
	return nullptr;
      }
    }
    // check for 2 arguments
    if (argc == 2) {
      if (quark == QUARK_SETATTR) {
	String name = argv->getstring (0);
        Object*   obj = argv->get (1);
        Literal* lobj = dynamic_cast <Literal*> (obj);
        if (lobj == nullptr) {
	  throw Exception ("type-error", "invalid object with set-attribute",
			   Object::repr (obj));
        }
        setattr (name, *lobj);
	return nullptr;
      }
    }
    // call the nameable method
    return Nameable::apply (zobj, nset, quark, argv);
  }
}
