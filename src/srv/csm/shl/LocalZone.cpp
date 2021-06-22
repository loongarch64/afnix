// ---------------------------------------------------------------------------
// - LocalZone.cpp                                                           -
// - afnix:csm module - local zone class implementation                      -
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

#include "Date.hpp"
#include "Vector.hpp"
#include "System.hpp"
#include "Utility.hpp"
#include "Pathname.hpp"
#include "FileInfo.hpp"
#include "InputFile.hpp"
#include "Directory.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"
#include "OutputFile.hpp"
#include "LocalZone.hpp"

namespace afnix {
 
  // -------------------------------------------------------------------------
  // - private section                                                       -
  // -------------------------------------------------------------------------

  // the local zone lock file
  static const String LZ_LOCK_DEF = ".#lock";
  // the local zone file scheme
  static const String LZ_FILE_URI = "file";

  // map a uri to a local zone path
  static String lz_topath (const String& root, const Uri& uri) {
    // get the uri scheme
    String schm = uri.getscheme ();
    String path;
    // check for zone uri
    if (schm == WorkZone::getschm ()) {
      path = System::join (root, uri.getpath ());
    }
    // check for a file scheme
    else if (schm == LZ_FILE_URI) {
      path = uri.getpath ();
    } else {
      // invalid scheme
      throw Exception ("localzone-error", "invalid uri scheme", schm);
    }
    // check the root path for security
    String rdir = System::xdir (path);
    if (rdir != root) {
      throw Exception ("localzone-error", "invalid root directory", rdir);
    }
    return path;
  }
  
  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a temporary localzone

  LocalZone::LocalZone (void) {
    // create a temporay root name
    String root = System::temppth ("localzone");
    // make sure the root directory exists
    if (System::isdir (root) == false) System::mhdir (root);
    if (System::isdir (root) == false) {
      throw Exception ("localzone-error",
		       "cannot create local zone root directory",
		       root);
    }
    // set the local root and lock
    d_root = root;
    d_name = System::xbase (root);
    Object::iref (p_lock = new Lockf (System::join (d_root, LZ_LOCK_DEF)));
  }
  
  // create a local zone by root and name

  LocalZone::LocalZone (const String& root) {
    // fix the root directory localy
    Pathname pnam = root; String rdir = pnam.getsysp ();
    // make sure the root directory exists
    if (System::isdir (rdir) == false) System::mhdir (rdir);
    if (System::isdir (rdir) == false) {
      throw Exception ("localzone-error",
		       "cannot create local zone root directory",
		       root);
    }
    // set the local root and lock
    d_root = rdir;
    d_name = System::xbase (rdir);
    Object::iref (p_lock = new Lockf (System::join (d_root, LZ_LOCK_DEF)));
  }

  // create a local zone by root and name

  LocalZone::LocalZone (const String& root,
			const String& name) : WorkZone (name) {
    // fix the root directory localy
    Pathname pnam = root; String rdir = pnam.getsysp ();
    // make sure the root directory exists
    if (System::isdir (rdir) == false) System::mhdir (rdir);
    if (System::isdir (rdir) == false) {
      throw Exception ("localzone-error",
		       "cannot create local zone root directory",
		       root);
    }
    // set the local root and lock
    d_root = rdir;
    Object::iref (p_lock = new Lockf (System::join (d_root, LZ_LOCK_DEF)));
  }

  // create a local zone by name, info and root directory

  LocalZone::LocalZone (const String& root, const String& name,
			const String& info) : WorkZone (name, info) {
    // fix the root directory localy
    Pathname pnam = root; String rdir = pnam.getsysp ();
    // make sure the root directory exists
    if (System::isdir (rdir) == false) System::mhdir (rdir);
    if (System::isdir (rdir) == false) {
      throw Exception ("localzone-error",
		       "cannot create local zone root directory",
		       root);
    }
    // set the local root and lock
    d_root = rdir;
    Object::iref (p_lock = new Lockf (System::join (d_root, LZ_LOCK_DEF)));
  }
  
  // destroy the local zone

  LocalZone::~LocalZone (void) {
    Object::dref (p_lock);
  }

  // return the class name

  String LocalZone::repr (void) const {
    return "LocalZone";
  }

  // map a name to a normalized uri string

  String LocalZone::tonuri (const String& name) const {
    rdlock ();
    try {
      // map the name by zone
      String path = System::join (d_root, name);
      // normalize it
      String result = Uri::nrmname (path);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // check if an entity exists by uri

  bool LocalZone::exists (const Uri& uri) const {
    rdlock ();
    try {
      // map the uri to a path
      String path = lz_topath (d_root, uri);
      // check for valid path
      bool result = System::isfile (path);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // rename an entity by uri

  bool LocalZone::rename (const Uri& nuri, const Uri& turi) {
    wrlock ();
    try {
      // map the uris to name and path
      String name = lz_topath (d_root, nuri);
      String path = lz_topath (d_root, turi);
      // try to move the file
      bool result = System::mvfile (name, path);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // remove an entity by uri

  bool LocalZone::remove (const Uri& uri) {
    wrlock ();
    try {
      // map the uri to a path
      String path = lz_topath (d_root, uri);
      // try to remove the file
      bool result = System::rmfile (path);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // clean the zone

  bool LocalZone::clean (void) {
    wrlock ();
    p_lock->wrlock();
    Strvec* elst = nullptr;
    try {
      // get the entity list
      elst = getelst ();
      if (elst == nullptr) {
	unlock ();
	return false;
      }
      // prepare result
      bool result = true;
      // remove each entry
      long elen = elst->length ();
      for (long k = 0L; k < elen; k++) {
	// get the file path
	String name = elst->get (k);
	// remove in the zone
	result = result && WorkZone::remove (name);
      }
      if (result == true) p_lock->setrmoc (true);
      // clean and return
      delete elst;
      p_lock->unlock ();
      unlock ();
      return result;
    } catch (...) {
      delete elst;
      p_lock->unlock ();
      unlock ();
      throw;
    }
  }
  
  // get an input stream by zone and uri

  InputStream* LocalZone::getis (const Uri& uri) const {
    rdlock ();    
    try {
      // map the uri to a path
      String path = lz_topath (d_root, uri);
      // create a local input stream
      InputStream* is = new InputFile (path);
      unlock ();
      return is;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get an output stream by zone and uri

  OutputStream* LocalZone::getos (const Uri& uri) const {
    rdlock ();
    try {
      // map the uri to a path
      String path = lz_topath (d_root, uri);
      // create a local input stream
      OutputStream* os = new OutputFile (path);
      unlock ();
      return os;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get a local zone entity list

  Strvec* LocalZone::getelst (void) const {
    rdlock ();
    p_lock->rdlock ();
    try {
      // locally map the zone as a directory
      Directory zdir (d_root);
      // get the file list
      Strvec* result = zdir.getfiles ();
      if (result != nullptr) result->remove (LZ_LOCK_DEF);
      // unlock and return
      p_lock->unlock ();
      unlock ();
      return result;
    } catch (...) {
      p_lock->unlock ();
      unlock ();
      throw;
    }
  }

  // get a local zone entity list by identity

  Strvec* LocalZone::getelst (const String& idty) const {
    // check for a valid identity
    if (idty.isnil () == true) return getelst ();
    // lock and collect
    rdlock ();
    p_lock->rdlock ();
    // the entity list
    Strvec* elst = nullptr;
    Strvec* result = new Strvec;
    try {
      // get the entity list
      elst = getelst ();
      // loop in the list
      long elen = (elst == nullptr) ? 0L : elst->length ();
      for (long k = 0L; k < elen; k++) {
	// get the file name and path
	String name = elst->get (k);
	String path = lz_topath (d_root, tonuri(name));
	// get the file information
	FileInfo info (path, true);
	// compare the identity
	if (info.identity () == idty) result->add (name);
      }
      delete elst;
      // unlock and return
      p_lock->unlock ();
      unlock ();
      return result;
    } catch (...) {
      delete elst;
      delete result;
      p_lock->unlock ();
      unlock ();
      throw;
    }
  }
  
  // get a local zone entity print table

  PrintTable* LocalZone::toeptbl (void) const {
    rdlock ();
    p_lock->rdlock ();
    PrintTable* ptbl = nullptr;
    try {
      // prepare the print table
      ptbl = new PrintTable (3);
      ptbl->sethead (0, "Name");
      ptbl->sethead (1, "Size");
      ptbl->sethead (2, "Description");
      // get the entity list
      Strvec* elst = getelst ();
      // loop in the list
      long elen = (elst == nullptr) ? 0L : elst->length ();
      for (long k = 0L; k < elen; k++) {
	// get the file name and path
	String name = elst->get (k);
	String path = lz_topath (d_root, tonuri(name));
	// get the file information
	FileInfo info (path, true);
	// set the table
	long row = ptbl->add ();
	ptbl->set (row, 0, name);
	ptbl->set (row, 1, Utility::tostring (info.length ()));
	ptbl->set (row, 2, info.identity().toupper());
      }
      p_lock->unlock ();
      unlock ();
      return ptbl;
    } catch (...) {
      delete ptbl;
      p_lock->unlock ();
      unlock ();
      throw;
    }
  }

  // get the root directoryy
  
  String LocalZone::getroot (void) const {
    rdlock ();
    try {
      String result = d_root;
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
  static const long QUARK_ZONE_LENGTH = 1;
  static QuarkZone  zone (QUARK_ZONE_LENGTH);

  // the object supported quarks
  static const long QUARK_GETROOT  = zone.intern ("get-root");

  // create a new object in a generic way

  Object* LocalZone::mknew (Vector* argv) {
    long argc = (argv == nullptr) ? 0 : argv->length ();
    // check for 0 argument
    if (argc == 0) return new LocalZone;
    // check for 1 argument
    if (argc == 1) {
      String root = argv->getstring (0);
      return new LocalZone (root);
    }
    // check for 2 arguments
    if (argc == 2) {
      String root = argv->getstring (0);
      String name = argv->getstring (1);
      return new LocalZone (root, name);
    }
    // check for 3 arguments
    if (argc == 3) {
      String root = argv->getstring (0);
      String name = argv->getstring (1);
      String info = argv->getstring (2);
      return new LocalZone (root, name, info);
    }
    throw Exception ("argument-error",
		     "too many argument with localzone ");
  }

  // return true if the given quark is defined

  bool LocalZone::isquark (const long quark, const bool hflg) const {
    rdlock ();
    try {
      if (zone.exists (quark) == true) {
	unlock ();
	return true;
      }
      bool result = hflg ? WorkZone::isquark (quark, hflg) : false;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // apply this object with a set of arguments and a quark
  
  Object* LocalZone::apply (Evaluable* zobj, Nameset* nset, const long quark,
			    Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();
    
    // check for 0 argument
    if (argc == 0) {
      if (quark == QUARK_GETROOT) return new String (getroot ());
    }
    // call the workzone method
    return WorkZone::apply (zobj, nset, quark, argv);
  }
}
