// ---------------------------------------------------------------------------
// - Lockf.cpp                                                               -
// - standard object library - file lock class implementation                -
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

#include "Lockf.hpp"
#include "Vector.hpp"
#include "System.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"
#include "csio.hpp"

namespace afnix {
  
  // -------------------------------------------------------------------------
  // - private section                                                       -
  // -------------------------------------------------------------------------
  
  // this function open a file by name and return a file id
  static int lf_open (const String& path) {
    // check the file path
    if (path.isnil () == true) {
      throw Exception ("lockf-error", "nil input lock file path");
    }
    // try to open the file
    char* pc = path.tochar ();
    int sid  = c_openpd (pc);
    // clean, check and return
    delete [] pc;
    if (sid < 0) {
      throw Exception ("lockf-error", "cannot open lock file", path);
    }
    return sid;
  }
  
  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a new file lock

  Lockf::Lockf (const String& path) {
    d_path = path;
    d_rmoc = false;
    d_sid  = lf_open (path);
  }

  // destroy this file lock

  Lockf::~Lockf (void) {
    c_close (d_sid);
    if (d_rmoc == true) System::rmfile (d_path);
  }
  
  // return the class name
  
  String Lockf::repr (void) const {
    return "Lockf";
  }
  
  // lock this file lock for reading

  void Lockf::rdlock (void) const {
    Object::rdlock ();
    try {
      if (c_flock (d_sid, false) == false) {
	throw Exception ("lockf-error", "cannot lock file for reading", d_path);
      }
      Object::unlock ();
    } catch (...) {
      Object::unlock ();
      throw;
    }
  }

  // lock this file lock for writing

  void Lockf::wrlock (void) const {
    Object::wrlock ();
    try {
      if (c_flock (d_sid, true) == false) {
	throw Exception ("lockf-error", "cannot lock file for writing", d_path);
      }
      Object::unlock ();
    } catch (...) {
      Object::unlock ();
      throw;
    }
  }

  // unlock a file lock

  void Lockf::unlock (void) const {
    Object::wrlock ();
    try {
      if (c_funlock (d_sid) == false) {
	throw Exception ("lockf-error", "cannot unlock file", d_path);
      }
      Object::unlock ();
    } catch (...) {
      Object::unlock ();
      throw;
    }
  }

  // get the file lock path

  String Lockf::getpath (void) const {
    Object::rdlock ();
    try {
      String result = d_path;
      Object::unlock ();
      return result;
    } catch (...) {
      Object::unlock ();
      throw;
    }
  }

  // set the remove on close flag

  void Lockf::setrmoc (const bool rmoc) {
    Object::wrlock ();
    try {
      d_rmoc = rmoc;
      Object::unlock ();
    } catch (...) {
      Object::unlock ();
      throw;
    }
  }
  
  // -------------------------------------------------------------------------
  // - object section                                                        -
  // -------------------------------------------------------------------------

  // the quark zone
  static const long QUARK_ZONE_LENGTH = 1;
  static QuarkZone  zone (QUARK_ZONE_LENGTH);

  // the object supported quarks
  static const long QUARK_GETPATH = zone.intern ("get-path");

  // create a new object in a generic way

  Object* Lockf::mknew (Vector* argv) {
    long argc = (argv == nullptr) ? 0 : argv->length ();
    // check for 1 argument
    if (argc == 1) {
      String path = argv->getstring (0);
      return new Lockf (path);
    }
    throw Exception ("argument-error", 
		     "invalid arguments with with lock file"); 
  }

  // return true if the given quark is defined

  bool Lockf::isquark (const long quark, const bool hflg) const {
    rdlock ();
    try {
      if (zone.exists (quark) == true) {
	unlock ();
	return true;
      }
      // check the object class
      bool result = hflg ? Object::isquark (quark, hflg) : false;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // apply this object with a set of arguments and a quark

  Object* Lockf::apply (Evaluable* zobj, Nameset* nset, const long quark,
			Vector* argv) {
    
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();

    // dispatch 0 argument
    if (argc == 0) {
      if (quark == QUARK_GETPATH) return new String (getpath ());
    }
    // check the object class
    return Object::apply (zobj, nset, quark, argv);
  }
}
