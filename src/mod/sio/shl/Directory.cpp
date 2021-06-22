// ---------------------------------------------------------------------------
// - Directory.cpp                                                           -
// - afnix:sio module - directory class implementation                       -
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

#include "Interp.hpp"
#include "System.hpp"
#include "Vector.hpp"
#include "Directory.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"
#include "csio.hpp"
#include "cdir.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - public section                                                        -
  // -------------------------------------------------------------------------

  // recursively destroy a directory

  void Directory::rmall (const String& path) {
    Strvec* slst = nullptr;
    try {
      // check that we have a directory
      if (System::isdir (path) == false) return;
      // create a directory object
      Directory pdir (path);
      // get the list of sub directories
      slst = pdir.getdirs ();
      long slen = slst->length ();
      // get local and upper directories
      String ldir = System::getldir ();
      String udir = System::getudir ();
      // start by removing the sub directories
      for (long k = 0; k < slen; k++) {
	String name = slst->get (k);
	if ((name == ldir) || (name == udir)) continue;
	Directory::rmall (System::join (path, name));
      }
      delete slst; slst = nullptr;
      // now remove everything else
      slst = pdir.getfiles ();
      slen = slst->length ();
      for (long k = 0; k < slen; k++) pdir.rmfile (slst->get (k));
      delete slst; slst = nullptr;
      // now remove the directory
      System::rmdir (path);
    } catch (...) {
      delete slst;
      throw;
    }
  }

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // open the default directory

  Directory::Directory (void) {
    // update directory handle
    char* dnam = c_getcwd ();
    d_name     = dnam;
    p_handle   = c_opendir (dnam);
    // clean result
    delete [] dnam;
    if (p_handle == nullptr)
      throw Exception ("dir-error", "cannot open directory", d_name);
  }

  // open a directory by name

  Directory::Directory (const String& name) {
    // update directory handle
    d_name     = name;
    char* dnam = name.tochar ();
    p_handle   = c_opendir (dnam);
    // clean result
    delete [] dnam;
    if (p_handle == nullptr) 
      throw Exception ("dir-error", "cannot open directory", name);
  }

  // destroy this directory 

  Directory::~Directory (void) {
    c_closedir (p_handle);
  }

  // return the class name

  String Directory::repr (void) const {
    return "Directory";
  }

  // return the directory name
    
  String Directory::getname (void) const {
    rdlock ();
    String result = d_name;
    unlock ();
    return result;
  }

  // return the next name available

  String Directory::nxtname (void) {
    rdlock ();
    try {
      char*    name = c_readdir (p_handle);
      String result = name;
      delete [] name;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return the next path available

  String Directory::nxtpath (void) {
    rdlock ();
    try {
      char*    name = c_readdir (p_handle);
      String result = System::join (d_name, name);
      delete [] name;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return the next file name available

  String Directory::nxtfnam (void) {
    rdlock ();
    try {
      String result = "";
      while (true) {
	char* name = c_readdir (p_handle);
	if (name == nullptr) break;
	result = name;
	delete [] name;
	if (System::isfile (d_name, result) == true) break;
	result = "";
      }
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return the next file path available

  String Directory::nxtfpth (void) {
    rdlock ();
    try {
      String result = "";
      while (true) {
	char* name = c_readdir (p_handle);
	if (name == nullptr) break;
	result = System::join (d_name, name);
	delete [] name;
	if (System::isfile (result) == true) break;
	result = "";
      }
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return the next directory name available

  String Directory::nxtdnam (void) {
    rdlock ();
    try {
      String result = "";
      while (true) {
	char* name = c_readdir (p_handle);
	if (name == nullptr) break;
	if (c_isdot (name) == true) continue;
	result = name;
	delete [] name;
	if (System::isdir (d_name, result) == true) break;
	result = "";
      }
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return the next directory path available

  String Directory::nxtdpth (void) {
    rdlock ();
    try {
      String result = "";
      while (true) {
	char* name = c_readdir (p_handle);
	if (name == nullptr) break;
	if (c_isdot (name) == true) continue;
	result = System::join (d_name, name);
	delete [] name;
	if (System::isdir (result) == true) break;
	result = "";
      }
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return a list of this directory names

  Strvec* Directory::getlist (void) {
    rdlock ();
    Strvec* result = new Strvec (true);
    try {
      // get the list
      while (true) {
	char* name = c_readdir (p_handle);
	if (name == nullptr) break;
	result->add (name);
	delete [] name;
      }
      // rewind the directory
      c_rewinddir (p_handle);
      unlock ();
      return result;
    } catch (...) {
      delete result;
      c_rewinddir (p_handle);
      unlock ();
      throw;
    }
  }

  // return a list of this directory with their path

  Strvec* Directory::getplist (void) {
    rdlock ();
    Strvec* result = new Strvec (true);
    try {
      // get the list
      while (true) {
	char* name = c_readdir (p_handle);
	if (name == nullptr) break;
	String path = System::join (d_name, name);
	result->add (path);
	delete [] name;
      }
      // rewind the directory
      c_rewinddir (p_handle);
      unlock ();
      return result;
    } catch (...) {
      delete result;
      c_rewinddir (p_handle);
      unlock ();
      throw;
    }
  }

  // return a list of this directory files

  Strvec* Directory::getfiles (void) {
    rdlock ();
    Strvec* result = new Strvec (true);
    try {
      // get the list
      while (true) {
	char* name = c_readdir (p_handle);
	if (name == nullptr) break;
	if (System::isfile (d_name, name) == true) {
	  result->add (name);
	}
	delete [] name;
      }
      // rewind the directory
      c_rewinddir (p_handle);
      unlock ();
      return result;
    } catch (...) {
      delete result;
      c_rewinddir (p_handle);
      unlock ();
      throw;
    }
  }

  // return a list of this directory files with their path

  Strvec* Directory::getpfiles (void) {
    rdlock ();
    Strvec* result = new Strvec (true);
    try {
      // get the list
      while (true) {
	char* name = c_readdir (p_handle);
	if (name == nullptr) break;
	String path = System::join (d_name, name);
	if (System::isfile (path) == true) {
	  result->add (path);
	}
	delete [] name;
      }
      // rewind the directory
      c_rewinddir (p_handle);
      unlock ();
      return result;
    } catch (...) {
      delete result;
      c_rewinddir (p_handle);
      unlock ();
      throw;
    }
  }


  // return a list of the sub directories

  Strvec* Directory::getdirs (void) {
    rdlock ();
    Strvec* result = new Strvec (true);
    try {
      // get the list
      while (true) {
	char* name = c_readdir (p_handle);
	if (name == nullptr) break;
	if (System::isdir (d_name, name) == true) {
	  result->add (name);
	}
	delete [] name;
      }
      // rewind the directory
      c_rewinddir (p_handle);
      unlock ();
      return result;
    } catch (...) {
      delete result;
      c_rewinddir (p_handle);
      unlock ();
      throw;
    }
  }

  // return a list of the sub directories with their path

  Strvec* Directory::getpdirs (void) {
    rdlock ();
    Strvec* result = new Strvec (true);
    try {
      // get the list
      while (true) {
	char* name = c_readdir (p_handle);
	if (name == nullptr) break;
	String path = System::join (d_name, name);
	if (System::isdir (path) == true) {
	  result->add (path);
	}
	delete [] name;
      }
      // rewind the directory
      c_rewinddir (p_handle);
      unlock ();
      return result;
    } catch (...) {
      delete result;
      c_rewinddir (p_handle);
      unlock ();
      throw;
    }
  }

  // create a new directory by name

  void Directory::mkdir (const String& name) {
    wrlock ();
    try {
      // join the name
      String path = System::join (d_name, name);
      char*  buf  = path.tochar ();
      // try to create the directory
      if (c_mkdir (buf) == false) {
	delete [] buf;
	throw Exception ("directory-error", "cannot create directory", path);
      }
      delete [] buf;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // remove an empty directory by name

  void Directory::rmdir (const String& name) {
    wrlock ();
    try {
      String path = System::join (d_name, name);
      if (System::rmdir (path) == false) {
	throw Exception ("directory-error", "cannot remove directory", path);
      }
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // remove a file in a directory

  void Directory::rmfile (const String& name) {
    wrlock ();
    try {
      String path = System::join (d_name, name);
      if (System::rmfile (path) == false) {
	throw Exception ("directory-error", "cannot remove file", path);
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
  static const long QUARK_ZONE_LENGTH = 16;
  static QuarkZone  zone (QUARK_ZONE_LENGTH);

  // the object supported quarks
  static const long QUARK_MKDIR     = zone.intern ("mkdir");
  static const long QUARK_RMDIR     = zone.intern ("rmdir");
  static const long QUARK_RMFILE    = zone.intern ("rmdir");
  static const long QUARK_GETNAME   = zone.intern ("get-name");
  static const long QUARK_GETLIST   = zone.intern ("get-list");
  static const long QUARK_GETFILES  = zone.intern ("get-files");
  static const long QUARK_GETSDIRS  = zone.intern ("get-subdirs");
  static const long QUARK_GETPLIST  = zone.intern ("get-list-path");
  static const long QUARK_GETPFILES = zone.intern ("get-files-path");
  static const long QUARK_GETPSDIRS = zone.intern ("get-subdirs-path");
  static const long QUARK_NXTNAME   = zone.intern ("next-name");
  static const long QUARK_NXTPATH   = zone.intern ("next-path");
  static const long QUARK_NXTFNAM   = zone.intern ("next-file-name");
  static const long QUARK_NXTFPTH   = zone.intern ("next-file-path");
  static const long QUARK_NXTDNAM   = zone.intern ("next-dir-name");
  static const long QUARK_NXTDPTH   = zone.intern ("next-dir-path");


  // create a new object in a generic way

  Object* Directory::mknew (Vector* argv) {
    long argc = (argv == nullptr) ? 0 : argv->length ();
    // check for 0 arguments
    if (argc == 0) return new Directory;
    // check for 1 argument
    if (argc == 1) {
      String val = argv->getstring (0);
      return new Directory (val);
    }
    throw Exception ("argument-error",
		     "too many argument with directory open");
  }

  // return true if the given quark is defined

  bool Directory::isquark (const long quark, const bool hflg) const {
    rdlock ();
    if (zone.exists (quark) == true) {
      unlock ();
      return true;
    }
    bool result = hflg ? Object::isquark (quark, hflg) : false;
    unlock ();
    return result;
  }

  // apply this object with a set of arguments and a quark
  
  Object* Directory::apply (Evaluable* zobj, Nameset* nset, const long quark,
			    Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();
 
    // check for 0 argument
    if (argc == 0) {
      if (quark == QUARK_GETNAME)   return new String (getname ());
      if (quark == QUARK_NXTNAME)   return new String (nxtname ());
      if (quark == QUARK_NXTPATH)   return new String (nxtpath ());
      if (quark == QUARK_NXTFNAM)   return new String (nxtfnam ());
      if (quark == QUARK_NXTFPTH)   return new String (nxtfpth ());
      if (quark == QUARK_NXTDNAM)   return new String (nxtdnam ());
      if (quark == QUARK_NXTDPTH)   return new String (nxtdpth ());
      if (quark == QUARK_GETLIST)   return getlist   ();
      if (quark == QUARK_GETFILES)  return getfiles  ();
      if (quark == QUARK_GETSDIRS)  return getdirs   ();
      if (quark == QUARK_GETPLIST)  return getplist  ();
      if (quark == QUARK_GETPFILES) return getpfiles ();
      if (quark == QUARK_GETPSDIRS) return getpdirs  ();
    }

    // check for 1 argument
    if (argc == 1) {
      if (quark == QUARK_MKDIR) {
	String val = argv->getstring (0);
	mkdir (val);
	return new Directory (System::join (d_name, val));
      }
      if (quark == QUARK_RMDIR) {
	String val = argv->getstring (0);
	rmdir (val);
	return nullptr;
      }
      if (quark == QUARK_RMFILE) {
	String val = argv->getstring (0);
	rmdir (val);
	return nullptr;
      }
    }
    // call the object method 
    return Object::apply (zobj, nset, quark, argv);
  }
}
