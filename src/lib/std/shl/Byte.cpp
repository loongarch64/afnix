// ---------------------------------------------------------------------------
// - Byte.cpp                                                                -
// - standard object library - byte class implementation                     -
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

#include "Byte.hpp"
#include "Ascii.hpp"
#include "Stdsid.hxx"
#include "Vector.hpp"
#include "Utility.hpp"
#include "Integer.hpp"
#include "Unicode.hpp"
#include "Boolean.hpp"
#include "Evaluable.hpp"
#include "QuarkZone.hpp"
#include "Character.hpp"
#include "Exception.hpp"
#include "InputStream.hpp"
#include "OutputStream.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a new default byte

  Byte::Byte (void) {
    d_value = nilc;
  }

  // create a new byte from a character

  Byte::Byte (const char value) {
    d_value = value;
  }

  // create a new byte from a native byte
  
  Byte::Byte (const t_byte value) {
    d_value = value;
  }
  
  // copy constructor for this byte class

  Byte::Byte (const Byte& that) {
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

  String Byte::repr (void) const {
    return "Byte";
  }

  // return a clone of this object

  Object* Byte::clone (void) const {
    return new Byte (*this);
  }

  // return the byte did

  t_word Byte::getdid (void) const {
    return SRL_DEOD_STD;
  }

  // return the byte sid

  t_word Byte::getsid (void) const {
    return SRL_BYTE_SID;
  }

  // serialize this byte

  void Byte::wrstream (OutputStream& os) const {
    rdlock ();
    try {
      os.write ((char) d_value);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // deserialize this byte

  void Byte::rdstream (InputStream& is) {
    wrlock ();
    try {
      d_value = (t_byte) is.read ();
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return a native byte

  t_byte Byte::tobyte (void) const {
    rdlock ();
    try {
      t_byte result = d_value;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return a native quad

  t_quad Byte::toquad (void) const {
    rdlock ();
    try {
      t_quad result = Unicode::toquad (d_value);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // return a hexadecimal representation
  
  String Byte::tohexa (void) const {
    rdlock ();
    try {
      String result = Utility::tohexa (d_value, true, true);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // return a hexadecimal string
  
  String Byte::tohstr (void) const {
    rdlock ();
    try {
      String result = Utility::tohexa (d_value, true, false);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // set an byte with a native value

  Byte& Byte::operator = (const char value) {
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

  // assign a byte to this one

  Byte& Byte::operator = (const Byte& that) {
    // check for self-assignation
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
  
  // add a byte with a native value

  Byte Byte::operator + (const long value)  const {
    wrlock ();
    try {
      char result = d_value + value;
      unlock ();
      return Byte (result);
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // sub a byte with a native value

  Byte Byte::operator - (const long value)  const {
    wrlock ();
    try {
      char result = d_value - value;
      unlock ();
      return Byte (result);
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // add a native value to this byte
  
  Byte& Byte::operator += (const long value) {
    wrlock ();
    try {
      d_value = d_value + value;
      unlock ();
      return *this;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // sub a native value to this byte

  Byte& Byte::operator -= (const long value) {
    wrlock ();
    try {
      d_value = d_value - value;
      unlock ();
      return *this;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // prefix add one to the byte

  Byte& Byte::operator ++ (void) {
    wrlock ();
    try {
      ++d_value;
      unlock ();
      return *this;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // postfix add one to the byte

  Byte Byte::operator ++ (int) {
    wrlock ();
    try {
      Byte result = *this;
      d_value++;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // prefix sub one to the byte

  Byte& Byte::operator -- (void) {
    wrlock ();
    try {
      --d_value;
      unlock ();
      return *this;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // postfix sub one to the byte

  Byte Byte::operator -- (int) {
    wrlock ();
    try {
      Byte result = *this;
      d_value--;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // compare an byte with a native value
  
  bool Byte::operator == (const char value) const {
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
  
  bool Byte::operator != (const char value) const {
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
  
  // compare two byte bytes
  
  bool Byte::operator == (const Byte& value) const {
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
  
  bool Byte::operator != (const Byte& value) const {
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
  
  bool Byte::operator < (const Byte& value) const {
    rdlock ();
    value.rdlock ();
    try {
      bool result = (d_value < value.d_value);
      unlock ();
      value.unlock ();
      return result;
    } catch (...) {
      unlock ();
      value.unlock ();
      throw;
    }
  }
  
  bool Byte::operator <= (const Byte& value) const {
    rdlock ();
    value.rdlock ();
    try {
      bool result = (d_value <= value.d_value);
      unlock ();
      value.unlock ();
      return result;
    } catch (...) {
      unlock ();
      value.unlock ();
      throw;
    }
  }
  
  bool Byte::operator > (const Byte& value) const {
    rdlock ();
    value.rdlock ();
    try {
      bool result = (d_value > value.d_value);
      unlock ();
      value.unlock ();
      return result;
    } catch (...) {
      unlock ();
      value.unlock ();
      throw;
    }
  }
  
  bool Byte::operator >= (const Byte& value) const {
    rdlock ();
    value.rdlock ();
    try {
      bool result = (d_value >= value.d_value);
      unlock ();
      value.unlock ();
      return result;
    } catch (...) {
      unlock ();
      value.unlock ();
      throw;
    }
  }
  
  // return true if the byte is an eos

  bool Byte::iseos (void) const {
    rdlock ();
    try {
      bool result = (d_value == eosc);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return true if the byte is nil

  bool Byte::isnil (void) const {
    rdlock ();
    try {
      bool result = (d_value == nilc);
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
  static const long QUARK_ZONE_LENGTH = 18;
  static QuarkZone  zone (QUARK_ZONE_LENGTH);

  // the object supported quarks
  static const long QUARK_OPP    = zone.intern ("++");
  static const long QUARK_OMM    = zone.intern ("--");
  static const long QUARK_ADD    = zone.intern ("+");
  static const long QUARK_SUB    = zone.intern ("-");
  static const long QUARK_EQL    = zone.intern ("==");
  static const long QUARK_NEQ    = zone.intern ("!=");
  static const long QUARK_LTH    = zone.intern ("<");
  static const long QUARK_LEQ    = zone.intern ("<=");
  static const long QUARK_GTH    = zone.intern (">");
  static const long QUARK_GEQ    = zone.intern (">=");
  static const long QUARK_AEQ    = zone.intern ("+=");
  static const long QUARK_SEQ    = zone.intern ("-=");
  static const long QUARK_EOSP   = zone.intern ("eos-p");
  static const long QUARK_NILP   = zone.intern ("nil-p");
  static const long QUARK_TOINTG = zone.intern ("to-integer");
  static const long QUARK_TOCHAR = zone.intern ("to-character");
  static const long QUARK_TOHEXA = zone.intern ("to-hexa");
  static const long QUARK_TOHSTR = zone.intern ("to-hexa-string");

  // create a new object in a generic way

  Object* Byte::mknew (Vector* argv) {
    if ((argv == nullptr) || (argv->length () == 0)) return new Byte;
    if (argv->length () != 1) 
      throw Exception ("argument-error", 
		       "too many argument with byte constructor");
    // try to map the byte argument
    Object* obj = argv->get (0);
    if (obj == nullptr) return new Byte;

    // try an integer object
    Integer* ival = dynamic_cast <Integer*> (obj);
    if (ival != nullptr) return new Byte ((t_byte) ival->tolong ());

    // try a byte object
    Byte* cval = dynamic_cast <Byte*> (obj);
    if (cval != nullptr) return new Byte (*cval);

    // illegal object
    throw Exception ("type-error", "illegal object with byte constructor",
		     obj->repr ());
  }

  // return true if the given quark is defined

  bool Byte::isquark (const long quark, const bool hflg) const {
    rdlock ();
    try {
      if (zone.exists (quark) == true) {
	unlock ();
	return true;
      }
      bool result = hflg ? Serial::isquark (quark, hflg) : false;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // operate this object with another object

  Object* Byte::oper (t_oper type, Object* object) {
    Integer* iobj = dynamic_cast <Integer*> (object);
    Byte*    cobj = dynamic_cast <Byte*>    (object);
    switch (type) {
    case Object::OPER_ADD:
      if (iobj != nullptr) return new Byte (*this + iobj->tolong ());
      break;
    case Object::OPER_SUB:
      if (iobj != nullptr) return new Byte (*this - iobj->tolong ());
      break;
    case Object::OPER_EQL:
    case Object::OPER_QEQ:
      if (cobj != nullptr) return new Boolean (*this == *cobj);
      break;
    case Object::OPER_NEQ:
      if (cobj != nullptr) return new Boolean (*this != *cobj);
      break;
    case Object::OPER_GEQ:
      if (cobj != nullptr) return new Boolean (*this >= *cobj);
      break;
    case Object::OPER_GTH:
      if (cobj != nullptr) return new Boolean (*this > *cobj);
      break;
    case Object::OPER_LEQ:
      if (cobj != nullptr) return new Boolean (*this <= *cobj);
      break;
    case Object::OPER_LTH:
      if (cobj != nullptr) return new Boolean (*this < *cobj);
      break;
    default:
      throw Exception ("operator-error", "unsupported byte operator");
    }
    throw Exception ("type-error", "invalid operand with byte",
		     Object::repr (object));
  }

  // set an object to this byte

  Object* Byte::vdef (Evaluable* zobj, Nameset* nset, Object* object) {
    wrlock ();
    try {
      Byte* cobj = dynamic_cast <Byte*> (object);
      if (cobj != nullptr) {
	*this = *cobj;
	zobj->post (this);
	unlock ();
	return this;
      }
      throw Exception ("type-error", "invalid object with byte vdef",
		       Object::repr (object));
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // apply this object with a set of arguments and a quark

  Object* Byte::apply (Evaluable* zobj, Nameset* nset, const long quark,
			    Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();

    // dispatch 0 argument
    if (argc == 0) {
      if (quark == QUARK_TOINTG) return new Integer   (tobyte ());
      if (quark == QUARK_TOCHAR) return new Character (toquad ());
      if (quark == QUARK_TOHEXA) return new String    (tohexa ());
      if (quark == QUARK_TOHSTR) return new String    (tohstr ());
      if (quark == QUARK_OPP) {
	wrlock ();
	try {
	  ++(*this);
	  zobj->post (this);
	  unlock ();
	  return this;
	} catch (...) {
	  unlock ();
	  throw;
	}
      }
      if (quark == QUARK_OMM) {
	wrlock ();
	try {
	  --(*this);
	  zobj->post (this);
	  unlock ();
	  return this;
	} catch (...) {
	  unlock ();
	  throw;
	}
      }
      if (quark == QUARK_EOSP) return new Boolean (iseos ());
      if (quark == QUARK_NILP) return new Boolean (isnil ());
    }
    // dispatch 1 argument
    if (argc == 1) {
      if (quark == QUARK_ADD) return oper (Object::OPER_ADD, argv->get (0));
      if (quark == QUARK_SUB) return oper (Object::OPER_SUB, argv->get (0));
      if (quark == QUARK_EQL) return oper (Object::OPER_EQL, argv->get (0));
      if (quark == QUARK_NEQ) return oper (Object::OPER_NEQ, argv->get (0));
      if (quark == QUARK_LTH) return oper (Object::OPER_LTH, argv->get (0));
      if (quark == QUARK_LEQ) return oper (Object::OPER_LEQ, argv->get (0));
      if (quark == QUARK_GTH) return oper (Object::OPER_GTH, argv->get (0));
      if (quark == QUARK_GEQ) return oper (Object::OPER_GEQ, argv->get (0));

      if (quark == QUARK_AEQ) {
	wrlock ();
	try {
	  long val = argv->getlong (0);
	  *this += val;
	  zobj->post (this);
	  unlock ();
	  return this;
	} catch (...) {
	  unlock ();
	  throw;
	}
      }
      if (quark == QUARK_SEQ) {
	wrlock ();
	try {
	  long val = argv->getlong (0);
	  *this -= val;
	  zobj->post (this);
	  unlock ();
	  return this;
	} catch (...) {
	  unlock ();
	  throw;
	}
      }
    }
    // call the serial method
    return Serial::apply (zobj, nset, quark, argv);
  }
}
