// ---------------------------------------------------------------------------
// - XmlTag.cpp                                                              -
// - afnix:xml module - xml tag node class implementation                    -
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
#include "Utility.hpp"
#include "Boolean.hpp"
#include "Integer.hpp"
#include "Evaluable.hpp"
#include "XmlSystem.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a tag node by name

  XmlTag::XmlTag (const String& name) {
    d_name = name;
  }

  // create a node by name and empty flag

  XmlTag::XmlTag (const String& name, const bool eflg) {
    d_name = name;
    d_eflg = eflg;
  }


  // return the class name

  String XmlTag::repr (void) const {
    return "XmlTag";
  }

  // get a clone of this node

  Object* XmlTag::clone (void) const {
    rdlock ();
    try {
      // create a new node
      XmlTag* result = new XmlTag (d_name);
      // copy the arguments
      result->d_alst = d_alst; 
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

  // check if a node name is valid

  bool XmlTag::isname (const String& name) const {
    rdlock ();
    try {
      bool result = (d_name == name);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // clear the tag node attribute list

  void XmlTag::clrattr (void) {
    wrlock ();
    try {
      d_alst.reset ();
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // check if a node attribute is valid

  bool XmlTag::isattr (const String& name) const {
    rdlock ();
    try {
      bool result = d_alst.exists (name);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // check if a node attribute is valid

  bool XmlTag::isattr (const String& name, const String& pval) const {
    rdlock ();
    try {
      Property* prop = getattr (name);
      if (prop == nullptr) {
	unlock ();
	return false;
      }
      bool result = (prop->getpval () == pval);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the tag node name

  String XmlTag::getname (void) const {
    rdlock ();
    String result = d_name;
    unlock ();
    return result;
  }

  // set the tag node name

  void XmlTag::setname (const String& name) {
    wrlock ();
    try {
      d_name = name;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the number of attributes

  long XmlTag::lenattr (void) const {
    rdlock ();
    try {
      long result = d_alst.length ();
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // add a node attribute with a property

  void XmlTag::addattr (Property* prop) {
    wrlock ();
    try {
      d_alst.add (prop);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }
    
  // set a node attribute

  void XmlTag::setattr (const String& name, const Literal& lval) {
    wrlock ();
    try {
      d_alst.set (name, lval);
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set a node attribute

  void XmlTag::setattr (const String& name, const t_long xval) {
    wrlock ();
    try {
      String lval = Utility::tostring (xval);
      setattr (name, lval);
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set a node attribute

  void XmlTag::setattr (const String& name, const t_octa xval) {
    wrlock ();
    try {
      String lval = Utility::tohexa (xval, true, true);
      setattr (name, lval);
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get an attribute by index

  Property* XmlTag::getattr (const long index) const {
    rdlock ();
    try {
      Property* result = d_alst.get (index);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get an attribute by name

  Property* XmlTag::getattr (const String& name) const {
    rdlock ();
    try {
      Property* result = d_alst.find (name);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get an attribute by name or throw an exception

  Property* XmlTag::lookattr (const String& name) const {
    rdlock ();
    try {
      Property* result = d_alst.find (name);
      if (result == nullptr) {
	throw Exception ("lookup-error", "cannot find attribute", name);
      }
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get an attribute value by name

  String XmlTag::getpval (const String& name) const {
    rdlock ();
    try {
      Property* prop = lookattr (name);
      String result = prop->getpval ();
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // add an attribute list

  void XmlTag::addalst (const Plist& alst) {
    wrlock ();
    try {
      d_alst += alst;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // set an attribute list

  void XmlTag::setalst (const Plist& alst) {
    wrlock ();
    try {
      d_alst = alst;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // get an attribute list

  Plist XmlTag::getalst (void) const {
    rdlock ();
    try {
      Plist result = d_alst;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // merge a tag into this one

  void XmlTag::merge (const XmlTag& node) {
    wrlock ();
    try {
      // check for valid node
      if ((d_eflg == true) || (node.d_eflg == true)) {
	throw Exception ("tag-error", "cannot merge empty node");
      }
      if (d_name != node.d_name) {
	throw Exception ("tag-error", "inconsistent tag to merge", d_name);
      }
      // merge the attributes
      d_alst+= node.d_alst;
      // merge the child nodes
      long clen = node.lenchild ();
      for (long k = 0L; k < clen; k++) {
	addchild (node.getchild (k));
      }
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }
  // write a node to a buffer

  void XmlTag::write (Buffer& buf) const {
    rdlock ();
    try {
      // add the node name
      buf.add ("<");
      buf.add (d_name);
      // get the attribute list
      String attr = XmlSystem::toattr (d_alst);
      if (attr.isnil () == false) {
	buf.add (' ');
	buf.add (attr);
      }
      // check for subnodes
      if (d_eflg == true) {
	// write the terminaison
	buf.add ("/>");
      } else {
	//write the terminaison
	buf.add (">");
	if (d_eolf == true) buf.add (eolq);
	// write the subtree
	long len = lenchild ();
	for (long i = 0; i < len; i++) {
	  XmlNode* node = getchild (i);
	  if (node == nullptr) continue;
	  node->write (buf);
	}
	// write the terminaison
	buf.add ("</");
	buf.add (d_name);
	buf.add (">");
      }
      if (d_eolf == true) buf.add (eolq);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // write a node to an output stream

  void XmlTag::write (OutputStream& os) const {
    rdlock ();
    try {
      // add the node name
      os.write ("<");
      os.write (d_name);
      // get the attribute list
      String attr = XmlSystem::toattr (d_alst);
      if (attr.isnil () == false) {
	os.write (' ');
	os.write (attr);
      }
      // check for subnodes
      if (d_eflg == true) {
	// write the terminaison
	os.write ("/>");
      } else {
	//write the terminaison
	os.write   (">");
	if (d_eolf == true) os.newline ();
	// write the subtree
	long len = lenchild ();
	for (long i = 0; i < len; i++) {
	  XmlNode* node = getchild (i);
	  if (node == nullptr) continue;
	  node->write (os);
	}
	// write the terminaison
	os.write ("</");
	os.write (d_name);
	os.write (">");
      }
      if (d_eolf == true) os.newline ();
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
  static const long QUARK_ZONE_LENGTH = 13;
  static QuarkZone  zone (QUARK_ZONE_LENGTH);

  // the object supported quarks
  static const long QUARK_MERGE    = zone.intern ("set-name");
  static const long QUARK_SETNAME  = zone.intern ("set-name");
  static const long QUARK_GETNAME  = zone.intern ("get-name");
  static const long QUARK_CLRATTR  = zone.intern ("clear-attribute");
  static const long QUARK_ADDATTR  = zone.intern ("add-attribute");
  static const long QUARK_SETATTR  = zone.intern ("set-attribute");
  static const long QUARK_GETATTR  = zone.intern ("get-attribute");
  static const long QUARK_GETPVAL  = zone.intern ("get-attribute-value");
  static const long QUARK_ADDALST  = zone.intern ("add-attribute-list");
  static const long QUARK_SETALST  = zone.intern ("set-attribute-list");
  static const long QUARK_GETALST  = zone.intern ("get-attribute-list");
  static const long QUARK_LENATTR  = zone.intern ("attribute-length");
  static const long QUARK_LOOKATTR = zone.intern ("lookup-attribute");

  // create a new object in a generic way

  Object* XmlTag::mknew (Vector* argv) {
    long argc = (argv == nullptr) ? 0 : argv->length ();
    // check for 1 argument
    if (argc == 1) {
      String name = argv->getstring (0);
      return new XmlTag (name);
    }
    // check for 2 argument
    if (argc == 2) {
      String name = argv->getstring (0);
      bool   eflg = argv->getbool   (1);
      return new XmlTag (name, eflg);
    }
    throw Exception ("argument-error",
                     "too many argument with xml tag node constructor");
  }

  // return true if the given quark is defined

  bool XmlTag::isquark (const long quark, const bool hflg) const {
    rdlock ();
    try {
      if (zone.exists (quark) == true) {
	unlock ();
	return true;
      }
      bool result = hflg ? XmlNode::isquark (quark, hflg) : false;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // apply this object with a set of arguments and a quark
  
  Object* XmlTag::apply (Evaluable* zobj, Nameset* nset, const long quark,
			 Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();

    // check for 0 argument
    if (argc == 0) {
      if (quark == QUARK_GETNAME) return new String  (getname ());
      if (quark == QUARK_LENATTR) return new Integer (lenattr ());
      if (quark == QUARK_GETALST) return new Plist   (getalst ());
      if (quark == QUARK_CLRATTR) {
	clrattr ();
	return nullptr;
      }
    }
    // check for 1 argument
    if (argc == 1) {
      if (quark == QUARK_SETNAME) {
	String name = argv->getstring (0);
	setname (name);
	return nullptr;
      }
      if (quark == QUARK_ADDATTR) {
        Object* obj = argv->get (0);
        Property* prop = dynamic_cast <Property*> (obj);
        if ((obj != nullptr) && (prop == nullptr)) {
          throw Exception ("type-error", "invalid object with add",
                           Object::repr (obj));
        }
        addattr (prop);
        return nullptr;
      }
      if (quark == QUARK_GETATTR) {
	Object* obj = argv->get (0);
	// check for an integer
	Integer* iobj = dynamic_cast <Integer*> (obj);
	if (iobj != nullptr) {
	  long index = iobj->tolong ();
	  rdlock ();
	  try {
	    Property* result = getattr (index);
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
	    Property* result = getattr (*sobj);
	    zobj->post (result);
	    unlock ();
	    return result;
	  } catch (...) {
	    unlock ();
	    throw;
	  }
	}
	throw Exception ("type-error", "invalid object with get-attribute",
			 Object::repr (obj));
      }
      if (quark == QUARK_LOOKATTR) {
	String name = argv->getstring (0);
	rdlock ();
	try {
	  Property* result = lookattr (name);
	  zobj->post (result);
	  unlock ();
	  return result;
	} catch (...) {
	  unlock ();
	  throw;
	}
      }
      if (quark == QUARK_GETPVAL) {
	String name = argv->getstring (0);
	return new String (getpval (name));
      }
      if (quark == QUARK_ADDALST) {
        Object* obj = argv->get (0);
        Plist* alst = dynamic_cast <Plist*> (obj);
        if ((obj != nullptr) && (alst == nullptr)) {
          throw Exception ("type-error",
			   "invalid object with add-attribute-list",
                           Object::repr (obj));
        }
        addalst (*alst);
        return nullptr;
      }
      if (quark == QUARK_SETALST) {
        Object* obj = argv->get (0);
        Plist* alst = dynamic_cast <Plist*> (obj);
        if ((obj != nullptr) && (alst == nullptr)) {
          throw Exception ("type-error",
			   "invalid object with set-attribute-list",
                           Object::repr (obj));
        }
        setalst (*alst);
        return nullptr;
      }
      if (quark == QUARK_MERGE) {
        Object* obj = argv->get (0);
        XmlTag* node = dynamic_cast <XmlTag*> (obj);
        if (node == nullptr) {
          throw Exception ("type-error", "invalid object with merge",
                           Object::repr (obj));
        }
        merge (*node);
        return nullptr;
      }
    }
    // check for 2 arguments
    if (argc == 2) {
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
    // check the xml node object
    return XmlNode::apply (zobj, nset, quark, argv);
  }
}
