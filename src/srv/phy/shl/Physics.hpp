// ---------------------------------------------------------------------------
// - Physics.hpp                                                             -
// - afnix:phy service - fundamental physical constants class definition     -
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

#ifndef  AFNIX_PHYSICS_HPP
#define  AFNIX_PHYSICS_HPP

#ifndef  AFNIX_UNIT_HPP
#include "Unit.hpp"
#endif

namespace afnix {

  /// The Physics class is a simple class that extends the unit class to
  /// support physical constants. Most of the time, the constant is expressed
  /// in derived unit. Therefore the class provides an extra "getunit" method
  /// which returns the derived unit.
  /// @author amaury darsch

  class Physics : public Unit {
  public:
    /// the speed of light
    static const String CN_C;
    static const t_real CV_C;
    /// the magnetic permeability
    static const String CN_U0;
    static const t_real CV_U0;
    /// the electric permitivity
    static const String CN_E0;
    static const t_real CV_E0;
    /// the gravitational constant
    static const String CN_G;
    static const t_real CV_G;
    /// the planck constant
    static const String CN_H;
    static const t_real CV_H;
    /// the reduced planck constant
    static const String CN_HB;
    static const t_real CV_HB;
    /// the elementary charge
    static const String CN_E;
    static const t_real CV_E;
    /// the avogadro constant
    static const String CN_NA;
    static const t_real CV_NA;
    /// the boltzmann constant
    static const String CN_KB;
    static const t_real CV_KB;
    /// the electron mass
    static const String CN_ME;
    static const t_real CV_ME;
    /// the proton mass
    static const String CN_MP;
    static const t_real CV_MP;
    /// the neutron mass
    static const String CN_MN;
    static const t_real CV_MN;
    
    /// @return a physical constant by name
    static Physics create (const String& name);
    
  private:
    /// the constant unit
    String d_unit;
    
  public:
    /// create a null constant
    Physics (void) =default;

    /// create a constant by name, info, value and unit
    /// @param name the constant name
    /// @param info the constant info
    /// @param cval the constant value
    /// @param unit the constant unit
    Physics (const String& name, const String& info, const t_real cval,
	     const String& unit);

    /// copy construct this object
    /// @param that the object to copy
    Physics (const Physics& that);

    /// assign an object to this one
    /// @param that the object to assign
    Physics& operator = (const Physics& that);

    /// @return the class name
    String repr (void) const;

    /// @return a clone of this object
    Object* clone (void) const;

    /// @return the physical constant unit
    virtual String getunit (void) const;
    
  public:
    /// create a new object in a generic way
    /// @param argv the argument vector
    static Object* mknew (Vector* argv);
    
    /// @return true if the given quark is defined
    bool isquark (const long quark, const bool hflg) const;
    
    /// apply this object with a set of arguments and a quark
    /// @param zobj  the current evaluable
    /// @param nset  the current nameset    
    /// @param quark the quark to apply these arguments
    /// @param argv  the arguments to apply
    Object* apply (Evaluable* zobj, Nameset* nset, const long quark,
                   Vector* argv);
  };
}

#endif
