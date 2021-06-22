// ---------------------------------------------------------------------------
// - Real.cpp                                                                -
// - standard object library - real class implementation                     -
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
#include "Math.hpp"
#include "Stdsid.hxx"
#include "Vector.hpp"
#include "Utility.hpp"
#include "Boolean.hpp"
#include "Numeral.hpp"
#include "Evaluable.hpp"
#include "Character.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"
#include "InputStream.hpp"
#include "OutputStream.hpp"
#include "ccnv.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - public section                                                       -
  // -------------------------------------------------------------------------

  // compute the opposite of a real

  Real operator - (const Real& x) {
    x.rdlock ();
    try {
      Real result = -x.d_value;
      x.unlock ();
      return result;
    } catch (...) {
      x.unlock ();
      throw;
    }
  }

  // add two reals together

  Real operator + (const Real& x, const Real& y) {
    x.rdlock ();
    y.rdlock ();
    try {
      Real result = x.d_value + y.d_value;
      x.unlock ();
      y.unlock ();
      return result;
    } catch (...) {
      x.unlock ();
      y.unlock ();
      throw;
    }
  }

  // subtract two reals

  Real operator - (const Real& x, const Real& y) {
    x.rdlock ();
    y.rdlock ();
    try {
      Real result = x.d_value - y.d_value;
      x.unlock ();
      y.unlock ();
      return result;
    } catch (...) {
      x.unlock ();
      y.unlock ();
      throw;
    }
  }

  // multiply two reals

  Real operator * (const Real& x, const Real& y) {
    x.rdlock ();
    y.rdlock ();
    try {
      Real result = x.d_value * y.d_value;
      x.unlock ();
      y.unlock ();
      return result;
    } catch (...) {
      x.unlock ();
      y.unlock ();
      throw;
    }
  }

  // divide two reals 

  Real operator / (const Real& x, const Real& y) {
    x.rdlock ();
    y.rdlock ();
    try {
      Real result = x.d_value / y.d_value;
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

  // create a new default real

  Real::Real (void) {
    d_value = 0.0;
  }

  // create a new real from a native real

  Real::Real (const t_real value) {
    d_value = value;
  }

  // create a new real from an Integer

  Real::Real (const Integer& value) {
    d_value = value.tolong ();
  }

  // create a new real from a string

  Real::Real (const String& value) {
    d_value = Utility::toreal (value);
  }
  
  // copy constructor for this real

  Real::Real (const Real& that) {
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

  String Real::repr (void) const {
    return "Real";
  }

  // return a clone of this object

  Object* Real::clone (void) const {
    return new Real (*this);
  }

  // clear this real

  void Real::clear (void) {
    wrlock ();
    try {
      d_value = 0.0;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return a literal representation of this real

  String Real::toliteral (void) const {
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

  // return a string representation of this real

  String Real::tostring (void) const {
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

  String Real::format (const Style& lstl) const {
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
	
  // return the real did

  t_word Real::getdid (void) const {
    return SRL_DEOD_STD;
  }

  // return the real sid

  t_word Real::getsid (void) const {
    return SRL_REAL_SID;
  }

  // serialize this real

  void Real::wrstream (OutputStream& os) const {
    rdlock ();
    try {
      t_byte data[8];
      c_rhton (d_value, data);
      os.write ((char*) data, 8);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // deserialize this real

  void Real::rdstream (InputStream& is) {
    wrlock ();
    try {
      t_byte data[8];
      for (long i = 0; i < 8; i++) data[i] = (t_byte) is.read ();
      d_value = c_ontor (data);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // set an real with a value

  Real& Real::operator = (const t_real value) {
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

  // assign a real to this one

  Real& Real::operator = (const Real& that) {
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

  // add a real to this one

  Real& Real::operator += (const Real& x) {
    wrlock   ();
    x.rdlock ();
    try {
      d_value += x.d_value;
      unlock ();
      x.unlock ();
      return *this;
    } catch (...) {
      unlock ();
      x.unlock ();
      throw;
    }
  }

  // subtract a real to this one

  Real& Real::operator -= (const Real& x) {
    wrlock   ();
    x.rdlock ();
    try {
      d_value -= x.d_value;
      unlock ();
      x.unlock ();
      return *this;
    } catch (...) {
      unlock ();
      x.unlock ();
      throw;
    }
  }

  // multiply a real with this one

  Real& Real::operator *= (const Real& x) {
    wrlock   ();
    x.rdlock ();
    try {
      d_value *= x.d_value;
      unlock ();
      x.unlock   ();
      return *this;
    } catch (...) {
      unlock ();
      x.unlock ();
      throw;
    }
  }

  // multiply a real with this one

  Real& Real::operator /= (const Real& x) {
    wrlock   ();
    x.rdlock ();
    try {
      d_value /= x.d_value;
      unlock ();
      x.unlock   ();
      return *this;
    } catch (...) {
      unlock ();
      x.unlock ();
      throw;
    }
  }

  // prefix add one to the real

  Real& Real::operator ++ (void) {
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

  // postfix add one to the real

  Real Real::operator ++ (int) {
    wrlock ();
    try {
      Real result = *this;
      d_value++;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // prefix sub one to the real

  Real& Real::operator -- (void) {
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

  // postfix sub one to the real

  Real Real::operator -- (int) {
    wrlock ();
    try {
      Real result = *this;
      d_value--;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // compare an real with a native value
  
  bool Real::operator == (const t_long value) const {
    rdlock ();
    try {
      bool result = (d_value == (t_real) value);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // compare an real with a native value
  
  bool Real::operator != (const t_long value) const {
    rdlock ();
    try {
      bool result = (d_value != (t_real) value);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // compare a real with a real value
  
  bool Real::operator == (const t_real value) const {
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
  
  // compare an real with a native value
  
  bool Real::operator != (const t_real value) const {
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

  // compare two reals

  bool Real::operator == (const Real& value) const {
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

  // compare two reals

  bool Real::operator != (const Real& value) const {
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

  // compare two reals

  bool Real::operator < (const Real& value) const {
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

  // compare two reals

  bool Real::operator <= (const Real& value) const {
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

  // compare two reals

  bool Real::operator > (const Real& value) const {
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

  // compare two reals

  bool Real::operator >= (const Real& value) const {
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


  // return true if the number is 0

  bool Real::iszero (void) const {
    rdlock ();
    try {
      bool result = (d_value == 0.0);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // compare with a real by absolute precision

  bool Real::cmp (const t_real value) const {
    rdlock ();
    try {
      bool result = Math::acmp (d_value, value);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return true if the number is nan

  bool Real::isnan (void) const {
    rdlock ();
    try {
      bool result = Math::isnan (d_value);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return true if the number is infinite

  bool Real::isinf (void) const {
    rdlock ();
    try {
      bool result = Math::isinf (d_value);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return the floor of this number

  Real Real::floor (void) const {
    rdlock ();
    try {
      Real result = Math::floor (d_value);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return the ceiling of this number

  Real Real::ceiling (void) const {
    rdlock ();
    try {
      Real result = Math::ceiling (d_value);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // return the round of this number

  Real Real::round (void) const {
    rdlock ();
    try {
      Real result = Math::round (d_value);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return the absolute value of this number
  
  Real Real::abs (void) const {
    rdlock ();
    try {
      Real result = Math::abs (d_value);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // return the remainder of this number with its argument
  
  Real Real::mod (const Real& x) const {
    rdlock   ();
    x.rdlock ();
    try {
      Real result = Math::mod (d_value, x.d_value);
      unlock   ();
      x.unlock ();
      return result;
    } catch (...) {
      unlock   ();
      x.unlock ();
      throw;
    }
  }
  
  // return the square root of this real
  
  Real Real::sqrt (void) const {
    rdlock ();
    try {
      Real result = Math::sqrt (d_value);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // return the natural logarithm of this real
  
  Real Real::ln (void) const {
    rdlock ();
    try {
      Real result = Math::ln (d_value);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // return the exponential of this real
  
  Real Real::exp (void) const {
    rdlock ();
    try {
      Real result = Math::exp (d_value);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // return the power of this number with the argument
  
  Real Real::pow (const Real& x) const {
    rdlock   ();
    x.rdlock ();
    try {
      Real result = Math::pow (d_value, x.d_value);
      unlock   ();
      x.unlock ();
      return result;
    } catch (...) {
      unlock ();
      x.unlock ();
      throw;
    }
  }
  
  // return the sine of this number
  
  Real Real::sin (void) const {
    rdlock ();
    try {
      Real result = Math::sin (d_value);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // return the cosine of this number
  
  Real Real::cos (void) const {
    rdlock ();
    try {
      Real result = Math::cos (d_value);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // return the tangent of this number
  
  Real Real::tan (void) const {
    rdlock ();
    try {
      Real result = Math::tan (d_value);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // return the arc sine of this number
  
  Real Real::asin (void) const {
    rdlock ();
    try {
      Real result = Math::asin (d_value);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // return the arc cosine of this number
  
  Real Real::acos (void) const {
    rdlock ();
    try {
      Real result = Math::acos (d_value);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // return the arc tangent of this number
  
  Real Real::atan (void) const {
    rdlock ();
    try {
      Real result = Math::atan (d_value);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // return the hyperbolic sine of this number
  
  Real Real::sinh (void) const {
    rdlock ();
    try {
      Real result = Math::sinh (d_value);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // return the hyperbolic cosine of this number
  
  Real Real::cosh (void) const {
    rdlock ();
    try {
      Real result = Math::cosh (d_value);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return the hyperbolic tangent of this number

  Real Real::tanh (void) const {
    rdlock ();
    try {
      Real result = Math::tanh (d_value);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // return the hyperbolic arc sine of this number

  Real Real::asinh (void) const {
    rdlock ();
    try {
      Real result = Math::asinh (d_value);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // return the hyperbolic arc cosine of this number
  
  Real Real::acosh (void) const {
    rdlock ();
    try {
      Real result = Math::acosh (d_value);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return the hyperbolic arc tangent of this number
  
  Real Real::atanh (void) const {
    rdlock ();
    try {
      Real result = Math::atanh (d_value);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return the real representation

  t_real Real::toreal (void) const {
    rdlock ();
    try {
      t_real result = d_value;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get an integer from this real
  
  t_long Real::tolong (void) const {
    rdlock ();
    try {
      t_long result = (t_long) d_value;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the viewable size
  
  long Real::tosize (void) const {
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
  
  t_byte* Real::tobyte (void) {
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
  
  const t_byte* Real::tobyte (void) const {
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
  static const long QUARK_ZONE_LENGTH = 41;
  static QuarkZone  zone (QUARK_ZONE_LENGTH);

  // the object supported quarks
  static const long QUARK_OPP      = zone.intern ("++");
  static const long QUARK_OMM      = zone.intern ("--");
  static const long QUARK_ADD      = zone.intern ("+");
  static const long QUARK_SUB      = zone.intern ("-");
  static const long QUARK_MUL      = zone.intern ("*");
  static const long QUARK_DIV      = zone.intern ("/");
  static const long QUARK_EQL      = zone.intern ("==");
  static const long QUARK_NEQ      = zone.intern ("!=");
  static const long QUARK_LTH      = zone.intern ("<");
  static const long QUARK_LEQ      = zone.intern ("<=");
  static const long QUARK_GTH      = zone.intern (">");
  static const long QUARK_GEQ      = zone.intern (">=");
  static const long QUARK_AEQ      = zone.intern ("+=");
  static const long QUARK_SEQ      = zone.intern ("-=");
  static const long QUARK_MEQ      = zone.intern ("*=");
  static const long QUARK_DEQ      = zone.intern ("/=");
  static const long QUARK_QEQ      = zone.intern ("?=");
  static const long QUARK_ABS      = zone.intern ("abs");
  static const long QUARK_MOD      = zone.intern ("mod");  
  static const long QUARK_POW      = zone.intern ("pow");  
  static const long QUARK_LN       = zone.intern ("ln");
  static const long QUARK_EXP      = zone.intern ("exp");
  static const long QUARK_SIN      = zone.intern ("sin");
  static const long QUARK_COS      = zone.intern ("cos");
  static const long QUARK_TAN      = zone.intern ("tan");
  static const long QUARK_SQRT     = zone.intern ("sqrt");
  static const long QUARK_NANP     = zone.intern ("nan-p");
  static const long QUARK_INFP     = zone.intern ("infinity-p");
  static const long QUARK_ASIN     = zone.intern ("asin");
  static const long QUARK_ACOS     = zone.intern ("acos");
  static const long QUARK_ATAN     = zone.intern ("atan");
  static const long QUARK_SINH     = zone.intern ("sinh");
  static const long QUARK_COSH     = zone.intern ("cosh");
  static const long QUARK_TANH     = zone.intern ("tanh");
  static const long QUARK_ZEROP    = zone.intern ("zero-p");
  static const long QUARK_ASINH    = zone.intern ("asinh");
  static const long QUARK_ACOSH    = zone.intern ("acosh");
  static const long QUARK_ATANH    = zone.intern ("atanh");
  static const long QUARK_FLOOR    = zone.intern ("floor");
  static const long QUARK_ROUND    = zone.intern ("round");
  static const long QUARK_CEILING  = zone.intern ("ceiling");

  // evaluate an object to a real value

  t_real Real::evalto (Evaluable* zobj, Nameset* nset, Object* object) {
    Object* obj = (object == nullptr) ? nullptr : object->eval (zobj, nset);
    Real*   val = dynamic_cast <Real*> (obj);
    if (val == nullptr) {
      throw Exception ("type-error", "nil object to evaluate");
    }
    return val->toreal ();
  }

  // create a new real in a generic way

  Object* Real::mknew (Vector* argv) {
    if ((argv == nullptr) || (argv->length () == 0)) return new Real;
    if (argv->length () != 1) 
      throw Exception ("argument-error", 
		       "too many argument with real constructor");
    // try to map the real argument
    Object* obj = argv->get (0);
    if (obj == nullptr) return new Real;

    // try an integer object
    auto ival = dynamic_cast <Integer*> (obj);
    if (ival != nullptr) return new Real (ival->tolong ());
    // try a real object
    auto nval = dynamic_cast <Numeral*> (obj);
    if (nval != nullptr) return new Real (nval->toreal());
    // try a real object
    auto rval = dynamic_cast <Real*> (obj);
    if (rval != nullptr) return new Real (*rval);
    // try a character object
    Character* cval = dynamic_cast <Character*> (obj);
    if (cval != nullptr) return new Real (cval->toquad ());
    // try a string object
    String* sval = dynamic_cast <String*> (obj);
    if (sval != nullptr) return new Real (*sval);
    // illegal object
    throw Exception ("type-error", "illegal object with real constructor",
		     obj->repr ());
  }

  // return true if the given quark is defined

  bool Real::isquark (const long quark, const bool hflg) const {
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

  // operate this real with another object

  Object* Real::oper (t_oper type, Object* object) {
    auto iobj = dynamic_cast <Integer*> (object);
    auto nobj = dynamic_cast <Numeral*> (object);
    auto dobj = dynamic_cast <Real*>    (object);
    switch (type) {
    case Object::OPER_ADD:
      if (iobj != nullptr) return new Real (*this + *iobj);
      if (nobj != nullptr) return new Real (*this + nobj->toreal());
      if (dobj != nullptr) return new Real (*this + *dobj);
      break;
    case Object::OPER_SUB:
      if (iobj != nullptr) return new Real (*this - *iobj);
      if (nobj != nullptr) return new Real (*this - nobj->toreal());
      if (dobj != nullptr) return new Real (*this - *dobj);
      break;
    case Object::OPER_MUL:
      if (iobj != nullptr) return new Real (*this * *iobj);
      if (nobj != nullptr) return new Real (*this * nobj->toreal());
      if (dobj != nullptr) return new Real (*this * *dobj);
      break;
    case Object::OPER_DIV:
      if (iobj != nullptr) return new Real (*this / *iobj);
      if (nobj != nullptr) return new Real (*this / nobj->toreal());
      if (dobj != nullptr) return new Real (*this / *dobj);
      break;
    case Object::OPER_UMN:
      return new Real (-(*this));
      break;
    case Object::OPER_EQL:
      if (iobj != nullptr) return new Boolean (*this == *iobj);
      if (nobj != nullptr) return new Boolean (*this == nobj->toreal());
      if (dobj != nullptr) return new Boolean (*this == *dobj);
      break;
    case Object::OPER_QEQ:
      if (iobj != nullptr) return new Boolean (cmp(iobj->tolong()));
      if (nobj != nullptr) return new Boolean (cmp(nobj->toreal()));
      if (dobj != nullptr) return new Boolean (cmp(dobj->toreal()));
      break;
    case Object::OPER_NEQ:
      if (iobj != nullptr) return new Boolean (*this != *iobj);
      if (nobj != nullptr) return new Boolean (*this != nobj->toreal());
      if (dobj != nullptr) return new Boolean (*this != *dobj);
      break;
    case Object::OPER_GEQ:
      if (iobj != nullptr) return new Boolean (*this >= *iobj);
      if (nobj != nullptr) return new Boolean (*this >= nobj->toreal());
      if (dobj != nullptr) return new Boolean (*this >= *dobj);
      break;
    case Object::OPER_GTH:
      if (iobj != nullptr) return new Boolean (*this > *iobj);
      if (nobj != nullptr) return new Boolean (*this > nobj->toreal());
      if (dobj != nullptr) return new Boolean (*this > *dobj);
      break;
    case Object::OPER_LEQ:
      if (iobj != nullptr) return new Boolean (*this <= *iobj);
      if (nobj != nullptr) return new Boolean (*this <= nobj->toreal());
      if (dobj != nullptr) return new Boolean (*this <= *dobj);
      break;
    case Object::OPER_LTH:
      if (iobj != nullptr) return new Boolean (*this < *iobj);
      if (nobj != nullptr) return new Boolean (*this < nobj->toreal());
      if (dobj != nullptr) return new Boolean (*this < *dobj);
      break;
    }
    throw Exception ("type-error", "invalid operand with real",
		     Object::repr (object));
  }

  // set an object to this real

  Object* Real::vdef (Evaluable* zobj, Nameset* nset, Object* object) {
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
	*this = nobj->toreal();
	zobj->post (this);
	unlock ();
	return this;
      }
      auto dobj = dynamic_cast <Real*> (object);
      if (dobj != nullptr) {
	*this = *dobj;
	zobj->post (this);
	unlock ();
	return this;
      }
      throw Exception ("type-error", "invalid object with real vdef",
		       Object::repr (object));
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // apply this object with a set of arguments and a quark

  Object* Real::apply (Evaluable* zobj, Nameset* nset, const long quark,
		       Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();

    // dispatch 0 argument
    if (argc == 0) {
      if (quark == QUARK_LN)       return new Real    (ln      ());
      if (quark == QUARK_ABS)      return new Real    (abs     ());
      if (quark == QUARK_EXP)      return new Real    (exp     ());
      if (quark == QUARK_SIN)      return new Real    (sin     ());
      if (quark == QUARK_COS)      return new Real    (cos     ());
      if (quark == QUARK_TAN)      return new Real    (tan     ());
      if (quark == QUARK_SINH)     return new Real    (sinh    ());
      if (quark == QUARK_COSH)     return new Real    (cosh    ());
      if (quark == QUARK_TANH)     return new Real    (tanh    ());
      if (quark == QUARK_ASIN)     return new Real    (asin    ());
      if (quark == QUARK_ACOS)     return new Real    (acos    ());
      if (quark == QUARK_ATAN)     return new Real    (atan    ());
      if (quark == QUARK_SQRT)     return new Real    (sqrt    ());
      if (quark == QUARK_NANP)     return new Boolean (isnan   ());
      if (quark == QUARK_INFP)     return new Boolean (isinf   ());
      if (quark == QUARK_ASINH)    return new Real    (asinh   ());
      if (quark == QUARK_ACOSH)    return new Real    (acosh   ());
      if (quark == QUARK_ATANH)    return new Real    (atanh   ());
      if (quark == QUARK_FLOOR)    return new Real    (floor   ());
      if (quark == QUARK_ROUND)    return new Real    (round   ());
      if (quark == QUARK_CEILING)  return new Real    (ceiling ());
      if (quark == QUARK_ZEROP)    return new Boolean (iszero  ());

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

      if (quark == QUARK_MOD) {
	t_real val = argv->getrint (0);
	return new Real (mod (val));
      }
      if (quark == QUARK_AEQ) {
	wrlock ();
	try {
	  t_real val = argv->getrint (0);
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
	  t_real val = argv->getrint (0);
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
	  t_real val = argv->getrint (0);
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
	  t_real val = argv->getrint (0);
	  *this /= val;
	  zobj->post (this);
	  unlock ();
	  return this;
	} catch (...) {
	  unlock ();
	  throw;
	}
      }
      if (quark == QUARK_QEQ) {
	t_real val = argv->getreal (0);
	return new Boolean (cmp (val));
      }
      if (quark == QUARK_POW) {
	t_real val = argv->getrint (0);
	return new Real (pow (val));
      }
    }
    // call the number method
    return Number::apply (zobj, nset, quark, argv);
  }
}
