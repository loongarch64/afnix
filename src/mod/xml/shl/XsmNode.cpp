// ---------------------------------------------------------------------------
// - XsmNode.cpp                                                             -
// - afnix:xml module - xsm node class implementation                        -
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

#include "Item.hpp"
#include "Vector.hpp"
#include "Integer.hpp"
#include "Boolean.hpp"
#include "XsmNode.hpp"
#include "Evaluable.hpp"
#include "XsmBuffer.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - private section                                                       -
  // -------------------------------------------------------------------------

  // the xsm special characters
  static const t_quad XSM_CHAR_EP = 0x00000021; // !
  static const t_quad XSM_CHAR_LB = 0x0000005B; // [

  // this procedure convert a reference to a string
  static String ref_to_string (const String& xref) {
    // check for known reference
    if (xref == "lt") return "<";
    if (xref == "gt") return ">";
    // build default reference
    String result = "&";
    result += xref;
    result += ";";
    return result;
  }

  // this procedure check that a string corresponds to a reserved node
  static bool is_resv (const String& xval) {
    // check the size
    if (xval.isnil () == true) return false;
    // check for a special starter
    if (xval[0] == XSM_CHAR_EP) return true;
    if (xval[0] == XSM_CHAR_LB) return true;
    return false;
  }

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a default xsm node

  XsmNode::XsmNode (void) {
    d_type = TXT_NODE;
    d_tsub = TAG_TEXT;
    d_lnum = 0;
  }

  // create a text node by value

  XsmNode::XsmNode (const String& xval) {
    d_type = TXT_NODE;
    d_tsub = TAG_TEXT;
    d_xval = xval;
    d_lnum = 0;
  }

  // create a node by type and value

  XsmNode::XsmNode (const t_xsmt type, const String& xval) {
    d_type = type;
    d_tsub = ((type == TAG_NODE) && is_resv (xval)) ? TAG_RESV : TAG_TEXT;
    d_xval = xval;
    d_lnum = 0;
  }

  // copy construct this node

  XsmNode::XsmNode (const XsmNode& that) {
    that.rdlock ();
    try {
      d_type = that.d_type;
      d_tsub = that.d_tsub;
      d_xval = that.d_xval;
      d_lnum = that.d_lnum;
      d_snam = that.d_snam;  
      that.unlock ();
    } catch (...) {
      that.unlock ();
      throw;
    }
  }

  // return the class name

  String XsmNode::repr (void) const {
    return "XsmNode";
  }

  // get a clone of this node

  Object* XsmNode::clone (void) const {
    return new XsmNode (*this);
  }

  void XsmNode::clear (void) {
    wrlock ();
    try {
      d_type = TXT_NODE;
      d_tsub = TAG_TEXT;
      d_xval = "";
      d_snam = "";
      d_lnum = 0L;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the node string value

  String XsmNode::tostring (void) const {
    rdlock ();
    String result;
    switch (d_type) {
    case TXT_NODE:
    case TAG_NODE:
    case END_NODE:
      result = d_xval;
      break;
    case REF_NODE:
      result = ref_to_string (d_xval);
      break;
    }
    unlock ();
    return result;
  }

  // get the node literal value

  String XsmNode::toliteral (void) const {
    rdlock ();
    String result;
    switch (d_type) {
    case TXT_NODE:
      result = d_xval;
      break;
    case TAG_NODE:
      result  = "<";
      result += d_xval;
      result += ">";
      break;
    case END_NODE:
      result  = "</";
      result += d_xval;
      result += ">";
      break;
    case REF_NODE:
      result  = "&";
      result += d_xval;
      result += ";";
      break;
    }
    unlock ();
    return result;
  }

  // assign a node to this one

  XsmNode& XsmNode::operator = (const XsmNode& that) {
    // check for self-assignatiion
    if (this == &that) return *this;
    // lock and assign
    wrlock ();
    that.rdlock ();
    try {
      d_type = that.d_type;
      d_tsub = that.d_tsub;
      d_xval = that.d_xval;
      d_lnum = that.d_lnum;
      d_snam = that.d_snam;    
      unlock ();
      that.unlock ();
      return *this;
    } catch (...) {
      unlock ();
      that.unlock ();
      throw;
    }
  }

  // get the node type

  XsmNode::t_xsmt XsmNode::gettype (void) const {
    rdlock ();
    XsmNode::t_xsmt result = d_type;
    unlock ();
    return result;
  }

  // get the node subtype
  
  XsmNode::t_xsub XsmNode::gettsub (void) const {
    rdlock ();
    XsmNode::t_xsub result = d_tsub;
    unlock ();
    return result;
  }

  // set the source line number
  
  void XsmNode::setlnum (const long lnum) {
    wrlock ();
    d_lnum = lnum;
    unlock ();
  }

  // get the source line number

  long XsmNode::getlnum (void) const {
    rdlock ();
    long result= d_lnum;
    unlock ();
    return result;
  }

  // set the node source name
  
  void XsmNode::setsnam (const String& snam) {
    wrlock ();
    d_snam = snam;
    unlock ();
  }

  // get the node source name

  String XsmNode::getsnam (void) const {
    rdlock ();
    String result= d_snam;
    unlock ();
    return result;
  }

  // return true if the node is a text node
  
  bool XsmNode::istext (void) const {
    rdlock ();
    bool result = (d_type == TXT_NODE);
    unlock ();
    return result;
  }

  // return true if the node is a tag node

  bool XsmNode::istag (void) const {
    rdlock ();
    bool result = (d_type == TAG_NODE);
    unlock ();
    return result;
  }

  // return true if the node is a normal tag

  bool XsmNode::isntag (void) const {
    rdlock ();
    bool result = (d_type == TAG_NODE) && (d_tsub == TAG_TEXT);
    unlock ();
    return result;
  }

  // return true if the node is reserved
  
  bool XsmNode::isresv (void) const {
    rdlock ();
    bool result = (d_type == TAG_NODE) && (d_tsub == TAG_RESV);
    unlock ();
    return result;
  }

  // return true if the node is a text value node
  
  bool XsmNode::isxval (void) const {
    rdlock ();
    bool result = ((d_type == TXT_NODE) || (d_type == REF_NODE));
    unlock ();
    return result;
  }
  
  // return true if the node is a reference node

  bool XsmNode::isref (void) const {
    rdlock ();
    bool result = (d_type == REF_NODE);
    unlock ();
    return result;
  }

  // return true if the node is an end node

  bool XsmNode::isend (void) const {
    rdlock ();
    bool result = (d_type == END_NODE);
    unlock ();
    return result;
  }

  // get the node name

  String XsmNode::getname (void) const {
    return getname (false);
  }

  // get the node name by case flag

  String XsmNode::getname (const bool lwcf) const {
    rdlock ();
    try {
      // create a buffer
      XsmBuffer xbuf = d_xval;
      // get the next string
      String   name = xbuf.getnstr ();
      String result = lwcf ? name.tolower () : name;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get an attribute by name
  
  Property* XsmNode::getattr (const String& name) const {
    return getattr (name, false);
  }

  // get an attribute by name and flag
  
  Property* XsmNode::getattr (const String& name, const bool lwcf) const {
    rdlock ();
    try {
      // prepare result
      Property* result = nullptr;
      // create a buffer
      XsmBuffer xbuf = d_xval;
      // remove the node name
      xbuf.getnstr ();
      // loop in the buffer
      while (xbuf.isnext () == true) {
	// get the property and name
	Property prop = xbuf.getattr ();
	String   pnam = lwcf ? prop.getname().tolower () : prop.getname ();
	// check for name
	if (pnam != name) continue;
	result = new Property (prop);
	break;
      }
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      return nullptr;
    }
  }

  // get the tag attribute list

  Plist XsmNode::getattr (void) const {
    return getattr (false);
  }

  // get the tag attribute list by case flag

  Plist XsmNode::getattr (const bool lwcf) const {
    rdlock ();
    // create the result plist
    Plist result;
    // try to fill the list
    try {
      // create a buffer
      XsmBuffer xbuf = d_xval;
      // remove the name
      xbuf.getnstr ();
      while (xbuf.isnext () == true) {
	Property prop = xbuf.getattr ();
	String name = prop.getname ();
	String pval = prop.getpval ();
	if (lwcf == true) {
	  result.add (name.tolower (), pval);
	} else {
	  result.add (name, pval);
	}
      }
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      return result;
    }
  }

  // get a vector of words from a text node

  Strvec XsmNode::getwords (void) const {
    rdlock ();
    Strvec result;
    try {
      // check for the node
      if (d_type != TXT_NODE) {
	unlock ();
	return result;
      }
      // create a working buffer
      XsmBuffer xbuf = d_xval;
      // lookp for words
      while (xbuf.empty () == false) {
	String word = xbuf.getword ();
	if (word.isnil () == false) result.add (word);
      }
      // here is the vector
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      return result;
    }
  }

  // -------------------------------------------------------------------------
  // - object section                                                        -
  // -------------------------------------------------------------------------

  // the object eval quarks
  static const long QUARK_XSMNODE = String::intern ("XsmNode");
  static const long QUARK_TXTNODE = String::intern ("TXT");
  static const long QUARK_TAGNODE = String::intern ("TAG");
  static const long QUARK_REFNODE = String::intern ("REF");
  static const long QUARK_ENDNODE = String::intern ("END");

  // the quark zone
  static const long QUARK_ZONE_LENGTH = 14;
  static QuarkZone  zone (QUARK_ZONE_LENGTH);

  // the object supported quarks
  static const long QUARK_TAGP     = zone.intern ("tag-p");
  static const long QUARK_REFP     = zone.intern ("reference-p");
  static const long QUARK_ENDP     = zone.intern ("end-p");
  static const long QUARK_TEXTP    = zone.intern ("text-p");
  static const long QUARK_NTAGP    = zone.intern ("normal-p");
  static const long QUARK_RESVP    = zone.intern ("reserved-p");
  static const long QUARK_XVALP    = zone.intern ("textable-p");
  static const long QUARK_GETNAME  = zone.intern ("get-name");
  static const long QUARK_GETALST  = zone.intern ("get-attribute-list");
  static const long QUARK_GETLNUM  = zone.intern ("get-source-line");
  static const long QUARK_SETLNUM  = zone.intern ("set-source-line");
  static const long QUARK_GETSNAM  = zone.intern ("get-source-name");
  static const long QUARK_SETSNAM  = zone.intern ("set-source-name");
  static const long QUARK_GETWORDS = zone.intern ("get-words");

  // map an item to a node type
  static inline XsmNode::t_xsmt item_to_type (const Item& item) {
    // check for a key item
    if (item.gettid () != QUARK_XSMNODE)
      throw Exception ("item-error", "item is not a xsm node item");
    // map the item to the enumeration
    long quark = item.getquark ();
    if (quark == QUARK_TXTNODE) return XsmNode::TXT_NODE;
    if (quark == QUARK_TAGNODE) return XsmNode::TAG_NODE;
    if (quark == QUARK_REFNODE) return XsmNode::REF_NODE;
    if (quark == QUARK_ENDNODE) return XsmNode::END_NODE;
    throw Exception ("item-error", "cannot map item to xsm node type");
  }

  // evaluate an object data member

  Object* XsmNode::meval (Evaluable* zobj, Nameset* nset, const long quark) {
    if (quark == QUARK_TXTNODE)
      return new Item (QUARK_XSMNODE, QUARK_TXTNODE);
    if (quark == QUARK_TAGNODE)
      return new Item (QUARK_XSMNODE, QUARK_TAGNODE);
    if (quark == QUARK_REFNODE)
      return new Item (QUARK_XSMNODE, QUARK_REFNODE);
    if (quark == QUARK_ENDNODE)
      return new Item (QUARK_XSMNODE, QUARK_ENDNODE);
    throw Exception ("eval-error", "cannot evaluate member",
                     String::qmap (quark));
  }

  // create a new object in a generic way

  Object* XsmNode::mknew (Vector* argv) {
    long argc = (argv == nullptr) ? 0 : argv->length ();
    // check for 0 argument
    if (argc == 0) return new XsmNode;
    // check for 1 argument
    if (argc == 1) {
      String xval = argv->getstring (0);
      return new XsmNode (xval);
    }
    // check for 2 arguments
    if (argc == 2) {
      Object* obj = argv->get (0);
      String xval = argv->getstring (1);
      // check for an item type
      Item* iobj = dynamic_cast <Item*> (obj);
      if (iobj != nullptr) {
        t_xsmt type = item_to_type (*iobj);
        return new XsmNode (type, xval);
      }
      throw Exception ("type-error", "invalid object with xsm node",
		       Object::repr (obj));
    }
    throw Exception ("argument-error",
                     "too many argument with xsm node constructor");
  }
  // return true if the given quark is defined

  bool XsmNode::isquark (const long quark, const bool hflg) const {
    rdlock ();
    if (zone.exists (quark) == true) {
      unlock ();
      return true;
    }
    bool result = hflg ? Literal::isquark (quark, hflg) : false;
    unlock ();
    return result;
  }

  // apply this object with a set of arguments and a quark

  Object* XsmNode::apply (Evaluable* zobj, Nameset* nset, const long quark,
			  Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();

    // check for 0 argument
    if (argc == 0) {
      if (quark == QUARK_TAGP)     return new Boolean (istag   ());
      if (quark == QUARK_REFP)     return new Boolean (isref   ());
      if (quark == QUARK_ENDP)     return new Boolean (isend   ());
      if (quark == QUARK_TEXTP)    return new Boolean (istext  ());
      if (quark == QUARK_NTAGP)    return new Boolean (isntag  ());
      if (quark == QUARK_RESVP)    return new Boolean (isresv  ());
      if (quark == QUARK_XVALP)    return new Boolean (isxval  ());
      if (quark == QUARK_GETNAME)  return new String  (getname ());
      if (quark == QUARK_GETLNUM)  return new Integer (getlnum ());
      if (quark == QUARK_GETSNAM)  return new String  (getsnam ());
      if (quark == QUARK_GETALST)  return new Plist   (getattr ());
      if (quark == QUARK_GETWORDS) {
	Strvec words = getwords ();
	return words.tovector ();
      }
    }

    // check for 1 argument
    if (argc == 1) {
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
      if (quark == QUARK_GETNAME) {
	bool lwcf = argv->getbool (0);
	return new String (getname (lwcf));
      }
      if (quark == QUARK_GETALST) {
	bool lwcf = argv->getbool (0);
	return new Plist (getattr (lwcf));
      }
    }
    // call the literal method
    return Literal::apply (zobj, nset, quark, argv);
  }
}
