// ---------------------------------------------------------------------------
// - Ecc.hpp                                                                 -
// - afnix:sec module - elliptic curve cryptography  class definition        -
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

#ifndef  AFNIX_ECC_HPP
#define  AFNIX_ECC_HPP

#ifndef  AFNIX_ECP_HPP
#include "Ecp.hpp"
#endif

namespace afnix {

  /// The Ecc class is the base that provides support for elliptic curve
  /// cryptopraphy. The class provides the methods for ecc arithmetic.
  /// @author amaury darsch

  class Ecc : public Object {
  public:
    /// add two points
    /// @param px the point argument
    /// @param py the point argument
    virtual Ecp add (const Ecp& px, const Ecp& py) const =0;
    
    /// negate a point
    /// @param p the point argument
    virtual Ecp neg (const Ecp& p) const =0;

    /// substract two points
    /// @param px the point argument
    /// @param py the point argument
    virtual Ecp neg (const Ecp& px, const Ecp& py) const =0;

    /// multiply a point by a scalar
    /// @param s the scalar argument
    /// @param p the point  argument
    virtual Ecp mul (const Relatif& s, const Ecp& p) const;

    /// @return true is the curve is valid
    virtual bool valid (void) const =0;

    /// validate a point
    /// @param p the point to validate
    virtual bool valid (const Ecp& p) const =0;
    
  public:
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
