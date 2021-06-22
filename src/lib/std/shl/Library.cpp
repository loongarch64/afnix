// ---------------------------------------------------------------------------
// - Library.cpp                                                             -
// - standard object library - shared library class implementation           -
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
#include "Boolean.hpp"
#include "Library.hpp"
#include "Evaluable.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"
#include "cshl.hpp"

namespace afnix {
  
  // -------------------------------------------------------------------------
  // - private section                                                       -
  // -------------------------------------------------------------------------

  // this function normalizes the '-' or '+' characters to '_' character
  static String lib_normalize_name (const String& name) {
    String result;
    long   len = name.length ();
    for (long i = 0; i < len; i++) {
      char c = name[i];
      if ((c == '-') || (c == '+')) c = '_';
      result = result + c;
    }
    return result;
  }

  // this function compute the dynamic entry point by adding the "dli" prefix
  static String lib_normalize_entry (const String& name) {
    String result = "dli_";
    result = result + lib_normalize_name (name);
    return result;
  }

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a self library name

  Library::Library  (void) {
    d_name = "main";
    d_aflg = false;
    d_rflg = false;
    d_iflg = false;
    p_hand = c_dlmain ();
    // cannot open main handle
    if (p_hand == nullptr) {
      throw Exception ("name-error", "cannot open self library");
    }
  }

  // create a new shared library object

  Library::Library (const String& name) {
    // register the library name and flag
    d_name = name;
    d_aflg = false;
    d_rflg = false;
    d_iflg = false;
    p_hand = nullptr;
    // try to open the library
    char* lname = name.tochar ();
    p_hand = c_dlopen (lname);
    delete [] lname;
    // not found
    if (p_hand == nullptr) {
      throw Exception ("name-error", "cannot open library", name);
    }
    // check for the alien flag
    String rnam = lib_normalize_name (d_name + "_resident_library");
    if (find (rnam) != nullptr) d_aflg = true;
  }

  // create a registered library by name and handle

  Library::Library (const String& name, void* hand) {
    // register the library name and flag
    d_name = name;
    d_rflg = true;
    d_iflg = false;
    p_hand = hand;
    // check the handle
    if (p_hand == nullptr) {
      throw Exception ("handle-error", "cannot register library", name);
    }
  }

  // delete this shared library by closing it

  Library::~Library (void) {
    if ((d_aflg == false) && (d_rflg == false)) c_dlclose (p_hand);
  }
  
  // return the class name

  String Library::repr (void) const {
    return "Library";
  }

  // return the library name

  String Library::getname (void) const {
    rdlock ();
    try {
      String result = d_name;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set the library alien flag

  void Library::setaflg (const bool aflg) {
    wrlock ();
    try {
      d_aflg = aflg;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the alien flag

  bool Library::getaflg (void) const {
    rdlock ();
    try {
      bool result = d_aflg;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // try to find a symbol by name in this library 

  void* Library::find (const String& name) const {
    rdlock ();
    char* sname = nullptr;
    try {
      sname = name.tochar ();
      void* sym = c_dlsym (sname, p_hand);
      delete [] sname; sname = nullptr;
      unlock ();
      return sym;
    } catch (...) {
      delete [] sname;
      unlock ();
      throw;
    }
  }

  // try to find a symbol by name in this library - this is for internal test

  void* Library::lookup (const String& name) const {
    rdlock ();
    try {
      void* sym = find (name);
      if (sym == nullptr) { 
	throw Exception ("library-error", "cannot find symbol", name);
      }
      unlock ();
      return sym;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // initialize a dynamic library

  Object* Library::dlinit (Evaluable* zobj, Vector* argv) const {
    wrlock ();
    try {
      if (d_iflg == true) {
	unlock ();
	return nullptr;
      }
      // get the symbol
      void* sym = d_rflg ? p_hand : lookup (lib_normalize_entry (d_name));
      // map the symbol and call it
      using t_func = Object* (*) (Evaluable*, Vector*);
      t_func func = reinterpret_cast <t_func> (sym);
      Object* result = func (zobj, argv);
      // post and mark initialized
      zobj->post (result);
      d_iflg = true;
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
  static const long QUARK_ZONE_LENGTH = 2;
  static QuarkZone  zone (QUARK_ZONE_LENGTH);

  // the object supported quarks
  static const long QUARK_LIBINIT = zone.intern ("initialize");
  static const long QUARK_GETAFLG = zone.intern ("resident-p");

  // create a new shared library in a generic way

  Object* Library::mknew (Vector* argv) {
    long argc = (argv == nullptr) ? 0 : argv->length ();
    // check for 0 argument
    if (argc == 0) return new Library;
    // check for 1 argument
    if (argc == 1) {
      String name = argv->getstring (0);
      return new Library (name);
    }
    // invalid argument 
    throw Exception ("argument-error", 
		     "invalid number of arguments with library");
  }

  // return true if the given quark is defined

  bool Library::isquark (const long quark, const bool hflg) const {
    rdlock ();
    if (zone.exists (quark) == true) {
      unlock ();
      return true;
    }      
    bool result = hflg ? Nameable::isquark (quark, hflg) : false;
    unlock ();
    return result;
  }
  
  // apply this object with a set of arguments and a quark

  Object* Library::apply (Evaluable* zobj, Nameset* nset, const long quark,
			  Vector* argv) {
    // map the generic procedure
    if (quark == QUARK_LIBINIT) {
      wrlock ();
      try {
	Object* result = dlinit (zobj, argv);
	zobj->post (result);
	unlock ();
	return result;
      } catch (...) {
	unlock ();
	throw;
      }
    }
    // gett the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();
    // check for 0 argument
    if (argc == 0) {
      if (quark == QUARK_GETAFLG) return new Boolean (getaflg ());
    }
    // call the nameable method
    return Nameable::apply (zobj, nset, quark, argv);
  }
}
