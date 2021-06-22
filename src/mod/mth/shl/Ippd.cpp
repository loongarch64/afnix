// ---------------------------------------------------------------------------
// - Ippd.cpp                                                             -
// - afnix:mth module - integer plane point class implementation             -
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

#include "Ippd.hpp"
#include "Vector.hpp"
#include "Integer.hpp"
#include "Evaluable.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"
 
namespace afnix {

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a default point

  Ippd::Ippd (void) {
    d_x = 0L;
    d_y = 0L;
  }
  
  // create a point by coordinates

  Ippd::Ippd (const long x, const long y) {
    d_x = x;
    d_y = y;
  }

  // copy construct this object

  Ippd::Ippd (const Ippd& that) {
    that.rdlock ();
    try {
      d_x = that.d_x;
      d_y = that.d_y;
      that.unlock ();
    } catch (...) {
      that.unlock ();
      throw;
    }
  }

  // return the class name
  
  String Ippd::repr (void) const {
    return "Ippd";
  }
  
  // return a clone of this object
  
  Object* Ippd::clone (void) const {
    return new Ippd (*this);
  }

  // assign an object to this one

  Ippd& Ippd::operator = (const Ippd& that) {
    // check for self assignation
    if (this == &that) return *this;
    // lock and assign
    wrlock ();
    that.rdlock ();
    try {
      d_x = that.d_x;
      d_y = that.d_y;
      unlock ();
      that.unlock ();
      return *this;
    } catch (...) {
      unlock ();
      that.unlock ();
      throw;
    }
  }

  // set the point x coordinate

  void Ippd::setx (const long x) {
    wrlock ();
    try {
      d_x = x;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the point x coordinate

  long Ippd::getx (void) const {
    rdlock ();
    try {
      long result = d_x;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set the point y coordinate

  void Ippd::sety (const long y) {
    wrlock ();
    try {
      d_y = y;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the point y coordinate

  long Ippd::gety (void) const {
    rdlock ();
    try {
      long result = d_y;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      
      throw;
    }
  }

  // set the point coordinates

  void Ippd::set (const long x, const long y) {
    wrlock ();
    try {
      d_x = x;
      d_y = y;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // -------------------------------------------------------------------------
  // - object section                                                        -
  // -------------------------------------------------------------------------

  // the quark zone
  static const long QUARK_ZONE_LENGTH = 5;
  static QuarkZone  zone (QUARK_ZONE_LENGTH);

  // the object supported quarks
  static const long QUARK_SET  = zone.intern ("set");
  static const long QUARK_SETX = zone.intern ("set-x");
  static const long QUARK_GETX = zone.intern ("get-x");
  static const long QUARK_SETY = zone.intern ("set-y");
  static const long QUARK_GETY = zone.intern ("get-y");

  // create a new object in a generic way
  
  Object* Ippd::mknew (Vector* argv) {
    long argc = (argv == nullptr) ? 0 : argv->length ();
    
    // check for 0 argument
    if (argc == 0) return new Ippd;
    // check for 2 arguments
    if (argc == 2) {
      long x = argv->getlong (0);
      long y = argv->getlong (1);
      return new Ippd (x, y);
    }
    // invalid arguments
    throw Exception ("argument-error", 
		     "invalid arguments with plane point object");
  }

  // return true if the given quark is defined

  bool Ippd::isquark (const long quark, const bool hflg) const {
    rdlock ();
    if (zone.exists (quark) == true){
      unlock ();
      return true;
    }
    bool result = hflg ? Object::isquark (quark, hflg) : false;
    unlock ();
    return result;
  }
  
  // apply this object with a set of arguments and a quark
  
  Object* Ippd::apply (Evaluable* zobj, Nameset* nset, const long quark,
		       Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();
    
    // dispatch 0 argument
    if (argc == 0) {
      if (quark == QUARK_GETX) return new Integer (getx ());
      if (quark == QUARK_GETY) return new Integer (gety ());
    }
    // dispatch 1 argument
    if (argc == 1) {
      if (quark == QUARK_SETX) {
	long x = argv->getlong (0);
	setx (x);
	return nullptr;
      }
      if (quark == QUARK_SETY) {
	long y = argv->getlong (0);
	sety (y);
	return nullptr;
      }
    }
    // dispatch 2 arguments
    if (argc == 2) {
      if (quark == QUARK_SET) {
	long x = argv->getlong (0);
	long y = argv->getlong (1);
	set (x, y);
	return nullptr;
      }
    }
    // call the object
    return Object::apply (zobj, nset, quark, argv);
  }
}
