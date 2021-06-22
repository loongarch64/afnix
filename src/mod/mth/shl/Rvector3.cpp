// ---------------------------------------------------------------------------
// - Rvector3.cpp                                                            -
// - afnix:mth module - real vector 3 implementation                         -
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
#include "Rvector3.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"

namespace afnix {
  
  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a default vector

  Rvector3::Rvector3 (void) : Rvector (3) {
    clear ();
  }
  
  // create a vector by component

  Rvector3::Rvector3 (const t_real x, const t_real y,
		      const t_real z) : Rvector (3) {
    clear ();
    p_vtab[0] = x;
    p_vtab[1] = y;
    p_vtab[2] = z;
  }

  // copy construct this vector

  Rvector3::Rvector3 (const Rvector3& that) {
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

  Rvector3& Rvector3::operator = (const Rvector3& that) {
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

  String Rvector3::repr (void) const {
    return "Rvector3";
  }

  // return a clone of this object

  Object* Rvector3::clone (void) const {
    return new Rvector3 (*this);
  }

  // return the serial did
  
  t_word Rvector3::getdid (void) const {
    return SRL_DEOD_MTH;
  }
  
  // return the serial sid

  t_word Rvector3::getsid (void) const {
    return SRL_VEC3_SID;
  }

  // serialize this vector

  void Rvector3::wrstream (OutputStream& os) const {
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

  void Rvector3::rdstream (InputStream& is) {
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

  t_real Rvector3::getx (void) const {
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

  t_real Rvector3::gety (void) const {
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
  
  // get the vector z component

  t_real Rvector3::getz (void) const {
    rdlock ();
    try {
      t_real result = p_vtab[2];
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
  static const long QUARK_GETX = zone.intern ("get-x");
  static const long QUARK_GETY = zone.intern ("get-y");
  static const long QUARK_GETZ = zone.intern ("get-z");

  // create a new object in a generic way

  Object* Rvector3::mknew (Vector* argv) {
    long argc = (argv == nullptr) ? 0 : argv->length ();
    // check for 0 argument
    if (argc == 0) return new Rvector3;
    // check for 3 arguments
    if (argc == 3) {
      t_real x = argv->getrint (0);
      t_real y = argv->getrint (1);
      t_real z = argv->getrint (2);
      return new Rvector3 (x, y, z);
    }
    throw Exception ("argument-error", "invalid arguments with vector 3"); 
  }

  // return true if the given quark is defined
  
  bool Rvector3::isquark (const long quark, const bool hflg) const {
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

  Object* Rvector3::apply (Evaluable* zobj, Nameset* nset, const long quark,
			   Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();
    
    // dispatch 0 argument
    if (argc == 0) {
      if (quark == QUARK_GETX) return new Real (getx ());
      if (quark == QUARK_GETY) return new Real (gety ());
      if (quark == QUARK_GETZ) return new Real (getz ());
    }
    // call the vector method
    return Rvector::apply (zobj, nset, quark, argv);
  }
}
