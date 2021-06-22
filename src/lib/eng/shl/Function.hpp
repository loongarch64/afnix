// ---------------------------------------------------------------------------
// - Function.hpp                                                            -
// - afnix engine - function class definition                                -
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

#ifndef  AFNIX_FUNCTION_HPP
#define  AFNIX_FUNCTION_HPP

#ifndef  AFNIX_STRING_HPP
#include "String.hpp"
#endif

namespace afnix {

  /// The Function class is a wrapper class for c++ function which do not
  /// require a class implementation. The function instance holds a pointer
  /// to a function which can later be called during the apply call.
  /// @author amaury darsch

  class Function : public Object {
  public:
    /// the standard function prototype
    using t_func = Object* (*) (Evaluable*, Nameset*, Cons*);

  private:
    /// the function to call during apply
    t_func p_func;

  public:
    /// create a default function
    Function (void);

    /// create a new function 
    /// @param func the function to apply
    Function (t_func func);
    
    /// @return the class name
    String repr (void) const;

  private:
    // make the copy constructor private
    Function (const Function&);
    // make the assignment operator private
    Function& operator = (const Function&);

  public:
    /// apply this object with a set of arguments
    /// @param zobj the current evaluable
    /// @param nset the current nameset
    /// @param args the arguments to apply with
    Object* apply (Evaluable* zobj, Nameset* nset, Cons* args);
  };
}

#endif
