// ---------------------------------------------------------------------------
// - Physics.cpp                                                             -
// - afnix:phy service - fundamental physical constants class implementation -
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
#include "Vector.hpp"
#include "Physics.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - private section                                                       -
  // -------------------------------------------------------------------------

  // physics constant parameters
  static const String PN_PHY_C  = "c";
  static const String PI_PHY_C  = "SPEED OF LIGHT IN VACUUM";
  static const t_real PV_PHY_C  = 299792458.0;
  static const String PU_PHY_C  = "m.s^-1";

  static const String PN_PHY_U0 = "µ₀";
  static const String PI_PHY_U0 = "MAGNETIC PERMEABILITY IN VACUUM";
  static const t_real PV_PHY_U0 = 1.25663706212E-6;
  static const String PU_PHY_U0 = "H/m";

  static const String PN_PHY_E0 = "ε₀";
  static const String PI_PHY_E0 = "ELECTRIC PERMITIVITY IN VACUUM";
  static const t_real PV_PHY_E0 = 8.8541878128E-12;
  static const String PU_PHY_E0 = "F/m";

  static const String PN_PHY_G  = "G";
  static const String PI_PHY_G  = "CONSTANT OF GRAVITATION";
  static const t_real PV_PHY_G  = 6.67430E-11;
  static const String PU_PHY_G  = "m^3.kg^−1.s";

  static const String PN_PHY_H  = "h";
  static const String PI_PHY_H  = "PLANCK CONSTANT";
  static const t_real PV_PHY_H  = 6.62607015E-34;
  static const String PU_PHY_H  = "J⋅s";

  static const String PN_PHY_HB = "ħ";
  static const String PI_PHY_HB = "REDUCED PLANCK CONSTANT";
  static const t_real PV_PHY_HB = 1.054571817E-34;
  static const String PU_PHY_HB = "J⋅s";

  static const String PN_PHY_E  = "e";
  static const String PI_PHY_E  = "ELEMENTARY CHARGE";
  static const t_real PV_PHY_E  = 1.602176634E-19;
  static const String PU_PHY_E  = "C";
  
  static const String PN_PHY_NA = "Na";
  static const String PI_PHY_NA = "AVOGADRO CONSTANT";
  static const t_real PV_PHY_NA = 6.02214076E23;
  static const String PU_PHY_NA = "mol^-1";
  
  static const String PN_PHY_KB = "kb";
  static const String PI_PHY_KB = "BOLTZMANN CONSTANT";
  static const t_real PV_PHY_KB = 1.380649E-23;
  static const String PU_PHY_KB = "J.K^-1";

  static const String PN_PHY_ME = "mₑ";
  static const String PI_PHY_ME = "ELECTRON MASS";
  static const t_real PV_PHY_ME = 9.1093837015E-31;
  static const String PU_PHY_ME = "kg";

  static const String PN_PHY_MP = "mₚ";
  static const String PI_PHY_MP = "PROTON MASS";
  static const t_real PV_PHY_MP = 1.67262192369E-27;
  static const String PU_PHY_MP = "kg";

  static const String PN_PHY_MN = "mₙ";
  static const String PI_PHY_MN = "NEUTRON MASS";
  static const t_real PV_PHY_MN = 1.67492749804E-27 ;
  static const String PU_PHY_MN = "kg";
  
  // -------------------------------------------------------------------------
  // - public section                                                        -
  // -------------------------------------------------------------------------

  // the constant names
  const String Physics::CN_C    = PN_PHY_C;
  const String Physics::CN_U0   = PN_PHY_U0;
  const String Physics::CN_E0   = PN_PHY_E0;
  const String Physics::CN_G    = PN_PHY_G;
  const String Physics::CN_H    = PN_PHY_H;
  const String Physics::CN_HB   = PN_PHY_HB;
  const String Physics::CN_E    = PN_PHY_E;
  const String Physics::CN_NA   = PN_PHY_NA;
  const String Physics::CN_KB   = PN_PHY_KB;
  const String Physics::CN_ME   = PN_PHY_ME;
  const String Physics::CN_MP   = PN_PHY_MP;
  const String Physics::CN_MN   = PN_PHY_MN;
  // the constant values
  const t_real Physics::CV_C    = PV_PHY_C;
  const t_real Physics::CV_U0   = PV_PHY_U0;
  const t_real Physics::CV_E0   = PV_PHY_E0;
  const t_real Physics::CV_G    = PV_PHY_G;
  const t_real Physics::CV_H    = PV_PHY_H;
  const t_real Physics::CV_HB   = PV_PHY_HB;
  const t_real Physics::CV_E    = PV_PHY_E;
  const t_real Physics::CV_NA   = PV_PHY_NA;
  const t_real Physics::CV_KB   = PV_PHY_KB;
  const t_real Physics::CV_ME   = PV_PHY_ME;
  const t_real Physics::CV_MP   = PV_PHY_MP;
  const t_real Physics::CV_MN   = PV_PHY_MN;

  // create a physical constant by name

  Physics Physics::create (const String& name) {
    if (name == PN_PHY_C)
      return Physics(PN_PHY_C,  PI_PHY_C,  PV_PHY_C,  PU_PHY_C);
    if (name == PN_PHY_U0)
      return Physics(PN_PHY_U0, PI_PHY_U0, PV_PHY_U0, PU_PHY_U0);
    if (name == PN_PHY_E0)
      return Physics(PN_PHY_E0, PI_PHY_E0, PV_PHY_E0, PU_PHY_E0);
    if (name == PN_PHY_G)
      return Physics(PN_PHY_G,  PI_PHY_G,  PV_PHY_G,  PU_PHY_G);
    if (name == PN_PHY_H)
      return Physics(PN_PHY_H,  PI_PHY_H,  PV_PHY_H,  PU_PHY_H);
    if (name == PN_PHY_HB)
      return Physics(PN_PHY_HB, PI_PHY_HB, PV_PHY_HB, PU_PHY_HB); 
    if (name == PN_PHY_E)
      return Physics(PN_PHY_E,  PI_PHY_E,  PV_PHY_E,  PU_PHY_E); 
    if (name == PN_PHY_NA)
      return Physics(PN_PHY_NA, PI_PHY_NA, PV_PHY_NA, PU_PHY_NA); 
    if (name == PN_PHY_KB)
      return Physics(PN_PHY_KB, PI_PHY_KB, PV_PHY_KB, PU_PHY_KB); 
    if (name == PN_PHY_ME)
      return Physics(PN_PHY_ME, PI_PHY_ME, PV_PHY_ME, PU_PHY_ME); 
    if (name == PN_PHY_MP)
      return Physics(PN_PHY_MP, PI_PHY_MP, PV_PHY_MP, PU_PHY_MP); 
    if (name == PN_PHY_MN)
      return Physics(PN_PHY_MN, PI_PHY_MP, PV_PHY_MN, PU_PHY_MN); 
    // invalid constant
    throw Exception ("physics-error", "cannot create constant", name);
  }
  
  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a constant by name, info, value and unit

  Physics::Physics (const String& name, const String& info, const t_real cval,
		    const String& unit) {
    // preset the unit
    Unit::operator = (Unit::create (unit));
    // update locally
    d_name = name;
    d_info = info;
    d_scal = cval;
    d_unit = unit;
  }

  // copy constucts this object

  Physics::Physics (const Physics& that) {
    that.rdlock ();
    try {
      // copy base unit
      Unit::operator = (that);
      // copy locally
      d_unit = that.d_unit;
      that.unlock ();
    } catch (...) {
      that.unlock ();
      throw;
    }
  }

  // assign an object to this one

  Physics& Physics::operator = (const Physics& that) {
    // check for self assignation
    if (this == &that) return *this;
    // lock and assign
    wrlock ();
    that.rdlock ();
    try {
      // copy base unit
      Unit::operator = (that);
      // copy locally
      d_unit = that.d_unit;
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

  String Physics::repr (void) const {
    return "Physics";
  }

  // return a clone of this object

  Object* Physics::clone (void) const {
    return new Physics (*this);
  }

  // get the physical constant unit

  String Physics::getunit (void) const {
    rdlock ();
    try {
      String result = d_unit;
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
  static const long QUARK_GETUNIT = zone.intern ("get-unit");


  // create a new object in a generic way

  Object* Physics::mknew (Vector* argv) {
    long argc = (argv == nullptr) ? 0 : argv->length ();
    // check for 0 argument
    if (argc == 0) return new Physics;
    // check for 4 arguments
    if (argc == 4) {
      String name = argv->getstring (0);
      String info = argv->getstring (1);
      t_real cval = argv->getreal   (2);
      String unit = argv->getstring (3);
      return new Physics (name, info, cval, unit);
    }
    // wrong arguments
    throw Exception ("argument-error", 
		     "too many arguments with physics constructor");
  }
  
  // return true if the given quark is defined
  
  bool Physics::isquark (const long quark, const bool hflg) const {
    rdlock ();
    try {
      if (zone.exists (quark) == true) {
	unlock ();
	return true;
      }
      bool result = hflg ? Unit::isquark (quark, hflg) : false;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // apply this object with a set of arguments and a quark
  
  Object* Physics::apply (Evaluable* zobj, Nameset* nset, const long quark,
			  Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();
    
    // dispatch 0 argument
    if (argc == 0) {
      if (quark == QUARK_GETUNIT) return new String (getunit ());
    }
    // call the unit method
    return Unit::apply (zobj, nset, quark, argv);
  }
}
