// ---------------------------------------------------------------------------
// - SinglePart.cpp                                                          -
// - afnix:nwg module - single part content class implementation             -
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
#include "Evaluable.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"
#include "SinglePart.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a create a default single part

  SinglePart::SinglePart (void) {
    p_pbuf = nullptr;
  }
  
  // copy construct this single part

  SinglePart::SinglePart (const SinglePart& that) {
    that.rdlock ();
    try {
      // copy the base
      HeaderPart::operator = (that);
      // copy locally
      p_pbuf = (p_pbuf == nullptr) ? nullptr : new Buffer (*that.p_pbuf);
      Object::iref (p_pbuf);
      that.unlock ();
    } catch (...) {
      that.unlock ();
      throw;
    }
  }

  // destroy this object

  SinglePart::~SinglePart (void) {
    Object::dref (p_pbuf);
  }

  // return the class name
  
  String SinglePart::repr (void) const {
    return "SinglePart";
  }

  // return a  clone of this object

  Object* SinglePart::clone (void) const {
    return new SinglePart (*this);
  }

  // assign a single part to this one

  SinglePart& SinglePart::operator = (const SinglePart& that) {
    // check for self-assignation
    if (this == &that) return *this;
    // lock and assign
    wrlock ();
    that.rdlock ();
    try {
      // assign the base object
      HeaderPart::operator = (that);
      // copy locally
      Object::dref (p_pbuf);
      p_pbuf = (p_pbuf == nullptr) ? nullptr : new Buffer (*that.p_pbuf);
      Object::iref (p_pbuf);
      // done
      unlock ();
      return *this;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set the single part buffer

  void SinglePart::setpbuf (Buffer* pbuf) {
    wrlock ();
    try {
      Object::iref (pbuf);
      Object::dref (p_pbuf);
      p_pbuf = pbuf;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the single part buffer

  Buffer* SinglePart::getpbuf (void) const {
    rdlock ();
    try {
      Buffer* result = p_pbuf;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // convert the single part buffer to a string

  String SinglePart::tostring (void) const {
    rdlock ();
    try {
      String result;
      if (p_pbuf != nullptr) result = p_pbuf->tostring ();
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
  static const long QUARK_GETPBUF  = zone.intern ("get-buffer");
  static const long QUARK_TOSTRING = zone.intern ("to-string");

  // create a new object in a generic way

  Object* SinglePart::mknew (Vector* argv) {
    long argc = (argv == nullptr) ? 0 : argv->length ();

    // check for 0 argument
    if (argc == 0) return new SinglePart ();
    // invalid arguments
    throw Exception ("argument-error", "invalid arguments with multipart");
  }

  // return true if the given quark is defined

  bool SinglePart::isquark (const long quark, const bool hflg) const {
    rdlock ();
    if (zone.exists (quark) == true) {
      unlock ();
      return true;
    }
    bool result = hflg ? HeaderPart::isquark (quark, hflg) : false;
    unlock ();
    return result;
  }

  // apply this object with a set of arguments and a quark

  Object* SinglePart::apply (Evaluable* zobj, Nameset* nset, const long quark,
			     Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();
    
    // dispatch 0 argument
    if (argc == 0) {
      if (quark == QUARK_TOSTRING) return new String (tostring ());
      if (quark == QUARK_GETPBUF) {
	rdlock ();
	try {
	  Object* result = getpbuf ();
	  zobj->post (result);
	  unlock ();
	  return result;
	} catch (...) {
	  unlock ();
	  throw;
	}
      }
    }
    // call the header part method
    return HeaderPart::apply (zobj, nset, quark, argv);
  }
}

