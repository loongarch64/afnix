// ---------------------------------------------------------------------------
// - XsmTree.cpp                                                             -
// - afnix:xml module - xsm tree class implementation                        -
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

#include "Vector.hpp"
#include "Integer.hpp"
#include "XsmTree.hpp"
#include "Evaluable.hpp"
#include "InputFile.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"
#include "XsmBuffer.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a default xsm tree

  XsmTree::XsmTree (void) {
    p_tree = nullptr;
  }

  // destroy this xsm tree

  XsmTree::~XsmTree (void) {
    Object::dref (p_tree);
  }

  // return the xsm tree class name

  String XsmTree::repr (void) const {
    return "XsmTree";
  }

  // return the xsm tree name

  String XsmTree::getname (void) const {
    rdlock ();
    String result = d_name;
    unlock ();
    return result;
  }

  // set the xsm tree name

  void XsmTree::setname (const String& name) {
    wrlock ();
    d_name = name;
    unlock ();
  }
  
  // return the xsm tree length

  long XsmTree::length (void) const {
    rdlock ();
    long result = (p_tree == nullptr) ? 0 : p_tree->length ();
    unlock ();
    return result;
  }

  // get a node by index

  XsmNode* XsmTree::getnode (const long index) const {
    rdlock ();
    try {
      if (p_tree == nullptr) {
	throw Exception ("index-error", "out of bound node index");
      }
      XsmNode* result = dynamic_cast <XsmNode*> (p_tree->get (index));
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get an info node by index

  XsoInfo* XsmTree::getinfo (const long index) const {
    return getinfo (index, false);
  }

  // get an info node by index and case flag

  XsoInfo* XsmTree::getinfo (const long index, const bool lwcf) const {
    rdlock ();
    try {
      // check we have a tag node
      XsmNode* tag = getnode (index);
      if ((tag == nullptr) || (tag->isntag () == false)) {
	throw Exception ("type-error", "invalid node index for get-info");
      }
      // get the tag name
      String name = tag->getname (lwcf);
      // get the tag attribute list
      Plist  alst = tag->getattr (lwcf);
      // get the tag text info
      XsmBuffer xbuf;
      bool xvok = false;
      // loop until the end tag and accumulate text
      long len = length ();
      for (long i = index + 1; i < len; i++) {
	XsmNode* node = getnode (i);
	if (node == nullptr) continue;
	// check if we have a end node
	if (node->isend () == true) {
	  if (name == node->getname (lwcf)) {
	    xvok = true;
	    break;
	  }
	  continue;
	}
	// check if we have a normal tag match and avoid recursion
	if (node->isntag () == true) {
	  if (name == node->getname (lwcf)) break;
	}
	// check if we have a xval node
	if (node->isxval () == true) xbuf.add (node->tostring ());
      }
      // check if the text is ok
      if (xvok == true) {
	xbuf.stripm ();
	xbuf.strip  ();
      } else {
	xbuf.reset ();
      }
      String xval = xbuf.tostring ();
      // create here the info node
      XsoInfo* result = new XsoInfo (name, alst, xval);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get an info vector by name

  Vector* XsmTree::getivec (const String& name) const {
    return getivec (name, false);
  }

  // get an info vector by name and case flag

  Vector* XsmTree::getivec (const String& name, const bool lwcf) const {
    rdlock ();
    // make sure the argument follow the flag
    String tnam = lwcf ? name.tolower () : name;
    // create the result vector
    Vector* result = new Vector;
    // loop in the xsm tree
    try {
      long dlen = length ();
      for (long i = 0; i < dlen; i++) {
	XsmNode* node = getnode (i);
	if (node == nullptr) continue;
	if (node->isntag () == false) continue;
	if (node->getname (lwcf) == tnam) result->add (getinfo (i));
      }
      unlock ();
      return result;
    } catch (...) {
      delete result;
      unlock ();
      throw;
    }
  }

  // get a vector of words

  Vector* XsmTree::getwords (void) const {
    rdlock ();
    Vector* result = new Vector;
    try {
      // loop in the xsm tree nodes
      long dlen = length ();
      for (long i = 0; i < dlen; i++) {
	// get the node
	XsmNode* node = getnode (i);
	if (node == nullptr) continue;
	// get the word vector
	Strvec words = node->getwords ();
	// copy the vector into the results
	long vlen = words.length ();
	for (long j = 0; j < vlen; j++) {
	  result->add (new String (words.get (j)));
	}
      }
      // here it is
      unlock ();
      return result;
    } catch (...) {
      delete result;
      unlock ();
      throw;
    }
  }

  // -------------------------------------------------------------------------
  // - object section                                                        -
  // -------------------------------------------------------------------------

  // the quark zone
  static const long QUARK_ZONE_LENGTH = 6;
  static QuarkZone  zone (QUARK_ZONE_LENGTH);

  // the object supported quarks
  static const long QUARK_LENGTH   = zone.intern ("length");
  static const long QUARK_GETNODE  = zone.intern ("get-node");
  static const long QUARK_SETNAME  = zone.intern ("set-name");
  static const long QUARK_GETINFO  = zone.intern ("get-info");
  static const long QUARK_GETIVEC  = zone.intern ("get-info-vector");
  static const long QUARK_GETWORDS = zone.intern ("get-words");

  // return true if the given quark is defined

  bool XsmTree::isquark (const long quark, const bool hflg) const {
    rdlock ();
    if (zone.exists (quark) == true) {
      unlock ();
      return true;
    }
    bool result = hflg ? Nameable::isquark (quark, hflg) : false;
    unlock ();
    return result;
  }

  // apply this object with a set of arguments and a quark

  Object* XsmTree::apply (Evaluable* zobj, Nameset* nset, const long quark,
			  Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();

    // check for 0 argument
    if (argc == 0) {
      if (quark == QUARK_LENGTH)   return new Integer (length ());
      if (quark == QUARK_GETWORDS) return getwords ();
    }
    // check for 1 argument
    if (argc == 1) {
      if (quark == QUARK_GETNODE) {
	long index = argv->getlong (0);
	rdlock ();
	Object* result = getnode (index);
	zobj->post (result);
	unlock ();
	return result;
      }
      if (quark == QUARK_SETNAME) {
	String name = argv->getstring (0);
	setname (name);
	return nullptr;
      }
      if (quark == QUARK_GETINFO) {
	long index = argv->getlong (0);
	return getinfo (index);
      }
      if (quark == QUARK_GETIVEC) {
	String name = argv->getstring (0);
	return getivec (name);
      }
    }
    // check for 2 arguments
    if (argc == 2) {
      if (quark == QUARK_GETINFO) {
	long index = argv->getlong (0);
	bool lwcf  = argv->getbool (1);
	return getinfo (index, lwcf);
      }
      if (quark == QUARK_GETIVEC) {
	String name = argv->getstring (0);
	bool   lwcf = argv->getbool   (1);
	return getivec (name, lwcf);
      }
    }
    // call the nameable method
    return Nameable::apply (zobj, nset, quark, argv);
  }
}
