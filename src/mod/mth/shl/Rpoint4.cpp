// ---------------------------------------------------------------------------
// - Rpoint4.cpp                                                             -
// - afnix:mth module - real point 4 implementation                          -
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
#include "Rpoint4.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"

namespace afnix {
  
  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a default point

  Rpoint4::Rpoint4 (void) : Rpoint (4) {
    clear ();
  }
  
  // create a point by coordinate

  Rpoint4::Rpoint4 (const t_real x, const t_real y,
		    const t_real z, const t_real w) : Rpoint (4) {
    clear ();
    p_ptab[0] = x;
    p_ptab[1] = y;
    p_ptab[2] = z;
    p_ptab[3] = w;
  }

  // copy construct this point

  Rpoint4::Rpoint4 (const Rpoint4& that) {
    that.rdlock ();
    try {
      Rpoint::operator = (that);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }  
  }

  // assign a point to this one

  Rpoint4& Rpoint4::operator = (const Rpoint4& that) {
    // check for self-assignation
    if (this == &that) return *this;
    // lock and assign
    wrlock ();
    that.rdlock ();
    try {
      Rpoint::operator = (that);
      unlock ();
      that.unlock ();
      return *this;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return the class name

  String Rpoint4::repr (void) const {
    return "Rpoint4";
  }

  // return a clone of this object

  Object* Rpoint4::clone (void) const {
    return new Rpoint4 (*this);
  }

  // return the serial did
  
  t_word Rpoint4::getdid (void) const {
    return SRL_DEOD_MTH;
  }
  
  // return the serial sid

  t_word Rpoint4::getsid (void) const {
    return SRL_PNT4_SID;
  }

  // serialize this point

  void Rpoint4::wrstream (OutputStream& os) const {
    rdlock ();
    try {
      // serialize the point
      Rpoint::wrstream (os);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // deserialize this point

  void Rpoint4::rdstream (InputStream& is) {
    wrlock ();
    try {
      // deserialize the point
      Rpoint::rdstream (is);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // get the point x coordinate

  t_real Rpoint4::getx (void) const {
    rdlock ();
    try {
      t_real result = p_ptab[0];
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // get the point y coordinate

  t_real Rpoint4::gety (void) const {
    rdlock ();
    try {
      t_real result = p_ptab[1];
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // get the point z coordinate

  t_real Rpoint4::getz (void) const {
    rdlock ();
    try {
      t_real result = p_ptab[2];
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // get the point w coordinate

  t_real Rpoint4::getw (void) const {
    rdlock ();
    try {
      t_real result = p_ptab[3];
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

  Object* Rpoint4::mknew (Vector* argv) {
    long argc = (argv == nullptr) ? 0 : argv->length ();
    // check for 0 argument
    if (argc == 0) return new Rpoint4;
    // check for 4 arguments
    if (argc == 4) {
      t_real x = argv->getrint (0);
      t_real y = argv->getrint (1);
      t_real z = argv->getrint (2);
      t_real w = argv->getrint (3);
      return new Rpoint4 (x, y, z, w);
    }
    throw Exception ("argument-error", "invalid arguments with point 4"); 
  }

  // return true if the given quark is defined
  
  bool Rpoint4::isquark (const long quark, const bool hflg) const {
    rdlock ();
    try {
      if (zone.exists (quark) == true) {
	unlock ();
	return true;
      }
      bool result = hflg ? Rpoint::isquark (quark, hflg) : false;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // apply this object with a set of arguments and a quark

  Object* Rpoint4::apply (Evaluable* zobj, Nameset* nset, const long quark,
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
    // call the point method
    return Rpoint::apply (zobj, nset, quark, argv);
  }
}
