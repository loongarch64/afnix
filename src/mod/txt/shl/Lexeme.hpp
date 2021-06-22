// ---------------------------------------------------------------------------
// - Lexeme.hpp                                                              -
// - afnix:txt module - lexeme object class definition                       -
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

#ifndef  AFNIX_LEXEME_HPP
#define  AFNIX_LEXEME_HPP

#ifndef  AFNIX_STRING_HPP
#include "String.hpp"
#endif
 
namespace afnix {
  
  /// A Lexeme object is a literal object that is built by the matcher class
  /// during an input string parsing. A lexeme consists in string (i.e the
  /// lexeme value), a tag and eventually a source name (i.e file name) and 
  /// a source index (line number).

  class Lexeme : public Literal {
  private:
    /// the lexeme value
    String d_lval;
    /// the lexeme tag
    long d_ltag;
    /// the lexeme source
    String d_lsrc;
    /// the lexeme index
    long d_lidx;

  public:
    /// create a default lexeme
    Lexeme (void);

    /// create a lexeme by value
    /// @param lval the value to set
    Lexeme (const String& lval);

    /// create a lexeme by value and tag
    /// @param lval the value to set
    /// @param ltag the lexeme tag 
    Lexeme (const String& lval, const long tag);

    /// copy construct this lexeme
    /// @param that the lexeme the copy
    Lexeme (const Lexeme& that);

    /// @return the class name
    String repr (void) const;

    /// @return a clone of this lexeme
    Object* clone (void) const;

    /// clear this lexeme
    void clear (void);

    /// @return a literal representation of this string
    String toliteral (void) const;

    /// @return a printable representation of this string
    String tostring (void) const;

    /// assign a lexeme to this one
    /// @param lex the lexeme to assign
    Lexeme& operator = (const Lexeme& lex);

    /// set the lexeme value
    /// @param lval the value to set
    void setvalue (const String& lval);

    /// @return the lexeme value
    String getvalue (void) const;

    /// set the lexeme tag
    /// @param ltag the tag to set
    void settag (const long ltag);

    /// @return the lexeme tag
    long gettag (void) const;

    /// set the lexeme source
    /// @param lsrc the source to set
    void setsrc (const String& lsrc);

    /// @return the lexeme source
    String getsrc (void) const;

    /// set the lexeme  index
    /// @param lidx the index to set
    void setidx (const long lidx);

    /// @return the lexeme index
    long getidx (void) const;

  public:
    /// create a new object in a generic way
    /// @param argv the argument vector
    static Object* mknew (Vector* argv);

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
