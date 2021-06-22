// ---------------------------------------------------------------------------
// - Rvector4.cpp                                                            -
// - afnix:mth module - real vector 4 implementation                         -
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
#include "Rvector4.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"

namespace afnix {
  
  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a default vector

  Rvector4::Rvector4 (void) : Rvector (4) {
    clear ();
  }
  
  // create a vector by component

  Rvector4::Rvector4 (const t_real x, const t_real y,
		      const t_real z, const t_real w) : Rvector (4) {
    clear ();
    p_vtab[0] = x;
    p_vtab[1] = y;
    p_vtab[2] = z;
    p_vtab[3] = w;
  }

  // copy construct this vector

  Rvector4::Rvector4 (const Rvector4& that) {
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

  Rvector4& Rvector4::operator = (const Rvector4& that) {
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

  String Rvector4::repr (void) const {
    return "Rvector4";
  }

  // return a clone of this object

  Object* Rvector4::clone (void) const {
    return new Rvector4 (*this);
  }

  // return the serial did
  
  t_word Rvector4::getdid (void) const {
    return SRL_DEOD_MTH;
  }
  
  // return the serial sid

  t_word Rvector4::getsid (void) const {
    return SRL_VEC4_SID;
  }

  // serialize this vector

  void Rvector4::wrstream (OutputStream& os) const {
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

  void Rvector4::rdstream (InputStream& is) {
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

  t_real Rvector4::getx (void) const {
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

  t_real Rvector4::gety (void) const {
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

  t_real Rvector4::getz (void) const {
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
  
  // get the vector w component

  t_real Rvector4::getw (void) const {
    rdlock ();
    try {
      t_real result = p_vtab[3];
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
  static const long QUARK_GETX = zone.intern ("get-x");
  static const long QUARK_GETY = zone.intern ("get-y");
  static const long QUARK_GETZ = zone.intern ("get-z");
  static const long QUARK_GETW = zone.intern ("get-w");

  // create a new object in a generic way

  Object* Rvector4::mknew (Vector* argv) {
    long argc = (argv == nullptr) ? 0 : argv->length ();
    // check for 0 argument
    if (argc == 0) return new Rvector4;
    // check for 4 arguments
    if (argc == 4) {
      t_real x = argv->getrint (0);
      t_real y = argv->getrint (1);
      t_real z = argv->getrint (2);
      t_real w = argv->getrint (3);
      return new Rvector4 (x, y, z, w);
    }
    throw Exception ("argument-error", "invalid arguments with vector 4"); 
  }

  // return true if the given quark is defined
  
  bool Rvector4::isquark (const long quark, const bool hflg) const {
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

  Object* Rvector4::apply (Evaluable* zobj, Nameset* nset, const long quark,
			   Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();
    
    // dispatch 0 argument
    if (argc == 0) {
      if (quark == QUARK_GETX) return new Real (getx ());
      if (quark == QUARK_GETY) return new Real (gety ());
      if (quark == QUARK_GETZ) return new Real (getz ());
      if (quark == QUARK_GETW) return new Real (getw ());
    }
    // call the vector method
    return Rvector::apply (zobj, nset, quark, argv);
  }
}
