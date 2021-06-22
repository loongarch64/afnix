// ---------------------------------------------------------------------------
// - Expressable.hpp                                                         -
// - afnix:wgt module - expressable abstract class definition                -
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

#ifndef  AFNIX_EXPRESSABLE_HPP
#define  AFNIX_EXPRESSABLE_HPP

#ifndef  AFNIX_CONDITIONAL_HPP
#include "Conditional.hpp"
#endif

namespace afnix {

  /// The Expressable class is an abstract class which provides an interface
  /// for conditional object, that is an object capable of evaluating a
  /// conditional expression. It is up to the expressable object to make
  /// sense out of the conditional parameters.
  /// operator argument.
  /// @author amaury darsch

  class Expressable : public virtual Object {
  public:
    /// create a default expressable
    Expressable (void) =default;
    
    /// copy move this expressable object
    /// @param that the expressable to move
    Expressable (Expressable&& that) noexcept;

    /// move a expressable into this one
    /// @param that the expressable to move
    Expressable& operator = (Expressable&& that) noexcept;
    
    /// valuate an expression by operator
    /// @param oper the operator name
    /// @param llhs the literal lhs
    virtual bool valuate (const String& oper, const Literal& llhs) const =0;

    /// valuate an expression by operator
    /// @param oper the operator name
    /// @param llhs the literal lhs
    /// @param lrhs the literal rhs
    virtual bool valuate (const String&  oper,
			  const Literal& llhs, const Literal& lrhs) const =0;

    /// valuate a conditional
    /// @param cond the conditional to valuate
    virtual bool valuate (const Conditional& cond) const;
    
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
