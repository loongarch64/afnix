// ---------------------------------------------------------------------------
// - FileInfo.cpp                                                            -
// - standard object library - file info class implementation                -
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
#include "Integer.hpp"
#include "FileInfo.hpp"
#include "InputFile.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"
#include "csio.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a new file information by name

  FileInfo::FileInfo (const String& name) {
    d_name = name;
    update (false);
  }

  // create a new file information by name

  FileInfo::FileInfo (const String& name, const bool idty) {
    d_name = name;
    update (idty);
  }

  // return the class name

  String FileInfo::repr (void) const {
    return "FileInfo";
  }

  // get the file name

  String FileInfo::getname (void) const {
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

  // update the file information structure

  void FileInfo::update (const bool idty) {
    wrlock ();
    try {
      // check the file name
      if (d_name.isnil () == true) {
	throw Exception ("name-error", "nil input file name for update");
      }
      // get the file information
      char*    fname = d_name.tochar ();
      s_finfo* finfo = c_finfo (fname);
      // clean and check
      delete [] fname;
      if (finfo == nullptr) {
	throw Exception ("open-error", "cannot get file information", d_name);
      }
      // update the information structure
      d_size = finfo->d_size;
      d_mtim = finfo->d_mtime;
      // clean the structure
      delete finfo;
      // check for identity
      if (idty == true) {
	InputFile is (d_name);
	d_idty = Serial::identify (is);
      }
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return the input file size

  t_long FileInfo::length (void) const {
    rdlock ();
    try {
      t_long result = d_size;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return the file modification time

  t_long FileInfo::mtime (void) const {
    rdlock ();
    try {
      t_long result = d_mtim;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // return the file identity

  String FileInfo::identity (void) const {
    rdlock ();
    try {
      String result = d_idty;
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
  static const long QUARK_ZONE_LENGTH = 4;
  static QuarkZone  zone (QUARK_ZONE_LENGTH);

  // the object supported quarks
  static const long QUARK_IDTY   = zone.intern ("get-identity");
  static const long QUARK_MTIME  = zone.intern ("get-modification-time");
  static const long QUARK_LENGTH = zone.intern ("length");
  static const long QUARK_UPDATE = zone.intern ("update");

  // create a new object in a generic way

  Object* FileInfo::mknew (Vector* argv) {
    long argc = (argv == nullptr) ? 0 : argv->length ();
    // check for 1 argument
    if (argc == 1) {
      String name = argv->getstring (0);
      return new FileInfo (name);
    }
    // check for 2 arguments
    if (argc == 2) {
      String name = argv->getstring (0);
      bool   idty = argv->getbool (1);
      return new FileInfo (name, idty);
    }
    throw Exception ("argument-error", 
		     "invalid arguments with with file information"); 
  }

  // return true if the given quark is defined

  bool FileInfo::isquark (const long quark, const bool hflg) const {
    rdlock ();
    try {
      if (zone.exists (quark) == true) {
	unlock ();
	return true;
      }
      // check the nameable class
      bool result = hflg ? Nameable::isquark (quark, hflg) : false;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // apply this object with a set of arguments and a quark

  Object* FileInfo::apply (Evaluable* zobj, Nameset* nset, const long quark,
			   Vector* argv) {

    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();

    // dispatch 0 argument
    if (argc == 0) {
      if (quark == QUARK_IDTY)   return new String  (identity ());
      if (quark == QUARK_MTIME)  return new Integer (mtime  ());
      if (quark == QUARK_LENGTH) return new Integer (length ());
      if (quark == QUARK_UPDATE) {
	update (false);
	return nullptr;
      }
    }
    // dispatch 1 argument
    if (argc == 1) {
      if (quark == QUARK_UPDATE) {
	bool idty = argv->getbool (0);
	update (idty);
	return nullptr;
      }
    }
    // call the nameable class
    return Nameable::apply (zobj, nset, quark, argv);
  }
}
