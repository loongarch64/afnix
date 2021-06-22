// ---------------------------------------------------------------------------
// - XmlPlist.cpp                                                            -
// - afnix:xml module - xml plist class implementation                       -
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
#include "XmlPlist.hpp"
#include "Evaluable.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - private section                                                       -
  // -------------------------------------------------------------------------

  // the default plist tag name
  static const String XML_PLST_NAME = "plist";
  // the default property tag name
  static const String XML_PROP_NAME = "prop";
  // the default name attribute
  static const String XML_NAME_ATTR = "name";
  // the default value attribute
  static const String XML_PVAL_ATTR = "value";
  
  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a default xml plist

  XmlPlist::XmlPlist (void) {
    d_pltn = XML_PLST_NAME;
    d_potn = XML_PROP_NAME;
    d_natt = XML_NAME_ATTR;
    d_vatt = XML_PVAL_ATTR;
  }

  // create a xml plist by property list

  XmlPlist::XmlPlist (const Plist& plst) {
    d_pltn = XML_PLST_NAME;
    d_potn = XML_PROP_NAME;
    d_natt = XML_NAME_ATTR;
    d_vatt = XML_PVAL_ATTR;
    setplist (plst);
  }

  // create a xml plist by property list

  XmlPlist::XmlPlist (const String& name, const Plist& plst) {
    d_name = name;
    d_pltn = XML_PLST_NAME;
    d_potn = XML_PROP_NAME;
    d_natt = XML_NAME_ATTR;
    d_vatt = XML_PVAL_ATTR;
    setplist (plst);
  }

  // copy construct this xml plist

  XmlPlist::XmlPlist (const XmlPlist& that) {
    that.rdlock ();
    try {
      XmlDocument::operator = (that);
      d_pltn = that.d_pltn;
      d_potn = that.d_potn;
      d_natt = that.d_natt;
      d_vatt = that.d_vatt;
      that.unlock ();
    } catch (...) {
      that.unlock ();
      throw;
    }
  }

  // return the xml plist class name

  String XmlPlist::repr (void) const {
    return "XmlPlist";
  }

  // get a clone of this object

  Object* XmlPlist::clone (void) const {
    return new XmlPlist (*this);
  }

  // get the plist tag name

  String XmlPlist::getpltn (void) const {
    rdlock ();
    try {
      String result = d_pltn;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set the plist tag name

  void XmlPlist::setpltn (const String& pltn) {
    wrlock ();
    try {
      d_pltn = pltn;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the property tag name

  String XmlPlist::getpotn (void) const {
    rdlock ();
    try {
      String result = d_potn;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }


  // set the property tag name

  void XmlPlist::setpotn (const String& potn) {
    wrlock ();
    try {
      d_potn = potn;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the property name attribute

  String XmlPlist::getnatt (void) const {
    rdlock ();
    try {
      String result = d_natt;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }


  // set the property name attribute

  void XmlPlist::setnatt (const String& natt) {
    wrlock ();
    try {
      d_natt = natt;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the property value attribute

  String XmlPlist::getvatt (void) const {
    rdlock ();
    try {
      String result = d_vatt;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set the property value attribute

  void XmlPlist::setvatt (const String& vatt) {
    wrlock ();
    try {
      d_vatt = vatt;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }


  // set the xml proprty list

  void XmlPlist::setplist (const Plist& plst) {
    wrlock ();
    Object::dref (p_root);
    p_root = nullptr;
    try {
      // create a new root node
      Object::iref (p_root = new XmlRoot);
      // add a declaration node
      p_root->addchild (new XmlDecl);
      // add a plist tag
      XmlTag* ltag = new XmlTag (d_pltn);
      p_root->addchild (ltag);
      // loop in the property list
      long plen = plst.length ();
      for (long i = 0; i < plen; i++) {
	// get the next property
	Property* prop = plst.get (i);
	if (prop == nullptr) continue;
	String name = prop->getname ();
	String pval = prop->getpval ();
	// create a new property tag
	XmlTag* ptag = new XmlTag (d_potn);
	ptag->setattr (d_natt, name);
	ptag->setattr (d_vatt, pval);
	// add the tag
	ltag->addchild (ptag);
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
  static const long QUARK_ZONE_LENGTH = 9;
  static QuarkZone  zone (QUARK_ZONE_LENGTH);

  // the object supported quarks
  static const long QUARK_SETPLST = zone.intern ("set-plist");
  static const long QUARK_SETPLTN = zone.intern ("set-plist-tag-name");
  static const long QUARK_GETPLTN = zone.intern ("get-plist-tag-name");
  static const long QUARK_SETPOTN = zone.intern ("set-property-tag-name");
  static const long QUARK_GETPOTN = zone.intern ("get-property-tag-name");
  static const long QUARK_SETNATT = zone.intern ("set-name-attribute");
  static const long QUARK_GETNATT = zone.intern ("get-name-attribute");
  static const long QUARK_SETVATT = zone.intern ("set-value-attribute");
  static const long QUARK_GETVATT = zone.intern ("get-value-attribute");

  // create a new object in a generic way

  Object* XmlPlist::mknew (Vector* argv) {
    long argc = (argv == nullptr) ? 0 : argv->length ();
    // create a default document object
    if (argc == 0) return new XmlPlist;
    // check for 1 argument
    if (argc == 1) {
      Object* obj = argv->get (0);
      Plist* plst = dynamic_cast <Plist*> (obj);
      if (plst == nullptr) {
	throw Exception ("type-error", 
			 "invalid object with xml plist constructor",
			 Object::repr (obj));
      }
      return new XmlPlist (*plst);
    }
    // check for 2 arguments
    if (argc == 2) {
      // get the xml plist name
      String name = argv->getstring (0);
      // get the plist object
      Object* obj = argv->get (0);
      Plist* plst = dynamic_cast <Plist*> (obj);
      if (plst == nullptr) {
	throw Exception ("type-error", 
			 "invalid object with xml plist constructor",
			 Object::repr (obj));
      }
      return new XmlPlist (name, *plst);
    }
    throw Exception ("argument-error",
                     "too many argument with xml plist constructor");
  }

  // return true if the given quark is defined

  bool XmlPlist::isquark (const long quark, const bool hflg) const {
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

  Object* XmlPlist::apply (Evaluable* zobj, Nameset* nset, const long quark,
			   Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();

    // check for 0 argument
    if (argc == 0) {
      if (quark == QUARK_GETPLTN) return new String (getpltn ());
      if (quark == QUARK_GETPOTN) return new String (getpotn ());
      if (quark == QUARK_GETNATT) return new String (getnatt ());
      if (quark == QUARK_GETVATT) return new String (getvatt ());
    }
    // check for 1 argument
    if (argc == 1) {
      if (quark == QUARK_SETPLTN) {
	String pltn = argv->getstring (0);
	setpltn (pltn);
	return nullptr;
      }
      if (quark == QUARK_SETPOTN) {
	String potn = argv->getstring (0);
	setpotn (potn);
	return nullptr;
      }
      if (quark == QUARK_SETNATT) {
	String natt = argv->getstring (0);
	setnatt (natt);
	return nullptr;
      }
      if (quark == QUARK_SETVATT) {
	String vatt = argv->getstring (0);
	setvatt (vatt);
	return nullptr;
      }
      if (quark == QUARK_SETPLST) {
	// get the plist object
	Object* obj = argv->get (0);
	Plist* plst = dynamic_cast <Plist*> (obj);
	if (plst == nullptr) {
	  throw Exception ("type-error", "invalid object with set-plist",
			   Object::repr (obj));
	}
	setplist (*plst);
	return nullptr;
      }
    }
    // call the xml document method
    return XmlDocument::apply (zobj, nset, quark, argv);
  }
}
