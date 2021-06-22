// ---------------------------------------------------------------------------
// - Integer.cpp                                                             -
// - standard object library - integer class implementation                  -
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

#include "Real.hpp"
#include "Stdsid.hxx"
#include "Vector.hpp"
#include "Utility.hpp"
#include "Boolean.hpp"
#include "Numeral.hpp"
#include "Evaluable.hpp"
#include "QuarkZone.hpp"
#include "Character.hpp"
#include "Exception.hpp"
#include "InputStream.hpp"
#include "OutputStream.hpp"
#include "ccnv.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - public section                                                        -
  // -------------------------------------------------------------------------

  // compute the opposite of the current integer
  
  Integer operator - (const Integer& x) {
    x.rdlock ();
    try {
      Integer result = -x.d_value;
      x.unlock ();
      return result;
    } catch (...) {
      x.unlock ();
      throw;
    }
  }
  
  // add two integers together from two integers

  Integer operator + (const Integer& x, const Integer& y) {
    x.rdlock ();
    y.rdlock ();
    try {
      Integer result = x.d_value + y.d_value;
      x.unlock ();
      y.unlock ();
      return result;
    } catch (...) {
      x.unlock ();
      y.unlock ();
      throw;
    }
  }

  // substract two integers together from two integers
  
  Integer operator - (const Integer& x, const Integer& y) {
    x.rdlock ();
    y.rdlock ();
    try {
      Integer result = x.d_value - y.d_value;
      x.unlock ();
      y.unlock ();
      return result;
    } catch (...) {
      x.unlock ();
      y.unlock ();
      throw;
    }
  }
  
  // multiply two integers together from two integers
  
  Integer operator * (const Integer& x, const Integer& y) {
    x.rdlock ();
    y.rdlock ();
    try {
      Integer result = x.d_value * y.d_value;
      x.unlock ();
      y.unlock ();
      return result;
    } catch (...) {
      x.unlock ();
      y.unlock ();
      throw;
    }
  }
    
  // divide two integers together from two integers
  
  Integer operator / (const Integer& x, const Integer& y) {
    x.rdlock ();
    y.rdlock ();
    try {
      if (y.d_value == 0) {
	throw Exception ("integer-error","division by zero");
      }
      Integer result = x.d_value / y.d_value;
      x.unlock ();
      y.unlock ();
      return result;
    } catch (...) {
      x.unlock ();
      y.unlock ();
      throw;
    }
  }

  // return the remainder of this number with its argument

  Integer operator % (const Integer& x, const Integer& y) {
    x.rdlock ();
    y.rdlock ();
    try {
      if (y.d_value == 0) {
	throw Exception ("integer-error","division by zero");
      }
      Integer result = x.d_value % y.d_value;
      x.unlock ();
      y.unlock ();
      return result;
    } catch (...) {
      x.unlock ();
      y.unlock ();
      throw;
    }
  }

  // bitwise negate an integer
  Integer operator ~ (const Integer& x) {
    x.rdlock ();
    try {
      Integer result = ~x.d_value;
      x.unlock ();
      return result;
    } catch (...) {
      x.unlock ();
      throw;
    }
  }

  // bitwise and an integer with another one

  Integer operator & (const Integer& x, const Integer& y) {
    x.rdlock ();
    y.rdlock ();
    try {
      Integer result = x.d_value & y.d_value;
      x.unlock ();
      y.unlock ();
      return result;
    } catch (...) {
      x.unlock ();
      y.unlock ();
      throw;
    }
  }

  // bitwise or an integer with another one

  Integer operator | (const Integer& x, const Integer& y) {
    x.rdlock ();
    y.rdlock ();
    try {
      Integer result = x.d_value | y.d_value;
      x.unlock ();
      y.unlock ();
      return result;
    } catch (...) {
      x.unlock ();
      y.unlock ();
      throw;
    }
  }

  // bitwise xor an integer with another one

  Integer operator ^ (const Integer& x, const Integer& y) {
    x.rdlock ();
    y.rdlock ();
    try {
      Integer result = x.d_value ^ y.d_value;
      x.unlock ();
      y.unlock ();
      return result;
    } catch (...) {
      x.unlock ();
      y.unlock ();
      throw;
    }
  }

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a new default integer
  
  Integer::Integer (void) {
    d_value = 0LL;
  }

  // create a new integer from a long integer

  Integer::Integer (const t_long value) {
    d_value = value;
  }

  // create a new integer from a string

  Integer::Integer (const String& value) {
    d_value = Utility::tolong (value);
  }
  
  // copy constructor for this integer

  Integer::Integer (const Integer& that) {
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

  String Integer::repr (void) const {
    return "Integer";
  }

  // return a clone of this object

  Object* Integer::clone (void) const {
    return new Integer (*this);
  }

  // clear this integer

  void Integer::clear (void) {
    wrlock ();
    try {
      d_value = 0LL;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return a literal representation of this integer

  String Integer::toliteral (void) const {
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

  // get a string representation on this integer

  String Integer::tostring (void) const {
    rdlock ();
    try {
      String result = Utility::tostring (d_value);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // format a styled string representation

  String Integer::format (const Style& lstl) const {
    rdlock ();
    try {
      String result = lstl.format (d_value);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return the integer did

  t_word Integer::getdid (void) const {
    return SRL_DEOD_STD;
  }

  // return the integer sid

  t_word Integer::getsid (void) const {
    return SRL_INTG_SID;
  }

  // serialize this integer

  void Integer::wrstream (OutputStream& os) const {
    rdlock ();
    try {
      t_byte data[8];
      c_ohton (d_value, data);
      os.write ((char*) data, 8);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // deserialize this integer

  void Integer::rdstream (InputStream& is) {
    wrlock ();
    try {
      t_byte data[8];
      for (long i = 0; i < 8; i++) data[i] = (t_byte) is.read ();
      d_value = c_ontoh (data);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // set an integer with a value

  Integer& Integer::operator = (const t_long value) {
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

  // set an integer with a value

  Integer& Integer::operator = (const Integer& that) {
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

  // add an integer to this one

  Integer& Integer::operator += (const Integer& x) {
    wrlock   ();
    x.rdlock ();
    try {
      d_value += x.d_value;
      unlock   ();
      x.unlock ();
      return *this;
    } catch (...) {
      unlock   ();
      x.unlock ();
      throw;
    }
  }

  // substract an integer to this one
  
  Integer& Integer::operator -= (const Integer& x) {
    wrlock   ();
    x.rdlock ();
    try {
      d_value -= x.d_value;
      unlock ();
      x.unlock   ();
      return *this;
    } catch (...) {
      unlock   ();
      x.unlock ();
      throw;
    }
  }

  // multiply an integer with this one
  
  Integer& Integer::operator *= (const Integer& x) {
    wrlock   ();
    x.rdlock ();
    try {
      d_value *= x.d_value;
      unlock ();
      x.unlock   ();
      return *this;
    } catch (...) {
      unlock   ();
      x.unlock ();
      throw;
    }
  }

  // divide an integer with this one
  
  Integer& Integer::operator /= (const Integer& x) {
    wrlock   ();
    x.rdlock ();
    try {
      if (x.d_value == 0) {
	throw Exception ("integer-error","division by zero");
      }
      d_value /= x.d_value;
      unlock ();
      x.unlock   ();
      return *this;
    } catch (...) {
      unlock   ();
      x.unlock ();
      throw;
    }
  }

  // prefix add one to the integer

  Integer& Integer::operator ++ (void) {
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

  // postfix add one to the integer

  Integer Integer::operator ++ (int) {
    wrlock ();
    try {
      Integer result = *this;
      d_value++;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // prefix sub one to the integer

  Integer& Integer::operator -- (void) {
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

  // postfix sub one to the integer

  Integer Integer::operator -- (int) {
    wrlock ();
    try {
      Integer result = *this;
      d_value--;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // shift left this integer

  Integer Integer::operator << (const long asl) const {
    rdlock ();
    try {
      Integer result = d_value << (asl % 64);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // shift right this integer

  Integer Integer::operator >> (const long asr) const {
    rdlock ();
    try {
      Integer result = d_value >> (asr % 64);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // compare an integer with a native value

  bool Integer::operator == (const t_long value) const {
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

  bool Integer::operator != (const t_long value) const {
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

  // compare two integers

  bool Integer::operator == (const Integer& value) const {
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

  // compare two integers

  bool Integer::operator != (const Integer& value) const {
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

  // compare two integers

  bool Integer::operator < (const Integer& value) const {
    rdlock ();
    value.rdlock ();
    try {
      bool result = (d_value <  value.d_value);
      unlock ();
      value.unlock ();
      return result;
    } catch (...) {
      unlock ();
      value.unlock ();
      throw;
    }
  }

  // compare two integers

  bool Integer::operator <= (const Integer& value) const {
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
  
  // compare two integers

  bool Integer::operator > (const Integer& value) const {
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

  // compare two integers

  bool Integer::operator >= (const Integer& value) const {
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

  // return true if the integer is zero

  bool Integer::iszero (void) const {
    rdlock ();
    try {
      bool result = (d_value == 0);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return true if the integer is even

  bool Integer::iseven (void) const {
    rdlock ();
    try {
      bool result = ((d_value & 1) == 0);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return true if the integer is odd

  bool Integer::isodd (void) const {
    rdlock ();
    try {
      bool result = ((d_value & 1) == 1);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return the absolute value of this number

  Integer Integer::abs (void) const {
    rdlock ();
    try {
      Integer result = (d_value < 0) ? -d_value : d_value;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // convert this integer into a hexadecimal representation

  String Integer::tohexa (void) const {
    rdlock ();
    try {
      // prepare result string
      String result = (d_value < 0) ? "-0x" : "0x";
      // bind the integer value
      t_octa data = d_value < 0 ? -d_value : d_value;
      result += Utility::tohexa (data);
      // unlock and return
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // convert this integer into a hexadecimal string

  String Integer::tohstr (void) const {
    rdlock ();
    try {
      // bind the integer value
      t_octa data = d_value < 0 ? -d_value : d_value;
      String result = Utility::tohexa (data);
      // unlock and return
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return a native integer value

  t_long Integer::tolong (void) const {
    rdlock ();
    try {
      t_long result = d_value;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the viewable size
  
  long Integer::tosize (void) const {
    rdlock ();
    try {
      long result = sizeof (d_value);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the viewable data
  
  t_byte* Integer::tobyte (void) {
    wrlock ();
    try {
      auto result = reinterpret_cast<t_byte*>(&d_value);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the viewable data
  
  const t_byte* Integer::tobyte (void) const {
    rdlock ();
    try {
      auto result = reinterpret_cast<const t_byte*>(&d_value);
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
  static const long QUARK_ZONE_LENGTH = 27;
  static QuarkZone  zone (QUARK_ZONE_LENGTH);

  // the object supported quarks
  static const long QUARK_OR    = zone.intern ("or");
  static const long QUARK_OPP   = zone.intern ("++");
  static const long QUARK_OMM   = zone.intern ("--");
  static const long QUARK_ADD   = zone.intern ("+");
  static const long QUARK_SUB   = zone.intern ("-");
  static const long QUARK_MUL   = zone.intern ("*");
  static const long QUARK_DIV   = zone.intern ("/");
  static const long QUARK_EQL   = zone.intern ("==");
  static const long QUARK_NEQ   = zone.intern ("!=");
  static const long QUARK_LTH   = zone.intern ("<");
  static const long QUARK_LEQ   = zone.intern ("<=");
  static const long QUARK_GTH   = zone.intern (">");
  static const long QUARK_GEQ   = zone.intern (">=");
  static const long QUARK_AEQ   = zone.intern ("+=");
  static const long QUARK_SEQ   = zone.intern ("-=");
  static const long QUARK_MEQ   = zone.intern ("*=");
  static const long QUARK_DEQ   = zone.intern ("/=");
  static const long QUARK_ABS   = zone.intern ("abs");
  static const long QUARK_AND   = zone.intern ("and");
  static const long QUARK_SHL   = zone.intern ("shl");
  static const long QUARK_SHR   = zone.intern ("shr");
  static const long QUARK_XOR   = zone.intern ("xor");
  static const long QUARK_MOD   = zone.intern ("mod");
  static const long QUARK_NOT   = zone.intern ("not");
  static const long QUARK_ODDP  = zone.intern ("odd-p");
  static const long QUARK_EVENP = zone.intern ("even-p");
  static const long QUARK_ZEROP = zone.intern ("zero-p");

  // evaluate an object to a native value

  t_long Integer::evalto (Evaluable* zobj, Nameset* nset, Object* object) {
    Object* obj = (object == nullptr) ? nullptr : object->eval (zobj, nset);
    Integer* val = dynamic_cast <Integer*> (obj);
    if (val == nullptr) {
      throw Exception ("type-error", "nil object to evaluate");
    }
    return val->tolong ();
  }

  // create a new object in a generic way

  Object* Integer::mknew (Vector* argv) {
    if ((argv == nullptr) || (argv->length () == 0)) return new Integer;
    if (argv->length () != 1) 
      throw Exception ("argument-error", 
		       "too many argument with integer constructor");
    // try to map the integer argument
    Object* obj = argv->get (0);
    if (obj == nullptr) return new Integer;

    // try an integer object
    auto ival = dynamic_cast <Integer*> (obj);
    if (ival != nullptr) return new Integer (*ival);

    // try a numeral object
    auto nval = dynamic_cast <Numeral*> (obj);
    if (nval != nullptr) return new Integer (nval->tolong());
    
    // try a real object
    auto rval = dynamic_cast <Real*> (obj);
    if (rval != nullptr) return new Integer (rval->tolong ());

    // try a character object
    Character* cval = dynamic_cast <Character*> (obj);
    if (cval != nullptr) return new Integer (cval->toquad ());

    // try a string object
    String* sval = dynamic_cast <String*> (obj);
    if (sval != nullptr) return new Integer (*sval);

    // illegal object
    throw Exception ("type-error", "illegal object with integer constructor",
		     obj->repr ());
  }

  // return true if the given quark is defined

  bool Integer::isquark (const long quark, const bool hflg) const {
    rdlock ();
    try {
      if (zone.exists (quark) == true) {
	unlock ();
	return true;
      }
      bool result = hflg ? Number::isquark (quark, hflg) : false;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // operate this object with another object

  Object* Integer::oper (t_oper type, Object* object) {
    auto iobj = dynamic_cast <Integer*> (object);
    auto nobj = dynamic_cast <Numeral*> (object);
    auto zobj = dynamic_cast <Real*>    (object);
    switch (type) {
    case Object::OPER_ADD:
      if (iobj != nullptr) return new Integer (*this + *iobj);
      if (nobj != nullptr) return new Integer (*this + nobj->tolong ());
      if (zobj != nullptr) return new Integer (*this + zobj->tolong ());
      break;
    case Object::OPER_SUB:
      if (iobj != nullptr) return new Integer (*this - *iobj);
      if (nobj != nullptr) return new Integer (*this - nobj->tolong ());
      if (zobj != nullptr) return new Integer (*this - zobj->tolong ());
      break;
    case Object::OPER_MUL:
      if (iobj != nullptr) return new Integer (*this * *iobj);
      if (nobj != nullptr) return new Integer (*this * nobj->tolong ());
      if (zobj != nullptr) return new Integer (*this * zobj->tolong ());
      break;
    case Object::OPER_DIV:
      if (iobj != nullptr) return new Integer (*this / *iobj);
      if (nobj != nullptr) return new Integer (*this / nobj->tolong ());
      if (zobj != nullptr) return new Integer (*this / zobj->tolong ());
      break;
    case Object::OPER_UMN:
      return new Integer (-(*this));
      break;
    case Object::OPER_EQL:
    case Object::OPER_QEQ:
      if (iobj != nullptr) return new Boolean (*this == *iobj);
      if (nobj != nullptr) return new Boolean (*this == nobj->tolong ());
      if (zobj != nullptr) return new Boolean (*this == zobj->tolong ());
      break;
    case Object::OPER_NEQ:
      if (iobj != nullptr) return new Boolean (*this != *iobj);
      if (nobj != nullptr) return new Boolean (*this != nobj->tolong ());
      if (zobj != nullptr) return new Boolean (*this != zobj->tolong ());
      break;
    case Object::OPER_GEQ:
      if (iobj != nullptr) return new Boolean (*this >= *iobj);
      if (nobj != nullptr) return new Boolean (*this >= nobj->tolong ());
      if (zobj != nullptr) return new Boolean (*this >= zobj->tolong ());
      break;
    case Object::OPER_GTH:
      if (iobj != nullptr) return new Boolean (*this > *iobj);
      if (nobj != nullptr) return new Boolean (*this > nobj->tolong ());
      if (zobj != nullptr) return new Boolean (*this > zobj->tolong ());
      break;
    case Object::OPER_LEQ:
      if (iobj != nullptr) return new Boolean (*this <= *iobj);
      if (nobj != nullptr) return new Boolean (*this <= nobj->tolong ());
      if (zobj != nullptr) return new Boolean (*this <= zobj->tolong ());
      break;
    case Object::OPER_LTH:
      if (iobj != nullptr) return new Boolean (*this < *iobj);
      if (nobj != nullptr) return new Boolean (*this < nobj->tolong ());
      if (zobj != nullptr) return new Boolean (*this < zobj->tolong ());
      break;
    }
    throw Exception ("type-error", "invalid operand with integer",
		     Object::repr (object));
  }

  // set an object to this integer

  Object* Integer::vdef (Evaluable* zobj, Nameset* nset, Object* object) {
    wrlock ();
    try {
      auto iobj = dynamic_cast <Integer*> (object);
      if (iobj != nullptr) {
	*this = *iobj;
	zobj->post (this);
	unlock ();
	return this;
      }
      auto nobj = dynamic_cast <Numeral*> (object);
      if (nobj != nullptr) {
	*this = nobj->tolong ();
	zobj->post (this);
	unlock ();
	return this;
      }
      auto dobj = dynamic_cast <Real*> (object);
      if (dobj != nullptr) {
	*this = dobj->tolong ();
	zobj->post (this);
	unlock ();
	return this;
      }
      Character* cobj = dynamic_cast <Character*> (object);
      if (cobj != nullptr) {
	*this = (t_long) cobj->toquad ();
	zobj->post (this);
	unlock ();
	return this;
      }
      throw Exception ("type-error", "invalid object with integer vdef",
		       Object::repr (object));
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // apply this object with a set of arguments and a quark

  Object* Integer::apply (Evaluable* zobj, Nameset* nset, const long quark,
			  Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();

    // dispatch 0 argument
    if (argc == 0) {
      if (quark == QUARK_ABS)   return new Integer (abs    ());
      if (quark == QUARK_EVENP) return new Boolean (iseven ());
      if (quark == QUARK_ODDP)  return new Boolean (isodd  ());
      if (quark == QUARK_ZEROP) return new Boolean (iszero ());

      if (quark == QUARK_NOT)   return new Integer (~(*this));
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
    }
    // dispatch 1 argument
    if (argc == 1) {
      if (quark == QUARK_ADD) return oper (Object::OPER_ADD, argv->get (0));
      if (quark == QUARK_SUB) return oper (Object::OPER_SUB, argv->get (0));
      if (quark == QUARK_MUL) return oper (Object::OPER_MUL, argv->get (0));
      if (quark == QUARK_DIV) return oper (Object::OPER_DIV, argv->get (0));
      if (quark == QUARK_EQL) return oper (Object::OPER_EQL, argv->get (0));
      if (quark == QUARK_NEQ) return oper (Object::OPER_NEQ, argv->get (0));
      if (quark == QUARK_LTH) return oper (Object::OPER_LTH, argv->get (0));
      if (quark == QUARK_LEQ) return oper (Object::OPER_LEQ, argv->get (0));
      if (quark == QUARK_GTH) return oper (Object::OPER_GTH, argv->get (0));
      if (quark == QUARK_GEQ) return oper (Object::OPER_GEQ, argv->get (0));

      if (quark == QUARK_AEQ) {
	wrlock ();
	try {
	  t_long val = argv->getlong (0);
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
	  t_long val = argv->getlong (0);
	  *this -= val;
	  zobj->post (this);
	  unlock ();
	  return this;
	} catch (...) {
	  unlock ();
	  throw;
	}
      }
      if (quark == QUARK_MEQ) {
	wrlock ();
	try {
	  t_long val = argv->getlong (0);
	  *this *= val;
	  zobj->post (this);
	  unlock ();
	  return this;
	} catch (...) {
	  unlock ();
	  throw;
	}
      }
      if (quark == QUARK_DEQ) {
	wrlock ();
	try {
	  t_long val = argv->getlong (0);
	  *this /= val;
	  zobj->post (this);
	  unlock ();
	  return this;
	} catch (...) {
	  unlock ();
	  throw;
	}
      }
      if (quark == QUARK_MOD) {
	t_long val = argv->getlong (0);
	return new Integer (*this % val);
      }
      if (quark == QUARK_SHL) {
	t_long asl = argv->getlong (0);
	Object* result = new Integer (*this << asl);
	return result;
      }
      if (quark == QUARK_SHR) {
	t_long asr = argv->getlong (0);
	Object* result = new Integer (*this >> asr);
	return result;
      }
      if (quark == QUARK_XOR) {
	t_long val = argv->getlong (0);
	Object* result = new Integer (*this ^ val);
	return result;
      }
      if (quark == QUARK_AND) {
	t_long val = argv->getlong (0);
	Object* result = new Integer (*this & val);
	return result;
      }
      if (quark == QUARK_OR) {
	t_long val = argv->getlong (0);
	Object* result = new Integer (*this | val);
	return result;
      }
    } 
    // call the number method
    return Number::apply (zobj, nset, quark, argv);
  }
}
