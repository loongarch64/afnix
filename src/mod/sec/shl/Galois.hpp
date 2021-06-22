// ---------------------------------------------------------------------------
// - Galois.hpp                                                              -
// - afnix:sec module - galois field arithmetic class definition             -
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

#ifndef  AFNIX_GALOIS_HPP
#define  AFNIX_GALOIS_HPP

#ifndef  AFNIX_RELATIF_HPP
#include "Relatif.hpp"
#endif

namespace afnix {

  /// The Galois class is an arithmetic class designed to perform finite
  /// field arithmetic. The class operates with a relatif which is to be
  /// interpreted as a reduction polynomial. This class is an operating
  /// class, not intended to be used as a number.
  /// @author amaury darsch

  class Galois : public Object {
  protected:
    /// the field polynomial
    Relatif d_poly;

  public:
    /// create a null galois field
    Galois (void);

    /// create a galois field by polynomial
    /// @param poly the reduction polynomial
    Galois (const Relatif& poly);

    /// copy construct this object
    /// @param that the object to copy
    Galois (const Galois& that);

    /// assign an object to this one
    /// @param that the object to assign
    Galois& operator = (const Galois& that);

    /// @return the class name
    String repr (void) const override;

    /// @return a clone of this object
    Object* clone (void) const override;

    /// get the modular of a relatif
    /// @param x the modular argument
    virtual Relatif mod (const Relatif& x) const;
    
    /// add two relatifs in this field
    /// @param x the left handside
    /// @param y the right handside
    virtual Relatif add (const Relatif& x, const Relatif& y) const;
    
    /// multiply two relatifs in this field
    /// @param x the left handside
    /// @param y the right handside
    virtual Relatif mul (const Relatif& x, const Relatif& y) const;
    
    /// gcm multiply two relatifs in this field
    /// @param x the left handside
    /// @param y the right handside
    virtual Relatif gmm (const Relatif& x, const Relatif& y) const;

    /// inverse a relatif in this field
    /// @param x the inverse argument
    virtual Relatif inv (const Relatif& x) const;

    /// divide two relatifs in this field
    /// @param x the left handside
    /// @param y the right handside
    virtual Relatif div (const Relatif& x, const Relatif& y) const;
    
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
