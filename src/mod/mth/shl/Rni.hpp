// ---------------------------------------------------------------------------
// - Rni.hpp                                                                 -
// - afnix:mth module - real non linear interface definitions                -
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

#ifndef  AFNIX_RNI_HPP
#define  AFNIX_RNI_HPP

#ifndef  AFNIX_RMI_HPP
#include "Rmi.hpp"
#endif

namespace afnix {

  /// This Rni class is an abstract class that models the behavior of a non
  /// linear system in the form of a computed lhs and rhs. This class is
  /// essentially designed to support the Newton method. The getlhs method
  /// produces a Jacobian matrix while the getrhs method returns the target
  /// vector to solve at a certain point. Both methods operates at a certain
  /// point represented by a vector.
  /// @author amaury darsch

  class Rni : public virtual Object {
  public:
    /// create a default rni
    Rni (void);
    
    /// copy move this rni object
    /// @param that the rni to move
    Rni (Rni&& that) noexcept;

    /// move a rni into this one
    /// @param that the rni to move
    Rni& operator = (Rni&& that) noexcept;

    /// get the system lhs at a vector operand
    /// @param vop the vector operand
    virtual Rmi* getlhs (Rvi* vop) const =0;

    /// get the system rhs at a vector operand
    /// @param vop the vector operand
    virtual Rvi* getrhs (Rvi* vop) const =0;

  public:
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
