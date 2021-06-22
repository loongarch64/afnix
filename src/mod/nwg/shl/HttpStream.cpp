// ---------------------------------------------------------------------------
// - HttppStream.cpp                                                         -
// - afnix:nwg module - http stream class implementation                     -
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
#include "Utility.hpp"
#include "Evaluable.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"
#include "HttpStream.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a default http stream

  HttpStream::HttpStream (void) {
    p_hpto = nullptr;
  }

  // create a http stream by protocole and stream

  HttpStream::HttpStream (HttpProto* pto, InputStream* is) {
    // clone the http proto
    p_hpto = nullptr;
    if (pto != nullptr) {
      // clone the http proto
      Object::iref (p_hpto = dynamic_cast <HttpProto*> (pto->clone ()));
      // get the content length
      long clen = (p_hpto == nullptr) ? 0 : p_hpto->getclen ();
      // bind the bound stream
      bind (is, clen);
      // check for a stream encoding
      if (p_hpto->isemod () == true) setemod (p_hpto->getemod ());
    }
  }

  // destroy this http stream

  HttpStream::~HttpStream (void) {
    Object::dref (p_hpto);
  }

  // return the class name

  String HttpStream::repr (void) const {
    return "HttpStream";
  }

  // get the http proto object

  HttpProto* HttpStream::getproto (void) const {
    rdlock ();
    try {
      HttpProto* result = p_hpto;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the http protocol media type

  String HttpStream::getmedia (void) const {
    rdlock ();
    try {
      if (p_hpto == nullptr) {
	throw Exception ("http-error", "nil http proto for media type");
      }
      String result = p_hpto->getmedia ();
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
  static const long QUARK_ZONE_LENGTH = 2;
  static QuarkZone  zone (QUARK_ZONE_LENGTH);
  
  // the object supported quarks
  static const long QUARK_GETPROTO = zone.intern ("get-protocol");  
  static const long QUARK_GETMEDIA = zone.intern ("get-media-type");

  // create a new object in a generic way
  
  Object* HttpStream::mknew (Vector* argv) {
    long argc = (argv == nullptr) ? 0 : argv->length ();
    // check for 0 argument
    if (argc == 0) return new HttpStream;
    // check for 2 arguments
    if (argc == 2) {
      // check for a http protocol
      Object*     obj = argv->get (0);
      HttpProto* pobj = dynamic_cast <HttpProto*> (obj);
      if (pobj == nullptr) {
	throw Exception ("type-error",
			 "invalid object for http stream constructor",
			 Object::repr (obj));
      }
      // check for an input stream
      obj = argv->get (1);
      InputStream* is = dynamic_cast <InputStream*> (obj);
      if (is == nullptr) {
	throw Exception ("type-error",
			 "invalid object for http stream constructor",
			 Object::repr (obj));
      }
      return new HttpStream (pobj, is);
    }
    throw Exception ("argument-error", 
		     "invalid arguments with http stream constructor");
  }

  // return true if the given quark is defined

  bool HttpStream::isquark (const long quark, const bool hflg) const {
    rdlock ();
    if (zone.exists (quark) == true) {
      unlock ();
      return true;
    }
    // check the input bound object
    bool result = hflg ? InputBound::isquark (quark, hflg) : false;
    unlock ();
    return result;
  }
  
  // apply this object with a set of arguments and a quark
  
  Object* HttpStream::apply (Evaluable* zobj, Nameset* nset, const long quark,
			     Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();
    
    // dispatch 0 argument
    if (argc == 0) {
      if (quark == QUARK_GETMEDIA) return new String (getmedia  ());
      if (quark == QUARK_GETPROTO) {
	rdlock ();
	try {
	  Object* result = getproto ();
	  zobj->post (result);
	  unlock ();
	  return result;
	} catch (...) {
	  unlock ();
	  throw;
	}
      }
    }
    // call the input bound method
    return InputBound::apply (zobj, nset, quark, argv);
  }
}
