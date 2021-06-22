// ---------------------------------------------------------------------------
// - Unit.hpp                                                                -
// - afnix:phy service - si unit class definition                            -
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

#ifndef  AFNIX_UNIT_HPP
#define  AFNIX_UNIT_HPP

#ifndef  AFNIX_INFOABLE_HPP
#include "Infoable.hpp"
#endif

namespace afnix {

  /// The Unit class is a class designed to manage the "systeme international"
  /// unit which contains the fundamental units as well as the derived units.
  /// The class provides also the mechanism to associate prefixes.
  /// @author amaury darsch

  class Unit : public Infoable {
  public:
    /// @return a unit by name
    static Unit create (const String& name);
    
  protected:
    /// the scaling factor
    t_real d_scal;
    /// the descriptor size
    long d_size;
    /// the descriptor array
    struct s_unit* p_desc;

  public:
    /// create a nil unit
    Unit (void);

    /// create a unit by name
    /// @param name the unit name
    Unit (const String& name);

    /// create a unit by name and info
    /// @param name the unit name
    /// @param name the unit info
    Unit (const String& name, const String& info);

    /// create a unit by name, info and descriptor
    /// @param name the unit name
    /// @param name the unit info
    /// @param dval the unit descriptor
    Unit (const String& name, const String& info, const String& dval);

    /// create a unit by name, info, descriptor and scaling factor
    /// @param name the unit name
    /// @param name the unit info
    /// @param dval the unit descriptor
    /// @param scal the unit factor
    Unit (const String& name, const String& info, const String& dval,
	  const t_real  scal);

    /// copy construct this unit
    /// @param that the unit to copy
    Unit (const Unit& that);

    /// destroy this unit
    ~Unit (void);

    /// assign a unit to this one
    /// @param that the unit to assign
    Unit& operator = (const Unit& that);
    
    /// @return the class name
    String repr (void) const;

    /// @return a clone of this object
    Object* clone (void) const;

    /// reset this unit
    virtual void reset (void);

    /// @return the unit scaling factor
    virtual t_real getscal (void) const;

    /// @return the unit si representation
    virtual String tosi (void) const;
    
    /// @return a string representation
    virtual String tostring (void) const;

    /// @return the unit factor
    virtual t_real tofactor (void) const;

    /// parse a unit descriptor 
    /// @param dval the descriptor value
    virtual bool parse (const String& dval);
    
    /// parse a unit descriptor with a constant
    /// @param dval the descriptor value
    /// @param cnst the constant value
    virtual bool parse (const String& dval, const t_real cnst);

    /// compute a unit value
    /// @param uval the unit value
    /// @param unit the unit object
    friend t_real operator * (const t_real uval, const Unit& unit);
    
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
