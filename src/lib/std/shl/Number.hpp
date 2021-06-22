// ---------------------------------------------------------------------------
// - Number.hpp                                                              -
// - standard object library - number abstract class definition              -
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

#ifndef  AFNIX_NUMBER_HPP
#define  AFNIX_NUMBER_HPP

#ifndef  AFNIX_LITERAL_HPP
#include "Literal.hpp"
#endif

#ifndef  AFNIX_VIEWABLE_HPP
#include "Viewable.hpp"
#endif

namespace afnix {

  /// The Number class is an abstract class for the afnix number system.
  /// All numbers such like integers, real and relatif are treated under
  /// this base class, especially when it come to format these number. As
  /// a literal object, the base representation is managed by the derived,
  /// while some extra formatting is done here.
  /// @author amaury darsch

  class Number : public Literal, public Viewable {
  public:
    /// check if a string is a decimal number
    static bool isdnum (const String& s);

    /// check if a string is an hexadecimal number
    static bool ishnum (const String& s);
    
  public:
    /// create a default number
    Number (void) =default;
    
    /// copy move this number
    /// @param that the number to move
    Number (Number&& that) noexcept;

    /// move an number into this one
    /// @param that the number to move
    Number& operator = (Number&& that) noexcept;

    /// @return a hexadecimal representation of this number
    virtual String tohexa (void) const;

    /// @return a hexadecimal string of this number
    virtual String tohstr (void) const;
    
    /// @return a formated string in scientific notation
    virtual String format (const bool sflg) const;

    /// @return a formated string based on the precision
    virtual String format (const long psiz) const;

    /// @return a formated string in scientific notation with a precision
    virtual String format (const long psiz, const bool sflg) const;

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
