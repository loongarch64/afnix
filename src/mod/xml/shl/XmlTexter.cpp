// ---------------------------------------------------------------------------
// - XmlTexter.cpp                                                           -
// - afnix:xml module - xml texter class implementation                      -
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

#include "XmlRoot.hpp"
#include "XmlText.hpp"
#include "Nameable.hpp"
#include "Evaluable.hpp"
#include "XsoStream.hpp"
#include "XmlBuffer.hpp"
#include "XmlTexter.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"
#include "InputString.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - private section                                                       -
  // -------------------------------------------------------------------------

  // the standard reference marker
  static const t_quad XML_SREF_MC = 0x7FFFFFFFUL;
  
  // this function create a xml text node from an input stream
  static XmlNode* get_xml_text (XsoStream& xis) {
    // create a xml buffer
    XmlBuffer xbuf;
    try {
      // loop as long as we have a valid stream
      while (xis.valid () == true) xbuf.add (xis.getu () & XML_SREF_MC);
      // create a xml text node
      XmlNode* result = new XmlText (xbuf.totext ());
      // here it is
      return result;
    } catch (Exception& e) {
      e.updlnum (xbuf.getlnum ());
      throw e;
    } catch (...) {
      throw;
    }
  }

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create an empty texter

  XmlTexter::XmlTexter (void) {
    d_xvid = XmlSystem::getxvid ();
    p_root = nullptr;
    reset ();
  }

  // create a xml texter with a version

  XmlTexter::XmlTexter (const String& xvid) {
    if (XmlSystem::isxvid (xvid) == false) {
      throw Exception ("xml-error", "invalid xml version", xvid);
    }
    d_xvid = xvid;
    p_root = nullptr;
    reset ();
  }
  
  // destroy this texter

  XmlTexter::~XmlTexter (void) {
    reset ();
  }

  // return the document class name

  String XmlTexter::repr (void) const {
    return "XmlTexter";
  }

  // reset this texter

  void XmlTexter::reset (void) {
    wrlock ();
    Object::iref (this);
    try {
      if (p_root != nullptr) p_root->release ();
      Object::dref (p_root);
      p_root = nullptr;
      Object::tref (this);
      unlock ();
    } catch (...) {
      Object::tref (this);
      unlock ();
      throw;
    }
  }

  // get the root node

  XmlRoot* XmlTexter::getroot (void) const {
    rdlock ();
    XmlRoot* result = p_root;
    unlock ();
    return result;
  }

  // parse the input stream

  void XmlTexter::parse (InputStream* is) {
    // check the input stream
    if (is == nullptr) return;
    // create a xml stream
    XsoStream xis (is);
    // lock and parse
    wrlock ();
    try {
      //check if we have a root node
      if (p_root == nullptr) Object::iref (p_root = new XmlRoot);
      // add a declaration node
      p_root->addchild (new XmlDecl  (d_xvid));
      // get the text node
      p_root->addchild (get_xml_text (xis));
      unlock ();
    } catch (Exception& e) {
      // set the line number
      e.updlnum (xis.getlnum ());
      // check for a name
      Nameable* nobj = dynamic_cast <Nameable*> (is);
      if (nobj != nullptr) e.setname (nobj->getname ());
      // reset and clean
      reset  ();
      unlock ();
      throw e;
    } catch (...) {
      reset  ();
      unlock ();
      throw;
    }
  }

  // parse a string 

  void XmlTexter::parse (const String& value) {
    // create an input stream
    InputStream* is = new InputString (value);
    // lock and parse
    wrlock ();
    try {
      parse  (is);
      delete is;
      unlock ();
    } catch (...) {
      delete is;
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
  static const long QUARK_RESET   = zone.intern ("reset");
  static const long QUARK_PARSE   = zone.intern ("parse");
  static const long QUARK_GETROOT = zone.intern ("get-root");

  // create a new object in a generic way

  Object* XmlTexter::mknew (Vector* argv) {
    long argc = (argv == nullptr) ? 0 : argv->length ();
    // create a default texter object
    if (argc == 0) return new XmlTexter;
    // argument error
    throw Exception ("argument-error",
                     "too many argument with xml texter constructor");
  }

  // return true if the given quark is defined

  bool XmlTexter::isquark (const long quark, const bool hflg) const {
    rdlock ();
    if (zone.exists (quark) == true) {
      unlock ();
      return true;
    }
    bool result = hflg ? Object::isquark (quark, hflg) : false;
    unlock ();
    return result;
  }

  // apply this object with a set of arguments and a quark

  Object* XmlTexter::apply (Evaluable* zobj, Nameset* nset, const long quark,
			      Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();

    // check for 0 argument
    if (argc == 0) {
      if (quark == QUARK_RESET) {
	reset ();
	return nullptr;
      }
      if (quark == QUARK_GETROOT) {
	rdlock ();
	Object* result = getroot ();
	zobj->post (result);
	unlock ();
	return result;
      }
    }
    // check for 1 argument
    if (argc == 1) {
      if (quark == QUARK_PARSE) {
	Object* obj = argv->get (0);
	if (obj == nullptr) return nullptr;
	// check for an input stream
	InputStream* is = dynamic_cast <InputStream*> (obj);
	if (is != nullptr) {
	  parse (is);
	  return nullptr;
	}
	// check for a string
	String* sobj = dynamic_cast <String*> (obj);
	if (sobj != nullptr) {
	  parse (*sobj);
	  return nullptr;
	}
	throw Exception ("type-error", "invalid object with parse",
			 Object::repr (obj));
      }
    }
    // call the object method
    return Object::apply (zobj, nset, quark, argv);
  }
}
