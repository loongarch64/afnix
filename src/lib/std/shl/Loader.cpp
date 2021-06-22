// ---------------------------------------------------------------------------
// - Loader.cpp                                                              -
// - standard object library - library loader class implementation           -
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

#include "Loader.hpp"
#include "Boolean.hpp"
#include "Integer.hpp"
#include "Evaluable.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a default loader
  
  Loader::Loader (void) {
    d_llib.reset ();
  }

  // return the class name

  String Loader::repr (void) const {
    return "Loader";
  }
  
  // add a new library in the list by name

  void Loader::add (const String& name) {
    wrlock ();
    if (exists (name) == true) {
      unlock ();
      return;
    }
    try {
      d_llib.add (new Library (name));
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // add a new registered library in the list

  void Loader::add (const String& name, void* hand) {
    wrlock ();
    if (exists (name) == true) {
      unlock ();
      return;
    }
    try {
      d_llib.add (new Library (name, hand));
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // add a new library in the list by object

  void Loader::add (Library* lib) {
    if (lib == nullptr) return;
    wrlock ();
    String name = lib->getname ();
    if (exists (name) == true) {
      unlock ();
      return;
    }
    try {
      d_llib.add (lib);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return the number of loaded libraries

  long Loader::length (void) const {
    rdlock ();
    try {
      long result = d_llib.length ();
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return true if a library exists

  bool Loader::exists (const String& name) const {
    rdlock ();
    try {
      long size = d_llib.length ();
      for (long i = 0; i < size; i++) {
	// get the library
	Object*  obj = d_llib.get (i);
	Library* lib = dynamic_cast <Library*> (obj);
	if (lib == nullptr) continue;
	// check by name
	if (lib->getname () == name) {
	  unlock ();
	  return true;
	}
      }
      unlock ();
      return false;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return a library by index

  Library* Loader::get (const long index) const {
    rdlock ();
    try {
      // get the library
      Object*  obj = d_llib.get (index);
      Library* lib = dynamic_cast <Library*> (obj);
      if (lib == nullptr) {
	unlock ();
	throw Exception ("loader-error", "cannot find library by index");
      }
      unlock ();
      return lib;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // find a library by name or throw an exception

  Library* Loader::lookup (const String& name) const {
    rdlock ();
    try {
      long size = d_llib.length ();
      for (long i = 0; i < size; i++) {
	// get the library
	Object*  obj = d_llib.get (i);
	Library* lib = dynamic_cast <Library*> (obj);
	if (lib == nullptr) continue;
	// check by name
	if (lib->getname () == name) {
	  unlock ();
	  return lib;
	}
      }
      // library not found
      throw Exception ("library-error", "cannot find library", name);
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // -------------------------------------------------------------------------
  // - object section                                                        -
  // -------------------------------------------------------------------------

  // the quark zone
  static const long QUARK_ZONE_LENGTH = 5;
  static QuarkZone  zone (QUARK_ZONE_LENGTH);

  // the object supported quarks
  static const long QUARK_ADD    = zone.intern ("add");
  static const long QUARK_GET    = zone.intern ("get");
  static const long QUARK_EXISTS = zone.intern ("exists-p");
  static const long QUARK_LOOKUP = zone.intern ("lookup");
  static const long QUARK_LENGTH = zone.intern ("length");

  // return true if the given quark is defined

  bool Loader::isquark (const long quark, const bool hflg) const {
    rdlock ();
    if (zone.exists (quark) == true) {
      unlock ();
      return true;
    }
    bool result = hflg ? Object::isquark (quark, hflg) : false;
    unlock ();
    return result;
  }
  
  // apply an object method with a set of arguments and a quark

  Object* Loader::apply (Evaluable* zobj, Nameset* nset, const long quark,
			 Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();
    
    // dispatch 0 argument
    if (argc == 0) {
      if (quark == QUARK_LENGTH) return new Integer (length ());
    }
    
    // dispatch 1 argument
    if (argc == 1) {
      if (quark == QUARK_EXISTS) {
	String name = argv->getstring (0);
	return new Boolean (exists (name));
      }
      if (quark == QUARK_LOOKUP) {
	rdlock ();
	try {
	  String name = argv->getstring (0);
	  Library* result = lookup (name);
	  zobj->post (result);
	  unlock ();
	  return result;
	} catch (...) {
	  unlock ();
	  throw;
	}
      }
      if (quark == QUARK_GET) {
	rdlock ();
	try {
	  long index = argv->getlong (0);
	  Library* result = get (index);
	  zobj->post (result);
	  unlock ();
	  return result;
	} catch (...) {
	  unlock ();
	  throw;
	}
      }
      if (quark == QUARK_ADD) {
	// get the object and check
	Object* obj = argv->get (0);
	// check for a string
	String* sobj = dynamic_cast<String*> (obj);
	if (sobj!= nullptr) {
	  add (*sobj);
	  return nullptr;
	}
	// check for a library
	Library* lobj = dynamic_cast<Library*> (obj);
	if (lobj!= nullptr) {
	  add (lobj);
	  return nullptr;
	}
	// invalid argument
	throw Exception ("argument-error", "invalid argument with add",
			 Object::repr (obj));
      }
    }
    // call the object method
    return Object::apply (zobj, nset, quark, argv);
  }
}
