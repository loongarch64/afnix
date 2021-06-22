// ---------------------------------------------------------------------------
// - Resolver.cpp                                                            -
// - afnix engine - file path resolver class implementation                  -
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
#include "Boolean.hpp"
#include "Resolver.hpp"
#include "Evaluable.hpp"
#include "InputFile.hpp"
#include "Librarian.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - private section                                                       -
  // -------------------------------------------------------------------------

  // the valid afnix extensions
  static const char* AXR_ALS_EXT = ".als";
  static const char* AXR_AXC_EXT = ".axc";
  
  // the resolver structure
  struct s_rpath {
    // the resolving path
    String d_path;
    // the librarian object
    Librarian* p_lib ;
    // next path in list
    s_rpath* p_next;
    // create a new path
    s_rpath (const String& path) {
      d_path = path;
      p_lib  = nullptr;
      p_next = nullptr;
      if (Librarian::valid (path) == true) {
	Object::iref (p_lib = new Librarian (path));
      } else {
	if (System::isdir (path) == false)
	  throw Exception ("path-error", "invalid path for resolver", path);
      }
    }
    // destroy this resolver path
    ~s_rpath (void) {
      Object::dref (p_lib);
      delete p_next;
    }
    // append a path at the end of this one
    void append (s_rpath* rpath) {
      s_rpath* last = this;
      while (last->p_next != nullptr) last = last->p_next;
      last->p_next = rpath;
    }
    // return true if the name is valid
    bool valid (const String& name) const {
      // check if we have a librarian
      if ((p_lib != nullptr) && (p_lib->exists (name) == true)) return true;
      // check for a directory
      String fpath = System::join (d_path, name);
      if (System::isfile (fpath) == true) return true;
      return false;
    }
    // return true if a start module exists
    bool isstm (void) const {
      // check for a librarian
      if (p_lib == nullptr) return false;
      // check if a start module exists
      return p_lib->isstm ();
    }
    // return a start module name
    String getstm (void) const {
      String result;
      if (p_lib != nullptr) result = p_lib->getstm ();
      return result;      
    }
    // return a path string if the name is valid
    String getpath (const String& name) const {
      String result;
      // check if we have a librarian
      if ((p_lib != nullptr) && (p_lib->exists (name) == true)) {
	result = p_lib->getname () + '!' + name;
	return result;
      }
      // check for a directory
      String fpath = System::join (d_path, name);
      if (System::isfile (fpath) == true) result = fpath;
      return result;
    }
    // return an input stream for this path
    InputStream* getstream (const String& name) const {
      // check if we have a librarian
      if ((p_lib != nullptr) && (p_lib->exists (name) == true)) 
	return p_lib->extract (name);
      // check for a directory
      String fpath = System::join (d_path, name);
      if (System::isfile (fpath) == true) return new InputFile (fpath);
      return nullptr;
    }
  };

  // this procedure returns the latest start module
  static String rslv_get_stm (s_rpath* rpath) {
    String result;
    while (rpath != nullptr) {
      // check for start module
      if (rpath->isstm () == true) result = rpath->getstm ();
      // next path
      rpath = rpath->p_next;
    }
    return result;
  }

  // this procedure return true if a path alrady exists in a path list
  static bool rslv_is_path (s_rpath* rpath, const String& path) {
    while (rpath != nullptr) {
      if (rpath->d_path == path) return true;
      rpath = rpath->p_next;
    }
    return false;
  }

  // this procedure returns a descriptor if a match is found
  static s_rpath* rslv_find_rpath (s_rpath* rpath, const String& name) {
    while (rpath != nullptr) {
      if (rpath->valid (name) == true) return rpath;
      rpath = rpath->p_next;
    }
    return nullptr;
  }

  // -------------------------------------------------------------------------
  // - public section                                                        -
  // -------------------------------------------------------------------------

  // get a stream by name locally or from the resolver
  
  InputStream* Resolver::tois (const String& name, Resolver* rslv) {
    try {
      // check locally
      if (System::isfile (name) == true) return new InputFile(name);
      // check with a resolver
      return (rslv != nullptr) ? rslv->get (name) : nullptr;
    } catch (...) {
      return nullptr;
    }
  }

  // get a buffer by name locally or from the resolver

  Buffer Resolver::tobuffer (const String& name, Resolver* rslv) {
    InputStream* is = nullptr;
    try {
      // get the stream by name
      is = Resolver::tois (name, rslv);
      if (is == nullptr) return Buffer();
      // consume the stream
      long slen = (is == nullptr) ? 0 : is->consume ();
      // map to a buffer for compare
      Buffer result = is->toibuf ();
      if (result.length () != slen) {
	throw Exception ("resolver-error", "inconsistent stream buffer size");
      }
      Object::dref (is);
      return result;
    } catch (...) {
      Object::dref (is);
      throw;
    }
  }
  
  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create an empty resolver

  Resolver::Resolver (void) {
    p_rpath = nullptr;
  }

  // create a resolver with on initial path

  Resolver::Resolver (const String& path) {
    // initialize to empty by default
    p_rpath = nullptr;
    // add the initial path
    add (path);
  }

  // create a resolver with a list of path

  Resolver::Resolver (const Strvec& paths) {
    // initialize to empty by default
    p_rpath = nullptr;
    // loop to add path
    long rlen = paths.length ();
    for (long i = 0; i < rlen; i++) add (paths.get (i));
  }

  // destroy this resolver

  Resolver::~Resolver (void) {
    delete p_rpath;
  }

  // return the class name

  String Resolver::repr (void) const {
    return "Resolver";
  }

  // add a path to this resolver

  void Resolver::add (const String& path) {
    // check the path
    if (path.isnil () == true ) return;
    // lock and process
    wrlock ();
    try {
      if (rslv_is_path (p_rpath, path) == true) {
	unlock ();
	return;
      }
      // create a new path
      s_rpath* rpath = new s_rpath (path);
      if (p_rpath == nullptr) {
	p_rpath = rpath;
      } else {
	p_rpath->append (rpath);
      }
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // add a librarian by resolving a name

  void Resolver::addlib (const String& name) {
    // check for nil
    if (name.isnil () == true) return;
    // lock and process
    wrlock ();
    try {
      // check for a valid librarian
      if (Librarian::valid (name) == true) {
	add (name);
	unlock ();
	return;
      }
      // try to resolve the name
      String path = getpath (name);
      if ((path.isnil () == true) || (Librarian::valid (path) == false)) {
	unlock ();
	return;
      }
      // add the path
      add (path);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return true if the name makes a valid path

  bool Resolver::valid (const String& name) const {
    // check first locally
    if (System::isfile (name) == true) return true;
    // look in the path
    rdlock ();
    try {
      bool result = (rslv_find_rpath (p_rpath, name) == nullptr) ? false : true;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get a start module name

  String Resolver::getstm (void) const {
    rdlock ();
    try {
      String result = rslv_get_stm (p_rpath);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return a resolved path

  String Resolver::getpath (const String& name) const {
    // check locally
    if (System::isfile (name) == true) return name;
    // lock and check
    rdlock ();
    try {
      String result;
      s_rpath* rpath = rslv_find_rpath (p_rpath, name);
      if (rpath != nullptr) result = rpath->getpath (name);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get an input stream by name

  InputStream* Resolver::get (const String& name) const {
    // check locally
    if (System::isfile (name) == true) return new InputFile (name);
    // check in the resolver
    rdlock ();
    try {
      s_rpath* rpath = rslv_find_rpath (p_rpath, name);
      InputStream* is = (rpath == nullptr) ? nullptr : rpath->getstream (name);
      unlock ();
      return is;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // lookup an input stream by name

  InputStream* Resolver::lookup (const String& name) const {
    // check locally
    if (System::isfile (name) == true) return new InputFile (name);
    // check in the resolver
    rdlock ();
    try {
      s_rpath* rpath = rslv_find_rpath (p_rpath, name);
      if (rpath == nullptr) {
	throw Exception ("resolver-error", "cannot resolve file", name);
      }
      InputStream* is = (rpath == nullptr) ? nullptr : rpath->getstream (name);
      unlock ();
      return is;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return true if the name is a valid afnix name

  bool Resolver::isvnam (const String& name) const {
    rdlock ();
    try {
      // check for an extension
      String fext = System::xext (name);
      if (fext.length () != 0) {
	bool result = valid (name);
	unlock ();
	return result;
      }
      // check for name first
      if (valid (name) == true) {
	 bool result = valid (name);
	 unlock ();
	 return result;
      }
      // check for axc
      String fname = name + AXR_AXC_EXT;
      if (valid (fname) == true) {
	unlock ();
	return true;
      }
      // check for als
      fname = name + AXR_ALS_EXT;
      if (valid (fname) == true) {
	unlock ();
	return true;
      }
      unlock ();
      return false;
    } catch (...) {
      unlock ();
      throw;
    }
  }


  // return the afnix path name from a name

  String Resolver::tornam (const String& name) const {
    rdlock ();
    try {
      String result;
      // check for an extension
      String fext = System::xext (name);
      if (fext.length () != 0) result = getpath (name);
      // check for name first
      else if (valid (name) == true) result = getpath (name);
      // check for axc
      else {
	String fname = name + AXR_AXC_EXT;
	if (valid (fname) == true) result = getpath (fname);
	else {
	  // check for als
	  fname = name + AXR_ALS_EXT;
	  if (valid (fname) == true) result = getpath (fname);
	}
      }
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get an input stream for an afnix file

  InputStream* Resolver::tostream (const String& name) const {
    rdlock ();
    try {
      InputStream* result = nullptr;
      // check for an extension
      String fext = System::xext (name);
      if (fext.length () != 0) result = get (name);
      // check for name first
      else if (valid (name) == true) result = get (name);
      // check for axc
      else {
	String fname = name + AXR_AXC_EXT;
	if (valid (fname) == true) result = get (fname);
	// check for als
	else {
	  fname = name + AXR_ALS_EXT;
	  if (valid (fname) == true) result = get (fname);
	}
      }
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // lookup an input stream for an afnix file

  InputStream* Resolver::tolookup (const String& name) const {
    rdlock ();
    try {
      InputStream* result = tostream (name);
      if (result == nullptr) {
	throw Exception ("resolver-error", "cannot resolve file", name);
      }
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // map a generic file as a string

  String Resolver::tostring (const String& name) const {
    rdlock ();
    try {
      // map the name to an input stream
      InputStream* is = lookup (name);
      // consume the stream
      long slen = (is == nullptr) ? 0 : is->consume ();
      // map to a string for compare
      String result = is->tostring ();
      if (result.length () != slen) {
	throw Exception ("resolver-error", "inconsistent stream string size");
      }
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
  static const long QUARK_ZONE_LENGTH = 7;
  static QuarkZone  zone (QUARK_ZONE_LENGTH);

  // the object supported quarks
  static const long QUARK_ADD      = zone.intern ("add");
  static const long QUARK_VALIDP   = zone.intern ("valid-p");
  static const long QUARK_ISVNAM   = zone.intern ("valid-name-p");
  static const long QUARK_ADDLIB   = zone.intern ("add-librarian");
  static const long QUARK_LOOKUP   = zone.intern ("lookup");
  static const long QUARK_GETPATH  = zone.intern ("get-path");
  static const long QUARK_TOSTRING = zone.intern ("to-string");

  // create a new object in a generic way

  Object* Resolver::mknew (Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();

    // check for 0 argument
    if (argc == 0) return new Resolver;
    // check for 1 argument
    if (argc == 1) {
      String path = argv->getstring (0);
      return new Resolver (path);
    }
    // illegal arguments
    throw Exception ("argument-error", "too many arguments with resolver");
  }

  // return true if the given quark is defined

  bool Resolver::isquark (const long quark, const bool hflg) const {
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

  Object* Resolver::apply (Evaluable* zobj, Nameset* nset, const long quark,
			   Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();

    // dispatch 1 argument
    if (argc == 1) {
      if (quark == QUARK_ADD) {
	String fpath = argv->getstring (0);
	add (fpath);
	return nullptr;
      }
      if (quark == QUARK_ADDLIB) {
	String name = argv->getstring (0);
	addlib (name);
	return nullptr;
      }
      if (quark == QUARK_VALIDP) {
	String fpath = argv->getstring (0);
	return new Boolean (valid (fpath));
      }
      if (quark == QUARK_ISVNAM) {
	String fpath = argv->getstring (0);
	return new Boolean (isvnam (fpath));
      }
      if (quark == QUARK_GETPATH) {
	String name = argv->getstring (0);
	return new String (getpath (name));
      }
      if (quark == QUARK_LOOKUP) {
	rdlock ();
	try {
	  String fpath = argv->getstring (0);
	  Object* result = lookup (fpath);
	  zobj->post (result);
	  unlock ();
	  return result;
	} catch (...) {
	  unlock ();
	  throw;
	}
      }
      if (quark == QUARK_TOSTRING) {
	String   name = argv->getstring (0);
	String result = tostring (name);
	return new String (result);
      }
    }
    // call the object method
    return Object::apply (zobj, nset, quark, argv);
  }
}
