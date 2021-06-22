// ---------------------------------------------------------------------------
// - Comparable.hpp                                                          -
// - standard object library - comparable abstract class definition          -
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

#ifndef  AFNIX_COMPARABLE_HPP
#define  AFNIX_COMPARABLE_HPP

#ifndef  AFNIX_STRING_HPP
#include "String.hpp"
#endif

namespace afnix {

  /// The Comparable class is an abstract class which define the virtual method
  /// "getname" for a class which can be named. A comparable class  is useful
  /// to put standard name on objects.
  /// @author amaury darsch

  class Comparable : public virtual Object {
  public:
    /// create a default comparable
    Comparable (void) =default;
    
    /// copy move this comparable object
    /// @param that the comparable to move
    Comparable (Comparable&& that) noexcept;

    /// move a comparable into this one
    /// @param that the comparable to move
    Comparable& operator = (Comparable&& that) noexcept;
    
    /// @return true on successful compare
    virtual bool equal (Object* cobj) const =0;
    
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
