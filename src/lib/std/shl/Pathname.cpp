// ---------------------------------------------------------------------------
// - Pathname.cpp                                                            -
// - standard object library - path name class implementation                -
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
#include "Boolean.hpp"
#include "Integer.hpp"
#include "Pathname.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - private section                                                       -
  // -------------------------------------------------------------------------

  // this procedure returns true if the name is absolute
  static bool is_root (const String& name) {
    // get the root name
    String rdir = System::getrdir ();
    long   rlen = rdir.length ();
    long   nlen = name.length ();
    // check the name
    if (nlen < rlen) return false;
    return (name.lsubstr (rlen) == rdir);
  }

  // this procedure return a path without the root name
  static String rm_root (const String& name) {
    // get the root name
    String rdir = System::getrdir ();
    long   rlen = rdir.length ();
    long   nlen = name.length ();
    // check the name
    if (nlen < rlen) return name;
    if (name.lsubstr (rlen) == rdir) return name.rsubstr (rlen);
    // not a root name
    return name;
  }

  // this procedure rebuilds a path from a directory vector
  static String to_dnam (const Strvec dvec) {
    // get vector length and check
    long dlen = dvec.length ();
    // initialize result
    String result = (dlen == 0) ? "" : dvec.get (0);
    if (dlen <= 1) return result;
    // rebuild path
    for (long i = 1; i < dlen; i++) {
      result = System::join (result, dvec.get (i));
    }
    return result;
  }

  // -------------------------------------------------------------------------
  // - public section                                                        -
  // -------------------------------------------------------------------------

  // normalize a system path

  String Pathname::normalize (const String& path){
    // check for nil first
    if (path.isnil () == true) return path;
    // check for root and remove
    bool   rflg = is_root (path);
    String rpth = rm_root (path);
    // split the path
    String dsep = System::getdsep ();
    String ldir = System::getldir ();
    String udir = System::getudir ();
    Strvec spth = Strvec::split (rpth, dsep);
    // rebuild the string vector
    long slen = spth.length ();
    Strvec npth;
    for (long k = 0; k < slen; k++) {
      String d = spth.get (k);
      if (d.isnil () == true) continue;
      if (d == ldir) continue;
      if (d == udir) {
	if (npth.empty () == false) npth.pop ();
	continue;
      }
      npth.add (d);
    }
    // rebuild the path
    String result = to_dnam (npth) ;
    if (rflg == true) result = System::join (System::getrdir (), result);
    return result;
  }

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a default path name

  Pathname::Pathname (void) {
    reset ();
  }

  // create a path name by file name

  Pathname::Pathname (const String& fnam) {
    d_dvec.reset ();
    setfnam (fnam);
  }

  // create a path name by file and directory name

  Pathname::Pathname (const String& fnam, const String& dnam) {
    reset   ();
    setdnam (dnam);
    setfnam (fnam);
  }

  // copy construct this path name

  Pathname::Pathname (const Pathname& that) {
    that.rdlock ();
    try {
      d_root = that.d_root;
      d_fnam = that.d_fnam;
      d_dvec = that.d_dvec;
      that.unlock ();
    } catch (...) {
      that.unlock ();
      throw;
    }
  }

  //  assign a path name to this one

  Pathname& Pathname::operator = (const Pathname& that) {
    wrlock ();
    that.rdlock ();
    try {
      d_root = that.d_root;
      d_fnam = that.d_fnam;
      d_dvec = that.d_dvec;
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

  String Pathname::repr (void) const {
    return "Pathname";
  }

  // reset this path name

  void Pathname::reset (void) {
    wrlock ();
    try {
      d_root = "";
      d_fnam = "";
      d_dvec.reset ();
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // check if the path is a directory

  bool Pathname::isdir (void) const {
    try {
      bool result = System::isdir (getfull ());
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // check if the path is a file

  bool Pathname::isfile (void) const {
    try {
      bool result = System::isfile (getfull ());
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set the file path name

  void Pathname::setfnam (const String& name) {
    wrlock ();
    try {
      if (is_root (name) == true) {
	// reset directory vector
	d_dvec.reset ();
	// save root name
	d_root = System::getrdir ();
      }
      // compute working name
      String wnam = rm_root (name);
      // split the name and update vector
      Strvec nvec = Strvec::split (wnam, System::getdsep ());
      long   nlen = nvec.length ();
      if (nlen > 0) {
	long   dlen = nlen - 1;
	for (long i = 0; i < dlen; i++) d_dvec.add (nvec.get (i));
	d_fnam = nvec.get (dlen);
      } else {
	d_fnam = "";
      }
      unlock ();
    } catch (...) {
      reset  ();
      unlock ();
      throw;
    }
  }

  // return the file path name
    
  String Pathname::getfnam (void) const {
    rdlock ();
    String result = d_fnam;
    unlock ();
    return result;
  }

  // add a directory path name

  void Pathname::adddnam (const String& name) {
    wrlock ();
    try {
      if (is_root (name) == true) {
	// reset directory vector
	d_dvec.reset ();
	// save root name
	d_root = System::getrdir ();
      }
      // compute working name
      String wnam = rm_root (name);
      // split the name and update vector
      Strvec nvec = Strvec::split (name, System::getdsep ());
      long   nlen = nvec.length ();
      for (long i = 0; i < nlen; i++) {
	String elem = nvec.get (i);
	if (elem.isnil () == false) d_dvec.add (elem);
      }
      unlock ();
    } catch(...) {
      reset  ();
      unlock ();
      throw;
    }
  }

  // set the directory path name

  void Pathname::setdnam (const String& name) {
    wrlock ();
    try {
      if (is_root (name) == true) {
	// reset directory vector
	d_dvec.reset ();
	// save root name
	d_root = System::getrdir ();
      }
      // compute working name
      String wnam = rm_root (name);
      // split the name and update vector
      d_dvec = Strvec::split (wnam, System::getdsep ());
      unlock ();
    } catch(...) {
      reset  ();
      unlock ();
      throw;
    }
  }

  // return the directory path name
    
  String Pathname::getdnam (void) const {
    rdlock ();
    try {
      String result = d_root;
      if (result.isnil () == true) {
	result = to_dnam (d_dvec);
      } else {
	result = System::join (result, to_dnam (d_dvec));
      }
      unlock ();
      return result;
    } catch(...) {
      unlock ();
      throw;
    }
  }

  // return the directory root value
    
  String Pathname::getroot (void) const {
    rdlock ();
    String result = d_root;
    unlock ();
    return result;
  }

  // return the full path name

  String Pathname::getfull (void) const {
    rdlock ();
    try {
      String   dnam = getdnam ();
      String result = dnam.isnil () ? d_fnam : System::join (dnam, d_fnam);
      unlock ();
      return result;
    } catch(...) {
      unlock ();
      throw;
    }
  }

  // return the system path name

  String Pathname::getsysp (void) const {
    rdlock ();
    try {
      String result = getfull ();
      if (d_root.isnil () == true) {
	result = System::join (System::getcwd (), result);
      }
      unlock ();
      return result;
    } catch(...) {
      unlock ();
      throw;
    }
  }

  // return the directory path length
    
  long Pathname::length (void) const {
    rdlock ();
    long result = d_dvec.length ();
    unlock ();
    return result;
  }

  // return the directory path by index
    
  String Pathname::getpath (const long index) const {
    rdlock ();
    try {
      // get the path element
      String result = d_dvec.get (index);
      // special case for index 0
      if ((index == 0) && (d_root.isnil () == false)) {
	result = System::join (d_root, result);
      }
      unlock ();
      return result;
    } catch(...) {
      unlock ();
      throw;
    }
  }

  // add a path by name

  void Pathname::addpath (const String& path) {
    // check for a valid path
    if (path.isnil () == true) return;
    // lock and add
    wrlock ();
    try {
      // check for root
      if (is_root (path) == true) {
	setfnam (path);
	unlock ();
	return;
      }
      // here we are not root, so add it to the directory vector
      Strvec nvec = Strvec::split (path, System::getdsep ());
      long   nlen = nvec.length () -  1;
      for (long i = 0; i < nlen; i++) {
	String elem = nvec.get (i);
	if (elem.isnil () == false) d_dvec.add (elem);
      }
      // process the last component
      String lelm = nvec.get (nlen);
      if (lelm.isnil () == false) {
	if (d_fnam.isnil () == false) d_dvec.add (d_fnam); 
	d_fnam = lelm;
      }
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // normalize a path with respect to its type

  void Pathname::normalize (void) {
    wrlock ();
    try {
      // get the full path
      String path = getfull ();
      // check for a directory
      if (System::isdir (path) == true) {
	if (d_fnam.isnil () == false) d_dvec.add (d_fnam);
	d_fnam = "";
      }
      // check for a file
      if (System::isfile (path) == true) {
	if (d_fnam.isnil () == true) d_fnam = d_dvec.rml ();
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
  static const long QUARK_ZONE_LENGTH = 15;
  static QuarkZone  zone (QUARK_ZONE_LENGTH);

  // the object supported quarks
  static const long QUARK_DIRP    = zone.intern ("dir-p");
  static const long QUARK_FILEP   = zone.intern ("file-p");
  static const long QUARK_RESET   = zone.intern ("reset");
  static const long QUARK_LENGTH  = zone.intern ("length");
  static const long QUARK_GETROOT = zone.intern ("get-root");
  static const long QUARK_GETFULL = zone.intern ("get-full");
  static const long QUARK_GETSYST = zone.intern ("get-system-path");
  static const long QUARK_ADDPATH = zone.intern ("add-path");
  static const long QUARK_GETPATH = zone.intern ("get-path");
  static const long QUARK_GETFNAM = zone.intern ("get-file-name");
  static const long QUARK_SETFNAM = zone.intern ("set-file-name");
  static const long QUARK_GETDNAM = zone.intern ("get-directory-name");
  static const long QUARK_ADDDNAM = zone.intern ("add-directory-name");
  static const long QUARK_SETDNAM = zone.intern ("set-directory-name");
  static const long QUARK_NORMALZ = zone.intern ("normalize");

  // create a new object in a generic way

  Object* Pathname::mknew (Vector* argv) {
    long argc = (argv == nullptr) ? 0 : argv->length ();
    // check for 0 arguments
    if (argc == 0) return new Pathname;
    // check for 1 argument
    if (argc == 1) {
      String fnam = argv->getstring (0);
      return new Pathname (fnam);
    }
    // check for 2 arguments
    if (argc == 2) {
      String fnam = argv->getstring (0);
      String dnam = argv->getstring (1);
      return new Pathname (fnam, dnam);
    }
    throw Exception ("argument-error",
		     "too many argument with pathname constructor");
  }

  // return true if the given quark is defined

  bool Pathname::isquark (const long quark, const bool hflg) const {
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
  
  Object* Pathname::apply (Evaluable* zobj, Nameset* nset, const long quark,
			    Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();
 
    // check for 0 argument
    if (argc == 0) {
      if (quark == QUARK_LENGTH)  return new Integer (length  ());
      if (quark == QUARK_GETFNAM) return new String  (getfnam ());
      if (quark == QUARK_GETDNAM) return new String  (getdnam ());
      if (quark == QUARK_GETROOT) return new String  (getroot ());
      if (quark == QUARK_GETFULL) return new String  (getfull ());
      if (quark == QUARK_GETSYST) return new String  (getsysp ());
      if (quark == QUARK_DIRP)    return new Boolean (isdir   ());
      if (quark == QUARK_FILEP)   return new Boolean (isfile  ());
      if (quark == QUARK_RESET) {
	reset ();
	return nullptr;
      }
      if (quark == QUARK_NORMALZ) {
	normalize ();
	return nullptr;
      }
    }

    // check for 1 argument
    if (argc == 1) {
      if (quark == QUARK_SETFNAM) {
	String fnam = argv->getstring (0);
	setfnam (fnam);
	return nullptr;
      }
      if (quark == QUARK_ADDDNAM) {
	String dnam = argv->getstring (0);
	adddnam (dnam);
	return nullptr;
      }
      if (quark == QUARK_SETDNAM) {
	String dnam = argv->getstring (0);
	setdnam (dnam);
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
    }
    // call the object method 
    return Object::apply (zobj, nset, quark, argv);
  }
}
