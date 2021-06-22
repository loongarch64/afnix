// ---------------------------------------------------------------------------
// - XmlRoot.cpp                                                             -
// - afnix:xml module - xml root node class implementation                   -
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
#include "Boolean.hpp"
#include "Evaluable.hpp"
#include "XmlSystem.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a default root node

  XmlRoot::XmlRoot (void) {}

  // create a root node with a declaration node

  XmlRoot::XmlRoot (const bool dflg) {
    if (dflg == true) addchild (new XmlDecl);
  }

  // return the class name

  String XmlRoot::repr (void) const {
    return "XmlRoot";
  }

  // get a clone of this node

  Object* XmlRoot::clone (void) const {
    rdlock ();
    try {
      // create a new node
      XmlRoot* result = new XmlRoot;
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

  // duplicate the root node without the declaration

  XmlRoot* XmlRoot::bdup (void) const {
    rdlock ();
    // the result node
    XmlRoot* result = new XmlRoot;
    try {
      // check if we have some nodes
      long len = lenchild ();
      // loop in the node
      for (long i = 0; i < len; i++) {
	XmlNode* node = getchild (i);
	if (node == nullptr) continue;
	if (dynamic_cast <XmlDecl*> (node) != nullptr) continue;
	result->addchild (node->copy ());
      }
      // here is the result node
      unlock ();
      return result;
      // get the first node
    } catch (...) {
      delete result;
      unlock ();
      throw;
    }
  }

  // return true if there is a declaration node

  bool XmlRoot::isdecl (void) const {
    rdlock ();
    try {
      // check if we have some nodes
      long len = lenchild ();
      // get the first node
      XmlNode* node = (len == 0) ? nullptr : getchild (0);
      // normaly the first node is the declaration node
      XmlDecl* decl = dynamic_cast <XmlDecl*> (node);
      // extract result
      bool result = (decl == nullptr) ? false : true;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the declaration node - if any

  XmlDecl* XmlRoot::getdecl (void) const {
    rdlock ();
    try {
      // check if we have some nodes
      long len = lenchild ();
      // get the first node
      XmlNode* node = (len == 0) ? nullptr : getchild (0);
      if (node == nullptr) {
	unlock ();
	return nullptr;
      }
      // normaly the first node is the declaration node
      XmlDecl* result = dynamic_cast <XmlDecl*> (node);
      if (result == nullptr) {
	throw Exception ("xml-error", "first node must be a declaration node",
			 Object::repr (node));
      }
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the root encoding mode

  String XmlRoot::getemod (void) const {
    rdlock ();
    try {
      // initialize result
      String result = XmlSystem::getemod ();
      // check if we have a declaration node
      if (isdecl () == true) {
	// get the declaration node
	XmlDecl* decl = getdecl ();
	// update the encoding mode
	result = decl->getemod ();
      }
      // here is the result
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // write a node to a buffer

  void XmlRoot::write (Buffer& buf) const {
    rdlock ();
    try {
      long len = lenchild ();
      for (long i = 0; i < len; i++) {
	XmlNode* node = getchild (i);
	if (node == nullptr) continue;
	node->write (buf);
      }
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // write a node to an output stream

  void XmlRoot::write (OutputStream& os) const {
    rdlock ();
    try {
      long len = lenchild ();
      for (long i = 0; i < len; i++) {
	XmlNode* node = getchild (i);
	if (node == nullptr) continue;
	node->write (os);
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
  static const long QUARK_DECLP   = zone.intern ("declaration-p");
  static const long QUARK_DUPBODY = zone.intern ("dup-body");
  static const long QUARK_GETDECL = zone.intern ("get-declaration");
  static const long QUARK_GETEMOD = zone.intern ("get-encoding");

  // create a new object in a generic way

  Object* XmlRoot::mknew (Vector* argv) {
    long argc = (argv == nullptr) ? 0 : argv->length ();
    // create a default root node
    if (argc == 0) return new XmlRoot;
    throw Exception ("argument-error",
                     "too many argument with xml root node constructor");
  }

  // return true if the given quark is defined

  bool XmlRoot::isquark (const long quark, const bool hflg) const {
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
  
  Object* XmlRoot::apply (Evaluable* zobj, Nameset* nset, const long quark,
			  Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();

    // check for 0 argument
    if (argc == 0) {
      if (quark == QUARK_DECLP)   return new Boolean (isdecl ());
      if (quark == QUARK_GETEMOD) return new String  (getemod ());
      if (quark == QUARK_DUPBODY) return bdup ();
      if (quark == QUARK_GETDECL) {
	rdlock ();
	try {
	  XmlDecl* result = getdecl ();
	  zobj->post (result);
	  unlock ();
	  return result;
	} catch (...) {
	  unlock ();
	  throw;
	}
      }
    }
    // check the xml node object
    return XmlNode::apply (zobj, nset, quark, argv);
  }
}
