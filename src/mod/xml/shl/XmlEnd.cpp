// ---------------------------------------------------------------------------
// - XmlEnd.cpp                                                              -
// - afnix:xml module - xml end node class implementation                    -
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

#include "XmlEnd.hpp"
#include "Evaluable.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a end node by name

  XmlEnd::XmlEnd (const String& name) {
    d_eflg = true;
    d_name = name;
  }

  // return the document class name

  String XmlEnd::repr (void) const {
    return "XmlEnd";
  }

  // get a clone of this node

  Object* XmlEnd::clone (void) const {
    rdlock ();
    try {
      // create a new node
      XmlEnd* result = new XmlEnd (d_name);
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

  // set the parent node

  void XmlEnd::setparent (XmlNode* node) {
    throw Exception ("xml-error", "attempting to set end node parent", d_name);
  }

  // check if a node name is valid

  bool XmlEnd::isname (const String& name) const {
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


  // set the end node name

  void XmlEnd::setname (const String& name) {
    wrlock ();
    try {
      d_name = name;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the end node name

  String XmlEnd::getname (void) const {
    rdlock ();
    String result = d_name;
    unlock ();
    return result;
  }

  // write a node to a buffer

  void XmlEnd::write (Buffer& buf) const {
    throw Exception ("xml-error", "attempting to write end node", d_name);
  }

  // write a node to an output stream

  void XmlEnd::write (OutputStream& os) const {
    throw Exception ("xml-error", "attempting to write end node", d_name);
  }

  // -------------------------------------------------------------------------
  // - object section                                                        -
  // -------------------------------------------------------------------------

  // the quark zone
  static const long QUARK_ZONE_LENGTH = 2;
  static QuarkZone  zone (QUARK_ZONE_LENGTH);

  // the object supported quarks
  static const long QUARK_SETNAME  = zone.intern ("set-name");
  static const long QUARK_GETNAME  = zone.intern ("get-name");

  // create a new object in a generic way

  Object* XmlEnd::mknew (Vector* argv) {
    long argc = (argv == nullptr) ? 0 : argv->length ();
    // check for 1 argument
    if (argc == 1) {
      String name = argv->getstring (0);
      return new XmlEnd (name);
    }
    throw Exception ("argument-error",
                     "too many argument with xml end node constructor");
  }

  // return true if the given quark is defined

  bool XmlEnd::isquark (const long quark, const bool hflg) const {
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
  
  Object* XmlEnd::apply (Evaluable* zobj, Nameset* nset, const long quark,
			 Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();

    // check for 0 argument
    if (argc == 0) {
      if (quark == QUARK_GETNAME) return new String (getname ());
    }
    // check for 1 argument
    if (argc == 1) {
      if (quark == QUARK_SETNAME) {
	String name = argv->getstring (0);
	setname (name);
	return nullptr;
      }
    }
    // check the xml node object
    return XmlNode::apply (zobj, nset, quark, argv);
  }
}
