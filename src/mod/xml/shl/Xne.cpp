// ---------------------------------------------------------------------------
// - Xne.cpp                                                                 -
// - afnix:xml module - xne class implementation                             -
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

#include "Xne.hpp"
#include "Exception.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - private section                                                       -
  // -------------------------------------------------------------------------
  
  // the expanded name separator
  static const t_quad XML_CHAR_DC = 0x0000003A; // :

  // the node selector type eval quarks
  static const long QUARK_ID    = String::intern ("ID");
  static const long QUARK_PI    = String::intern ("PI");
  static const long QUARK_GE    = String::intern ("GE");
  static const long QUARK_TAG   = String::intern ("TAG");
  static const long QUARK_ENT   = String::intern ("ENT");
  static const long QUARK_EREF  = String::intern ("EREF");
  static const long QUARK_CREF  = String::intern ("CREF");
  static const long QUARK_ELEM  = String::intern ("ELEM");
  static const long QUARK_TEXT  = String::intern ("TEXT");
  static const long QUARK_NAME  = String::intern ("NAME");
  static const long QUARK_CDATA = String::intern ("CDATA");
  static const long QUARK_INDEX = String::intern ("INDEX");
  static const long QUARK_XNE   = String::intern ("Xne");

  // -------------------------------------------------------------------------
  // - public section                                                        -
  // -------------------------------------------------------------------------

  // get a selector type by item

  Xne::t_xsel Xne::totype (const Item& item) {
    // check for a key item
    if (item.gettid () != QUARK_XNE) {
      throw Exception ("item-error", "item is not a xne item");
    }
    // map the item to the enumeration
    long quark = item.getquark ();
    if (quark == QUARK_ID)    return Xne::XNE_ID;
    if (quark == QUARK_PI)    return Xne::XNE_PI;
    if (quark == QUARK_GE)    return Xne::XNE_GE;
    if (quark == QUARK_TAG)   return Xne::XNE_TAG;
    if (quark == QUARK_ENT)   return Xne::XNE_ENT;
    if (quark == QUARK_EREF)  return Xne::XNE_EREF;
    if (quark == QUARK_CREF)  return Xne::XNE_CREF;
    if (quark == QUARK_ELEM)  return Xne::XNE_ELEM;
    if (quark == QUARK_TEXT)  return Xne::XNE_TEXT;
    if (quark == QUARK_NAME)  return Xne::XNE_NAME;
    if (quark == QUARK_CDATA) return Xne::XNE_CDATA;
    if (quark == QUARK_INDEX) return Xne::XNE_INDEX;
    throw Exception ("item-error", "cannot map item to xne selector type");
  }
  
  // get a selector type by object
  
  Xne::t_xsel Xne::totype (Object* obj) {
    Item* iobj = dynamic_cast <Item*> (obj);
    if (iobj == nullptr) {
      throw Exception ("type-error", "invalid object as a xne selector item",
		       Object::repr (obj));
    }
    return Xne::totype (*iobj);
  }
  
  // get an item by selector type
  
  Item* Xne::toitem (const t_xsel xsel) {
    switch (xsel) {
    case XNE_ID:
      return new Item (QUARK_XNE, QUARK_ID);
      break;
    case XNE_PI:
      return new Item (QUARK_XNE, QUARK_PI);
      break;
    case XNE_GE:
      return new Item (QUARK_XNE, QUARK_GE);
      break;
   case XNE_TAG:
      return new Item (QUARK_XNE, QUARK_TAG);
      break;
   case XNE_ENT:
      return new Item (QUARK_XNE, QUARK_ENT);
      break;
   case XNE_EREF:
      return new Item (QUARK_XNE, QUARK_EREF);
      break;
   case XNE_CREF:
      return new Item (QUARK_XNE, QUARK_CREF);
      break;
   case XNE_ELEM:
      return new Item (QUARK_XNE, QUARK_ELEM);
      break;
   case XNE_TEXT:
      return new Item (QUARK_XNE, QUARK_TEXT);
      break;
    case XNE_NAME:
      return new Item (QUARK_XNE, QUARK_NAME);
      break;
    case XNE_CDATA:
      return new Item (QUARK_XNE, QUARK_CDATA);
      break;
    case XNE_INDEX:
      return new Item (QUARK_XNE, QUARK_INDEX);
      break;
    }
    return nullptr;
  }

  // return true if the name is a valid nc name

  bool Xne::isncnm (const String& name) {
    // get the name length
    long len = name.length ();
    for (long i = 0; i < len; i++) {
      if (name[i] == XML_CHAR_DC) return false;
    }
    return true;
  }

  // get the prefix from a name

  String Xne::getpnam (const String& name) {
    // the result buffer
    Buffer xbuf (Encoding::EMOD_UTF8);
    // get the string length
    long len = name.length ();
    for (long i = 0; i < len; i++) {
      t_quad c = name[i];
      if (c == XML_CHAR_DC) {
        String result = xbuf.tostring ();
        return result;
      }
      xbuf.add (c);
    }
    // no prefix found
    return "";
  }

  // get the local name from a name

  String Xne::getlnam (const String& name) {
    // the result buffer
    Buffer xbuf (Encoding::EMOD_UTF8);
    // get the string length
    long len = name.length ();
    bool flg = false;
    for (long i = 0; i < len; i++) {
      t_quad c = name[i];
      if ((c == XML_CHAR_DC) && (flg == false)) {
        xbuf.reset ();
        flg = true;
      } else {
        xbuf.add (c);
      }
    }
    // here is the local name
    return xbuf.tostring ();
  }

  // return the expanded name from a prefix and a local name
  
  String Xne::toexnm (const String& pnam, const String& lnam) {
    // check for nil namespace
    if (pnam.isnil () == true) return lnam;
    // check the names before expension
    if (Xne::isncnm (pnam) == false) {
      throw Exception ("name-error", "invalid prefix name to expand", pnam);
    }
    if (Xne::isncnm (lnam) == false) {
      throw Exception ("name-error", "invalid local name to expand", lnam);
    }
    // expand name
    String result = pnam;
    result += XML_CHAR_DC;
    result += lnam;
    return result;
  }

  // -------------------------------------------------------------------------
  // - object section                                                        -
  // -------------------------------------------------------------------------

  // evaluate an object data member

  Object* Xne::meval (Evaluable* zobj, Nameset* nset, const long quark) {
    if (quark == QUARK_ID)    return new Item (QUARK_XNE, QUARK_ID);
    if (quark == QUARK_PI)    return new Item (QUARK_XNE, QUARK_PI);
    if (quark == QUARK_GE)    return new Item (QUARK_XNE, QUARK_GE);
    if (quark == QUARK_TAG)   return new Item (QUARK_XNE, QUARK_TAG);
    if (quark == QUARK_ENT)   return new Item (QUARK_XNE, QUARK_ENT);
    if (quark == QUARK_EREF)  return new Item (QUARK_XNE, QUARK_EREF);
    if (quark == QUARK_CREF)  return new Item (QUARK_XNE, QUARK_CREF);
    if (quark == QUARK_ELEM)  return new Item (QUARK_XNE, QUARK_ELEM);
    if (quark == QUARK_TEXT)  return new Item (QUARK_XNE, QUARK_TEXT);
    if (quark == QUARK_NAME)  return new Item (QUARK_XNE, QUARK_NAME);
    if (quark == QUARK_CDATA) return new Item (QUARK_XNE, QUARK_CDATA);
    if (quark == QUARK_INDEX) return new Item (QUARK_XNE, QUARK_INDEX);
    throw Exception ("eval-error", "cannot evaluate member",
                     String::qmap (quark));
  }
}
