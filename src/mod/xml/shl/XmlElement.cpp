// ---------------------------------------------------------------------------
// - XmlElement.cpp                                                           -
// - afnix:xml module - xml element node class implementation                 -
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
#include "XmlElement.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - private section                                                       -
  // -------------------------------------------------------------------------

  // the entity node name
  static const String XML_ELEM_NAME = "!ELEMENT";

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a element node by name and value

  XmlElement::XmlElement (const String& name, const String& xval) {
    d_eflg = true;
    d_name = name;
    d_xval = xval;
  }

  // return the document class name

  String XmlElement::repr (void) const {
    return "XmlElement";
  }

  // get a clone of this node

  Object* XmlElement::clone (void) const {
    rdlock ();
    try {
      // create a new node
      XmlElement* result = new XmlElement (d_name, d_xval);
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

  // check if the element name is valid

  bool XmlElement::isname (const String& name) const {
    rdlock ();
    try {
      bool result = (d_name == name);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set the element name

  void XmlElement::setname (const String& name) {
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

  String XmlElement::getname (void) const {
    rdlock ();
    String result = d_name;
    unlock ();
    return result;
  }

  // set the element value

  void XmlElement::setxval (const String& xval) {
    wrlock ();
    try {
      d_xval = xval;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the element value

  String XmlElement::getxval (void) const {
    rdlock ();
    String result = d_xval;
    unlock ();
    return result;
  }

  // write an element node to a buffer

  void XmlElement::write (Buffer& buf) const {
    rdlock ();
    try {
      // add the node name
      buf.add ("<");
      buf.add (XML_ELEM_NAME);
      buf.add (' ');
      buf.add (d_name);
      buf.add (' ');
      buf.add (d_xval);
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

  void XmlElement::write (OutputStream& os) const {
    rdlock ();
    try {
      // add the node name
      os.write ("<");
      os.write (XML_ELEM_NAME);
      os.write (' ');
      os.write (d_name);
      os.write (' ');
      os.write (d_xval);
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
  static const long QUARK_ZONE_LENGTH = 4;
  static QuarkZone  zone (QUARK_ZONE_LENGTH);

  // the object supported quarks
  static const long QUARK_SETNAME = zone.intern ("set-name");
  static const long QUARK_GETNAME = zone.intern ("get-name");
  static const long QUARK_SETXVAL = zone.intern ("set-xval");
  static const long QUARK_GETXVAL = zone.intern ("get-xval");

  // create a new object in a generic way

  Object* XmlElement::mknew (Vector* argv) {
    long argc = (argv == nullptr) ? 0 : argv->length ();
    // check for 2 arguments
    if (argc == 2) {
      String name = argv->getstring (0);
      String xval = argv->getstring (1);
      return new XmlElement (name, xval);
    }
    throw Exception ("argument-error",
                     "too many arguments with xml element constructor");
  }

  // return true if the given quark is defined

  bool XmlElement::isquark (const long quark, const bool hflg) const {
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

  Object* XmlElement::apply (Evaluable* zobj, Nameset* nset, const long quark,
			Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();

    // check for 0 argument
    if (argc == 0) {
      if (quark == QUARK_GETNAME) return new String (getname ());
      if (quark == QUARK_GETXVAL) return new String (getxval ());
    }
    // check for 1 argument
    if (argc == 1) {
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
    }
    // call the xml node method
    return XmlNode::apply (zobj, nset, quark, argv);
  }
}
