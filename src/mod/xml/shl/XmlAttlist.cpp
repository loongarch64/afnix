// ---------------------------------------------------------------------------
// - XmlAttlist.cpp                                                          -
// - afnix:xml module - xml attribute list node class implementation         -
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

#include "XmlSystem.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"
#include "XmlAttlist.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - private section                                                       -
  // -------------------------------------------------------------------------

  // the attribute list node name
  static const String XML_ATTR_NAME = "!ATTLIST";

  // the xml attribute types
  static const String XML_ATTR_FIXD = "#FIXED";
  static const String XML_ATTR_IMPL = "#IMPLIED";
  static const String XML_ATTR_REQR = "#REQUIRED";


  // this procedure parse a string and returns an attribute type
  static XmlAttlist::t_attr to_type (const String& type) {
    // check for defined type
    if (type == "CDATA")    return XmlAttlist::CDATA;
    if (type == "ID")       return XmlAttlist::ID;
    if (type == "IDREF")    return XmlAttlist::IDREF;
    if (type == "IDREFS")   return XmlAttlist::IDREFS;
    if (type == "ENTITY")   return XmlAttlist::ENTITY;
    if (type == "ENTITIES") return XmlAttlist::ENTITIES;
    if (type == "NMTOKEN")  return XmlAttlist::NMTOKEN;
    if (type == "NMTOKENS") return XmlAttlist::NMTOKENS;
    // invalid type
    throw Exception ("xml-error", "invalid attribute type", type);
  }

  // this procedure creates an enumeration from a string vector
  static String to_enum (const Strvec& xenm) {
    long len = xenm.length ();
    String result = "(";
    for (long i = 0; i < len; i++) {
      result += xenm.get (i);
      if (i < len - 1) result += " | ";
    }
    result += ")";
    return result;
  }

  // this procedure returns a string representation of a type
  static String to_type (const XmlAttlist::t_attr& type, const Strvec& xenm) {
    String result;
    switch (type) {
    case XmlAttlist::CDATA:
      result = "CDATA";
      break;
    case XmlAttlist::ID:
      result = "ID";
      break;
    case XmlAttlist::IDREF:
      result = "IDREF";
      break;
    case XmlAttlist::IDREFS:
      result = "IDREFS";
      break;
    case XmlAttlist::ENTITY:
      result = "ENTITY";
      break;
    case XmlAttlist::ENTITIES:
      result = "ENTITIES";
      break;
    case XmlAttlist::NMTOKEN:
      result = "NMTOKEN";
      break;
    case XmlAttlist::NMTOKENS:
      result = "NMTOKENS";
      break;
    case XmlAttlist::ENUMTYPE:
      result = to_enum (xenm);
      break;
    case XmlAttlist::NOTATION:
      result  = "NOTATION ";
      result += to_enum (xenm);
      break;
    }
    return result;
  }

  // this procedure returns a string representation of a default
  static String to_xdef (const XmlAttlist::t_xdef adef, const String& xdef) {
    String result;
    switch (adef) {
    case XmlAttlist::VALUE:
      result = xdef.toliteral ();
      break;
    case XmlAttlist::FIXED:
      result  = "#FIXED ";
      result += xdef.toliteral ();
      break;
    case XmlAttlist::IMPLIED:
      result = "#IMPLIED";
      break;
    case XmlAttlist::REQUIRED:
      result = "#REQUIRED";
      break;
    }
    return result;
  }

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create an attribute list by names

  XmlAttlist::XmlAttlist (const String& name, const String& attr) {
    d_eflg = true;
    d_name = name;
    d_attr = attr;
  }

  // return the document class name

  String XmlAttlist::repr (void) const {
    return "XmlAttlist";
  }

  // get a clone of this node

  Object* XmlAttlist::clone (void) const {
    rdlock ();
    try {
      // create a new node
      XmlAttlist* result = new XmlAttlist (d_name, d_attr);
      // copy the arguments
      result->d_type = d_type;
      result->d_xenm = d_xenm;
      result->d_adef = d_adef;
      result->d_xdef = d_xdef;
      // update the node info
      result->d_eflg = d_eflg;
      result->d_lnum = d_lnum;
      result->d_snam = d_snam;
      // here it is
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set the element name

  void XmlAttlist::setname (const String& name) {
    wrlock ();
    try {
      d_name = name;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the element name

  String XmlAttlist::getname (void) const {
    rdlock ();
    String result = d_name;
    unlock ();
    return result;
  }


  // set the attribute name

  void XmlAttlist::setattr (const String& attr) {
    wrlock ();
    try {
      d_attr = attr;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the attribute name

  String XmlAttlist::getattr (void) const {
    rdlock ();
    String result = d_attr;
    unlock ();
    return result;
  }

  // set the attribute type

  void XmlAttlist::settype (const String& type) {
    wrlock ();
    try {
      // reset the enumeration vector
      d_xenm.reset ();
      // check the type
      d_type = to_type (type);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set the enumeration type

  void XmlAttlist::settype (const Strvec& xenm, const bool xflg) {
    wrlock ();
    try {
      // set the type
      d_type = xflg ? NOTATION : ENUMTYPE;
      // save the enumeration vector
      d_xenm = xenm;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set the enumeration type

  void XmlAttlist::settype (Vector* xvec, const bool xflg) {
    // check for nil vector
    long argc = (xvec == nullptr) ? 0 : xvec->length ();
    if (argc == 0) return;
    // convert the vector
    Strvec xenm (argc);
    for (long i = 0; i < argc; i++) xenm.add (xvec->getstring (i));
    // set the enumeration type
    settype (xenm, xflg);
  }

  // set the attribute default value

  void XmlAttlist::setxdef (const String& xdef) {
    wrlock ();
    try {
      if (xdef == XML_ATTR_IMPL) {
	d_adef = IMPLIED;
	d_xdef = "";
      } else if (xdef == XML_ATTR_REQR) {
	d_adef = REQUIRED;
	d_xdef = "";
      } else if (xdef == XML_ATTR_FIXD) {
	throw Exception ("xml-error", "fixed type without value");
      } else {
	d_adef = VALUE;
	d_xdef = xdef;
      }
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  void XmlAttlist::setfixd (const String& xdef) {
    wrlock ();
    try {
      d_adef = FIXED;
      d_xdef = xdef;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // write an attribute node to a buffer

  void XmlAttlist::write (Buffer& buf) const {
    rdlock ();
    try {
      // add the node name
      buf.add ("<");
      buf.add (XML_ATTR_NAME);
      // add the element name
      buf.add (' ');
      buf.add (d_name);
      // add the attribute name
      buf.add (' ');
      buf.add (d_attr);
      //add the attribute type
      buf.add (' ');
      buf.add (to_type (d_type, d_xenm));
      // add the attribute default
      buf.add (' ');
      buf.add (to_xdef (d_adef, d_xdef));
      // add the terminaison
      buf.add (">");
      buf.add (eolq);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // write an element node to an output stream

  void XmlAttlist::write (OutputStream& os) const {
    rdlock ();
    try {
      // add the node name
      os.write ("<");
      os.write (XML_ATTR_NAME);
      os.write (' ');
      os.write (d_name);
      // add the attribute name
      os.write (' ');
      os.write (d_attr);
      //add the attribute type
      os.write (' ');
      os.write (to_type (d_type, d_xenm));
      // add the attribute default
      os.write (' ');
      os.write (to_xdef (d_adef, d_xdef));
      // add the terminaison
      os.write   (">");
      os.newline ();
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
  static const long QUARK_ZONE_LENGTH = 7;
  static QuarkZone  zone (QUARK_ZONE_LENGTH);

  // the object supported quarks
  static const long QUARK_SETTYPE = zone.intern ("set-type");
  static const long QUARK_SETFIXD = zone.intern ("set-fixed");
  static const long QUARK_SETXDEF = zone.intern ("set-default");
  static const long QUARK_SETNAME = zone.intern ("set-element-name");
  static const long QUARK_GETNAME = zone.intern ("get-element-name");
  static const long QUARK_SETATTR = zone.intern ("set-attribute-name");
  static const long QUARK_GETATTR = zone.intern ("get-attribute-name");

  // create a new object in a generic way

  Object* XmlAttlist::mknew (Vector* argv) {
    long argc = (argv == nullptr) ? 0 : argv->length ();
    // check for 2 arguments
    if (argc == 2) {
      String name = argv->getstring (0);
      String attr = argv->getstring (1);
      return new XmlAttlist (name, attr);
    }
    throw Exception ("argument-error",
                     "too many arguments with xml attribute list constructor");
  }

  // return true if the given quark is defined

  bool XmlAttlist::isquark (const long quark, const bool hflg) const {
    rdlock ();
    if (zone.exists (quark) == true) {
      unlock ();
      return true;
    }
    bool result = hflg ? XmlNode::isquark (quark, hflg) : false;
    unlock ();
    return result;
  }

  // apply this object with a set of arguments and a quark

  Object* XmlAttlist::apply (Evaluable* zobj, Nameset* nset, const long quark,
			Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();

    // check for 0 argument
    if (argc == 0) {
      if (quark == QUARK_GETNAME) return new String (getname ());
      if (quark == QUARK_GETATTR) return new String (getattr ());
    }
    // check for 1 argument
    if (argc == 1) {
      if (quark == QUARK_SETNAME) {
	String name = argv->getstring (0);
	setname (name);
	return nullptr;
      }
      if (quark == QUARK_SETATTR) {
	String attr = argv->getstring (0);
	setattr (attr);
	return nullptr;
      }
      if (quark == QUARK_SETTYPE) {
	String type = argv->getstring (0);
	settype (type);
	return nullptr;
      }
      if (quark == QUARK_SETXDEF) {
	String xdef = argv->getstring (0);
	setxdef (xdef);
	return nullptr;
      }
      if (quark == QUARK_SETFIXD) {
	String xdef = argv->getstring (0);
	setfixd (xdef);
	return nullptr;
      }
    }
    // check for 2 arguments
    if (argc == 2) {
      if (quark == QUARK_SETTYPE) {
        Object*  obj = argv->get (0);
        Vector* vobj = dynamic_cast <Vector*> (obj);
        if (vobj == nullptr) {
          throw Exception ("type-error", "invalid vector object",
                           Object::repr (obj));
        }
	bool xflg = argv->getbool (1);
	settype (vobj, xflg);
        return nullptr;
      }
    }
    // call the xml node method
    return XmlNode::apply (zobj, nset, quark, argv);
  }
}
