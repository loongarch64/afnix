// ---------------------------------------------------------------------------
// - Atom.cpp                                                                -
// - afnix:phy service - atomic element class implementation                 -
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
#include "Atom.hpp"
#include "Integer.hpp"
#include "Evaluable.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a null atomic element

  Atom::Atom (void) {
    d_anum = 0L;
    d_wght = 0.0;
    d_eion = 0.0;
  }

  // create an atom by name, info, number and properties

  Atom::Atom (const String& name, const String& info,
	      const t_real  prop[3]) : Infoable (name, info) {
    d_anum = (long) prop[0];
    d_wght = prop[1];
    d_eion = prop[2];
  }
  
  // copy constructs this object

  Atom::Atom (const Atom& that) {
    that.rdlock ();
    try {
      // copy base class
      Infoable::operator = (that);
      // copy locally
      d_anum = that.d_anum;
      d_wght = that.d_wght;
      d_eion = that.d_eion;
      that.unlock ();
    } catch (...) {
      that.unlock ();
      throw;
    }
  }

  // assign an object to this one

  Atom& Atom::operator = (const Atom& that) {
    // check for self assignation
    if (this == &that) return *this;
    // lock and assign
    wrlock ();
    that.rdlock ();
    try {
      // assign base class
      Infoable::operator = (that);
      // assign locally
      d_anum = that.d_anum;
      d_wght = that.d_wght;
      d_eion = that.d_eion;
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

  String Atom::repr (void) const {
    return "Atom";
  }

  // return a clone of this object

  Object* Atom::clone (void) const {
    return new Atom (*this);
  }

  // get the atomic number

  long Atom::getanum (void) const {
    rdlock ();
    try {
      long result = d_anum;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the atomic weight

  t_real Atom::getwght (void) const {
    rdlock ();
    try {
      t_real result = d_wght;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the atomic ionisation energy

  t_real Atom::geteion (void) const {
    rdlock ();
    try {
      t_real result = d_eion;
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
  static const long QUARK_GETANUM = zone.intern ("get-atomic-number");
  static const long QUARK_GETWGHT = zone.intern ("get-atomic-weight");
  static const long QUARK_GETEION = zone.intern ("get-ionisation-energy");

  // create a new object in a generic way

  Object* Atom::mknew (Vector* argv) {
    long argc = (argv == nullptr) ? 0 : argv->length ();
    // check for 0 argument
    if (argc == 0) return new Atom;
    // check for 5 arguments
    if (argc == 5) {
      String name = argv->getstring (0);
      String info = argv->getstring (1);
      long   anum = argv->getlong (2);
      t_real wght = argv->getreal (3);
      t_real eion = argv->getreal (4);
      t_real prop[3] = {(t_real) anum, wght, eion};
      return new Atom (name, info, prop);
    }
    // wrong arguments
    throw Exception ("argument-error", 
		     "too many arguments with atom constructor");
  }

  // return true if the given quark is defined

  bool Atom::isquark (const long quark, const bool hflg) const {
    rdlock ();
    try {
      if (zone.exists (quark) == true) {
	unlock ();
	return true;
      }
      bool result = hflg ? Infoable::isquark (quark, hflg) : false;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // apply this object with a set of arguments and a quark

  Object* Atom::apply (Evaluable* zobj, Nameset* nset, const long quark,
			   Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();
    
    // dispatch 0 argument
    if (argc == 0) {
      if (quark == QUARK_GETANUM) return new Integer (getanum ());
      if (quark == QUARK_GETWGHT) return new Real    (getwght ());
      if (quark == QUARK_GETEION) return new Real    (geteion ());
    }
    // call the infoable method
    return Object::apply (zobj, nset, quark, argv);
  }
}
