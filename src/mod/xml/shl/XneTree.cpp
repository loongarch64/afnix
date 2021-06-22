// ---------------------------------------------------------------------------
// - XneTree.hpp                                                             -
// - afnix:xml module - xne tree class implementation                        -
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

#include "XmlTag.hpp"
#include "Integer.hpp"
#include "XneTree.hpp"
#include "Evaluable.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - private section                                                       -
  // -------------------------------------------------------------------------

  // the xml id attribute name
  static const String XML_ID_ATTR = "id";
  // the xml id name prefix
  static const String XML_ID_PFIX = "ID";

  // this procedure compute the maximum of two numbers
  static inline long max (const long x, const long y) {
    return x > y ? x : y;
  }

  // this procedure return a id literal by value
  String get_id_pval (const long id) {
    String result = XML_ID_PFIX;
    result += id;
    return result;
  }
    
  // this procedure computes the depth of a node
  static long tree_depth (const XmlNode* node) {
    // get the number of childs
    long clen = (node == nullptr) ? 0 : node->lenchild ();
    if (clen == 0) return 0;
    // find the max in the tree
    long result = 0;
    for (long i = 0; i < clen; i++) {
      XmlNode* cnod = node->getchild (i);
      if (cnod == nullptr) continue;
      result = max (result, tree_depth (cnod));
    }
    return result + 1;
  }

  // this procedure computes the tree size from a node
  static long tree_size (const XmlNode* node) {
    // process nil node
    if (node == nullptr) return 0;
    // get the number of childs
    long clen = node->lenchild ();
    // start with this node
    long result = 1;
    // loop in the children
    for (long i = 0; i < clen; i++) {
      XmlNode* cnod = node->getchild (i);
      if (cnod == nullptr) continue;
      result += tree_size (cnod);
    }
    return result;
  }

  // this procedure set a unique id to the tag node
  static long tree_gen_id (XmlNode* node, const long nid) {
    // save the node id
    long tid = nid;
    // check for a tag
    XmlTag* tag = dynamic_cast <XmlTag*> (node);
    if (tag != nullptr) tag->setattr (XML_ID_ATTR, get_id_pval (tid++));
    // look for the child
    long len = (node == nullptr) ? 0 : node->lenchild ();
    for (long i = 0; i < len; i++) {
      tag = dynamic_cast <XmlTag*> (node->getchild (i));
      if (tag == nullptr) continue;
      tid = tree_gen_id (tag, tid);
    }
    // here is the updated tid
    return tid;
  }

  // this procedure set an attribute to each node of the tree
  static void tree_set_attr (XmlNode* node, const String& name,
			     const Literal& lval) {
    // check for a tag
    XmlTag* tag = dynamic_cast <XmlTag*> (node);
    if (tag != nullptr) tag->setattr (name, lval);
    // look for the children
    long len = (node == nullptr) ? 0 : node->lenchild ();
    for (long i = 0; i < len; i++) {
      XmlNode* cnod = node->getchild (i);
      if (cnod == nullptr) continue;
      tree_set_attr (cnod, name, lval);
    }
  }

  // this procedure set an attribute to each node of the tree by tag name
  static void tree_set_attr (XmlNode* node, const String& name,
			     const Literal& lval, const String& tnam) {
    // check for a tag
    XmlTag* tag = dynamic_cast <XmlTag*> (node);
    if ((tag != nullptr) && (tag->isname (tnam) == true)) {
      tag->setattr (name, lval);
    }
    // look for the children
    long len = (node == nullptr) ? 0 : node->lenchild ();
    for (long i = 0; i < len; i++) {
      XmlNode* cnod = node->getchild (i);
      if (cnod == nullptr) continue;
      tree_set_attr (cnod, name, lval, tnam);
    }
  }

  // this procedure clear the attributes of each node tree
  static void tree_clr_attr (XmlNode* node) {
    // check for a tag
    XmlTag* tag = dynamic_cast <XmlTag*> (node);
    if (tag != nullptr) tag->clrattr ();
    // look for the children
    long len = (node == nullptr) ? 0 : node->lenchild ();
    for (long i = 0; i < len; i++) {
      XmlNode* cnod = node->getchild (i);
      if (cnod == nullptr) continue;
      tree_clr_attr (cnod);
    }
  }

  // this procedure clear the attributes of each node tree by name
  static void tree_clr_attr (XmlNode* node, const String& name) {
    // check for a tag
    XmlTag* tag = dynamic_cast <XmlTag*> (node);
    if ((tag != nullptr) && (tag->isname (name) == true)) tag->clrattr ();
    // look for the children
    long len = (node == nullptr) ? 0 : node->lenchild ();
    for (long i = 0; i < len; i++) {
      XmlNode* cnod = node->getchild (i);
      if (cnod == nullptr) continue;
      tree_clr_attr (cnod);
    }
  }

  // this procedure set the prefix of a tag name
  static void node_set_pfix (XmlNode* node, const String& pnam) {
    // check for a tag
    XmlTag* tag = dynamic_cast <XmlTag*> (node);
    if (tag == nullptr) return;
    // get the tag name
    String name = tag->getname ();
    // get the local name and verify
    String lnam = Xne::getlnam (name);
    // expand to a new name
    tag->setname (Xne::toexnm (pnam, lnam));
  }

  // this procedure sets the prefix of each node name
  static void tree_set_pfix (XmlNode* node, const String& pnam) {
    // set the node first
    node_set_pfix (node, pnam);
    // look for the children
    long len = (node == nullptr) ? 0 : node->lenchild ();
    for (long i = 0; i < len; i++) {
      XmlNode* cnod = node->getchild (i);
      if (cnod == nullptr) continue;
      tree_set_pfix (cnod, pnam);
    }
  }

  // this procedure clear the prefix of a tag name
  static void node_clr_pfix (XmlNode* node) {
    // check for a tag
    XmlTag* tag = dynamic_cast <XmlTag*> (node);
    if (tag == nullptr) return;
    // get the tag name
    String name = tag->getname ();
    // get the local name and verify
    String lnam = Xne::getlnam (name);
    if (Xne::isncnm (lnam) == true) tag->setname (lnam);
  }

  // this procedure clear the prefix of each node name
  static void tree_clr_pfix (XmlNode* node) {
    // fix the node first
    node_clr_pfix (node);
    // look for the children
    long len = (node == nullptr) ? 0 : node->lenchild ();
    for (long i = 0; i < len; i++) {
      XmlNode* cnod = node->getchild (i);
      if (cnod == nullptr) continue;
      tree_clr_pfix (cnod);
    }
  }
  
  // find the child nodes that matches a condition
  static void node_cond_xsel (Vector* result, XmlNode* node,
			      const XneCond& cond) {
    // do nothing without result
    if (result == nullptr) return;
    // look for the children
    long len = (node == nullptr) ? 0 : node->lenchild ();
    for (long i = 0; i < len; i++) {
      XmlNode* cnod = node->getchild (i);
      if (cond.valid (cnod) == true) result->add (cnod);
    }
  }

  // find the nodes that matches a condition
  static void tree_cond_xsel (Vector* result, XmlNode* node,
			      const XneCond& cond) {
    // do nothing without result
    if (result == nullptr) return;
    // check the node condition
    if (cond.valid (node) == true) result->add (node);
    // look for the children
    long len = (node == nullptr) ? 0 : node->lenchild ();
    for (long i = 0; i < len; i++) {
      XmlNode* cnod = node->getchild (i);
      if (cnod == nullptr) continue;
      tree_cond_xsel (result, cnod, cond);
    }
  }

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a default xne tree

  XneTree::XneTree (void) {
    p_node = nullptr;
  }

  // create a xne tree by node

  XneTree::XneTree (XmlNode* node) {
    p_node = nullptr;
    setnode (node);
  }

  // destroy this xne tree

  XneTree::~XneTree (void) {
    Object::dref (p_node);
  }

  // return the class name

  String XneTree::repr (void) const {
    return "XneTree";
  }

  // attach a node to the xne tree

  void XneTree::setnode (XmlNode* node) {
    wrlock ();
    try {
      // set the node
      Object::iref (node);
      Object::dref (p_node);
      p_node = node;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return the node attached to the tree

  XmlNode* XneTree::getnode (void) const {
    rdlock ();
    try {
      XmlNode* result = p_node;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the tree depth

  long XneTree::depth (void) const {
    rdlock ();
    try {
      long result = tree_depth (p_node);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the tree size

  long XneTree::size (void) const {
    rdlock ();
    try {
      long result = tree_size (p_node);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // generate a unique id for all nodes in the tree


  void XneTree::genid (void) {
    wrlock ();
    try {
      tree_gen_id (p_node, 0);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set an attribute to each node of the tree

  void XneTree::setattr (const String& name, const Literal& lval) {
    wrlock ();
    try {
      tree_set_attr (p_node, name, lval);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set an attribute to each node of the tree by tag name

  void XneTree::setattr (const String& name, const Literal& lval, 
			 const String& tnam) {
    wrlock ();
    try {
      tree_set_attr (p_node, name, lval, tnam);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // clear the attributes of each node of the tree

  void XneTree::clrattr (void) {
    wrlock ();
    try {
      tree_clr_attr (p_node);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // clear the attributes of each node of the tree by name

  void XneTree::clrattr (const String& name) {
    wrlock ();
    try {
      tree_clr_attr (p_node, name);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set the tag name prefix

  void XneTree::setpfix (const String& pnam) {
    wrlock ();
    try {
      tree_set_pfix (p_node, pnam);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // clear the tag name prefix

  void XneTree::clrpfix (void) {
    wrlock ();
    try {
      tree_clr_pfix (p_node);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // normalize this tree

  void XneTree::normalize (void) {
    wrlock ();
    try {
      if (p_node != nullptr) p_node->normalize ();
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // select the nodes that matches a condition

  Vector* XneTree::select (const XneCond& cond, const bool hflg) const {
    rdlock ();
    // create a result set
    Vector* result = new Vector;
    try {
      if (hflg == true) {
	tree_cond_xsel (result, p_node, cond);
      } else {
	node_cond_xsel (result, p_node, cond);
      }
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
  static const long QUARK_ZONE_LENGTH = 11;
  static QuarkZone  zone (QUARK_ZONE_LENGTH);

  // the object supported quarks
  static const long QUARK_SIZE      = zone.intern ("size");
  static const long QUARK_DEPTH     = zone.intern ("depth");
  static const long QUARK_GENID     = zone.intern ("generate-id");
  static const long QUARK_SELECT    = zone.intern ("select");
  static const long QUARK_SETNODE   = zone.intern ("set-node");
  static const long QUARK_GETNODE   = zone.intern ("get-node");
  static const long QUARK_SETATTR   = zone.intern ("set-attribute");
  static const long QUARK_CLRATTR   = zone.intern ("clear-attribute");
  static const long QUARK_SETPFIX   = zone.intern ("set-prefix");
  static const long QUARK_CLRPFIX   = zone.intern ("clear-prefix");
  static const long QUARK_NORMALIZE = zone.intern ("normalize");

  // create a new object in a generic way

  Object* XneTree::mknew (Vector* argv) {
    long argc = (argv == nullptr) ? 0 : argv->length ();
    // check for 0 argument
    if (argc == 0) return new XneTree;
    // check for 1 argument
    if (argc == 1) {
      Object* obj = argv->get (0);
      // check for node
      XmlNode* node = dynamic_cast <XmlNode*> (obj);
      if ((node == nullptr) && (obj != nullptr)) {
	throw Exception ("type-error", 
			 "invalid object for xne tree constructor",
			 Object::repr (obj));
      }
      return new XneTree (node);
    }
    throw Exception ("argument-error",
                     "too many argument with xne tree constructor");
  }

  // return true if the given quark is defined

  bool XneTree::isquark (const long quark, const bool hflg) const {
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
  
  Object* XneTree::apply (Evaluable* zobj, Nameset* nset, const long quark,
			  Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();

    // check for 0 argument
    if (argc == 0) {
      if (quark == QUARK_SIZE)  return new Integer (size  ());
      if (quark == QUARK_DEPTH) return new Integer (depth ());
      if (quark == QUARK_GENID) {
	genid ();
	return nullptr;
      }
      if (quark == QUARK_GETNODE) {
	rdlock ();
	try {
	  Object* result = getnode ();
	  zobj->post (result);
	  unlock ();
	  return result;
	} catch (...) {
	  unlock ();
	  throw;
	}
      }
      if (quark == QUARK_CLRATTR) {
	clrattr ();
	return nullptr;
      }
      if (quark == QUARK_CLRPFIX) {
	clrpfix ();
	return nullptr;
      }
      if (quark == QUARK_NORMALIZE) {
	normalize ();
	return nullptr;
      }
    }

    // check for 1 argument
    if (argc == 1) {
      if (quark == QUARK_SETNODE) {
	Object* obj = argv->get (0);
	// check for node
	XmlNode* node = dynamic_cast <XmlNode*> (obj);
	if ((node == nullptr) && (obj != nullptr)) {
	  throw Exception ("type-error", "invalid object for xne tree node",
			   Object::repr (obj));
	}
	setnode (node);
	return nullptr;
      }
      if (quark == QUARK_CLRATTR) {
	String name = argv->getstring (0);
	clrattr (name);
	return nullptr;
      }
      if (quark == QUARK_SETPFIX) {
	String pnam = argv->getstring (0);
	setpfix (pnam);
	return nullptr;
      }
      if (quark == QUARK_SELECT) {
        Object*   obj = argv->get (0);
        XneCond* cobj = dynamic_cast <XneCond*> (obj);
        if (cobj == nullptr) {
          throw Exception ("type-error", "invalid object with select",
                           Object::repr (obj));
        }
        return select (*cobj, true);
      }
    }

    // check for 2 arguments
    if (argc == 2) {
      if (quark == QUARK_SELECT) {
        Object*   obj = argv->get (0);
        XneCond* cobj = dynamic_cast <XneCond*> (obj);
        if (cobj == nullptr) {
          throw Exception ("type-error", "invalid object with select",
                           Object::repr (obj));
        }
	bool hflg = argv->getbool (1); 
        return select (*cobj, hflg);
      }
      if (quark == QUARK_SETATTR) {
        String   name = argv->getstring (0);
        Object*   obj = argv->get (1);
        Literal* lobj = dynamic_cast <Literal*> (obj);
        if (lobj == nullptr) {
          throw Exception ("type-error", "invalid object with set-attribute",
                           Object::repr (obj));
        }
        setattr (name, *lobj);
        return nullptr;
      }
    }

    // check for 3 arguments
    if (argc == 3) {
      if (quark == QUARK_SETATTR) {
        String   name = argv->getstring (0);
        Object*   obj = argv->get (1);
        Literal* lobj = dynamic_cast <Literal*> (obj);
        if (lobj == nullptr) {
          throw Exception ("type-error", "invalid object with set-attribute",
                           Object::repr (obj));
        }
	String tnam = argv->getstring (2);
        setattr (name, *lobj, tnam);
        return nullptr;
      }
    }
    // check the object
    return Object::apply (zobj, nset, quark, argv);
  }
}
