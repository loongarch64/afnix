// ---------------------------------------------------------------------------
// - Symbol.cpp                                                              -
// - afnix engine - symbol class implementation                              -
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

#include "Vector.hpp"
#include "Symbol.hpp"
#include "Future.hpp"
#include "Lexical.hpp"
#include "Boolean.hpp"
#include "Promise.hpp"
#include "Evaluable.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a new symbol by name

  Symbol::Symbol (const String& name) {
    if (Lexical::valid (name) == false)
      throw Exception ("name-error", "invalid symbol name", name);
    d_quark  = name.toquark ();
    p_object = nullptr;
    d_const  = false;
  }

  // create a new symbol by quark

  Symbol::Symbol (const long quark) {
    d_quark  = quark;
    p_object = nullptr;
    d_const  = false;
  }

  // create a symbol by name with an object

  Symbol::Symbol (const String& name, Object* object) {
    if (Lexical::valid (name) == false) {
      throw Exception ("name-error", "invalid symbol name", name);
    }
    d_quark  = name.toquark ();
    p_object = Object::iref (object);
    d_const  = false;
  }

  // create a symbol by quark with an object

  Symbol::Symbol (const long quark, Object* object) {
    d_quark  = quark;
    p_object = Object::iref (object);
    d_const  = false;
  }

  // create a symbol by name and constant flag with an object

  Symbol::Symbol (const String& name, const bool cflag, Object* object) {
    if (Lexical::valid (name) == false) {
      throw Exception ("name-error", "invalid symbol name", name);
    }
    d_quark  = name.toquark ();
    p_object = Object::iref (object);
    d_const  = cflag;
  }

  // create a symbol by quark and constant flag with an object

  Symbol::Symbol (const long quark, const bool cflag, Object* object) {
    d_quark  = quark;
    p_object = Object::iref (object);
    d_const  = cflag;
  }

  // copy constructor for this symbol

  Symbol::Symbol (const Symbol& that) {
    that.rdlock ();
    try {
      d_quark  = that.d_quark;
      p_object = Object::iref (that.p_object);
      d_const  = that.d_const;
      that.unlock ();
    } catch (...) {
      that.unlock ();
      throw;
    }
  }

  // destroy this symbol

  Symbol::~Symbol (void) {
    Object::dref (p_object);
  }

  // return the class name

  String Symbol::repr (void) const {
    return "Symbol";
  }

  // return a clone of this object

  Object* Symbol::clone (void) const {
    return new Symbol (*this);
  }

  // clear this symbol

  void Symbol::clear (void) {
    wrlock ();
    try {
      d_quark = 0L;
      d_const = false;
      Object::dref (p_object); p_object = nullptr;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get a literal representation of this symbol

  String Symbol::toliteral (void) const {
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

  // get a string representation of this symbol

  String Symbol::tostring (void) const {
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

  // get the symbol quark

  long Symbol::toquark (void) const {
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

  // set the const flag for this symbol

  void Symbol::setconst (const bool flag) {
    wrlock ();
    try {
      d_const = flag;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the const flag for this symbol

  bool Symbol::getconst (void) const {
    rdlock ();
    try {
      bool result = d_const;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set the object for this symbol

  void Symbol::setobj (Object* object) {
    wrlock ();
    try {
      // check for the const flag
      if (d_const == true) {
	const String& name = String::qmap (d_quark);
	throw Exception ("const-error", "const violation for symbol", name);
      }
      Object::iref (object);
      Object::dref (p_object);
      p_object = object;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the symbol object

  Object* Symbol::getobj (void) const {
    rdlock ();
    try {
      Object* result = p_object;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // -------------------------------------------------------------------------
  // - object section                                                         -
  // -------------------------------------------------------------------------

  // the quark zone
  static const long QUARK_ZONE_LENGTH = 4;
  static QuarkZone  zone (QUARK_ZONE_LENGTH);

  // the symbol supported quarks
  static const long QUARK_GETCONST  = zone.intern ("get-const");
  static const long QUARK_SETCONST  = zone.intern ("set-const");
  static const long QUARK_GETOBJECT = zone.intern ("get-object");
  static const long QUARK_SETOBJECT = zone.intern ("set-object");

  // create a new object in a generic way

  Object* Symbol::mknew (Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();

    // check for 1 argument
    if (argc == 1) {
      String name = argv->getstring (0);
      return new Symbol (name);
    }
    // check for 2 arguments
    if (argc == 2) {
      String name = argv->getstring (0);
      return new Symbol (name, argv->get (1));
    }
    // illegal arguments
    throw Exception ("argument-error", "too many arguments with symbol");
  }

  // return true if the given quark is defined

  bool Symbol::isquark (const long quark, const bool hflg) const {
    rdlock ();
    if (zone.exists(quark) == true) {
      unlock ();
      return true;
    }
    bool result = hflg ? Object::isquark (quark, hflg) : false;
    unlock ();
    return result;
  }

  // set a constant object to this object

  Object* Symbol::cdef (Evaluable* zobj, Nameset* nset, Object* object) {
    // get the write lock
    wrlock ();
    // define constant
    try {
      setobj (object);
      d_const = true;
      zobj->post (object);
      unlock ();
      return object;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set an object to this object

  Object* Symbol::vdef (Evaluable* zobj, Nameset* nset, Object* object) {
    // get the write lock
    wrlock ();
    try {
      setobj (object);
      zobj->post (object);
      unlock ();
      return object;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // unreference a symbol object

  Object* Symbol::udef (Evaluable* zobj, Nameset* nset) {
    // get the write lock
    wrlock ();
    // unreference the object
    try {
      Object::dref (p_object);
      p_object = nullptr;
      d_const  = false;
      unlock ();
      return nullptr;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // evaluate this object

  Object* Symbol::eval (Evaluable* zobj, Nameset* nset) {
    // get the read lock
    rdlock ();
    try {
      Object* result = p_object;
      if (dynamic_cast <Promise*> (p_object) != nullptr) {
	result = p_object->eval (zobj, nset);
      }
      if (dynamic_cast <Future*> (p_object) != nullptr) {
	result = p_object->eval (zobj, nset);
      }
      if (zobj != nullptr) zobj->post (result);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // apply this object with a set of arguments and a quark

  Object* Symbol::apply (Evaluable* zobj, Nameset* nset, const long quark,
			 Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();

    // dispatch 0 argument
    if (argc == 0) {
      if (quark == QUARK_GETCONST)  return new Boolean (getconst ());
      if (quark == QUARK_GETOBJECT) {
	rdlock ();
	Object* result = getobj ();
	zobj->post (result);
	unlock ();
	return result;
      }
    }
    // dispatch 1 argument
    if (argc == 1) {
      if (quark == QUARK_SETCONST) {
	bool bval = argv->getbool (0);
	setconst (bval);
	return nullptr;
      }
      if (quark == QUARK_SETOBJECT) {
	setobj (argv->get (0));
	return nullptr;
      }
    }
    // call the literal method
    return Literal::apply (zobj, nset, quark, argv);
  }
}
