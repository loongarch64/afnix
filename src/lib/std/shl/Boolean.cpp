// ---------------------------------------------------------------------------
// - Boolean.cpp                                                             -
// - standard object library - boolean class implementation                  -
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

#include "Stdsid.hxx"
#include "Vector.hpp"
#include "Boolean.hpp"
#include "Evaluable.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"
#include "InputStream.hpp"
#include "OutputStream.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a new boolean - the initial value is false
  
  Boolean::Boolean (void) {
    d_value = false;
  }
  
  // create a boolean from a native value
  
  Boolean::Boolean (const bool value) {
    d_value = value;
  }
  
  // create a boolean from a string

  Boolean::Boolean (const String& value) {
    if (value == "false") {
      d_value = false;
    } else {
      if (value == "true") {
	d_value = true;
      } else {
	throw Exception ("literal-error","illegal boolean value", value);
      }
    }
  }

  // copy constructor for this boolean

  Boolean::Boolean (const Boolean& that) {
    that.rdlock ();
    try {
      d_value = that.d_value;
      that.unlock ();
    } catch (...) {
      that.unlock ();
      throw;
    }
  }

  // return the class name

  String Boolean::repr (void) const {
    return "Boolean";
  }

  // return a clone of this object

  Object* Boolean::clone (void) const {
    return new Boolean (*this);
  }

  // clear this boolean

  void Boolean::clear (void) {
    wrlock ();
    try {
      d_value = false;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return a literal representation of this boolean
  
  String Boolean::toliteral (void) const {
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

  // return a string representation of this boolean
  
  String Boolean::tostring (void) const {
    rdlock ();
    try {
      String result = d_value ? "true" : "false";
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return the boolean did

  t_word Boolean::getdid (void) const {
    return SRL_DEOD_STD;
  }

  // return the boolean sid

  t_word Boolean::getsid (void) const {
    return SRL_BOOL_SID;
  }

  // serialize this boolean

  void Boolean::wrstream (OutputStream& os) const {
    rdlock ();
    try {
      char c = d_value ? 0x01 : nilc;
      os.write (c);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // deserialize this boolean

  void Boolean::rdstream (InputStream& is) {
    wrlock ();
    try {
      char c = is.read ();
      d_value = (c == nilc) ? false : true;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return this boolean value

  bool Boolean::tobool (void) const {
    rdlock ();
    try {
      bool result = d_value;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // assign a boolean with a native value

  Boolean& Boolean::operator = (const bool value) {
    wrlock ();
    try {
      d_value = value;
      unlock ();
      return *this;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // assign a boolean with a boolean
  
  Boolean& Boolean::operator = (const Boolean& that) {
    // check for self assignation
    if (this == &that) return *this;
    // lock and assign
    wrlock ();
    that.rdlock ();
    try {
      d_value = that.d_value;
      unlock ();
      that.unlock ();
      return *this;
    } catch (...) {
      unlock ();
      that.unlock ();
      throw;
    }
  }
  
  // compare a boolean with a native value
  
  bool Boolean::operator == (const bool value) const {
    rdlock ();
    try {
      bool result = (d_value == value);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // compare two boolean
  
  bool Boolean::operator == (const Boolean& value) const {
    rdlock ();
    value.rdlock ();
    try {
      bool result = (d_value == value.d_value);
      unlock ();
      value.unlock ();
      return result;
    } catch (...) {
      unlock ();
      value.unlock ();
      throw;
    }
  }
  
  // compare a boolean with a native value
  
  bool Boolean::operator != (const bool value) const {
    rdlock ();
    try {
      bool result = (d_value != value);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // compare two boolean
  
  bool Boolean::operator != (const Boolean& value) const {
    rdlock ();
    value.rdlock ();
    try {
      bool result = (d_value != value.d_value);
      unlock ();
      value.unlock ();
      return result;
    } catch (...) {
      unlock ();
      value.unlock ();
      throw;
    }
  }

  // -------------------------------------------------------------------------
  // - object section                                                        -
  // -------------------------------------------------------------------------

  // the quark zone
  static const long QUARK_ZONE_LENGTH = 2;
  static QuarkZone  zone (QUARK_ZONE_LENGTH);

  // the object supported quarks
  static const long QUARK_EQL = zone.intern ("==");
  static const long QUARK_NEQ = zone.intern ("!=");

  // evaluate an object to a boolean value

  bool Boolean::evalto (Evaluable* zobj, Nameset* nset, Object* object) {
    Object* obj = (object == nullptr) ? nullptr : object->eval (zobj, nset);
    Boolean* val = dynamic_cast <Boolean*> (obj);
    if (val == nullptr) throw Exception ("type-error", "nil object to evaluate");
    return val->tobool ();
  }

  // create a new object in a generic way

  Object* Boolean::mknew (Vector* argv) {
    if ((argv == nullptr) || (argv->length () == 0)) return new Boolean;
    if (argv->length () != 1) 
      throw Exception ("argument-error", 
		       "too many argument with boolean constructor");
    // try to map the boolean argument
    Object* obj = argv->get (0);
    if (obj == nullptr) return new Boolean;

    // try a boolean object
    Boolean* bval = dynamic_cast <Boolean*> (obj);
    if (bval != nullptr) return new Boolean (*bval);

    // try a string object
    String* sval = dynamic_cast <String*> (obj);
    if (sval != nullptr) return new Boolean (*sval);

    // illegal object
    throw Exception ("type-error", "illegal object with boolean constructor",
		     obj->repr ());
  }

  // return true if the given quark is defined

  bool Boolean::isquark (const long quark, const bool hflg) const {
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

  Object* Boolean::oper (t_oper type, Object* object) {
    Boolean* bobj = dynamic_cast <Boolean*> (object);
    switch (type) {
    case Object::OPER_EQL:
    case Object::OPER_QEQ:
      if (bobj != nullptr) return new Boolean (*this == *bobj);
      break;
    case Object::OPER_NEQ:
      if (bobj != nullptr) return new Boolean (*this != *bobj);
      break;
    default:
      throw Exception ("operator-error", "unsupported boolean operator");
    }
    throw Exception ("type-error", "invalid operand with boolean",
		     Object::repr (object));
  }

  // set an object to this boolean

  Object* Boolean::vdef (Evaluable* zobj, Nameset* nset, Object* object) {
    wrlock ();
    try {
      Boolean* bobj = dynamic_cast <Boolean*> (object);
      if (bobj != nullptr) {
	*this = *bobj;
	zobj->post (this);
	unlock ();
	return this;
      }
      throw Exception ("type-error", "invalid object with boolean vdef",
		       Object::repr (object));
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // apply this object with a set of arguments and a quark

  Object* Boolean::apply (Evaluable* zobj, Nameset* nset, const long quark,
			  Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();

    // dispatch 1 argument
    if (argc == 1) {
      if (quark == QUARK_EQL) return oper (Object::OPER_EQL, argv->get (0));
      if (quark == QUARK_NEQ) return oper (Object::OPER_NEQ, argv->get (0));
    }
    // call the literal method
    return Literal::apply (zobj, nset, quark, argv);
  }
}
