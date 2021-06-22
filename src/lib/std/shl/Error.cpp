// ---------------------------------------------------------------------------
// - Error.cpp                                                               -
// - standard object library - error class implementation                    -
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

#include "Error.hpp"
#include "Integer.hpp"
#include "QuarkZone.hpp"
#include "cerr.hpp"

namespace afnix {

  // error code mapping
  const long Error::CODE_OTHR = AFNIX_ERR_OTHR;
  const long Error::CODE_INTR = AFNIX_ERR_INTR;
  const long Error::CODE_PRIV = AFNIX_ERR_PRIV;
  const long Error::CODE_PIPE = AFNIX_ERR_PIPE;
  const long Error::CODE_LONG = AFNIX_ERR_LONG;
  const long Error::CODE_CREF = AFNIX_ERR_CREF;
  const long Error::CODE_UNRE = AFNIX_ERR_UNRE;
  const long Error::CODE_TOUT = AFNIX_ERR_TOUT;
  const long Error::CODE_IUSE = AFNIX_ERR_IUSE;
  const long Error::CODE_POTO = AFNIX_ERR_POTO;
  const long Error::CODE_ADDR = AFNIX_ERR_ADDR;
  const long Error::CODE_UCON = AFNIX_ERR_UCON;
  const long Error::CODE_PRST = AFNIX_ERR_PRST;
  const long Error::CODE_IARG = AFNIX_ERR_IARG;
  
  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a default error

  Error::Error (void) {
    d_code = 0L;
  }
  
  // create a exception with an id and a reason
  
  Error::Error (const String& eid, const String& reason,
		const long code) : Exception (eid, reason) {
    d_code = code;
  }

  // copy construct this object
  
  Error::Error (const Error& that) {
    that.rdlock ();
    try {
      // copy the base object
      Exception::operator = (that);
      // copy locally
      d_code = that.d_code;
      that.unlock ();
    } catch (...) {
      that.unlock ();
      throw;
    }  
  }

  // assign an error to this one
  
  Error& Error::operator = (const Error& that) {
    // check for self-assignation
    if (this == &that) return *this;
    // lock and assign
    wrlock ();
    that.rdlock ();
    try {
      // copy the base object
      Exception::operator = (that);
      // copy locally
      d_code = that.d_code;
      unlock ();
      that.unlock ();
      return *this;
    } catch (...) {
      unlock ();
      that.unlock ();
      throw;
    }
  }
  
  // return the class name

  String Error::repr (void) const {
    return "Error";
  }

  // return a clone of this object

  Object* Error::clone (void) const {
    return new Error (*this);
  }

  // return the exception code

  long Error::getcode (void) const {
    rdlock ();
    try {
      long result = d_code;
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
  static const long QUARK_CODE = zone.intern ("code");

  // return true if the given quark is defined

  bool Error::isquark (const long quark, const bool hflg) const {
    rdlock ();
    try {
      if (zone.exists (quark) == true) {
	unlock ();
	return true;
      }
      bool result = hflg ? Exception::isquark (quark, hflg) : false;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // evaluate an object member by quark

  Object* Error::eval (Evaluable* zobj, Nameset* nset, const long quark) {
    // check the exception first
    if (quark == QUARK_CODE) return new Integer (getcode ());
    // evaluate in the exception
    return Exception::eval (zobj, nset, quark);
  }
}
