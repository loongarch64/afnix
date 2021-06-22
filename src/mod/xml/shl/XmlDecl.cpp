// ---------------------------------------------------------------------------
// - XmlDecl.cpp                                                             -
// - afnix:xml module - xml declaration node class implementation            -
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

#include "XmlDecl.hpp"
#include "Boolean.hpp"
#include "XmlSystem.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - private section                                                       -
  // -------------------------------------------------------------------------

  // the pi node name
  static const String XML_DECL_NAME = "xml";
  // the version attribute
  static const String XML_VERS_ATTR = "version";
  // the encoding attribute
  static const String XML_EMOD_ATTR = "encoding";
  // the standalone attribute
  static const String XML_STND_ATTR = "standalone";

  // this procedure return the pi string value
  static String to_pi_xval (const String& xvid) {
    String result = XML_VERS_ATTR;
    result += '=';
    result += xvid.isnil () ? XmlSystem::getxvid () : xvid.toliteral ();
    return result;
  }

  // this procedure return the pi string value
  static String to_pi_xval (const String& xvid, const String emod) {
    // set the version attribute
    String result = XML_VERS_ATTR;
    result += '=';
    result += xvid.isnil () ? XmlSystem::getxvid () : xvid.toliteral ();
    // set the encoding attribute
    result += ' ';
    result += XML_EMOD_ATTR;
    result += '=';
    result += emod.isnil () ? XmlSystem::getemod () : emod.toliteral ();
    return result;
  }

  // this procedure return the pi string value
  static String to_pi_xval (const String& xvid, const String emod, bool stnd) {
    // set the version attribute
    String result = XML_VERS_ATTR;
    result += '=';
    result += xvid.isnil () ? XmlSystem::getxvid () : xvid.toliteral ();
    // set the encoding attribute
    result += ' ';
    result += XML_EMOD_ATTR;
    result += '=';
    result += emod.isnil () ? XmlSystem::getemod () : emod.toliteral ();
    // set the standalone flag
    result += ' ';
    result += '=';
    result += stnd ? "\"true\"" : "\"false\"";
    return result;
  }

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a default xml declaration

  XmlDecl::XmlDecl (void) : XmlPi (XML_DECL_NAME) {
    // set the declaration default
    d_xvid = XmlSystem::getxvid ();
    d_emod = XmlSystem::getemod ();
    d_stnd = false;
    // set the pi value
    d_xval = to_pi_xval (d_xvid, d_emod);
  }

  // create a xml declaration node with a version

  XmlDecl::XmlDecl (const String& xvid) : XmlPi (XML_DECL_NAME) {
    // check xml version
    if (XmlSystem::isxvid (xvid) == false) {
      throw Exception ("xml-error", "invalid xml version", xvid);
    }
    // set the declaration values
    d_xvid = xvid;
    d_emod = XmlSystem::getemod ();
    d_stnd = false;
    // set the pi value
    d_xval = to_pi_xval (d_xvid);
  }

  // create a xml declaration node with a version

  XmlDecl::XmlDecl (const String& xvid, 
		    const String& emod) : XmlPi (XML_DECL_NAME) {
    // check xml version
    if (XmlSystem::isxvid (xvid) == false) {
      throw Exception ("xml-error", "invalid xml version", xvid);
    }
    // set the declaration values
    d_xvid = xvid;
    d_emod = emod;
    d_stnd = false;
    // set the pi value
    d_xval = to_pi_xval (d_xvid, d_emod);
  }

  // create a xml declaration node with a version

  XmlDecl::XmlDecl (const String& xvid, const String& emod,
		    const String& stnd) : XmlPi (XML_DECL_NAME) {
    // check xml version
    if (XmlSystem::isxvid (xvid) == false) {
      throw Exception ("xml-error", "invalid xml version", xvid);
    }
    // check standalone value
    if ((stnd != "true") && (stnd != "false")) {
      throw Exception ("xml-error", "invalid xml standalone value", stnd);
    }
    // set the declaration values
    d_xvid = xvid;
    d_emod = emod;
    if (stnd == "true")  d_stnd = true;
    if (stnd == "false") d_stnd = false;
    // set the pi value
    d_xval = to_pi_xval (d_xvid, d_emod, d_stnd);
  }

  // return the document class name

  String XmlDecl::repr (void) const {
    return "XmlDecl";
  }

  // get a clone of this node

  Object* XmlDecl::clone (void) const {
    rdlock ();
    try {
      // create a new node
      XmlDecl* result = new XmlDecl;
      // copy the arguments
      result->d_xvid = d_xvid;
      result->d_emod = d_emod;
      result->d_stnd = d_stnd;
      // copy the pi arguments
      result->d_xval = d_xval;
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
  // get the xml version

  String XmlDecl::getxvid (void) const {
    rdlock ();
    try {
      String result = d_xvid;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the xml encoding mode

  String XmlDecl::getemod (void) const {
    rdlock ();
    try {
      String result = d_emod;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the xml standalone mode

  bool XmlDecl::getstnd (void) const {
    rdlock ();
    try {
      bool result = d_stnd;
      unlock ();
      return result;
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
  static const long QUARK_STNDP   = zone.intern ("standalone-p");
  static const long QUARK_GETXVID = zone.intern ("get-version");
  static const long QUARK_GETEMOD = zone.intern ("get-encoding");

  // create a new object in a generic way

  Object* XmlDecl::mknew (Vector* argv) {
    long argc = (argv == nullptr) ? 0 : argv->length ();
    // check for 0 argument
    if (argc == 0) return new XmlDecl;
    // check for 1 argument
    if (argc == 1) {
      String xvid = argv->getstring (0);
      return new XmlDecl (xvid);
    }
    // check for 2 arguments
    if (argc == 2) {
      String xvid = argv->getstring (0);
      String emod = argv->getstring (1);
      return new XmlDecl (xvid, emod);
    }
    // check for 3 arguments
    if (argc == 3) {
      String xvid = argv->getstring (0);
      String emod = argv->getstring (1);
      String stnd = argv->getstring (2);
      return new XmlDecl (xvid, emod, stnd);
    }
    throw Exception ("argument-error",
                     "too many argument with xml declaration constructor");
  }

  // return true if the given quark is defined

  bool XmlDecl::isquark (const long quark, const bool hflg) const {
    rdlock ();
    if (zone.exists (quark) == true) {
      unlock ();
      return true;
    }
    bool result = hflg ? XmlPi::isquark (quark, hflg) : false;
    unlock ();
    return result;
  }

  // apply this object with a set of arguments and a quark

  Object* XmlDecl::apply (Evaluable* zobj, Nameset* nset, const long quark,
			  Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();

    // check for 0 argument
    if (argc == 0) {
      if (quark == QUARK_GETXVID) return new String  (getxvid ());
      if (quark == QUARK_GETEMOD) return new String  (getemod ());
      if (quark == QUARK_STNDP)   return new Boolean (getstnd ());
    }
    // call the xml pi method
    return XmlPi::apply (zobj, nset, quark, argv);
  }
}
