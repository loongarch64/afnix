// ---------------------------------------------------------------------------
// - UriQuery.cpp                                                            -
// - afnix:www module - uri query class implementation                       -
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
#include "Vector.hpp"
#include "Strvec.hpp"
#include "UriQuery.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a default query

  UriQuery::UriQuery (void) {
    reset ();
  }

  // create a uri query by value

  UriQuery::UriQuery (const String& query) {
    reset ();
    parse (query);
  }

  // create a uri query by initial parameter name and value

  UriQuery::UriQuery (const String& name, const String& pval) {
    reset ();
    add (name, pval);
  }

  // return the class name

  String UriQuery::repr (void) const {
    return "UriQuery";
  }

  // reset an uri query information

  void UriQuery::reset (void) {
    wrlock ();
    try {
      Plist::reset ();
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // parse an uri query and update the uri query data structure

  void UriQuery::parse (const String& query) {
    wrlock ();
    try {
      // reset everything
      reset ();
      // check for nothing first
      String qs = query.strip ();
      if (qs.isnil () == true) {
	unlock ();
	return;
      }
      // get the name/pair value
      Strvec npvec = Strvec::split (qs, "&");
      // now split each pair
      long nplen = npvec.length ();
      for (long i = 0; i < nplen; i++) {
	String pair = npvec.get (i);
	Strvec data = Strvec::split (pair, "=");
	long len = data.length ();
	if (len == 0) continue;
	if (len == 1) {
	  String name = Uri::pdecode (data.get (0));
	  String pval = "";
	  add (name, pval);
	  continue;
	}
	if (len == 2) {
	  String name = Uri::pdecode (data.get (0));
	  String pval = Uri::pdecode (data.get (1));
	  add (name, pval);
	  continue;
	}
	throw Exception ("query-error", "invalid query string", query);
      }
      unlock ();
    } catch (...) {
      reset  ();
      unlock ();
      throw;
    }
  }
  
  // return the query string

  String UriQuery::getquery (void) const {
    rdlock ();
    try {
      String result;
      long plen = length ();
      for (long i = 0; i < plen; i++) {
	// get the property
	Property* prop = get (i);
	if (prop == nullptr) continue;
	// get the encoded name/value
	String name = Uri::pencode (prop->getname ());
	String pval = Uri::pencode (prop->getpval ());
	// update the result
	if (result.isnil () == false) result += '&';
	result += name;
	result += '=';
	result += pval;
      }
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // add a path as prefix

  String UriQuery::prefix (const String& path) const {
    rdlock ();
    try {
      String result = path;
      String  query = getquery ();
      // check for existing query in path
      Strvec pvec = Strvec::split(path, "?");
      if (pvec.length() <= 1) {
	if (query.isnil () == false) {
	  result += '?';
	  result += query;
	}
      } else {
	if (query.isnil () == false) {
	  result += '&';
	  result += query;
	}
      }
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
  static const long QUARK_ZONE_LENGTH = 3;
  static QuarkZone  zone (QUARK_ZONE_LENGTH);

  // the object supported quarks
  static const long QUARK_PARSE    = zone.intern ("parse");
  static const long QUARK_PREFIX   = zone.intern ("prefix");
  static const long QUARK_GETQUERY = zone.intern ("get-query");

  // create a new object in a generic way

  Object* UriQuery::mknew (Vector* argv) {
    long argc = (argv == nullptr) ? 0 : argv->length ();

    // check for 0 argument
    if (argc == 0) return new UriQuery;
    // check for 1 argument
    if (argc == 1) {
      String query = argv->getstring (0);
      return new UriQuery (query);
    }
    // check for 2 arguments
    if (argc == 2) {
      String name = argv->getstring (0);
      String pval = argv->getstring (1);
      return new UriQuery (name, pval);
    }
    // invalid arguments
    throw Exception ("argument-error", "invalid arguments with uri query");
  }

  // return true if the given quark is defined

  bool UriQuery::isquark (const long quark, const bool hflg) const {
    rdlock ();
    if (zone.exists (quark) == true) {
      unlock ();
      return true;
    }
    bool result = hflg ? Plist::isquark (quark, hflg) : false;
    unlock ();
    return result;
  }

  // apply this object with a set of arguments and a quark

  Object* UriQuery::apply (Evaluable* zobj, Nameset* nset, const long quark,
			   Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();

    // dispatch 0 argument
    if (argc == 0) {
      if (quark == QUARK_GETQUERY) return new String (getquery ());
    }
    // dispatch 1 argument
    if (argc == 1) {
      if (quark == QUARK_PARSE) {
	String query = argv->getstring (0);
	parse (query);
	return nullptr;
      }
      if (quark == QUARK_PREFIX) {
	String path = argv->getstring (0);
	return new String (prefix (path));
      }
    }
    // call the plist method
    return Plist::apply (zobj, nset, quark, argv);
  }
}
