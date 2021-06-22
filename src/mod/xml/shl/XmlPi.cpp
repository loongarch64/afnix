// ---------------------------------------------------------------------------
// - XmlPi.cpp                                                               -
// - afnix:xml module - xml processing instruction node class implementation -
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

#include "XmlPi.hpp"
#include "XmlBuffer.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a pi node by value

  XmlPi::XmlPi (const String& name) : XmlNode (true) {
    d_name = name;
  }

  // create a pi node by name and value

  XmlPi::XmlPi (const String& name, const String& xval) : XmlNode (true) {
    d_name = name;
    d_xval = xval;
  }

  // return the document class name

  String XmlPi::repr (void) const {
    return "XmlPi";
  }

  // get a clone of this node

  Object* XmlPi::clone (void) const {
    rdlock ();
    try {
      // create a new node
      XmlPi* result = new XmlPi (d_name, d_xval);
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

  // check if a node name is valid

  bool XmlPi::isname (const String& name) const {
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

  // get the pi node name

  String XmlPi::getname (void) const {
    rdlock ();
    String result = d_name;
    unlock ();
    return result;
  }

  // set the pi node name

  void XmlPi::setname (const String& name) {
    wrlock ();
    try {
      d_name = name;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set the pi value

  void XmlPi::setxval (const String& xval) {
    wrlock ();
    try {
      d_xval = xval;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the pi value

  String XmlPi::getxval (void) const {
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

  // map the pi value to a property list

  Plist XmlPi::mapxval (void) const {
    rdlock ();
    try {
      // get default xml version
      String xvid = XmlSystem::getxvid ();
      // map to a list
      Plist result = mapxval (xvid);
      // here it is
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // map the pi value to a property list

  Plist XmlPi::mapxval (const String& xvid) const {
    rdlock ();
    try {
      // map the xml version
      XmlSystem::t_xmlv xmlv = XmlSystem::toxmlv (xvid);
      // the result property list
      Plist result;
      // create a xml buffer
      XmlBuffer xbuf (xmlv, d_xval);
      // loop and add attributes
      while (xbuf.isnext () == true) {
	Property attr = xbuf.getattr ();
	result.add (new Property (attr));
      }
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // write a pi node to a buffer

  void XmlPi::write (Buffer& buf) const {
    rdlock ();
    try {
      // add the node name
      buf.add ("<?");
      buf.add (d_name);
      // put the string value
      if (d_xval.isnil () == false) {
	buf.add (' ');
	buf.add (d_xval);
      }
      // add the terminaison
      buf.add ("?>");
      buf.add (eolq);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // write a node to an output stream

  void XmlPi::write (OutputStream& os) const {
    rdlock ();
    try {
      // add the node name
      os.write ("<?");
      os.write (d_name);
      // put the string value
      if (d_xval.isnil () == false) {
	os.write (' ');
	os.write (d_xval);
      }
      // add the terminaison
      os.write   ("?>");
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
  static const long QUARK_ZONE_LENGTH = 5;
  static QuarkZone  zone (QUARK_ZONE_LENGTH);

  // the object supported quarks
  static const long QUARK_SETNAME = zone.intern ("set-name");
  static const long QUARK_GETNAME = zone.intern ("get-name");
  static const long QUARK_SETXVAL = zone.intern ("set-xval");
  static const long QUARK_GETXVAL = zone.intern ("get-xval");
  static const long QUARK_MAPXVAL = zone.intern ("map-xval");

  // create a new object in a generic way

  Object* XmlPi::mknew (Vector* argv) {
    long argc = (argv == nullptr) ? 0 : argv->length ();
    // check for 1 argument
    if (argc == 1) {
      String name = argv->getstring (0);
      return new XmlPi (name);
    }
    // check for 2 arguments
    if (argc == 2) {
      String name = argv->getstring (0);
      String xval = argv->getstring (1);
      return new XmlPi (name, xval);
    }
    throw Exception ("argument-error",
                     "too many argument with xml pi node constructor");
  }

  // return true if the given quark is defined

  bool XmlPi::isquark (const long quark, const bool hflg) const {
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

  Object* XmlPi::apply (Evaluable* zobj, Nameset* nset, const long quark,
			Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();

    // check for 0 argument
    if (argc == 0) {
      if (quark == QUARK_GETNAME) return new String (getname ());
      if (quark == QUARK_GETXVAL) return new String (getxval ());
      if (quark == QUARK_MAPXVAL) return new Plist  (mapxval ());
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
      if (quark == QUARK_MAPXVAL) {
	String xvid = argv->getstring (0);
	Plist result = mapxval (xvid);
	return new Plist (result);
      }
    }
    // call the xml node method
    return XmlNode::apply (zobj, nset, quark, argv);
  }
}
