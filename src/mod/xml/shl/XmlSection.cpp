// ---------------------------------------------------------------------------
// - XmlSection.cpp                                                          -
// - afnix:xml module - xml section node class implementation                -
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
#include "XmlSection.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------


  // create a xml section by value

  XmlSection::XmlSection (const String& xval) {
    d_eflg = true;
    d_xval = xval;
    p_node = nullptr;
  }

  // destroy this section node

  XmlSection::~XmlSection (void) {
    if (p_node != nullptr) p_node->release ();
    Object::dref (p_node);
  }

  // return the document class name

  String XmlSection::repr (void) const {
    return "XmlSection";
  }
  
  // get a clone of this node

  Object* XmlSection::clone (void) const {
    rdlock ();
    try {
      // create a new node
      XmlSection* result = new XmlSection (d_xval);
      // copy the arguments
      if (p_node != nullptr) Object::iref (result->p_node = p_node->copy ());
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

  // get the node value

  String XmlSection::getxval (void) const {
    rdlock ();
    String result = d_xval;
    unlock ();
    return result;
  }

  // attach a node to the section node

  void XmlSection::setnode (XmlNode* node) {
    wrlock ();
    // set the node
    Object::iref (node);
    Object::dref (p_node);
    p_node = node;
    unlock ();
  }

  // write a section node to a buffer

  void XmlSection::write (Buffer& buf) const {
    rdlock ();
    try {
      // add the node name
      buf.add ("<![");
      // add the value
      buf.add (d_xval);
      // check if we have a node
      buf.add (" [ ");
      if (p_node != nullptr) {
	buf.add (eolq);
	p_node->write (buf);
      }
      buf.add ("]");
      // add the terminaison
      buf.add ("]>");
      buf.add (eolq);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // write a section node to an output stream

  void XmlSection::write (OutputStream& os) const {
    rdlock ();
    try {
      // add the node name
      os.write ("<![");
      // add the value
      os.write (d_xval);
      // check if we have a node
      os.write (" [ ");
      if (p_node != nullptr) {
	os.newline ();
	p_node->write (os);
      }
      os.write ("]");
      // add the terminaison
      os.write   ("]>");
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
  static const long QUARK_ZONE_LENGTH = 1;
  static QuarkZone  zone (QUARK_ZONE_LENGTH);

  // the object supported quarks
  static const long QUARK_GETXVAL = zone.intern ("get-xval");

  // create a new object in a generic way

  Object* XmlSection::mknew (Vector* argv) {
    long argc = (argv == nullptr) ? 0 : argv->length ();
    // check for 1 argument
    if (argc == 1) {
      String xval = argv->getstring (0);
      return new XmlSection (xval);
    }
    throw Exception ("argument-error",
                     "too many argument with xml section constructor");
  }

  // return true if the given quark is defined

  bool XmlSection::isquark (const long quark, const bool hflg) const {
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

  Object* XmlSection::apply (Evaluable* zobj, Nameset* nset, const long quark,
			     Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();

    // check for 0 argument
    if (argc == 0) {
      if (quark == QUARK_GETXVAL) return new String (getxval ());
    }
    // call the node method
    return XmlNode::apply (zobj, nset, quark, argv);
  }
}
