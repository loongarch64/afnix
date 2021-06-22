// ---------------------------------------------------------------------------
// - XmlDoctype.cpp                                                          -
// - afnix:xml module - xml document type node class implementation          -
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
#include "XmlDoctype.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - private section                                                       -
  // -------------------------------------------------------------------------

  // the doctype node name
  static const String XML_DOCT_NAME = "!DOCTYPE";

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------


  // create a xml doctype by value

  XmlDoctype::XmlDoctype (const String& xval) {
    d_eflg = true;
    d_xval = xval;
    p_node = nullptr;
  }

  // create a xml doctype by value and system literal

  XmlDoctype::XmlDoctype (const String& xval, const String& sysl) {
    d_eflg = true;
    d_xval = xval;
    d_sysl = sysl;
    p_node = nullptr;
  }

  // create a xml doctype by value and public/system literals

  XmlDoctype::XmlDoctype (const String& xval, const String& publ, 
			  const String& sysl) {
    d_eflg = true;
    d_xval = xval;
    d_publ = publ;
    d_sysl = sysl;
    p_node = nullptr;
  }

  // destroy this document type

  XmlDoctype::~XmlDoctype (void) {
    if (p_node != nullptr) p_node->release ();
    Object::dref (p_node);
  }

  // return the document class name

  String XmlDoctype::repr (void) const {
    return "XmlDoctype";
  }
  
  // get a clone of this node

  Object* XmlDoctype::clone (void) const {
    rdlock ();
    try {
      // create a new node
      XmlDoctype* result = new XmlDoctype (d_xval, d_publ, d_sysl);
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

  String XmlDoctype::getxval (void) const {
    rdlock ();
    String result = d_xval;
    unlock ();
    return result;
  }

  // get the public literal

  String XmlDoctype::getpubl (void) const {
    rdlock ();
    String result = d_publ;
    unlock ();
    return result;
  }

  // get the system literal

  String XmlDoctype::getsysl (void) const {
    rdlock ();
    String result = d_sysl;
    unlock ();
    return result;
  }

  // attach a node to the doctype node

  void XmlDoctype::setnode (XmlNode* node) {
    wrlock ();
    // set the node
    Object::iref (node);
    Object::dref (p_node);
    p_node = node;
    unlock ();
  }

  // write a doctype node to a buffer

  void XmlDoctype::write (Buffer& buf) const {
    rdlock ();
    try {
      // add the node name
      buf.add ("<");
      buf.add (XML_DOCT_NAME);
      // add the value
      buf.add (' ');
      buf.add (d_xval);
      // check if we have a public or system identifier
      if (d_publ.isnil () == false) {
	buf.add (" PUBLIC ");
	buf.add (d_publ.toliteral ());
	buf.add (' ');
	buf.add (d_sysl.toliteral ());
      } else if (d_sysl.isnil () == false) {
	buf.add (" SYSTEM ");
	buf.add (d_sysl.toliteral ());
      }
      // check if we have a node
      if (p_node != nullptr) {
	buf.add (" [ ");
	buf.add (eolq);
	p_node->write (buf);
	buf.add (" ]");
      }
      // add the terminaison
      buf.add (">");
      buf.add (eolq);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // write a doctype node to an output stream

  void XmlDoctype::write (OutputStream& os) const {
    rdlock ();
    try {
      // add the node name
      os.write ("<");
      os.write (XML_DOCT_NAME);
      // add the value
      os.write (' ');
      os.write (d_xval);
      // check if we have a public or system identifier
      if (d_publ.isnil () == false) {
	os.write (" PUBLIC ");
	os.write (d_publ.toliteral ());
	os.write (' ');
	os.write (d_sysl.toliteral ());
      } else if (d_sysl.isnil () == false) {
	os.write (" SYSTEM ");
	os.write (d_sysl.toliteral ());
      }
      // check if we have a node
      if (p_node != nullptr) {
	os.write (" [ ");
	os.newline ();
	p_node->write (os);
	os.write (" ]");
      }
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
  static const long QUARK_ZONE_LENGTH = 3;
  static QuarkZone  zone (QUARK_ZONE_LENGTH);

  // the object supported quarks
  static const long QUARK_GETXVAL = zone.intern ("get-xval");
  static const long QUARK_GETPUBL = zone.intern ("get-public-id");
  static const long QUARK_GETSYSL = zone.intern ("get-system-id");

  // create a new object in a generic way

  Object* XmlDoctype::mknew (Vector* argv) {
    long argc = (argv == nullptr) ? 0 : argv->length ();
    // check for 1 argument
    if (argc == 1) {
      String xval = argv->getstring (0);
      return new XmlDoctype (xval);
    }
    // check for 2 arguments
    if (argc == 2) {
      String xval = argv->getstring (0);
      String sysl = argv->getstring (1);
      return new XmlDoctype (xval, sysl);
    }
    // check for 3 arguments
    if (argc == 3) {
      String xval = argv->getstring (0);
      String publ = argv->getstring (1);
      String sysl = argv->getstring (2);
      return new XmlDoctype (xval, publ, sysl);
    }
    throw Exception ("argument-error",
                     "too many argument with xml doctype constructor");
  }

  // return true if the given quark is defined

  bool XmlDoctype::isquark (const long quark, const bool hflg) const {
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

  Object* XmlDoctype::apply (Evaluable* zobj, Nameset* nset, const long quark,
			     Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();

    // check for 0 argument
    if (argc == 0) {
      if (quark == QUARK_GETXVAL) return new String (getxval ());
      if (quark == QUARK_GETPUBL) return new String (getpubl ());
      if (quark == QUARK_GETSYSL) return new String (getsysl ());
    }
    // call the node method
    return XmlNode::apply (zobj, nset, quark, argv);
  }
}
