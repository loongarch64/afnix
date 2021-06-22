// ---------------------------------------------------------------------------
// - Oid.cpp                                                                 -
// - afnix:itu module - oid class implementation                             -
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

#include "Oid.hpp"
#include "Vector.hpp"
#include "Utility.hpp"
#include "Integer.hpp"
#include "Evaluable.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a default oid

  Oid::Oid (void) {
    d_nsid = 0;
    p_sidv = nullptr;
  }

  // copy construct this oid

  Oid::Oid (const Oid& that) {
    that.rdlock ();
    try {
      d_nsid = that.d_nsid;
      p_sidv = (d_nsid == 0) ? nullptr : new t_octa[d_nsid];
      for (long i = 0; i < d_nsid; i++) p_sidv[i] = that.p_sidv[i];
      that.unlock ();
    } catch (...) {
      that.unlock ();
      throw;
    }
  }

  // destroy this oid

  Oid::~Oid (void) {
    delete [] p_sidv;
  }

  // return the oid class name

  String Oid::repr (void) const {
    return "Oid";
  }

  // return a clone of this object

  Object* Oid::clone (void) const {
    return new Oid (*this);
  }
  
  // assign an oid to this one

  Oid& Oid::operator = (const Oid& that) {
    // check for self assign
    if (this == &that) return *this;
    // lock and assign
    wrlock ();
    that.rdlock ();
    try {
      delete [] p_sidv;
      d_nsid = that.d_nsid;
      p_sidv = (d_nsid == 0) ? nullptr : new t_octa[d_nsid];
      for (long i = 0; i < d_nsid; i++) p_sidv[i] = that.p_sidv[i];
      unlock ();
      that.unlock ();
      return *this;
    } catch (...) {
      unlock ();
      that.unlock ();
      throw;
    }
  }

  // reset this oid

  void Oid::reset (void) {
    wrlock ();
    try {
      delete [] p_sidv;
      d_nsid = 0;
      p_sidv = nullptr;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the number of sid

  long Oid::length (void) const {
    rdlock ();
    try {
      long result = d_nsid;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get a sid by index

  t_octa Oid::get (const long index) const {
    rdlock ();
    try {
      // check for valid index
      if ((index < 0) || (index >= d_nsid)) {
	throw Exception ("oid-error", "invalid sid index in getsid");
      }
      // get the sid
      t_octa result = p_sidv[index];
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // add a sid to the oid object

  void Oid::add (const t_octa sid) {
    wrlock ();
    try {
      // create a new sid vector
      long nsid = d_nsid + 1;
      t_octa* sidv = new t_octa[nsid];
      for (long i = 0; i < d_nsid; i++) sidv[i] = p_sidv[i];
      // add the new sid
      sidv[d_nsid] = sid;
      // clean the old vector and update
      delete [] p_sidv;
      p_sidv = sidv;
      d_nsid = nsid;
      // unlock and return
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // format an oid as a string

  String Oid::format (void) const {
    rdlock ();
    try {
      String result;
      for (long k = 0; k < d_nsid; k++) {
	if (k > 0) result += '.';
	result += Utility::tostring ((long) p_sidv[k]);
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
  static const long QUARK_ZONE_LENGTH = 4;
  static QuarkZone  zone (QUARK_ZONE_LENGTH);

  // the object supported quarks
  static const long QUARK_ADD    = zone.intern ("add");
  static const long QUARK_GET    = zone.intern ("get");
  static const long QUARK_RESET  = zone.intern ("reset");
  static const long QUARK_LENGTH = zone.intern ("length");
  static const long QUARK_FORMAT = zone.intern ("format");

  // create a new object in a generic way
 
  Object* Oid::mknew (Vector* argv) {
    long argc = (argv == nullptr) ? 0 : argv->length ();
    // create the result oid
    Oid* result = new Oid;
    // loop in the integer arguments
    for (long i = 0; i < argc; i++) {
      // get the associated sid
      t_octa sid = argv->getocta (i);
      // add it to the oid
      result->add (sid);
    }
    // here it is
    return result;
  }
  
  // return true if the given quark is defined
  
  bool Oid::isquark (const long quark, const bool hflg) const {
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

  Object* Oid::apply (Evaluable* zobj, Nameset* nset, const long quark,
		      Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();

    // check for special quark
    if (quark == QUARK_ADD) {
      for (long i = 0; i < argc; i++) {
	t_octa sid = argv->getocta (i);
	add (sid);
      }
      return nullptr;
    }
    // check for 0 argument
    if (argc == 0) {
      if (quark == QUARK_LENGTH) return new Integer (length ());
      if (quark == QUARK_FORMAT) return new String  (format ());
      if (quark == QUARK_RESET) {
	reset ();
	return nullptr;
      }
    }
    // check for 1 argument
    if (argc == 1) {
      if (quark == QUARK_GET) {
	long index = argv->getlong (0);
	t_octa sid = get (index);
	return new Integer (sid);
      }
    }
    // call the object method
    return Object::apply (zobj, nset, quark, argv);
  }
}
