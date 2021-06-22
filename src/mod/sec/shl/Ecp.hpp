// ---------------------------------------------------------------------------
// - Ecp.hpp                                                                 -
// - afnix:sec module - elliptic curve point class definition                -
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

#ifndef  AFNIX_ECP_HPP
#define  AFNIX_ECP_HPP

#ifndef  AFNIX_RELATIF_HPP
#include "Relatif.hpp"
#endif

namespace afnix {

  /// The Ecp class is a class that represents an elliptic curve point. The
  /// point is identified by two coordinates. The point at 'infinity' which
  /// is interpreted as a the group neutral element is coded as (0,0). Note
  /// that (0,0) is not a valid point by itself.
  /// @author amaury darsch

  /// the elliptic curve point
  class Ecp : public Object {
  protected:
    /// the x coordinate
    Relatif d_x;
    /// the y coordinate
    Relatif d_y;
    
  public:
    /// create an infinite point
    Ecp (void);
    
    /// create a point by coordinate
    /// @param x the x coordinate
    /// @param y the y coordinate
    Ecp (const Relatif& x, const Relatif& y);
        
    /// copy construct this point
    /// @param that the point to copy
    Ecp (const Ecp& that);

    /// copy move this point
    /// @param that the point to copy
    Ecp (Ecp&& that) noexcept;
    
    /// assign a point to this one
    /// @param that the point to assign
    Ecp& operator = (const Ecp& that);
    
    /// move a point to this one
    /// @param that the point to assign
    Ecp& operator = (Ecp&& that) noexcept;
    
    /// @return the class name
    String repr (void) const override;

    /// @return a clone of this object
    Object* clone (void) const override;

    /// reset this point
    virtual void reset (void);

    /// check if a point is null
    virtual bool isnil (void) const;
    
    /// set the point coordinates
    /// @param x the x coordinate
    /// @param y the y coordinate
    virtual void set (const Relatif& x, const Relatif& y);

    /// set the point x coordinate
    /// @param x the x coordinate
    virtual void setx (const Relatif& x);

    /// @return the point x coordinate
    virtual Relatif getx (void) const;
    
    /// set the point y coordinate
    /// @param y the y coordinate
    virtual void sety (const Relatif& y);

    /// @return the point y coordinate
    virtual Relatif gety (void) const;
    
  public:
    /// create a new object in a generic way
    /// @param argv the argument vector
    static Object* mknew (Vector* argv);
      
    /// @return true if the given quark is defined
    bool isquark (const long quark, const bool hflg) const override;
      
    /// apply this object with a set of arguments and a quark
    /// @param zobj  the current evaluable
    /// @param nset  the current nameset    
    /// @param quark the quark to apply these arguments
    /// @param argv  the arguments to apply
    Object* apply (Evaluable* zobj, Nameset* nset, const long quark,
		   Vector* argv) override;
  };
}

#endif
