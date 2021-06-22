// ---------------------------------------------------------------------------
// - XmlText.cpp                                                             -
// - afnix:xml module - xml text node class implementation                   -
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

#include "XmlText.hpp"
#include "XmlSystem.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"

namespace afnix {

  //
  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a default text node

  XmlText::XmlText (void) {
    d_eflg = true;
  }

  // create a text node by value

  XmlText::XmlText (const String& xval) {
    d_eflg = true;
    d_xval = xval;
  }

  // return the document class name

  String XmlText::repr (void) const {
    return "XmlText";
  }

  // get a clone of this node

  Object* XmlText::clone (void) const {
    rdlock ();
    try {
      // create a new node
      XmlText* result = new XmlText (d_xval);
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

  // set the text value

  void XmlText::setxval (const String& xval) {
    wrlock ();
    try {
      d_xval = xval;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the text value

  String XmlText::getxval (void) const {
    rdlock ();
    String result = d_xval;
    unlock ();
    return result;
  }
 
  // get the node content as a text

  String XmlText::totext (void) const {
    rdlock ();
    String result = d_xval;
    unlock ();
    return result;
  }

  // get the node content as a pre-normalized text

  String XmlText::prenorm (void) const {
    rdlock ();
    String result = XmlSystem::prenorm (d_xval);
    unlock ();
    return result;
  }

  // get the node content as a normalized text

  String XmlText::tonorm (void) const {
    rdlock ();
    String result = XmlSystem::tonorm (d_xval);
    unlock ();
    return result;
  }

  // normalize this text node

  void XmlText::normalize (void) {
    wrlock ();
    try {
      d_xval = XmlSystem::tonorm (d_xval);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // write a text node to a buffer

  void XmlText::write (Buffer& buf) const {
    rdlock ();
    try {
      if (buf.getemod () == Encoding::EMOD_BYTE) {
	buf.add (XmlSystem::touesc (d_xval));
      } else {
	buf.add (XmlSystem::totesc (d_xval));
      }	
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // write a text node to an output stream

  void XmlText::write (OutputStream& os) const {
    rdlock ();
    try {
      if (os.getemod () == Encoding::EMOD_BYTE) {
	os.write (XmlSystem::touesc(d_xval));
      } else {
	os.write (XmlSystem::totesc(d_xval));
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
  static const long QUARK_ZONE_LENGTH = 4;
  static QuarkZone  zone (QUARK_ZONE_LENGTH);

  // the object supported quarks
  static const long QUARK_TONORM  = zone.intern ("to-normal");
  static const long QUARK_PRENORM = zone.intern ("pre-normal");
  static const long QUARK_SETXVAL = zone.intern ("set-xval");
  static const long QUARK_GETXVAL = zone.intern ("get-xval");

  // create a new object in a generic way

  Object* XmlText::mknew (Vector* argv) {
    long argc = (argv == nullptr) ? 0 : argv->length ();
    // check for 0 argument
    if (argc == 0) return new XmlText;
    // check for 1 argument
    if (argc == 1) {
      String xval = argv->getstring (0);
      return new XmlText (xval);
    }
    throw Exception ("argument-error",
                     "too many argument with xml text node constructor");
  }

  // return true if the given quark is defined

  bool XmlText::isquark (const long quark, const bool hflg) const {
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

  Object* XmlText::apply (Evaluable* zobj, Nameset* nset, const long quark,
			  Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();

    // check for 0 argument
    if (argc == 0) {
      if (quark == QUARK_TONORM)  return new String (tonorm  ());
      if (quark == QUARK_PRENORM) return new String (prenorm ());
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
