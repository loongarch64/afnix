// ---------------------------------------------------------------------------
// - Secp.hpp                                                                -
// - afnix:sec module - ecc primary field class definition                   -
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

#ifndef  AFNIX_SECP_HPP
#define  AFNIX_SECP_HPP

#ifndef  AFNIX_ECC_HPP
#include "Ecc.hpp"
#endif

namespace afnix {

  /// The Secp class is the class that provides support for elliptic curve
  /// cryptopraphy over a primary field F(p). In this context an ecc curve
  /// is defined as y^2 = x^3 + a.x + b (mod p). The elliptic curve is
  /// defined by a tuple (p, a, b, G, n, h) where p is a prime number, a & b
  /// curve coefficients G a generator point, n the order of G and H the
  /// cofactor.
  /// @author amaury darsch

  class Secp : public Ecc {
  protected:
    /// the prime field
    Relatif d_p;
    /// the curve a coeficient
    Relatif d_a;
    /// the curve b coeficient
    Relatif d_b;
    /// the curve generator
    Ecp d_g;
    /// the curve order
    Relatif d_n;
    /// the curve cofactor
    Relatif d_h;
    
  public:
    /// create a null curve
    Secp (void);
    
    /// create an elliptic curve
    /// @param p the prime field
    /// @param a the a coefficient
    /// @param b the b coefficient
    /// @param g the generator point
    /// @param n the group order
    /// @param h the cofactor
    Secp (const Relatif& p, const Relatif& a, const Relatif& b,
	  const Ecp& g,     const Relatif& n, const Relatif& h);

    /// copy construct this curve
    /// @param that the curve to copy
    Secp (const Secp& that);

    /// assign a curve to this one
    /// @param that the curve to assign
    Secp& operator = (const Secp& that);

    /// @return the class name
    String repr (void) const override;

    /// @return a clone of this object
    Object* clone (void) const override;

    /// add two points
    /// @param px the point argument
    /// @param py the point argument
    Ecp add (const Ecp& px, const Ecp& py) const override;
    
    /// negate a point
    /// @param p the point argument
    Ecp neg (const Ecp& p) const override;

    /// substract two points
    /// @param px the point argument
    /// @param py the point argument
    Ecp neg (const Ecp& px, const Ecp& py) const override;

    /// @return true is the curve is valid
    bool valid (void) const override;

    /// validate a point
    /// @param p the point to validate
    bool valid (const Ecp& p) const override;

    /// @return the curve prime
    virtual Relatif getp (void) const;

    /// @return the curve a coefficient
    virtual Relatif geta (void) const;

    /// @return the curve b coefficient
    virtual Relatif getb (void) const;

    /// @return the curve generator
    virtual Ecp getg (void) const;

    /// @return the curve order
    virtual Relatif getn (void) const;
    
    /// @return the curve cofactor
    virtual Relatif geth (void) const;

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
