// ---------------------------------------------------------------------------
// - Rfi.hpp                                                                 -
// - afnix:mth module - real function interface definitions                  -
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

#ifndef  AFNIX_RFI_HPP
#define  AFNIX_RFI_HPP

#ifndef  AFNIX_OBJECT_HPP
#include "Object.hpp"
#endif

namespace afnix {

  /// The Rfi class is a base class that can acts as a real function. The
  /// function operates with a real and returns a real. A typical example
  /// of such function is a polynomial function. Additionnaly, the single
  /// real function type is also defined here.
  /// @author amaury darsch

  class Rfi : public Object {
  public:
    /// the real function type
    using t_rfi = t_real (*) (const t_real);

  public:
    /// map this object as a functor
    virtual t_real operator () (const t_real x) const;

    /// compute the function by value
    /// @param x the evaluation real point
    virtual t_real compute (const t_real x) const =0;

    /// compute the derivate by value
    /// @param x the evaluation real point
    virtual t_real derivate (const t_real x) const;

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
