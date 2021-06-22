// ---------------------------------------------------------------------------
// - LocalSpace.cpp                                                          -
// - afnix:csm module - local space class implementation                     -
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
#include "System.hpp"
#include "LocalZone.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"
#include "OutputFile.hpp"
#include "LocalSpace.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - private section                                                       -
  // -------------------------------------------------------------------------

  // the local space lock file
  static const String LS_LOCK_DEF = ".#lock";
  
  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a temporary local space

  LocalSpace::LocalSpace (void) {
    // create a temporay root name
    String root = System::temppth ("localspace");
    // make sure the root directory exists
    if (System::isdir (root) == false) System::mhdir (root);
    if (System::isdir (root) == false) {
      throw Exception ("localspace-error",
		       "cannot create local space root directory",
		       root);
    }
    // set the local root and lock
    d_root = root;
    d_name = System::xbase (root);
    Object::iref (p_lock = new Lockf (System::join (d_root, LS_LOCK_DEF)));
  }
  
  // create a localspace by root directory

  LocalSpace::LocalSpace (const String& root) {
    // make sure the root directory exists
    if (System::isdir (root) == false) System::mhdir (root);
    if (System::isdir (root) == false) {
      throw Exception ("localspace-error",
		       "cannot create local space root directory",
		       root);
    }
    // set the local root and lock
    d_root = root;
    d_name = System::xbase (root);
    Object::iref (p_lock = new Lockf (System::join (d_root, LS_LOCK_DEF)));
  }
  
  // create a localspace by root and name

  LocalSpace::LocalSpace (const String& root,
			  const String& name) : WorkSpace (name) {
    // make sure the root directory exists
    if (System::isdir (root) == false) System::mhdir (root);
    if (System::isdir (root) == false) {
      throw Exception ("localzone-error",
		       "cannot create local space root directory",
		       root);
    }
    // set the local root
    d_root = root;
    Object::iref (p_lock = new Lockf (System::join (d_root, LS_LOCK_DEF)));
  }
  
  // create a localspace by root, name and info

  LocalSpace::LocalSpace (const String& root, const String& name,
			  const String& info) : WorkSpace (name, info) {
    // make sure the root directory exists
    if (System::isdir (root) == false) System::mhdir (root);
    if (System::isdir (root) == false) {
      throw Exception ("localzone-error",
		       "cannot create local space root directory",
		       root);
    }
    // set the local root
    d_root = root;
    Object::iref (p_lock = new Lockf (System::join (d_root, LS_LOCK_DEF)));
  }
  
  // destroy the local space

  LocalSpace::~LocalSpace (void) {
    Object::dref (p_lock);
  }
  
  // return the class name

  String LocalSpace::repr (void) const {
    return "LocalSpace";
  }

  // check if a zone exists

  bool LocalSpace::iszone (const String& zone) const {
    rdlock ();
    try {
      // map the zone path
      String path = System::join (d_root, zone);
      // check for a zone
      p_lock->rdlock ();
      bool result = false;
      try {
	result = System::isdir (path);
	p_lock->unlock ();
      } catch (...) {
	p_lock->unlock ();
	throw;
      }
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // remove a zone by name

  bool LocalSpace::rmzone (const String& zone) {
    wrlock ();
    try {
      // get the zone root directory
      String root = System::join (d_root, zone);
      p_lock->wrlock ();
      // bind result
      bool result = false;
      // map the zone
      WorkZone* lzon = nullptr;
      try {
	// create a local zone
	lzon = new LocalZone (root);
	// clean the zone
	lzon->clean ();
	// close the zone
	delete lzon;
	// remove the zone
	result = System::rmdir (root);
	// unlock the local space
	p_lock->unlock ();
      } catch (...) {
	delete lzon;
	p_lock->unlock ();
	throw;
      }
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // add a new working zone by name
  
  WorkZone* LocalSpace::addzone (const String& zone) {
    wrlock ();
    try {
      // get the zone root directory
      String root = System::join (d_root, zone);
      p_lock->wrlock ();
      // create a zone
      WorkZone* result = nullptr;
      try {
	result = new LocalZone (root);
	p_lock->unlock ();
      } catch (...) {
	delete result;
	p_lock->unlock ();
	throw;
      }
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get a zone by name

  WorkZone* LocalSpace::getzone (const String& zone) const {
    rdlock ();
    try {
      // get the zone root directory
      String root = System::join (d_root, zone);
      p_lock->rdlock ();
      // check if the zone exists
      WorkZone* result = nullptr;
      if (iszone (zone) == true) {
	try {
	  result = new LocalZone (root);
	  p_lock->unlock ();
	} catch (...) {
	  delete result;
	  p_lock->unlock ();
	  throw;
	}
      }
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // map a name to a normalized uri

  String LocalSpace::tonuri (const String& zone, const String& name) const {
    rdlock ();
    try {
      // check if the zone exists
      if (iszone (zone) == false) {
	throw Exception ("localspace-error", "cannot find zone", zone);
      }
      // get the zone root directory
      String root = System::join (d_root, zone);
      // create a local zone
      LocalZone lzon (root, zone);
      // get the normalized uri
      String result = lzon.tonuri (name);
      unlock ();
      return result;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // check if an entity exists by zone and uri

  bool LocalSpace::exists (const String& zone, const Uri& uri) const {
    rdlock ();
    try {
      // check if the zone exists
      if (iszone (zone) == false) {
	unlock ();
	return false;
      }
      // get the zone root directory
      String root = System::join (d_root, zone);
      // create a local zone
      LocalZone lzon (root, zone);
      // check for existence
      bool result = lzon.exists (uri);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get an input stream by zone and uri

  InputStream* LocalSpace::getis (const String& zone, const Uri& uri) const {
    rdlock ();
    try {
      // check if the zone exists
      if (iszone (zone) == false) {
	throw Exception ("localspace-error", "cannot find zone", zone);
      }
      // get the zone root directory
      String root = System::join (d_root, zone);
      // create a local zone
      LocalZone lzon (root, zone);
      // get the input stream
      InputStream* is = lzon.getis (uri);
      unlock ();
      return is;
      unlock ();
      return is;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get an output stream by zone and uri

  OutputStream* LocalSpace::getos (const String& zone, const Uri& uri) const {
    rdlock ();
    try {
      // check if the zone exists
      if (iszone (zone) == false) {
	throw Exception ("localspace-error", "cannot find zone", zone);
      }
      // get the zone root directory
      String root = System::join (d_root, zone);
      // create a local zone
      LocalZone lzon (root, zone);
      // get the output stream
      OutputStream* os = lzon.getos (uri);
      unlock ();
      return os;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get a workspace zone entity list

  Strvec* LocalSpace::getelst (const String& zone) const {
    rdlock ();
    try {
      // check if the zone exists
      if (iszone (zone) == false) {
	throw Exception ("localspace-error", "cannot find zone", zone);
      }
      // get the zone root directory
      String root = System::join (d_root, zone);
      // create a local zone
      LocalZone lzon (root, zone);
      // get the entity list
      Strvec* result = lzon.getelst ();
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get a workspace zone entity print table

  PrintTable* LocalSpace::toeptbl (const String& zone) const {
    rdlock ();
    try {
      // check if the zone exists
      if (iszone (zone) == false) {
	throw Exception ("localspace-error", "cannot find zone", zone);
      }
      // get the zone root directory
      String root = System::join (d_root, zone);
      // create a local zone
      LocalZone lzon (root, zone);
      // get the entity print table
      PrintTable* result = lzon.toeptbl ();
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the root directoryy

  String LocalSpace::getroot (void) const {
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

  Object* LocalSpace::mknew (Vector* argv) {
    long argc = (argv == nullptr) ? 0 : argv->length ();
    // check for 0 argument
    if (argc == 0) return new LocalSpace;
    // check for 1 argument
    if (argc == 1) {
      String root = argv->getstring (0);
      return new LocalSpace (root);
    }
    // check for 2 arguments
    if (argc == 2) {
      String root = argv->getstring (0);
      String name = argv->getstring (1);
      return new LocalSpace (root, name);
    }
    // check for 3 arguments
    if (argc == 3) {
      String root = argv->getstring (0);
      String name = argv->getstring (1);
      String info = argv->getstring (2);
      return new LocalSpace (root, name, info);
    }
    throw Exception ("argument-error",
		     "too many argument with local space ");
  }

  // return true if the given quark is defined

  bool LocalSpace::isquark (const long quark, const bool hflg) const {
    rdlock ();
    try {
      if (zone.exists (quark) == true) {
	unlock ();
	return true;
      }
      bool result = hflg ? WorkSpace::isquark (quark, hflg) : false;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // apply this object with a set of arguments and a quark
  
  Object* LocalSpace::apply (Evaluable* zobj, Nameset* nset, const long quark,
			     Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();
    
    // check for 0 argument
    if (argc == 0) {
      if (quark == QUARK_GETROOT) return new String (getroot ());
    }
    // call the workspace method
    return WorkSpace::apply (zobj, nset, quark, argv);
  }
}
