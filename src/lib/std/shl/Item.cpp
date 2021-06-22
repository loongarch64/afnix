// ---------------------------------------------------------------------------
// - Item.cpp                                                                -
// - standard object library - item class implementation                     -
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
#include "Boolean.hpp"
#include "Evaluable.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"

namespace afnix {
  
  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a new enumeration item by id

  Item::Item (const long tid, const long quark) {
    d_type  = ITEM_STIC;
    d_tid   = tid;
    d_quark = quark;
  }

  // create a new enumeration item by object

  Item::Item (Object* obj, const long quark) {
    d_type = ITEM_DYNC;
    Object::iref (p_obj = obj);
    d_quark = quark;
  }

  // copy construct this enumeration item

  Item::Item (const Item& that) {
    that.rdlock ();
    try {
      d_type  = that.d_type;
      d_quark = that.d_quark;
      if (d_type == ITEM_STIC) {
	d_tid = that.d_tid;
      } else {
	Object::iref (p_obj = that.p_obj);
      }
      that.unlock ();
    } catch (...) {
      that.unlock ();
      throw;
    }
  }

  // destroy this enumeration item

  Item::~Item (void) {
    if (d_type == ITEM_DYNC) Object::dref (p_obj);
  }

  // return the class name

  String Item::repr (void) const {
    return "Item";
  }
 
  // return a clone of this object

  Object* Item::clone (void) const {
    return new Item (*this);
  }

  // clear this item

  void Item::clear (void) {
    wrlock ();
    try {
      if (d_type == ITEM_DYNC) Object::dref (p_obj);
      d_type  = ITEM_DYNC;
      p_obj   = nullptr;
      d_quark = 0L;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return a literal representation of this item

  String Item::toliteral (void) const {
    rdlock ();
    try {
      String result = tostring ();
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return a string representation of this item

  String Item::tostring (void) const {
    rdlock ();
    try {
      String result = String::qmap (d_quark);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // compare this item with another one

  bool Item::operator == (const Item& item) const {
    rdlock ();
    try {
      if (d_type != item.d_type) {
	unlock ();
	return false;
      }
      if (d_type == ITEM_DYNC) {
	if (p_obj   != item.p_obj) {
	  unlock ();
	  return false;
	}
	if (d_quark != item.d_quark){
	  unlock ();
	  return false;
	}
      }
      if (d_type == ITEM_STIC) {
	if (d_tid   != item.d_tid) {
	  unlock ();
	  return false;
	}
	if (d_quark != item.d_quark) {
	  unlock ();
	  return false;
	}
      }
      unlock ();
      return true;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  bool Item::operator != (const Item& item) const {
    rdlock ();
    try {
      bool result = (*this == item);
      unlock ();
      return result ? false : true;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return the item quark

  long Item::getquark (void) const {
    rdlock ();
    try {
      long result = d_quark;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return true if the item is static with that id
  
  bool Item::scheck (const long tid) const {
    rdlock ();
    try {
      bool result = ((d_type == ITEM_STIC) && (d_tid == tid));
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the item tid

  long Item::gettid (void) const {
    rdlock ();
    try {
      if (d_type != ITEM_STIC) {
	throw Exception ("item-error", "trying to access a dynamic item");
      }
      long result = d_tid;
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
  static const long QUARK_EQL     = zone.intern ("==");
  static const long QUARK_NEQ     = zone.intern ("!=");
  static const long QUARK_GETENUM = zone.intern ("get-enum");

  // return true if the given quark is defined

  bool Item::isquark (const long quark, const bool hflg) const {
    rdlock ();
    try {
      if (zone.exists (quark) == true) {
	unlock ();
	return true;
      }
      bool result = hflg ? Literal::isquark (quark, hflg) : false;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // operate this object with another object

  Object* Item::oper (t_oper type, Object* object) {
    Item* iobj = dynamic_cast <Item*> (object);
    switch (type) {
    case Object::OPER_EQL:
    case Object::OPER_QEQ:
      if (iobj != nullptr) return new Boolean (*this == *iobj);
      break;
    case Object::OPER_NEQ:
      if (iobj != nullptr) return new Boolean (*this != *iobj);
      break;
    default:
      break;
    }
    throw Exception ("type-error", "invalid operand with item",
		     Object::repr (object));
  }

  // apply this item with a set of arguments and a quark

  Object* Item::apply (Evaluable* zobj, Nameset* nset, const long quark,
		       Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();

    // dispatch 0 argument
    if (argc == 0) {
      if (quark == QUARK_GETENUM) {
	rdlock ();
	try {
	  if (d_type == ITEM_STIC) {
	    throw Exception ("item-error", "cannot access static enumeration");
	  }
	  Object* result = p_obj;
	  zobj->post (result);
	  unlock ();
	  return result;
	} catch (...) {
	  unlock ();
	  throw;
	}
      }
    }
    // dispatch 1 argument
    if (argc == 1) {
      if (quark == QUARK_EQL) {
	Object* result = oper (Object::OPER_EQL, argv->get (0));
	return result;
      }
      if (quark == QUARK_NEQ) {
	Object* result = oper (Object::OPER_NEQ, argv->get (0));
	return result;
      }
    } 
    // call the literal method
    return Literal::apply (zobj, nset, quark, argv);
  }
}
