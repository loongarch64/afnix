// ---------------------------------------------------------------------------
// - Pathlist.cpp                                                            -
// - standard object library - path list class implementation                -
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

#include "Regex.hpp"
#include "Vector.hpp"
#include "System.hpp"
#include "Integer.hpp"
#include "Boolean.hpp"
#include "Pathlist.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a default path list

  Pathlist::Pathlist (void) {
    reset ();
  }

  // create a path list with a local search flag

  Pathlist::Pathlist (const bool lsch) {
    reset ();
    setlsch (lsch);
  }

  // create a path list with an initial path

  Pathlist::Pathlist (const String& path) {
    reset ();
    addpath (path);
  }

  // copy construct this path list

  Pathlist::Pathlist (const Pathlist& that) {
    that.rdlock ();
    try {
      d_lsch = that.d_lsch;
      d_plst = that.d_plst;
      that.unlock ();
    } catch (...) {
      that.unlock ();
      throw;
    }
  }

  //  assign a path list to this one

  Pathlist& Pathlist::operator = (const Pathlist& that) {
    wrlock ();
    that.rdlock ();
    try {
      d_lsch = that.d_lsch;
      d_plst = that.d_plst;
      that.unlock ();
      unlock ();
      return *this;
    } catch (...) {
      that.unlock ();
      unlock ();
      throw;
    }
  }

  // return the class name

  String Pathlist::repr (void) const {
    return "Pathlist";
  }

  // reset this path list

  void Pathlist::reset (void) {
    wrlock ();
    try {
      d_lsch = false;
      d_plst.reset ();
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return the local search flag

  bool Pathlist::getlsch (void) const {
    rdlock ();
    try {
      bool result = d_lsch;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
    
  // set the local search flag

  void Pathlist::setlsch (const bool lsch) {
    wrlock ();
    try {
      d_lsch = lsch;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return the path list length
    
  long Pathlist::length (void) const {
    rdlock ();
    try {
      long result = d_plst.length ();
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return a path component by index
    
  String Pathlist::getpath (const long index) const {
    rdlock ();
    try {
      String result = d_plst.get (index);
      unlock ();
      return result;
    } catch(...) {
      unlock ();
      throw;
    }
  }

  // add a path component by index

  void Pathlist::addpath (const String& path) {
    wrlock ();
    try {
      d_plst.add (path);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // check if a file name can be resolved

  bool Pathlist::isfile (const String& name) const {
    rdlock ();
    try {
      // check for local first
      if ((d_lsch == true) && (System::isfile (name) == true)) {
	unlock ();
	return true;
      }
      // initialise result
      bool result = false;
      // check in the path list
      long plen = d_plst.length ();
      for (long i = 0; i < plen; i++) {
	String path = d_plst.get (i);
	if (System::isfile (path, name) == true) {
	  result = true;
	  break;
	}
      }
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // resolve a name in the path list by name

  String Pathlist::resolve (const String& name) const {
    rdlock ();
    try {
      // check for local first
      if ((d_lsch == true) && (System::isfile (name) == true)) {
	unlock ();
	return name;
      }
      // check in the path list
      long plen = d_plst.length ();
      for (long i = 0; i < plen; i++) {
	// build the resul path
	String path = System::join (d_plst.get (i), name);
	if (System::isfile (path) == true) {
	  unlock ();
	  return path;
	}
      }
      throw Exception ("path-error", "cannot resolve path", name);
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // -------------------------------------------------------------------------
  // - object section                                                        -
  // -------------------------------------------------------------------------

  // the quark zone
  static const long QUARK_ZONE_LENGTH = 8;
  static QuarkZone  zone (QUARK_ZONE_LENGTH);

  // the object supported quarks
  static const long QUARK_RESET   = zone.intern ("reset");
  static const long QUARK_FILEP   = zone.intern ("file-p");
  static const long QUARK_LENGTH  = zone.intern ("length");
  static const long QUARK_LOCALP  = zone.intern ("local-p");
  static const long QUARK_ADDPATH = zone.intern ("add-path");
  static const long QUARK_GETPATH = zone.intern ("get-path");
  static const long QUARK_RESOLVE = zone.intern ("resolve");
  static const long QUARK_SETLSCH = zone.intern ("set-local-search");

  // create a new object in a generic way

  Object* Pathlist::mknew (Vector* argv) {
    long argc = (argv == nullptr) ? 0 : argv->length ();
    // check for 0 arguments
    if (argc == 0) return new Pathlist;
    // check for 1 argument
    if (argc == 1) {
      Object* obj = argv->get (0);
      // check for a boolean
      Boolean* bobj = dynamic_cast <Boolean*> (obj);
      if (bobj != nullptr) return new Pathlist (bobj->tobool ());
      // check for a string
      String* sobj = dynamic_cast <String*> (obj);
      if (sobj != nullptr) return new Pathlist (*sobj);
      // invalid object
      throw Exception ("type-error", 
		       "invalid object with path list constructor",
		       Object::repr (obj));
    }
    throw Exception ("argument-error",
		     "too many argument with pathlist constructor");
  }

  // return true if the given quark is defined

  bool Pathlist::isquark (const long quark, const bool hflg) const {
    rdlock ();
    try {
      if (zone.exists (quark) == true) {
	unlock ();
	return true;
      }
      bool result = hflg ? Object::isquark (quark, hflg) : false;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // apply this object with a set of arguments and a quark
  
  Object* Pathlist::apply (Evaluable* zobj, Nameset* nset, const long quark,
			   Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();
 
    // check for 0 argument
    if (argc == 0) {
      if (quark == QUARK_LOCALP)  return new Boolean (getlsch ());
      if (quark == QUARK_LENGTH)  return new Integer (length  ());
      if (quark == QUARK_RESET) {
	reset ();
	return nullptr;
      }
    }

    // check for 1 argument
    if (argc == 1) {
      if (quark == QUARK_SETLSCH) {
	bool lsch = argv->getbool (0);
	setlsch (lsch);
	return nullptr;
      }
      if (quark == QUARK_ADDPATH) {
	String name = argv->getstring (0);
	addpath (name);
	return nullptr;
      }
      if (quark == QUARK_GETPATH) {
	long index = argv->getlong (0);
	return new String (getpath (index));
      }
      if (quark == QUARK_FILEP) {
	String name = argv->getstring (0);
	return new Boolean (isfile (name));
      }
      if (quark == QUARK_RESOLVE) {
	String name = argv->getstring (0);
	return new String (resolve (name));
      }
    }
    // call the object method 
    return Object::apply (zobj, nset, quark, argv);
  }
}
