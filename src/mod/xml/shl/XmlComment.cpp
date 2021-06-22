// ---------------------------------------------------------------------------
// - XmlComment.cpp                                                          -
// - afnix:xml module - xml comment node class implementation                -
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

#include "QuarkZone.hpp"
#include "Exception.hpp"
#include "XmlComment.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - private section                                                       -
  // -------------------------------------------------------------------------

  static const String XML_COMMENT_NAME = "!--";

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a default comment node

  XmlComment::XmlComment (void) {
    d_eflg = true;
  }

  // create a comment node by value

  XmlComment::XmlComment (const String& xval) {
    d_eflg = true;
    d_xval = xval;
  }

  // return the document class name

  String XmlComment::repr (void) const {
    return "XmlComment";
  }

  // get a clone of this node

  Object* XmlComment::clone (void) const {
    rdlock ();
    try {
      // create a new node
      XmlComment* result = new XmlComment (d_xval);
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

  // set the comment value

  void XmlComment::setxval (const String& xval) {
    wrlock ();
    try {
      d_xval = xval;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the comment value

  String XmlComment::getxval (void) const {
    rdlock ();
    String result = d_xval;
    unlock ();
    return result;
  }

  // write a comment node to a buffer

  void XmlComment::write (Buffer& buf) const {
    rdlock ();
    try {
      buf.add ("<!--");
      buf.add (d_xval);
      buf.add ("-->");
      buf.add (eolq);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // write a comment node to an output stream

  void XmlComment::write (OutputStream& os) const {
    rdlock ();
    try {
      // add the node name
      os.write ("<!--");
      os.write (d_xval);
      os.write ("-->");
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
  static const long QUARK_ZONE_LENGTH = 2;
  static QuarkZone  zone (QUARK_ZONE_LENGTH);

  // the object supported quarks
  static const long QUARK_SETXVAL = zone.intern ("set-xval");
  static const long QUARK_GETXVAL = zone.intern ("get-xval");

  // create a new object in a generic way

  Object* XmlComment::mknew (Vector* argv) {
    long argc = (argv == nullptr) ? 0 : argv->length ();
    // check for 0 argument
    if (argc == 0) return new XmlComment;
    // check for 1 argument
    if (argc == 1) {
      String xval = argv->getstring (0);
      return new XmlComment (xval);
    }
    throw Exception ("argument-error",
                     "too many argument with xml comment node constructor");
  }

  // return true if the given quark is defined

  bool XmlComment::isquark (const long quark, const bool hflg) const {
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

  Object* XmlComment::apply (Evaluable* zobj, Nameset* nset, const long quark,
			     Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();

    // check for 0 argument
    if (argc == 0) {
      if (quark == QUARK_GETXVAL) return new String (getxval ());
    }
    // check for 1 argument
    if (argc == 1) {
      if (quark == QUARK_SETXVAL) {
	String xval = argv->getstring (0);
	setxval (xval);
	return nullptr;
      }
    }
    // call the node method
    return XmlNode::apply (zobj, nset, quark, argv);
  }
}
