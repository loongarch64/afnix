// ---------------------------------------------------------------------------
// - Character.cpp                                                           -
// - standard object library - unicode character class implementation        -
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
#include "Unicode.hpp"
#include "Character.hpp"
#include "Integer.hpp"
#include "Boolean.hpp"
#include "Evaluable.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"
#include "InputStream.hpp"
#include "OutputStream.hpp"
#include "ccnv.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a new default character

  Character::Character (void) {
    d_value = nilq;
  }

  // create a new character from a native character

  Character::Character (const char value) {
    d_value = Unicode::toquad (value);
  }

  // create a new character from a native character

  Character::Character (const t_quad value) {
    d_value = value;
  }
  
  // create a new character from a string representation

  Character::Character (const String& s) {
    d_value = Unicode::toquad (s);
  }

  // copy constructor for this character class

  Character::Character (const Character& that) {
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

  String Character::repr (void) const {
    return "Character";
  }

  // return a clone of this object

  Object* Character::clone (void) const {
    return new Character (*this);
  }

  // clear this character

  void Character::clear (void) {
    wrlock ();
    try {
      d_value = nilq;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return a literal representation of this character

  String Character::toliteral (void) const {
    rdlock ();
    try {
      String result = Unicode::toliteral (d_value);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return a string representation of this character

  String Character::tostring (void) const {
    rdlock ();
    try {
      String result = d_value;
      unlock ();
      return result;    
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return the character did

  t_word Character::getdid (void) const {
    return SRL_DEOD_STD;
  }

  // return the character sid

  t_word Character::getsid (void) const {
    return SRL_CHAR_SID;
  }

  // serialize this character

  void Character::wrstream (OutputStream& os) const {
    rdlock ();
    try {
      t_byte data[4];
      c_qhton (d_value, data);
      os.write ((char*) data, 4);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // deserialize this character

  void Character::rdstream (InputStream& is) {
    wrlock ();
    try {
      t_byte data[4];
      for (long i = 0; i < 4; i++) data[i] = (t_byte) is.read ();
      d_value = c_qntoh (data);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return a native character

  char Character::tochar (void) const {
    rdlock ();
    try {
      char result = Unicode::tochar (d_value);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return a native quad

  t_quad Character::toquad (void) const {
    rdlock ();
    try {
      t_quad result = d_value;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set an character with a native value

  Character& Character::operator = (const char value) {
    wrlock ();
    try {
      d_value = Unicode::toquad (value);
      unlock ();
      return *this;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set an character with a native value

  Character& Character::operator = (const t_quad value) {
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

  // assign a character to this one

  Character& Character::operator = (const Character& that) {
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
  
  // add a character with a native value

  Character Character::operator + (const long value)  const {
    wrlock ();
    try {
      Character result = (t_quad) (d_value + value);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // sub a character with a native value

  Character Character::operator - (const long value) const {
    wrlock ();
    try {
      Character result = (t_quad) (d_value - value);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // add a native value to this character

  Character& Character::operator += (const long value) {
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

  // sub a native value to this character

  Character& Character::operator -= (const long value) {
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

  // prefix add one to the character

  Character& Character::operator ++ (void) {
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

  // postfix add one to the character

  Character Character::operator ++ (int) {
    wrlock ();
    try {
      Character result = *this;
      d_value++;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // prefix sub one to the character

  Character& Character::operator -- (void) {
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

  // postfix sub one to the character

  Character Character::operator -- (int) {
    wrlock ();
    try {
      Character result = *this;
      d_value--;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // compare an character with a native value
  
  bool Character::operator == (const char value) const {
    rdlock ();
    try {
      bool result = (d_value == Unicode::toquad (value));
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  bool Character::operator != (const char value) const {
    rdlock ();
    try {
      bool result = (d_value != Unicode::toquad (value));
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // compare an character with a native value
  
  bool Character::operator == (const t_quad value) const {
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
  
  bool Character::operator != (const t_quad value) const {
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
  
  // compare two character characters
  
  bool Character::operator == (const Character& value) const {
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
  
  bool Character::operator != (const Character& value) const {
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
  
  bool Character::operator < (const Character& value) const {
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
  
  bool Character::operator <= (const Character& value) const {
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
  
  bool Character::operator > (const Character& value) const {
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
  
  bool Character::operator >= (const Character& value) const {
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
  
  // return true if the character is a letter

  bool Character::isletter (void) const {
    rdlock ();
    try {
      bool result = Unicode::isletter (d_value);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return true if the character is a digit

  bool Character::isdigit (void) const {
    rdlock ();
    try {
      bool result = Unicode::isdigit (d_value);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return true if the character is alphanumeric

  bool Character::isalpha (void) const {
    rdlock ();
    try {
      bool result = Unicode::isalpha (d_value);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return true if the character is a blank or tab
  
  bool Character::isblank (void) const {
    rdlock ();
    try {
      bool result = Unicode::isblank (d_value);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return true if the character is an ascii character
  
  bool Character::isascii (void) const {
    rdlock ();
    try {
      bool result = Unicode::isascii (d_value);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return true if the character is a latin character
  
  bool Character::islatin (void) const {
    rdlock ();
    try {
      bool result = Unicode::islatin (d_value);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return true if the character is an eol

  bool Character::iseol (void) const {
    rdlock ();
    try {
      bool result = (d_value == eolq);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // return true if the character is an eos

  bool Character::iseos (void) const {
    rdlock ();
    try {
      bool result = (d_value == eosq);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return true if the character is nil

  bool Character::isnil (void) const {
    rdlock ();
    try {
      bool result = (d_value == nilq);
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
  static const long QUARK_ZONE_LENGTH = 20;
  static QuarkZone  zone (QUARK_ZONE_LENGTH);

  // the object supported quarks
  static const long QUARK_OPP     = zone.intern ("++");
  static const long QUARK_OMM     = zone.intern ("--");
  static const long QUARK_ADD     = zone.intern ("+");
  static const long QUARK_SUB     = zone.intern ("-");
  static const long QUARK_EQL     = zone.intern ("==");
  static const long QUARK_NEQ     = zone.intern ("!=");
  static const long QUARK_LTH     = zone.intern ("<");
  static const long QUARK_LEQ     = zone.intern ("<=");
  static const long QUARK_GTH     = zone.intern (">");
  static const long QUARK_GEQ     = zone.intern (">=");
  static const long QUARK_AEQ     = zone.intern ("+=");
  static const long QUARK_SEQ     = zone.intern ("-=");
  static const long QUARK_EOLP    = zone.intern ("eol-p");
  static const long QUARK_EOSP    = zone.intern ("eos-p");
  static const long QUARK_NILP    = zone.intern ("nil-p");
  static const long QUARK_TOINT   = zone.intern ("to-integer");
  static const long QUARK_LETERP  = zone.intern ("letter-p");
  static const long QUARK_DIGITP  = zone.intern ("digit-p");
  static const long QUARK_ALPHAP  = zone.intern ("alpha-p");
  static const long QUARK_BLANKP  = zone.intern ("blank-p");

  // evaluate an object to a native value

  t_quad Character::evalto (Evaluable* zobj, Nameset* nset, Object* object) {
    Object*    obj = (object == nullptr) ? nullptr : object->eval (zobj, nset);
    Character* val = dynamic_cast <Character*> (obj);
    if (val == nullptr) throw Exception ("type-error", "nil object to evaluate");
    return val->toquad ();
  }

  // create a new object in a generic way

  Object* Character::mknew (Vector* argv) {
    if ((argv == nullptr) || (argv->length () == 0)) return new Character;
    if (argv->length () != 1) 
      throw Exception ("argument-error", 
		       "too many argument with character constructor");
    // try to map the character argument
    Object* obj = argv->get (0);
    if (obj == nullptr) return new Character;

    // try an integer object
    Integer* ival = dynamic_cast <Integer*> (obj);
    if (ival != nullptr) return new Character ((t_quad) ival->tolong ());

    // try a unicode character object
    Character* cval = dynamic_cast <Character*> (obj);
    if (cval != nullptr) return new Character (*cval);

    // try a string object
    String* sval = dynamic_cast <String*> (obj);
    if (sval != nullptr) return new Character (*sval);

    // illegal object
    throw Exception ("type-error", "illegal object with character constructor",
		     obj->repr ());
  }

  // return true if the given quark is defined

  bool Character::isquark (const long quark, const bool hflg) const {
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

  Object* Character::oper (t_oper type, Object* object) {
    Integer*   iobj = dynamic_cast <Integer*>   (object);
    Character* cobj = dynamic_cast <Character*> (object);
    switch (type) {
    case Object::OPER_ADD:
      if (iobj != nullptr) return new Character (*this + iobj->tolong ());
      break;
    case Object::OPER_SUB:
      if (iobj != nullptr) return new Character (*this - iobj->tolong ());
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
      throw Exception ("operator-error", "unsupported unicode operator");
    }
    throw Exception ("type-error", "invalid operand with unicode character",
		     Object::repr (object));
  }

  // set an object to this character

  Object* Character::vdef (Evaluable* zobj, Nameset* nset, Object* object) {
    wrlock ();
    try {
      Character* cobj = dynamic_cast <Character*> (object);
      if (cobj != nullptr) {
	*this = *cobj;
	zobj->post (this);
	unlock ();
	return this;
      }
      throw Exception ("type-error", "invalid object with character vdef",
		       Object::repr (object));
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // apply this object with a set of arguments and a quark

  Object* Character::apply (Evaluable* zobj, Nameset* nset, const long quark,
			    Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();

    // dispatch 0 argument
    if (argc == 0) {
      if (quark == QUARK_TOINT)  return new Integer (toquad ());
      
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
      if (quark == QUARK_LETERP) return new Boolean (isletter ());
      if (quark == QUARK_DIGITP) return new Boolean (isdigit  ());
      if (quark == QUARK_ALPHAP) return new Boolean (isalpha  ());
      if (quark == QUARK_BLANKP) return new Boolean (isblank  ());
      if (quark == QUARK_EOLP)   return new Boolean (iseol ());
      if (quark == QUARK_EOSP)   return new Boolean (iseos ());
      if (quark == QUARK_NILP)   return new Boolean (isnil ());
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
    // call the literal method
    return Literal::apply (zobj, nset, quark, argv);
  }
}
