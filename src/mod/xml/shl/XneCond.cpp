// ---------------------------------------------------------------------------
// - XneCond.cpp                                                             -
// - afnix:xml module - xne condition class implementation                   -
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

#include "XmlPi.hpp"
#include "XmlGe.hpp"
#include "XmlTag.hpp"
#include "XmlCref.hpp"
#include "XmlEref.hpp"
#include "XmlData.hpp"
#include "XmlText.hpp"
#include "Boolean.hpp"
#include "Integer.hpp"
#include "XneCond.hpp"
#include "Evaluable.hpp"
#include "XmlEntity.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"
#include "XmlElement.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - private section                                                       -
  // -------------------------------------------------------------------------

  // the xml attributes names
  static const String XML_ID_ATTR = "id";

  // the condition structure
  
  struct t_cond {
    // the node selector type
    Xne::t_xsel d_type;
    // the condition name
    String d_name;
    // the condition index
    long d_indx;
    // the next condition
    t_cond* p_next;
    // create a condition by type
    t_cond (const Xne::t_xsel type) {
      switch (type) {
      case Xne::XNE_PI:
      case Xne::XNE_GE:
      case Xne::XNE_TAG:
      case Xne::XNE_ENT:
      case Xne::XNE_EREF:
      case Xne::XNE_CREF:
      case Xne::XNE_ELEM:
      case Xne::XNE_TEXT:
      case Xne::XNE_CDATA:
	d_type = type;
	d_indx = 0;
	p_next = nullptr;
	break;
      default:
	throw Exception ("xne-error", "invalid node type condition");
	break;
      }
    }
    // create a condition by type and name
    t_cond (const Xne::t_xsel type, const String& name) {
      switch (type) {
      case Xne::XNE_ID:
      case Xne::XNE_PI:
      case Xne::XNE_TAG:
      case Xne::XNE_NAME:
	d_type = type;
	d_name = name;
	d_indx = 0;
	p_next = nullptr;
	break;
      default:
	throw Exception ("xne-error", "invalid named condition");
	break;
      }
    }
    // create a condition by type and index
    t_cond (const Xne::t_xsel type, const long indx) {
      switch (type) {
      case Xne::XNE_INDEX:
	d_type = type;
	d_indx = indx;
	p_next = nullptr;
	break;
      default:
	throw Exception ("xne-error", "invalid index condition");
	break;
      }
    }
    // destroy this condition
    ~t_cond (void) {
      delete p_next;
    }
    // check if a node match a condition
    bool valid (const XmlNode* node) const{
      // check for nil node
      if (node == nullptr) return false;
      // check for id
      if (d_type == Xne::XNE_ID) {
	return node->isattr (XML_ID_ATTR, d_name);
      }
      // check for pi
      if (d_type == Xne::XNE_PI) {
	if (dynamic_cast <const XmlPi*> (node) == nullptr) return false;
	if (d_name.isnil () == true) return true;
	return node->isname (d_name);
      }
      // check for ge
      if (d_type == Xne::XNE_GE) {
	return (dynamic_cast <const XmlGe*> (node) == nullptr) ? false : true;
      }
      // check for tag
      if (d_type == Xne::XNE_TAG) {
	if (dynamic_cast <const XmlTag*> (node) == nullptr) return false;
	if (d_name.isnil () == true) return true;
	return node->isname (d_name);
      }
      // check for ent
      if (d_type == Xne::XNE_ENT) {
	return (dynamic_cast <const XmlEntity*> (node) == nullptr) ? false : true;
      }
      // check for eref
      if (d_type == Xne::XNE_EREF) {
	return (dynamic_cast <const XmlEref*> (node) == nullptr) ? false : true;
      }
      // check for cref
      if (d_type == Xne::XNE_CREF) {
	return (dynamic_cast <const XmlCref*> (node) == nullptr) ? false : true;
      }
      // check for element
      if (d_type == Xne::XNE_ELEM) {
	return (dynamic_cast <const XmlElement*> (node) == nullptr) ? false : true;
      }
      // check for text
      if (d_type == Xne::XNE_TEXT) {
	return (dynamic_cast <const XmlText*> (node) == nullptr) ? false : true;
      }
      // check for name
      if (d_type == Xne::XNE_NAME) {
	return node->isname (d_name);
      }
      // check for cdata
      if (d_type == Xne::XNE_CDATA) {
	return (dynamic_cast <const XmlData*> (node) == nullptr) ? false : true;
      }
      // check for index
      if (d_type == Xne::XNE_INDEX) {
	return node->isnidx (d_indx);
      }
      // invalid type
      throw Exception ("internal-error", "unknown xne condition type");
    }
  };

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create an empty xne condition

  XneCond::XneCond (void) {
    p_cond = nullptr;
  }

  // destroy this condition

  XneCond::~XneCond (void) {
    delete p_cond;
  }

  // return the class name

  String XneCond::repr (void) const {
    return "XneCond";
  }

  // reset this condition

  void XneCond::reset (void) {
    wrlock ();
    try {
      delete p_cond;
      p_cond = nullptr;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }
     
  // check if a node match a condition

  bool XneCond::valid (const XmlNode* node) const {
    rdlock ();
    try {
      // loop in the conditions
      t_cond* cond = p_cond;
      while (cond != nullptr) {
	if (cond->valid (node) == false) {
	  unlock ();
	  return false;
	}
	cond = cond->p_next;
      }
      unlock ();
      return true;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // add a condition by type
  
  void XneCond::add (const Xne::t_xsel type) {
    wrlock ();
    try {
      // create a new condition
      t_cond* cond = new t_cond (type);
      // link to root
      cond->p_next = p_cond;
      p_cond = cond;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // add a condition by type and name
  
  void XneCond::add (const Xne::t_xsel type, const String& name) {
    wrlock ();
    try {
      // create a new condition
      t_cond* cond = new t_cond (type, name);
      // link to root
      cond->p_next = p_cond;
      p_cond = cond;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // add a condition by type and index
  
  void XneCond::add (const Xne::t_xsel type, const long indx) {
    wrlock ();
    try {
      // create a new condition
      t_cond* cond = new t_cond (type, indx);
      // link to root
      cond->p_next = p_cond;
      p_cond = cond;
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
  static const long QUARK_ADD    = zone.intern ("add");
  static const long QUARK_RESET  = zone.intern ("reset");
  static const long QUARK_VALIDP = zone.intern ("valid-p");

  // create a new object in a generic way

  Object* XneCond::mknew (Vector* argv) {
    long argc = (argv == nullptr) ? 0 : argv->length ();
    // check for 0 argument
    if (argc == 0) return new XneCond;
    throw Exception ("argument-error",
                     "too many argument with xne condition constructor");
  }

  // return true if the given quark is defined

  bool XneCond::isquark (const long quark, const bool hflg) const {
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
  
  Object* XneCond::apply (Evaluable* zobj, Nameset* nset, const long quark,
			 Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();

    // check for 0 argument
    if (argc == 0) {
      if (quark == QUARK_RESET) {
	reset ();
	return nullptr;
      }
    }
    // check for 1 argument
    if (argc == 1) {
      if (quark == QUARK_VALIDP) {
        Object* obj = argv->get (0);
        XmlNode* node = dynamic_cast <XmlNode*> (obj);
        if ((obj != nullptr) && (node == nullptr)) {
          throw Exception ("type-error", "invalid object with valid-p",
                           Object::repr (obj));
        }
        return new Boolean (valid (node));
      }
      if (quark == QUARK_ADD) {
	// get the type
	Xne::t_xsel type = Xne::totype (argv->get(0));
	// add by type
	add (type);
	return nullptr;
      }
    }
    if (argc == 2) {
      if (quark == QUARK_ADD) {
	// get the type
	Xne::t_xsel type = Xne::totype (argv->get(0));
	// check for a string
	Object*  obj = argv->get (1);
        String* sobj = dynamic_cast <String*> (obj);
	if (sobj != nullptr) {
	  add (type, *sobj);
	  return nullptr;
	}
	// check for an integer
        Integer* iobj = dynamic_cast <Integer*> (obj);
	if (iobj != nullptr) {
	  add (type, iobj->tolong ());
	  return nullptr;
	}
	throw Exception ("type-error", "invalid object with add",
			 Object::repr (obj));
      }
    }
    // check the object
    return Object::apply (zobj, nset, quark, argv);
  }
}
