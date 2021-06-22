// ---------------------------------------------------------------------------
// - Enum.cpp                                                                -
// - afnix engine - enumeration class implementation                         -
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

#include "Cons.hpp"
#include "Item.hpp"
#include "Enum.hpp"
#include "Vector.hpp"
#include "Lexical.hpp"
#include "Integer.hpp"
#include "Boolean.hpp"
#include "Builtin.hpp"
#include "Evaluable.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a new enumeration object

  Enum::Enum (void) {
    d_enum.reset ();
  }

  // return the class name

  String Enum::repr (void) const {
    return "Enum";
  }

  // reset this enumeration

  void Enum::reset (void) {
    wrlock ();
    d_enum.reset ();
    unlock ();
  }

  // return the number of items

  long Enum::length (void) const {
    rdlock ();
    try {
      long result = d_enum.length ();
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return true if an item exists by quark

  bool Enum::exists (const long quark) const {
    rdlock ();
    try {
      bool result = d_enum.exists (quark);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return true if an item exists by string

  bool Enum::exists (const String& name) const {
   // check for validity
    if (Lexical::valid (name) == false) {
      throw Exception ("name-error", "invalid item name", name);
    }
    // check if the item exists
    return exists (name.toquark ());
  }

  // return  an item value by index

  String Enum::get (const long index) const {
    rdlock ();
    try {
      long quark = d_enum.get (index);
      String result = String::qmap (quark);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw Exception ("index-error", "cannot access item by index");
    }
  }

  // add a new item by string

  void Enum::add (const String& name) {
    // check for validity
    if (Lexical::valid (name) == false)
      throw Exception ("name-error", "invalid enumeration name", name);
    // map to quark and add
    add (name.toquark ());
  }

  // add a new item by quark
  
  void Enum::add (const long quark) {
    wrlock ();
    try {
      if (d_enum.exists (quark) == false) {
	d_enum.add (quark);
      }
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // -------------------------------------------------------------------------
  // - object section                                                        -
  // -------------------------------------------------------------------------

  // the quark zone
  static const long QUARK_ZONE_LENGTH = 4;
  static QuarkZone  zone (QUARK_ZONE_LENGTH);

  // the object supported quarks
  static const long QUARK_ADD    = zone.intern ("add");
  static const long QUARK_GET    = zone.intern ("get");
  static const long QUARK_RESET  = zone.intern ("reset");
  static const long QUARK_LENGTH = zone.intern ("length");
  static const long QUARK_EXISTS = zone.intern ("exists-p");

  // create a new object in a generic way

  Object* Enum::mknew (Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();
    Enum* result = new Enum;
    // add the enumeration string
    try {
      for (long i = 0; i < argc; i++) result->add (argv->getstring (i));
    } catch (...) {
      delete result;
      throw;
    }
    return result;
  }

  // return true if the given quark is defined

  bool Enum::isquark (const long quark, const bool hflg) const {
    rdlock ();
    if (zone.exists (quark) == true) {
      unlock ();
      return true;
    }
    bool result = hflg ? Object::isquark (quark, hflg) : false;
    unlock ();
    return result;
  }

  // evaluate an object member by quark

  Object* Enum::eval (Evaluable* zobj, Nameset* nset, const long quark) {
    rdlock ();
    try {
      if (d_enum.exists (quark) == true) {
	Object* result = new Item (this, quark);
	zobj->post (result);
	unlock ();
	return result;
      }
      unlock ();
      return Object::eval (zobj, nset, quark);
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // apply this object with a set of arguments and a quark

  Object* Enum::apply (Evaluable* zobj, Nameset* nset, const long quark,
		       Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();

    // dispatch 0 argument
    if (argc == 0) {
      if (quark == QUARK_LENGTH) return new Integer (length ());
      if (quark == QUARK_RESET) {
	reset ();
	return nullptr;
      }
    }
    // dispatch 1 argument
    if (argc == 1) {
      if (quark == QUARK_ADD) {
	String name = argv->getstring (0);
	add (name);
	return nullptr;
      }
      if (quark == QUARK_GET) {
	long index = argv->getlong (0);
	return new String (get (index));
      }
      if (quark == QUARK_EXISTS) {
	String name = argv->getstring (0);
	return new Boolean (exists (name));
      }
    }
    // call the object method
    return Object::apply (zobj, nset, quark, argv);
  }

  // create a new enumeration object - this is the builtin function

  Object* Builtin::sfenum (Evaluable* zobj, Nameset* nset, Cons* args) {
    Enum* result = new Enum;
    // loop into the cons cell
    while (args != nullptr) {
      Lexical* lex = dynamic_cast <Lexical*> (args->getcar ());
      if (lex == nullptr) {
	delete result;
	throw Exception ("argument-error", 
			 "only symbol can be used as argument");
      }
      result->add (lex->toquark ());
      args = args->getcdr ();
    }
    return result;
  }
}
