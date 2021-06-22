// ---------------------------------------------------------------------------
// - XmlGe.cpp                                                               -
// - afnix:xml module - xml general entity node class implementation         -
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

#include "XmlGe.hpp"
#include "XmlSystem.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - private section                                                       -
  // -------------------------------------------------------------------------

  // the entity node name
  static const String XML_GENT_NAME = "!ENTITY";

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------


  // create a general entity by name and value

  XmlGe::XmlGe (const String& name, const String& xval) : XmlEntity (name) {
    d_xval = xval;
  }

  // create a general entity by name and public/system literals

  XmlGe::XmlGe (const String& name, 
		const String& publ,const String& sysl) : XmlEntity (name) {
    d_publ = publ;
    d_sysl = sysl;
  }

  // create a general entity by name, public/system literals and data

  XmlGe::XmlGe (const String& name, const String& publ,
		const String& sysl, const String& data) : XmlEntity (name) {
    d_publ = publ;
    d_sysl = sysl;
    d_data = data;
  }

  // get a clone of this node

  Object* XmlGe::clone (void) const {
    rdlock ();
    try {
      // create a new node
      XmlGe* result = new XmlGe (d_name, d_xval);
      // copy the arguments
      result->d_publ = d_publ;
      result->d_sysl = d_sysl;
      result->d_data = d_data;
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

  // return the document class name

  String XmlGe::repr (void) const {
    return "XmlGe";
  }
  
  // get the node value

  String XmlGe::getxval (void) const {
    rdlock ();
    String result = d_xval;
    unlock ();
    return result;
  }

  // get the public literal

  String XmlGe::getpubl (void) const {
    rdlock ();
    String result = d_publ;
    unlock ();
    return result;
  }

  // get the system literal

  String XmlGe::getsysl (void) const {
    rdlock ();
    String result = d_sysl;
    unlock ();
    return result;
  }

  // get the data value

  String XmlGe::getdata (void) const {
    rdlock ();
    String result = d_data;
    unlock ();
    return result;
  }


  // write a general entity node to a buffer

  void XmlGe::write (Buffer& buf) const {
    rdlock ();
    try {
      // add the node name
      buf.add ("<");
      buf.add (XML_GENT_NAME);
      buf.add (' ');
      buf.add (d_name);
      // add the value if any
      if (d_xval.isnil () == false) {
	buf.add (' ');
	buf.add (d_xval.toliteral());
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
	// check if we have a data extension
	if (d_data.isnil () == false) {
	  buf.add (" NDATA ");
	  buf.add (d_data);
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

  // write a general entity node to an output stream

  void XmlGe::write (OutputStream& os) const {
    rdlock ();
    try {
      // add the node name
      os.write ("<");
      os.write (XML_GENT_NAME);
      os.write (' ');
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
	// check if we have data
	if (d_data.isnil () == false) {
	  os.write (" NDATA ");
	  os.write (d_data);
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
  static const long QUARK_ZONE_LENGTH = 4;
  static QuarkZone  zone (QUARK_ZONE_LENGTH);

  // the object supported quarks
  static const long QUARK_GETXVAL = zone.intern ("get-xval");
  static const long QUARK_GETDATA = zone.intern ("get-data");
  static const long QUARK_GETPUBL = zone.intern ("get-public-id");
  static const long QUARK_GETSYSL = zone.intern ("get-system-id");

  // create a new object in a generic way

  Object* XmlGe::mknew (Vector* argv) {
    long argc = (argv == nullptr) ? 0 : argv->length ();
    // check for 2 arguments
    if (argc == 2) {
      String name = argv->getstring (0);
      String xval = argv->getstring (1);
      return new XmlGe (name, xval);
    }
    // check for 3 arguments
    if (argc == 3) {
      String name = argv->getstring (0);
      String publ = argv->getstring (1);
      String sysl = argv->getstring (2);
      return new XmlGe (name, publ, sysl);
    }
    // check for 4 arguments
    if (argc == 4) {
      String name = argv->getstring (0);
      String publ = argv->getstring (1);
      String sysl = argv->getstring (2);
      String data = argv->getstring (3);
      return new XmlGe (name, publ, sysl, data);
    }
    throw Exception ("argument-error",
                     "too many arguments with xml general entity constructor");
  }

  // return true if the given quark is defined

  bool XmlGe::isquark (const long quark, const bool hflg) const {
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

  Object* XmlGe::apply (Evaluable* zobj, Nameset* nset, const long quark,
			     Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();

    // check for 0 argument
    if (argc == 0) {
      if (quark == QUARK_GETXVAL) return new String (getxval ());
      if (quark == QUARK_GETDATA) return new String (getdata ());
      if (quark == QUARK_GETPUBL) return new String (getpubl ());
      if (quark == QUARK_GETSYSL) return new String (getsysl ());
    }
    // call the entity method
    return XmlEntity::apply (zobj, nset, quark, argv);
  }
}
