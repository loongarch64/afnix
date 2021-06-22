// ---------------------------------------------------------------------------
// - Meta.hpp                                                                -
// - afnix engine - meta class definition                                    -
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

#ifndef  AFNIX_META_HPP
#define  AFNIX_META_HPP

#ifndef  AFNIX_STRING_HPP
#include "String.hpp"
#endif

#ifndef  AFNIX_VECTOR_HPP
#include "Vector.hpp"
#endif

namespace afnix {

  /// The Meta class is a wrapper class for c++ class. The class holds 
  /// a pointer to a static member function which creates the object. The
  /// apply method is used to create an instance wrapped by this object.
  /// @author amaury darsch

  class Meta : public Object {
  public:
    /// the static evaluator
    using t_eval = Object* (*) (Evaluable*, Nameset*, const long);
    /// the static constructor prototype
    using t_meta = Object* (*) (Vector*);

  private:
    /// the static evaluator
    t_eval p_eval;
    /// the static constructor
    t_meta p_func;

  public:
    /// create an empty meta class
    Meta (void);

    /// create a new meta class with an evaluator
    /// @param func the constructor function
    Meta (t_eval func);

    /// create a new meta class with a constructor function
    /// @param func the constructor function
    Meta (t_meta func);

    /// create a new meta class with an evaluator and a constructor
    /// @param feval the evaluator function
    /// @param fmeta the constructor function
    Meta (t_eval feval, t_meta fmeta);

    /// @return the class name
    String repr (void) const;
  
  private:
    // make the copy constructor private
    Meta (const Meta&);
    // make the assignment operator private
    Meta& operator = (const Meta&);  

  public:
    /// evaluate this object by quark
    /// @param zobj  the current evaluable
    /// @param nset  the current nameset
    /// @param quark the quark to evaluate
    Object* eval (Evaluable* zobj, Nameset* nset, const long quark);

    /// apply this object with a set of arguments
    /// @param zobj the current evaluable
    /// @param nset the current nameset
    /// @param args the arguments to apply
    Object* apply (Evaluable* zobj, Nameset* nset, Cons* args);
  };
}

#endif
