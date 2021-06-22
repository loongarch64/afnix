// ---------------------------------------------------------------------------
// - XmlNode.cpp                                                             -
// - afnix:xml module - xml base node class implementation                   -
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

#include "XmlNode.hpp"
#include "Integer.hpp"
#include "Boolean.hpp"
#include "Loopable.hpp"
#include "XmlSystem.hpp"
#include "XmlReader.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - private section                                                       -
  // -------------------------------------------------------------------------
  
  // the default empty flag
  static const bool XML_EFLG_DEF = false;
  // the default eol flag
  static const bool XML_EOLF_DEF = true;

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create an empty node

  XmlNode::XmlNode (void) {
    d_eflg = XML_EFLG_DEF;
    d_eolf = XML_EOLF_DEF;
    p_pnod = nullptr;
  }

  // create a node with an empty flag

  XmlNode::XmlNode (const bool eflg) {
    d_eflg = eflg;
    d_eolf = XML_EOLF_DEF;
    p_pnod = nullptr;
  }

  // destroy this node

  XmlNode::~XmlNode (void) {
    Object::dref (p_pnod);
  }

  // return the node class name

  String XmlNode::repr (void) const {
    return "XmlNode";
  }

  // release all xml node links

  void XmlNode::release (void) {
    wrlock ();
    try {
      // protect us
      Object::iref (this);
      // disconnect ourself
      setparent (nullptr);
      // get the child length
      long len = lenchild ();
      for (long i = 0; i < len; i++) {
	// get the child node
	XmlNode* node = getchild (i);
	if (node == nullptr) continue;
	// check if we disconnect the node
	if (node->getparent () == nullptr) continue;
	// call the node release
	node->release ();
      }
      Object::tref (this);
      unlock ();
    } catch (...) {
      Object::tref (this);
      unlock ();
      throw;
    }
  }

  // get the empty flag

  bool XmlNode::geteflg (void) const {
    rdlock ();
    try {
      bool result = d_eflg;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set the source line number
  
  void XmlNode::setlnum (const long lnum) {
    wrlock ();
    try {
      d_lnum = lnum;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the source line number

  long XmlNode::getlnum (void) const {
    rdlock ();
    try {
      long result= d_lnum;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set the node source name
  
  void XmlNode::setsnam (const String& snam) {
    wrlock ();
    try {
      d_snam = snam;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the node source name

  String XmlNode::getsnam (void) const {
    rdlock ();
    try {
      String result= d_snam;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // check if a node name is valid

  bool XmlNode::isname (const String& name) const {
    rdlock ();
    try {
      bool result = false;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // check if a node index is valid

  bool XmlNode::isnidx (const long nidx) const {
    rdlock ();
    try {
      // assume not valid
      bool result = false;
      // check with parent node
      if (p_pnod != nullptr) {
	long index = p_pnod->getnidx (this);
	result = (index == nidx);
      }
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // check if a node attribute exists

  bool XmlNode::isattr (const String& name) const {
    rdlock ();
    try {
      bool result = false;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // check if a node attribute is valid

  bool XmlNode::isattr (const String& name, const String& pval) const {
    rdlock ();
    try {
      bool result = false;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the parent node

  XmlNode* XmlNode::getparent (void) const {
    rdlock ();
    try {
      XmlNode* result = p_pnod;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set the parent node

  void XmlNode::setparent (XmlNode* node) {
    wrlock ();
    try {
      Object::iref (node);
      Object::dref (p_pnod);
      p_pnod = node;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }


  // get a copy of the node tree

  XmlNode* XmlNode::copy (void) const {
    rdlock ();
    XmlNode* result = nullptr;
    try {
      // get a clone of this node
      result = dynamic_cast <XmlNode*> (clone ());
      if (result == nullptr) {
	throw Exception ("clone-error", "unable to clone node");
      }
      // loop in the child nodes and copy
      long len = lenchild ();
      for (long i = 0; i < len; i++) {
	XmlNode* node = getchild (i);
	if (node == nullptr) continue;
	result->addchild (node->copy ());
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

  // return true if there is no children

  bool XmlNode::nilchild (void) const {
    rdlock ();
    try {
      bool result = d_chld.empty ();
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return true if a child node exists by name

  bool XmlNode::ischild (const String& name) const {
    rdlock ();
    try {
      long clen = lenchild ();
      for (long i = 0; i < clen; i++) {
	XmlNode* node = getchild (i);
	if (node == nullptr) continue;
	if (node->isname (name) == true) {
	  unlock ();
	  return true;
	}
      }
      unlock ();
      return false;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return true if a child node exists by name with an attribute

  bool XmlNode::ischild (const String& name, const String& anam) const {
    rdlock ();
    try {
      long clen = lenchild ();
      for (long i = 0; i < clen; i++) {
	XmlNode* node = getchild (i);
	if ((node != nullptr) && (node->isname (name) == true)) {
	  if (node->isattr (anam) == true) {
	    unlock ();
	    return true;
	  }
	}
      }
      unlock ();
      return false;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return true if a child node exists by name with an attribute name
  // and attribute value

  bool XmlNode::ischild (const String& name, 
			 const String& anam, const String& pval) const {
    rdlock ();
    try {
      long clen = lenchild ();
      for (long i = 0; i < clen; i++) {
	XmlNode* node = getchild (i);
	if ((node != nullptr) && (node->isname (name) == true)) { 
	  if (node->isattr (anam, pval) == true) {
	    unlock ();
	    return true;
	  }
	}
      }
      unlock ();
      return false;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return true if a child node exists by attribute name

  bool XmlNode::isachild (const String& anam) const {
    rdlock ();
    try {
      long clen = lenchild ();
      for (long i = 0; i < clen; i++) {
	XmlNode* node = getchild (i);
	if ((node != nullptr) && (node->isattr (anam) == true)) {
	  unlock ();
	  return true;
	}
      }
      unlock ();
      return false;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return true if a child node exists by attribute name and value

  bool XmlNode::isachild (const String& anam, const String& pval) const {
    rdlock ();
    try {
      long clen = lenchild ();
      for (long i = 0; i < clen; i++) {
	XmlNode* node = getchild (i);
	if ((node != nullptr) && (node->isattr (anam, pval) == true)) {
	  unlock ();
	  return true;
	}
      }
      unlock ();
      return false;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return the number of children nodes

  long XmlNode::lenchild (void) const {
    rdlock ();
    try {
      long result = d_chld.length ();
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return the number of children nodes by name

  long XmlNode::lenchild (const String& name) const {
    rdlock ();
    try {
      long result = 0;
      long   clen = lenchild ();
      for (long i = 0; i < clen; i++) {
	XmlNode* node = getchild (i);
	if ((node != nullptr) && (node->isname (name) == true)) result++;
      }
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // add a child node to this node

  void XmlNode::addchild (XmlNode* node) {
    // check for nil first
    if (node == nullptr) return;
    // lock, mark parent and add
    wrlock ();
    try {
      if (d_eflg == true) {
	throw Exception ("xml-error", "trying to add node in empty node");
      }
      // set the parent node
      node->setparent (this);
      // add the node
      d_chld.add (node);
      unlock ();
    } catch (...) {
      unlock();
      throw;
    }
  }

  // add a child node to this node at a certain position

  void XmlNode::addchild (XmlNode* node, const long nidx) {
    // check for nil first
    if (node == nullptr) return;
    // lock, mark parent and add
    wrlock ();
    try {
      if (d_eflg == true) {
	throw Exception ("xml-error", "trying to add node in empty node");
      }
      // set the parent node
      node->setparent (this);
      // add the node
      d_chld.add (nidx, node);
      unlock ();
    } catch (...) {
      unlock();
      throw;
    }
  }

  // set a child node to this node at a certain position

  void XmlNode::setchild (XmlNode* node, const long nidx) {
    // check for nil first
    if (node == nullptr) return;
    // lock, mark parent and add
    wrlock ();
    try {
      if (d_eflg == true) {
	throw Exception ("xml-error", "trying to add node in empty node");
      }
      // set the parent node
      node->setparent (this);
      // add the node
      d_chld.set (nidx, node);
      unlock ();
    } catch (...) {
      unlock();
      throw;
    }
  }

  // get a child node by index

  XmlNode* XmlNode::getchild (const long index) const {
    rdlock ();
    try {
      XmlNode* node = dynamic_cast <XmlNode*> (d_chld.get (index));
      unlock ();
      return node;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get a child node by name if possible

  XmlNode* XmlNode::getchild (const String& name) const {
    rdlock ();
    try {
      long clen = lenchild ();
      for (long i = 0; i < clen; i++) {
	XmlNode* node = getchild (i);
	if (node == nullptr) continue;
	if (node->isname (name) == true) {
	  unlock ();
	  return node;
	}
      }
      unlock ();
      return nullptr;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // find a node by name or throw an exception

  XmlNode* XmlNode::lookchild (const String& name) const {
    rdlock ();
    try {
      XmlNode* result = getchild (name);
      if (result == nullptr) {
	throw Exception ("lookup-error", "cannot find child node", name);
      }
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // remove a child node by index

  void XmlNode::delchild (const long nidx) {
    wrlock ();
    try {
      // protect us
      Object::iref (this);
      d_chld.remove (nidx);
      // update self reference
      Object::tref (this);
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // remove the child node by name

  void XmlNode::delchild (const String& name) {
    wrlock ();
    try {
      for (long i = 0; i < lenchild (); i++) {
	XmlNode* node = getchild (i);
	if ((node != nullptr) && (node->isname (name) == true)) {
	  // remove the child
	  delchild (i);
	  // restart from start
	  i = 0;
	  continue;
	}
      }
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // remove the child node by name and attribute name

  void XmlNode::delchild (const String& name, const String& anam) {
    wrlock ();
    try {
      for (long i = 0; i < lenchild (); i++) {
	XmlNode* node = getchild (i);
	if ((node != nullptr) && (node->isname (name) == true)) {
	  if (node->isattr (name) == true) {
	    // remove the child
	    delchild (i);
	    // restart from start
	    i = 0;
	    continue;
	  }
	}
      }
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // remove the child node by name, attribute name and value

  void XmlNode::delchild (const String& name, const String& anam,
			  const String& pval) {
    wrlock ();
    try {
      for (long i = 0; i < lenchild (); i++) {
	XmlNode* node = getchild (i);
	if ((node != nullptr) && (node->isname (name) == true)) {
	  if (node->isattr (name, pval) == true) {
	    // remove the child
	    delchild (i);
	    // restart from start
	    i = 0;
	    continue;
	  }
	}
      }
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // remove the child node by attribute name

  void XmlNode::delachild (const String& anam) {
    wrlock ();
    try {
      for (long i = 0; i < lenchild (); i++) {
	XmlNode* node = getchild (i);
	if ((node != nullptr) && (node->isattr (anam) == true)) {
	  // remove the child
	  delchild (i);
	  // restart from start
	  i = 0;
	  continue;
	}
      }
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // remove the child node by attribute name

  void XmlNode::delachild (const String& anam, const String& pval) {
    wrlock ();
    try {
      for (long i = 0; i < lenchild (); i++) {
	XmlNode* node = getchild (i);
	if ((node != nullptr) && (node->isattr (anam, pval) == true)) {
	  // remove the child
	  delchild (i);
	  // restart from start
	  i = 0;
	  continue;
	}
      }
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // clear the child node list

  void XmlNode::clrchild (void) {
    wrlock ();
    // protect us
    Object::iref (this);
    try {
      // clear child list
      d_chld.reset ();
      // update self
      Object::tref (this);
      unlock ();
    } catch (...) {
      Object::tref (this);
      unlock ();
      throw;
    }
  }

  // find a child node index by node

  long XmlNode::getnidx (const XmlNode* node) const {
    rdlock ();
    try {
      long clen = lenchild ();
      for (long i = 0; i < clen; i++) {
	if (getchild (i) == node) {
	  unlock ();
	  return i;
	}
      }
      throw Exception ("node-error", "cannot find node index");
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // merge a child node at a certain position

  void XmlNode::merge (const XmlNode* snod, const long nidx) {
    // do nothing if nil
    if (snod == nullptr) return;
    // lock and merge
    wrlock ();
    try {
      // remove the child node at index
      delchild (nidx);
      // set the insert index
      long index = nidx;
      // get the source node length
      long slen = snod->lenchild ();
      for (long i = 0; i < slen; i++) {
	XmlNode* node = snod->getchild (i);
	if (node == nullptr) continue;
	addchild (node->copy (), index++);
      }
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // parse a string and add the result as child nodes

  void XmlNode::parse (const String& s) {
    wrlock ();
    try {
      // create a default reader
      XmlReader xmlr;
      // parse the string
      xmlr.parse (s);
      // get the result root node
      XmlRoot* root = xmlr.getroot ();
      // get the child length and copy the nodes
      long nlen = (root == nullptr) ? 0 : root->lenchild ();
      for (long i = 0; i < nlen; i++) addchild (root->getchild (i));
      // done
      unlock ();
    } catch (...) {
      unlock ();
      throw Exception ("xml-error", "cannot parse string", s);
    }
  }

  // return a text content of the node tree

  String XmlNode::totext (void) const {
    rdlock ();
    try {
      String result;
      long len = lenchild ();
      for (long i = 0; i < len; i++) {
	XmlNode* node = getchild (i);
	if (node == nullptr) continue;
	result += node->totext ();
      }
      unlock ();
      return result;	
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // normalize this node and its children

  void XmlNode::normalize (void) {
    wrlock ();
    try {
      long len = lenchild ();
      for (long i = 0; i < len; i++) {
	XmlNode* node = getchild (i);
	if (node != nullptr) node->normalize ();
      }
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // normalize this node and its children

  bool XmlNode::replace (XmlNode* rnod, XmlNode* snod) {
    // check for a reference node
    if ((rnod == nullptr) || (rnod == this)) return false;
    wrlock ();
    try {
      // preset result and loop length
      bool result = false;
      long    len = lenchild ();
      // loop with children
      for (long i = 0; i < len; i++) {
	XmlNode* node = getchild (i);
	if (node == rnod) {
	  if (snod == nullptr) {
	    delchild (i);
	  } else {
	    setchild (snod, i);
	  }
	  result = true;
	}
      }
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
  static const long QUARK_ZONE_LENGTH = 23;
  static QuarkZone  zone (QUARK_ZONE_LENGTH);

  // the object supported quarks
  static const long QUARK_COPY      = zone.intern ("copy");
  static const long QUARK_MERGE     = zone.intern ("merge");
  static const long QUARK_WRITE     = zone.intern ("write");
  static const long QUARK_NAMEP     = zone.intern ("name-p");
  static const long QUARK_ATTRP     = zone.intern ("attribute-p");
  static const long QUARK_PARSE     = zone.intern ("parse");
  static const long QUARK_TOTEXT    = zone.intern ("to-text");
  static const long QUARK_CHILDP    = zone.intern ("child-p");
  static const long QUARK_ACHILDP   = zone.intern ("attribute-child-p");
  static const long QUARK_REPLACE   = zone.intern ("replace");
  static const long QUARK_GETLNUM   = zone.intern ("get-source-line");
  static const long QUARK_SETLNUM   = zone.intern ("set-source-line");
  static const long QUARK_GETSNAM   = zone.intern ("get-source-name");
  static const long QUARK_SETSNAM   = zone.intern ("set-source-name");
  static const long QUARK_ADDCHILD  = zone.intern ("add-child");
  static const long QUARK_GETCHILD  = zone.intern ("get-child");
  static const long QUARK_DELCHILD  = zone.intern ("del-child");
  static const long QUARK_GETINDEX  = zone.intern ("get-index");
  static const long QUARK_CHILDLEN  = zone.intern ("child-length");
  static const long QUARK_CLRCHILD  = zone.intern ("clear-child");
  static const long QUARK_DELACHILD = zone.intern ("del-attribute-child");
  static const long QUARK_GETPARENT = zone.intern ("get-parent");
  static const long QUARK_SETPARENT = zone.intern ("set-parent");
  static const long QUARK_NILCHILDP = zone.intern ("nil-child-p");
  static const long QUARK_LOOKCHILD = zone.intern ("lookup-child");
  static const long QUARK_NORMALIZE = zone.intern ("normalize");

  // return true if the given quark is defined

  bool XmlNode::isquark (const long quark, const bool hflg) const {
    rdlock ();
    if (zone.exists (quark) == true) {
      unlock ();
      return true;
    }
    bool result = hflg ? Collectable::isquark (quark, hflg) : false;
    unlock ();
    return result;
  }

  // apply this object with a set of arguments and a quark

  Object* XmlNode::apply (Evaluable* zobj, Nameset* nset, const long quark,
			  Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();

    // check for 0 argument
    if (argc == 0) {
      if (quark == QUARK_TOTEXT)    return new String  (totext   ());
      if (quark == QUARK_CHILDLEN)  return new Integer (lenchild ());
      if (quark == QUARK_GETLNUM)   return new Integer (getlnum  ());
      if (quark == QUARK_GETSNAM)   return new String  (getsnam  ());
      if (quark == QUARK_NILCHILDP) return new Boolean (nilchild ());
      if (quark == QUARK_GETPARENT) {
	rdlock ();
	Object* result = getparent ();
	zobj->post (result);
	unlock ();
	return result;
      }
      if (quark == QUARK_WRITE) {
	auto lobj = dynamic_cast<Loopable*>(zobj);
	OutputStream* os = (lobj == nullptr) ? nullptr : lobj->getos ();
	if (os != nullptr) write (*os);
	return nullptr;
      }
      if (quark == QUARK_COPY) {
	Object* result = copy ();
	zobj->post (result);
	return result;
      }
      if (quark == QUARK_CLRCHILD) {
	clrchild ();
	return nullptr;
      }
      if (quark == QUARK_NORMALIZE) {
        normalize ();
	return nullptr;
      }
    }
    // check for 1 argument
    if (argc == 1) {
      if (quark == QUARK_NAMEP) {
	String name= argv->getstring (0);
	return new Boolean (isname (name));
      }
      if (quark == QUARK_ATTRP) {
	String name= argv->getstring (0);
	return new Boolean (isattr (name));
      }
      if (quark == QUARK_CHILDP) {
	String name= argv->getstring (0);
	return new Boolean (ischild (name));
      }
      if (quark == QUARK_CHILDLEN) {
	String name= argv->getstring (0);
	return new Integer (lenchild (name));
      }
      if (quark == QUARK_ACHILDP) {
	String anam= argv->getstring (0);
	return new Boolean (isachild (anam));
      }
      if (quark == QUARK_PARSE) {
	String s = argv->getstring (0);
	parse (s);
	return nullptr;
      }
      if (quark == QUARK_SETPARENT) {
	// get the node object
	Object*   obj = argv->get (0);
	XmlNode* node = dynamic_cast <XmlNode*> (obj);
	if ((obj != nullptr) && (node == nullptr)) {
	  throw Exception ("type-error", "invalid object with set-parent",
			   Object::repr (obj));
	}
	// set the node
	setparent (node);
	return nullptr;
      }
      if (quark == QUARK_ADDCHILD) {
	// get the node object
	Object*   obj = argv->get (0);
	XmlNode* node = dynamic_cast <XmlNode*> (obj);
	if ((obj != nullptr) && (node == nullptr)) {
	  throw Exception ("type-error", "invalid object with add-child",
			   Object::repr (obj));
	}
	// set the node
	addchild (node);
	return nullptr;
      }
      if (quark == QUARK_WRITE) {
	Object* obj = argv->get (0);
	// check for an output stream
	OutputStream* os = dynamic_cast <OutputStream*> (obj);
	if (os != nullptr) {
	  write (*os);
	  return nullptr;
	}
	// check for a buffer
	Buffer* buf = dynamic_cast <Buffer*> (obj);
	if (buf != nullptr) {
	  write (*buf);
	  return nullptr;
	}
	throw Exception ("type-error", "invalid object with write",
			 Object::repr (obj));
      }
      if (quark == QUARK_GETCHILD) {
	Object* obj = argv->get (0);
	// check for an integer
	Integer* iobj = dynamic_cast <Integer*> (obj);
	if (iobj != nullptr) {
	  long index = iobj->tolong ();
	  rdlock ();
	  try {
	    XmlNode* result = getchild (index);
	    zobj->post (result);
	    unlock ();
	    return result;
	  } catch (...) {
	    unlock ();
	    throw;
	  }
	}
	// check for a string
	String* sobj = dynamic_cast <String*> (obj);
	if (sobj != nullptr) {
	  rdlock ();
	  try {
	    XmlNode* result = getchild (*sobj);
	    zobj->post (result);
	    unlock ();
	    return result;
	  } catch (...) {
	    unlock ();
	    throw;
	  }
	}
	throw Exception ("type-error", "invalid object with get-child",
			 Object::repr (obj));
      }
      if (quark == QUARK_DELACHILD) {
	String anam = argv->getstring (0);
	delachild (anam);
	return nullptr;
      }
      if (quark == QUARK_LOOKCHILD) {
	String name = argv->getstring (0);
	rdlock ();
	try {
	  XmlNode* result = lookchild (name);
	  zobj->post (result);
	  unlock ();
	  return result;
	} catch (...) {
	  unlock ();
	  throw;
	}
      }
      if (quark == QUARK_GETINDEX) {
	// get the node object
	Object*   obj = argv->get (0);
	XmlNode* node = dynamic_cast <XmlNode*> (obj);
	if ((obj != nullptr) && (node == nullptr)) {
	  throw Exception ("type-error", "invalid object with get-index",
			   Object::repr (obj));
	}
	// get the node index
	return new Integer (getnidx (node));
      }
      if (quark == QUARK_DELCHILD) {
	Object* obj = argv->get (0);
	// check for an integer
	Integer* iobj = dynamic_cast <Integer*> (obj);
	if (iobj != nullptr) {
	  long nidx = iobj->tolong ();
	  delchild (nidx);
	  return nullptr;
	}
	// check for a string
	String* sobj = dynamic_cast <String*> (obj);
	if (sobj != nullptr) {
	  delchild (*sobj);
	  return nullptr;
	}
	// invalid object
	throw Exception ("type-error", "invalid object with del-child",
			 Object::repr (obj));
      }
      if (quark == QUARK_SETLNUM) {
	long lnum = argv->getlong (0);
	setlnum (lnum);
	return nullptr;
      }
      if (quark == QUARK_SETSNAM) {
	String snam = argv->getstring (0);
	setsnam (snam);
	return nullptr;
      }
    }
    // check for 2 arguments
    if (argc == 2) {
      if (quark == QUARK_CHILDP) {
	String name = argv->getstring (0);
	String anam = argv->getstring (1);
	return new Boolean (ischild (name, anam));
      }
      if (quark == QUARK_ACHILDP) {
	String anam = argv->getstring (0);
	String pval = argv->getstring (1);
	return new Boolean (isachild (anam, pval));
      }
      if (quark == QUARK_ATTRP) {
	String name = argv->getstring (0);
	String pval = argv->getstring (1);
	return new Boolean (isattr (name, pval));
      }
      if (quark == QUARK_DELCHILD) {
	String name = argv->getstring (0);
	String anam = argv->getstring (1);
	delchild (name, anam);
	return nullptr;
      }
      if (quark == QUARK_DELACHILD) {
	String anam = argv->getstring (0);
	String pval = argv->getstring (1);
	delachild (anam, pval);
	return nullptr;
      }
      if (quark == QUARK_ADDCHILD) {
	// get the node object
	Object*   obj = argv->get (0);
	XmlNode* node = dynamic_cast <XmlNode*> (obj);
	if ((obj != nullptr) && (node == nullptr)) {
	  throw Exception ("type-error", "invalid object with add-child",
			   Object::repr (obj));
	}
	// get the index and add
	long nidx = argv->getlong (1);
	addchild (node, nidx);
	return nullptr;
      }
      if (quark == QUARK_MERGE) {
	// get the node object
	Object*   obj = argv->get (0);
	XmlNode* node = dynamic_cast <XmlNode*> (obj);
	if ((obj != nullptr) && (node == nullptr)) {
	  throw Exception ("type-error", "invalid object with merge",
			   Object::repr (obj));
	}
	// get the index and add
	long nidx = argv->getlong (1);
	merge (node, nidx);
	return nullptr;
      }
      if (quark == QUARK_REPLACE) {
	// get the reference node object
	Object*   obj = argv->get (0);
	XmlNode* rnod = dynamic_cast <XmlNode*> (obj);
	if ((obj != nullptr) && (rnod == nullptr)) {
	  throw Exception ("type-error", "invalid object with replace",
			   Object::repr (obj));
	}
	// get the source node object
	obj = argv->get (1);
	XmlNode* snod = dynamic_cast <XmlNode*> (obj);
	if ((obj != nullptr) && (snod == nullptr)) {
	  throw Exception ("type-error", "invalid object with replace",
			   Object::repr (obj));
	}
	bool result = replace (rnod, snod);
	return new Boolean (result);
      }
    }
    // check for 3 arguments
    if (argc == 3) {
      if (quark == QUARK_CHILDP) {
	String name = argv->getstring (0);
	String anam = argv->getstring (1);
	String pval = argv->getstring (2);
	return new Boolean (ischild (name, anam, pval));
      }
      if (quark == QUARK_DELCHILD) {
	String name = argv->getstring (0);
	String anam = argv->getstring (1);
	String pval = argv->getstring (2);
	delchild (name, anam, pval);
	return nullptr;
      }
    }
    // call the collectable method
    return Collectable::apply (zobj, nset, quark, argv);
  }
}
