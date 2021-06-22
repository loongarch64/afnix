// ---------------------------------------------------------------------------
// - Literal.hpp                                                             -
// - standard object library - literal abstract class definition             -
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

#ifndef  AFNIX_LITERAL_HPP
#define  AFNIX_LITERAL_HPP

#ifndef  AFNIX_STYLE_HPP
#include "Style.hpp"
#endif

namespace afnix {

  /// The Literal class is an abstract class which is used by object which
  /// have a constant literal representation. such object includes boolean,
  /// integer, relatif, real, character, string and regex. The Literal class 
  /// defines the tostring and toliteral methods as representation methods.
  /// @author amaury darsch

  class Literal : public virtual Serial {
  public:
    /// create a default literal
    Literal (void) =default;
    
    /// copy move this literal
    /// @param that the literal to move
    Literal (Literal&& that) noexcept;

    /// move an literal into this one
    /// @param that the literal to move
    Literal& operator = (Literal&& that) noexcept;
    
    /// clear this literal
    virtual void clear (void) =0;

    /// @return a literal representation of this object
    virtual class String toliteral (void) const =0;

    /// @return a printable representation of this literal
    virtual class String tostring (void) const =0;

    /// @return a style printable representation of this literal
    virtual class String format (const Style& lstl) const;

    /// @return true if both literal are equal
    virtual bool operator == (const Literal& lobj) const;

    /// @return true if both literal are not equal
    virtual bool operator != (const Literal& lobj) const;

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
