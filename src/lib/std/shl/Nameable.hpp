// ---------------------------------------------------------------------------
// - Nameable.hpp                                                            -
// - standard object library - nameable abstract class definition            -
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

#ifndef  AFNIX_NAMEABLE_HPP
#define  AFNIX_NAMEABLE_HPP

#ifndef  AFNIX_STRING_HPP
#include "String.hpp"
#endif

namespace afnix {

  /// The Nameable class is an abstract class which define the virtual method
  /// "getname" for a class which can be named. A nameable class  is useful
  /// to put standard name on objects.
  /// @author amaury darsch

  class Nameable : public virtual Object {
  public:
    /// create a default nameable
    Nameable (void) =default;
    
    /// copy move this nameable object
    /// @param that the nameable to move
    Nameable (Nameable&& that) noexcept;

    /// move a nameable into this one
    /// @param that the nameable to move
    Nameable& operator = (Nameable&& that) noexcept;
    
    /// @return the object name
    virtual String getname (void) const =0;
    
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
