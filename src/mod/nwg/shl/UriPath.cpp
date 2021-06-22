// ---------------------------------------------------------------------------
// - UriPath.cpp                                                             -
// - afnix:www module - uri path mapping class implementation                -
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

#include "Uri.hpp"
#include "Regex.hpp"
#include "Vector.hpp"
#include "System.hpp"
#include "UriPath.hpp"
#include "Pathname.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - private section                                                       - 
  // -------------------------------------------------------------------------
  
  // normalize the root path
  static String to_root (const String& root) {
    // check if the string is a uri
    if (Uri::isuri (root) == false) return Pathname::normalize (root);
    // map it to a uri
    Uri uri (root);
    // get the system directory path
    return System::xdir (uri.getsysp ());
  }

  // normalize the directory index
  static String to_didx (const String& didx) {
    // check if the string is a uri
    if (Uri::isuri (didx) == false) return "";
    // map it to a uri
    Uri uri (didx);
    // get the system directory path
    return System::xname (uri.getsysp ());
  }

  // get a path uri by request uri
  static String to_puri (const String& ruri, const String& auth) {
    // check for a valid uri
    if (Uri::isuri (ruri) == true) {
      // map the uri
      Uri uri (ruri);
      // validate the autorithy
      if ((auth.isnil () == false) && (uri.getauth () != auth)) {
	throw Exception ("uri-error", "cannot validate request uri authority");
      }
      return uri.getpath ();
    }
    // fix the request uri as a path uri
    String puri = ruri;
    Regex  rpth ("(<$a+-_=.%!:@*,;/\\'`~()&^[]{}$<$>>*)(<?#>?$N*)");
    if (rpth == ruri) puri = rpth.getstring (0);
    // check for a valid starting path uri
    if (puri[0] == '/') return Uri::pdecode (puri);
    // relative uri are mapped with respect to the authority
    if (auth.isnil () == true) {
      String suri = "/";
      suri += puri;
      return Uri::pdecode (suri);
    }
    // rebuild a uri from the authority
    Uri uri (auth);
    uri.addpath (ruri);
    return uri.getpath ();
  }

  // normalize a uri path to a string vector
  static Strvec to_vuri (const String ruri, const String& auth) {
    // get the path uri
    String puri = to_puri (ruri, auth);
    // split the uri
    Strvec upth = Strvec::split (puri, "/");
    // rebuild the string vector
    long slen = upth.length ();
    Strvec npth;
    for (long k = 0; k < slen; k++) {
      String d = upth.get (k);
      if (d.isnil () == true) continue;
      if (d == ".") continue;
      if (d == "..") {
	if (npth.empty () == false) npth.pop ();
        continue;
      }
      npth.add (d);
    }
    return npth;
  }

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a default uri path

  UriPath::UriPath (void) {
    d_root = System::getrdir ();
  }

  // create a uri path by root value

  UriPath::UriPath (const String& root) {
    d_root = to_root (root);
    d_didx = to_didx (root);
  }

  // create a uri path by root and authority

  UriPath::UriPath (const String& root, const String& didx) {
    d_root = to_root (root);
    d_didx = Pathname::normalize (didx);
  }

  // create a uri path by root and authority

  UriPath::UriPath (const String& root, const String& didx, 
		    const String& auth) {
    d_root = to_root (root);
    d_didx = Pathname::normalize (didx);
    d_auth = Uri(Uri::nrmname (auth)).getauth ();
  }

  // copy construct this uri path

  UriPath::UriPath (const UriPath& that) {
    that.rdlock ();
    try {
      d_root = that.d_root;
      d_didx = that.d_didx;
      d_auth = that.d_auth;
      that.unlock ();
    } catch (...) {
      that.unlock ();
      throw;
    }
  }
      
  // return the class name

  String UriPath::repr (void) const {
    return "UriPath";
  }

  // return a clone of this object

  Object* UriPath::clone (void) const {
    return new UriPath (*this);
  }

  // assign a uri path to this one

  UriPath& UriPath::operator = (const UriPath& that) {
    // check for self-assignation
    if (this == &that) return *this;
    // lock and assign
    wrlock ();
    that.rdlock ();
    try {
      d_root = that.d_root;
      d_didx = that.d_didx;
      d_auth = that.d_auth;
      unlock ();
      that.unlock ();
      return *this;
    } catch (...) {
      unlock ();
      that.unlock ();
      throw;
    }
  }

  // get the uri path root path

  String UriPath::getroot (void) const {
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

  // get the uri path directory index

  String UriPath::getdidx (void) const {
    rdlock ();
    try {
      String result = d_didx;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the uri path authority

  String UriPath::getauth (void) const {
    rdlock ();
    try {
      String result = d_auth;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // map a request uri to a path
  
  String UriPath::mapruri (const String& ruri) const {
    rdlock ();
    try {
      // normalize the request uri
      Strvec vuri = to_vuri (ruri, d_auth);
      long   vlen = vuri.length ();
      // combine the root path with the request
      String result;
      if (vlen == 0) {
	result = System::join (d_root, d_didx);
      } else {
	// recombine the request uri
	result = d_root;
	for (long k = 0; k < vlen; k++) {
	  result = System::join (result, vuri.get (k));
	}
      }
      unlock ();
      return result;
    } catch (...) {
      unlock();
      throw;
    }
  }

  // normalize a path to an absolute path

  String UriPath::normalize (const String& path) const {
    rdlock ();
    try {
      // create an absolute path
      String apth = System::join (d_root, path);
      // normalize the path
      String result = Pathname::normalize (apth);
      // here it is
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
  static const long QUARK_ZONE_LENGTH = 5;
  static QuarkZone  zone (QUARK_ZONE_LENGTH);

  // the object supported quarks
  static const long QUARK_GETROOT   = zone.intern ("get-root");
  static const long QUARK_GETDIDX   = zone.intern ("get-index");
  static const long QUARK_GETAUTH   = zone.intern ("get-authority");
  static const long QUARK_MAPRURI   = zone.intern ("map-request-uri");
  static const long QUARK_NORMALIZE = zone.intern ("normalize");

  // create a new object in a generic way

  Object* UriPath::mknew (Vector* argv) {
    long argc = (argv == nullptr) ? 0 : argv->length ();

    // check for 0 argument
    if (argc == 0) return new UriPath;
    // check for 1 argument
    if (argc == 1) {
      String root = argv->getstring (0);
      return new UriPath (root);
    }
    // check for 2 arguments
    if (argc == 2) {
      String root = argv->getstring (0);
      String didx = argv->getstring (1);
      return new UriPath (root, didx);
    }
    // check for 3 arguments
    if (argc == 3) {
      String root = argv->getstring (0);
      String didx = argv->getstring (1);
      String auth = argv->getstring (2);
      return new UriPath (root, didx, auth);
    }
    // invalid arguments
    throw Exception ("argument-error", "invalid arguments with uri path");
  }

  // return true if the given quark is defined

  bool UriPath::isquark (const long quark, const bool hflg) const {
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

  Object* UriPath::apply (Evaluable* zobj, Nameset* nset, const long quark,
			  Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();

    // dispatch 0 argument
    if (argc == 0) {
      if (quark == QUARK_GETROOT) return new String (getroot ());
      if (quark == QUARK_GETDIDX) return new String (getdidx ());
      if (quark == QUARK_GETAUTH) return new String (getauth ());
    }
    // dispatch 1 argument
    if (argc == 1) {
      if (quark == QUARK_MAPRURI) {
	String ruri = argv->getstring (0);
	return new String (mapruri (ruri));
      }
      if (quark == QUARK_NORMALIZE) {
	String path = argv->getstring (0);
	return new String (normalize (path));
      }
    }
    // call the object method
    return Object::apply (zobj, nset, quark, argv);
  }
}
