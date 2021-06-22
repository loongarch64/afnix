// ---------------------------------------------------------------------------
// - CookieJar.cpp                                                           -
// - afnix:nwg module - cookie jar class implementation                      -
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
#include "Strvec.hpp"
#include "Boolean.hpp"
#include "Integer.hpp"
#include "CookieJar.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create an empty jar

  CookieJar::CookieJar (void) {
    reset ();
  }

  // create a cookie by string

  CookieJar::CookieJar (const String& sval) {
    reset ();
    parse (sval);
  }

  String CookieJar::repr (void) const {
    return "CookieJar";
  }

  // reset this cookie jar

  void CookieJar::reset (void) {
    wrlock ();
    try {
      d_cjar.reset ();
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // parse a string header value

  void CookieJar::parse (const String& sval) {
    wrlock ();
    try {
      // check for nothing first
      if (sval.isnil () == true) {
	unlock ();
	return;
      }
      // eventually, remove the quotes
      String ws = sval.rmquote ();
      // split the string with individual components
      Strvec svec = Strvec::split (ws, ';');
      long   slen = svec.length ();
      // loop in the name/value pairs
      for (long k = 0; k < slen; k++) {
	// get the stripped property
	String pair = svec.get(k).strip ();
	if (pair.isnil () == true) continue;
	// create a new cookie and add it the jar
	Cookie cook (pair);
	add (cook);
      }
      // done
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // check if a cookie exists by name

  bool CookieJar::exists (const String& name) const {
    rdlock ();
    try {
      // preset result
      bool result = false;
      // loop in the jar
      long clen = d_cjar.length ();
      for (long k = 0; k < clen; k++) {
	// get a cookie by index
	Cookie* cook = dynamic_cast <Cookie*> (d_cjar.get (k));
	if (cook == nullptr) continue;
	// check by name
	if (cook->getname () == name) {
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

  // get the number of cookies in the jar

  long CookieJar::length (void) const {
    rdlock ();
    try {
      long result = d_cjar.length ();
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // add a cokie in the jar

  void CookieJar::add (const Cookie& cook) {
    wrlock ();
    try {
      // get the cookie name
      String name = cook.getname ();
      // loop in the jar
      long clen = d_cjar.length ();
      for (long k = 0; k < clen; k++) {
	// get a cookie by index
	Cookie* cobj = dynamic_cast <Cookie*> (d_cjar.get (k));
	if (cobj == nullptr) continue;
	// check by name
	if (cobj->getname () == name) {
	  *cobj = cook;
	  unlock ();
	  return;
	}
      }
      // simply add the cookie in the jar
      d_cjar.add (new Cookie (cook));
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get a cookie by index

  Cookie CookieJar::get (const long cidx) const {
    rdlock ();
    try {
      // get the cookie object by index
      Cookie* cobj = dynamic_cast <Cookie*> (d_cjar.get (cidx));
      // map it by copy
      Cookie result = *cobj;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get a cookie by name

  Cookie CookieJar::getcook (const String& name) const {
    rdlock ();
    try {
      // loop in the jar
      long clen = d_cjar.length ();
      for (long k = 0; k < clen; k++) {
	// get a cookie by index
	Cookie* cook = dynamic_cast <Cookie*> (d_cjar.get (k));
	if (cook == nullptr) continue;
	// check by name
	if (cook->getname () == name) {
	  Cookie result = *cook;
	  unlock ();
	  return result;
	}
      }
      throw Exception ("jar-error", "cannot find cookie by name", name);
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get a cookie value by name

  String CookieJar::getcval (const String& name) const {
    rdlock ();
    try {
      // get the cookie by name
      Cookie cook = getcook (name);
      // get the value
      String result = cook.getvalue ();
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
  static const long QUARK_ZONE_LENGTH = 8;
  static QuarkZone  zone (QUARK_ZONE_LENGTH);

  // the object supported quarks
  static const long QUARK_ADD     = zone.intern ("add");
  static const long QUARK_GET     = zone.intern ("get");
  static const long QUARK_RESET   = zone.intern ("reset");
  static const long QUARK_PARSE   = zone.intern ("parse");
  static const long QUARK_LENGTH  = zone.intern ("length");
  static const long QUARK_EXISTSP = zone.intern ("exists-p");
  static const long QUARK_GETCOOK = zone.intern ("get-cookie");
  static const long QUARK_GETCVAL = zone.intern ("get-cookie-value");

  // create a new object in a generic way

  Object* CookieJar::mknew (Vector* argv) {
    long argc = (argv == nullptr) ? 0 : argv->length ();

    // check for 0 argument
    if (argc == 0) return new CookieJar;
    // check for 1 argument
    if (argc == 1) {
      String sval  = argv->getstring (0);
      return new CookieJar (sval);
    }
    // invalid arguments
    throw Exception ("argument-error", "invalid arguments with cookie jar");
  }

  // return true if the given quark is defined

  bool CookieJar::isquark (const long quark, const bool hflg) const {
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

  Object* CookieJar::apply (Evaluable* zobj, Nameset* nset, const long quark,
			 Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();
    
    // dispatch 0 argument
    if (argc == 0) {
      if (quark == QUARK_LENGTH) return new Integer (length ());      
      if (quark == QUARK_RESET) {
	reset ();
	return nullptr;
      }
    }

    // dispatch 1 argument
    if (argc == 1) {
      if (quark == QUARK_GET) {
	long cidx = argv->getlong (0);
	return new Cookie (get (cidx));
      }
      if (quark == QUARK_ADD) {
	Object*  obj = argv->get (0);
	Cookie* cook = dynamic_cast <Cookie*> (obj);
	if (cook == nullptr) {
	  throw Exception ("type-error", "invalid object with cookie add",
			   Object::repr (obj));
	}
	add (*cook);
	return nullptr;
      }
      if (quark == QUARK_PARSE) {
	String sval = argv->getstring (0);
	parse (sval);
	return nullptr;
      }
      if (quark == QUARK_EXISTSP) {
	String name = argv->getstring (0);
	return new Boolean (exists (name));
      }
      if (quark == QUARK_GETCOOK) {
	String name = argv->getstring (0);
	return new Cookie (getcook (name));
      }
      if (quark == QUARK_GETCVAL) {
	String name = argv->getstring (0);
	return new String (getcval (name));
      }
    }
    // call the object method
    return Object::apply (zobj, nset, quark, argv);
  }
}
