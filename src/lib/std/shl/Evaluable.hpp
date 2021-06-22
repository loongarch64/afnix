// ---------------------------------------------------------------------------
// - Evaluable.hpp                                                            -
// - standard object library - evaluable abstract class definition            -
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

#ifndef  AFNIX_EVALUABLE_HPP
#define  AFNIX_EVALUABLE_HPP

#ifndef  AFNIX_OBJECT_HPP
#include "Object.hpp"
#endif

namespace afnix {

  /// The Evaluable class is the virtual class used as an interface to the 
  /// interpreter for evaluating object. The class provides a mechanism for
  /// an object to be evaluated in a given context.
  /// @author amaury darsch

  class Evaluable : public virtual Object  {
  public:
    /// create a default evaluable
    Evaluable (void) =default;

    /// @return the evaluable global nameset
    virtual Nameset* getgset (void) const =0;

    /// post an object in this evaluable class
    /// @param object the object to post
    virtual void post (Object* object) =0;

  private:
    // definitively no copy move
    Evaluable (Evaluable&& that) noexcept =delete;
    // definitively no move operator
    Evaluable& operator = (Evaluable&& that) noexcept =delete;

  public:
    /// evaluate an object in this evaluable
    /// @param object the object to evaluate
    virtual Object* eval (Object* object) = 0;

    /// evaluate an object in this evaluable by quark
    /// @param object the object to evaluate
    /// @param quark  the quark to evaluate
    virtual Object* eval (Object* object, const long quark) = 0;
    
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
