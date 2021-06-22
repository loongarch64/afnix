// ---------------------------------------------------------------------------
// - Rvector2.cpp                                                            -
// - afnix:mth module - real vector 2 implementation                         -
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

#include "Real.hpp"
#include "Mthsid.hxx"
#include "Vector.hpp"
#include "Rvector2.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"

namespace afnix {
  
  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a default vector

  Rvector2::Rvector2 (void) : Rvector (2) {
    clear ();
  }
  
  // create a vector by component

  Rvector2::Rvector2 (const t_real x, const t_real y) : Rvector (2) {
    clear ();
    p_vtab[0] = x;
    p_vtab[1] = y;
  }

  // copy construct this vector

  Rvector2::Rvector2 (const Rvector2& that) {
    that.rdlock ();
    try {
      Rvector::operator = (that);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }  
  }

  // assign a vector to this one

  Rvector2& Rvector2::operator = (const Rvector2& that) {
    // check for self-assignation
    if (this == &that) return *this;
    // lock and assign
    wrlock ();
    that.rdlock ();
    try {
      Rvector::operator = (that);
      unlock ();
      that.unlock ();
      return *this;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return the class name

  String Rvector2::repr (void) const {
    return "Rvector2";
  }

  // return a clone of this object

  Object* Rvector2::clone (void) const {
    return new Rvector2 (*this);
  }

  // return the serial did
  
  t_word Rvector2::getdid (void) const {
    return SRL_DEOD_MTH;
  }
  
  // return the serial sid

  t_word Rvector2::getsid (void) const {
    return SRL_VEC2_SID;
  }

  // serialize this vector

  void Rvector2::wrstream (OutputStream& os) const {
    rdlock ();
    try {
      // serialize the vector
      Rvector::wrstream (os);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // deserialize this vector

  void Rvector2::rdstream (InputStream& is) {
    wrlock ();
    try {
      // deserialize the vector
      Rvector::rdstream (is);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // get the vector x component

  t_real Rvector2::getx (void) const {
    rdlock ();
    try {
      t_real result = p_vtab[0];
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // get the vector y component

  t_real Rvector2::gety (void) const {
    rdlock ();
    try {
      t_real result = p_vtab[1];
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
  static const long QUARK_GETX = zone.intern ("get-x");
  static const long QUARK_GETY = zone.intern ("get-y");

  // create a new object in a generic way

  Object* Rvector2::mknew (Vector* argv) {
    long argc = (argv == nullptr) ? 0 : argv->length ();
    // check for 0 argument
    if (argc == 0) return new Rvector2;
    // check for 2 arguments
    if (argc == 2) {
      t_real x = argv->getrint (0);
      t_real y = argv->getrint (1);
      return new Rvector2 (x, y);
    }
    throw Exception ("argument-error", "invalid arguments with vector 3"); 
  }

  // return true if the given quark is defined
  
  bool Rvector2::isquark (const long quark, const bool hflg) const {
    rdlock ();
    try {
      if (zone.exists (quark) == true) {
	unlock ();
	return true;
      }
      bool result = hflg ? Rvector::isquark (quark, hflg) : false;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // apply this object with a set of arguments and a quark

  Object* Rvector2::apply (Evaluable* zobj, Nameset* nset, const long quark,
			   Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();
    
    // dispatch 0 argument
    if (argc == 0) {
      if (quark == QUARK_GETX) return new Real (getx ());
      if (quark == QUARK_GETY) return new Real (gety ());
    }
    // call the vector method
    return Rvector::apply (zobj, nset, quark, argv);
  }
}
