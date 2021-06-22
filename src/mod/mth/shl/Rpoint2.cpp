// ---------------------------------------------------------------------------
// - Rpoint2.cpp                                                             -
// - afnix:mth module - real point 2 implementation                         -
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
#include "Rpoint2.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"

namespace afnix {
  
  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a default point

  Rpoint2::Rpoint2 (void) : Rpoint (2) {
    clear ();
  }
  
  // create a point by coordinate

  Rpoint2::Rpoint2 (const t_real x, const t_real y) : Rpoint (2) {
    clear ();
    p_ptab[0] = x;
    p_ptab[1] = y;
  }

  // copy construct this point

  Rpoint2::Rpoint2 (const Rpoint2& that) {
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

  Rpoint2& Rpoint2::operator = (const Rpoint2& that) {
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

  String Rpoint2::repr (void) const {
    return "Rpoint2";
  }

  // return a clone of this object

  Object* Rpoint2::clone (void) const {
    return new Rpoint2 (*this);
  }

  // return the serial did
  
  t_word Rpoint2::getdid (void) const {
    return SRL_DEOD_MTH;
  }
  
  // return the serial sid

  t_word Rpoint2::getsid (void) const {
    return SRL_PNT2_SID;
  }

  // serialize this point

  void Rpoint2::wrstream (OutputStream& os) const {
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

  void Rpoint2::rdstream (InputStream& is) {
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

  t_real Rpoint2::getx (void) const {
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

  t_real Rpoint2::gety (void) const {
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

  Object* Rpoint2::mknew (Vector* argv) {
    long argc = (argv == nullptr) ? 0 : argv->length ();
    // check for 0 argument
    if (argc == 0) return new Rpoint2;
    // check for 2 arguments
    if (argc == 2) {
      t_real x = argv->getrint (0);
      t_real y = argv->getrint (1);
      return new Rpoint2 (x, y);
    }
    throw Exception ("argument-error", "invalid arguments with point 2"); 
  }

  // return true if the given quark is defined
  
  bool Rpoint2::isquark (const long quark, const bool hflg) const {
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

  Object* Rpoint2::apply (Evaluable* zobj, Nameset* nset, const long quark,
			  Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();
    
    // dispatch 0 argument
    if (argc == 0) {
      if (quark == QUARK_GETX) return new Real (getx ());
      if (quark == QUARK_GETY) return new Real (gety ());
    }
    // call the point method
    return Rpoint::apply (zobj, nset, quark, argv);
  }
}
