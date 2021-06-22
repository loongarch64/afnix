// ---------------------------------------------------------------------------
// - Rpoint3.cpp                                                             -
// - afnix:mth module - real point 3 implementation                          -
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
#include "Rpoint3.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"

namespace afnix {
  
  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a default point

  Rpoint3::Rpoint3 (void) : Rpoint (3) {
    clear ();
  }
  
  // create a point by coordinate

  Rpoint3::Rpoint3 (const t_real x, const t_real y,
		    const t_real z) : Rpoint (3) {
    clear ();
    p_ptab[0] = x;
    p_ptab[1] = y;
    p_ptab[2] = z;
  }

  // copy construct this point

  Rpoint3::Rpoint3 (const Rpoint3& that) {
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

  Rpoint3& Rpoint3::operator = (const Rpoint3& that) {
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

  String Rpoint3::repr (void) const {
    return "Rpoint3";
  }

  // return a clone of this object

  Object* Rpoint3::clone (void) const {
    return new Rpoint3 (*this);
  }

  // return the serial did
  
  t_word Rpoint3::getdid (void) const {
    return SRL_DEOD_MTH;
  }
  
  // return the serial sid

  t_word Rpoint3::getsid (void) const {
    return SRL_PNT3_SID;
  }

  // serialize this point

  void Rpoint3::wrstream (OutputStream& os) const {
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

  void Rpoint3::rdstream (InputStream& is) {
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

  t_real Rpoint3::getx (void) const {
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

  t_real Rpoint3::gety (void) const {
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

  t_real Rpoint3::getz (void) const {
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

  Object* Rpoint3::mknew (Vector* argv) {
    long argc = (argv == nullptr) ? 0 : argv->length ();
    // check for 0 argument
    if (argc == 0) return new Rpoint3;
    // check for 3 arguments
    if (argc == 3) {
      t_real x = argv->getrint (0);
      t_real y = argv->getrint (1);
      t_real z = argv->getrint (2);
      return new Rpoint3 (x, y, z);
    }
    throw Exception ("argument-error", "invalid arguments with point 3"); 
  }

  // return true if the given quark is defined
  
  bool Rpoint3::isquark (const long quark, const bool hflg) const {
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

  Object* Rpoint3::apply (Evaluable* zobj, Nameset* nset, const long quark,
			  Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();
    
    // dispatch 0 argument
    if (argc == 0) {
      if (quark == QUARK_GETX) return new Real (getx ());
      if (quark == QUARK_GETY) return new Real (gety ());
      if (quark == QUARK_GETZ) return new Real (getz ());
    }
    // call the point method
    return Rpoint::apply (zobj, nset, quark, argv);
  }
}
