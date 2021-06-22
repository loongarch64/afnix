// ---------------------------------------------------------------------------
// - XhtmlTitle.cpp                                                          -
// - afnix:wax module - xhtml title node class implementation                -
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
#include "QuarkZone.hpp"
#include "Exception.hpp"
#include "XhtmlTitle.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - private section                                                       -
  // -------------------------------------------------------------------------

  // the title node name
  static const String XML_TAG_NAME = "title";

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a xhtml title node by title

  XhtmlTitle::XhtmlTitle (const String& title) : XmlTag (XML_TAG_NAME) {
    settitle (title);
  }

  // return the class name

  String XhtmlTitle::repr (void) const {
    return "XhtmlTitle";
  }

  // set the title by value

  void XhtmlTitle::settitle (const String& title) {
    wrlock ();
    try {
      // clear all child nodes
      clrchild ();
      // add the title text node
      addchild (new XmlText (title));
      // done
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
  static const long QUARK_SETTITLE = zone.intern ("set-title");

  // create a new object in a generic way

  Object* XhtmlTitle::mknew (Vector* argv) {
    long argc = (argv == nullptr) ? 0 : argv->length ();
    // check for 1 argument
    if (argc == 1) {
      String title = argv->getstring (0);
      return new XhtmlTitle (title);
    }
    // wrong arguments
    throw Exception ("argument-error", 
		     "too many arguments with xhtml title constructor");
  }

  // return true if the given quark is defined

  bool XhtmlTitle::isquark (const long quark, const bool hflg) const {
    rdlock ();
    if (zone.exists (quark) == true) {
      unlock ();
      return true;
    }
    bool result = hflg ? XmlTag::isquark (quark, hflg) : false;
    unlock ();
    return result;
  }

  // apply this object with a set of arguments and a quark

  Object* XhtmlTitle::apply (Evaluable* zobj, Nameset* nset, const long quark,
			    Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();
    
    // dispatch 1 argument
    if (argc == 1) {
      if (quark == QUARK_SETTITLE) {
	String title = argv->getstring (0);
	settitle (title);
	return nullptr;
      }
    }

    // call the xml tag method
    return XmlTag::apply (zobj, nset, quark, argv);
  }
}
