// ---------------------------------------------------------------------------
// - XmlPe.cpp                                                               -
// - afnix:xml module - xml parameter entity node class implementation       -
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

#include "XmlPe.hpp"
#include "XmlSystem.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - private section                                                       -
  // -------------------------------------------------------------------------

  // the entity node name
  static const String XML_PENT_NAME = "!ENTITY";

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------


  // create a parameter entity by name and value

  XmlPe::XmlPe (const String& name, const String& xval) : XmlEntity (name) {
    d_xval = xval;
  }

  // create a parameter entity by name and public/system literals

  XmlPe::XmlPe (const String& name, 
		const String& publ,const String& sysl) : XmlEntity (name) {
    d_publ = publ;
    d_sysl = sysl;
  }

  // return the document class name

  String XmlPe::repr (void) const {
    return "XmlPe";
  }
  
  // get a clone of this node

  Object* XmlPe::clone (void) const {
    rdlock ();
    try {
      // create a new node
      XmlPe* result = new XmlPe (d_name, d_xval);
      // copy the arguments
      result->d_publ = d_publ;
      result->d_sysl = d_sysl;
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

  String XmlPe::getxval (void) const {
    rdlock ();
    String result = d_xval;
    unlock ();
    return result;
  }

  // get the public literal

  String XmlPe::getpubl (void) const {
    rdlock ();
    String result = d_publ;
    unlock ();
    return result;
  }

  // get the system literal

  String XmlPe::getsysl (void) const {
    rdlock ();
    String result = d_sysl;
    unlock ();
    return result;
  }

  // write a parameter entity node to a buffer

  void XmlPe::write (Buffer& buf) const {
    rdlock ();
    try {
      // add the node name
      buf.add ("<");
      buf.add (XML_PENT_NAME);
      buf.add (" % ");
      buf.add (d_name);
      // add the value if any
      if (d_xval.isnil () == false) {
	buf.add (' ');
	buf.add (d_xval.toliteral ());
      } else {
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

  // write a parameter entity node to an output stream

  void XmlPe::write (OutputStream& os) const {
    rdlock ();
    try {
      // add the node name
      os.write ("<");
      os.write (XML_PENT_NAME);
      os.write (" % ");
      os.write (d_name);
      // add the value if any
      if (d_xval.isnil () == false) {
	os.write (' ');
	os.write (d_xval.toliteral ());
      } else {
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

  Object* XmlPe::mknew (Vector* argv) {
    long argc = (argv == nullptr) ? 0 : argv->length ();
    // check for 2 arguments
    if (argc == 2) {
      String name = argv->getstring (0);
      String xval = argv->getstring (1);
      return new XmlPe (name, xval);
    }
    // check for 3 arguments
    if (argc == 3) {
      String name = argv->getstring (0);
      String publ = argv->getstring (1);
      String sysl = argv->getstring (2);
      return new XmlPe (name, publ, sysl);
    }
    throw Exception ("argument-error",
                     "too many arguments with parameter entity constructor");
  }

  // return true if the given quark is defined

  bool XmlPe::isquark (const long quark, const bool hflg) const {
    rdlock ();
    if (zone.exists (quark) == true) {
      unlock ();
      return true;
    }
    bool result = hflg ? XmlEntity::isquark (quark, hflg) : false;
    unlock ();
    return result;
  }

  // apply this object with a set of arguments and a quark

  Object* XmlPe::apply (Evaluable* zobj, Nameset* nset, const long quark,
			     Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();

    // check for 0 argument
    if (argc == 0) {
      if (quark == QUARK_GETXVAL) return new String (getxval ());
      if (quark == QUARK_GETPUBL) return new String (getpubl ());
      if (quark == QUARK_GETSYSL) return new String (getsysl ());
    }
    // call the entity method
    return XmlEntity::apply (zobj, nset, quark, argv);
  }
}
