// ---------------------------------------------------------------------------
// - Lexer.hpp                                                               -
// - afnix engine - lexical analyzer class definition                        -
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

#ifndef  AFNIX_LEXER_HPP
#define  AFNIX_LEXER_HPP

#ifndef  AFNIX_BUFFER_HPP
#include "Buffer.hpp"
#endif

#ifndef  AFNIX_TOKEN_HPP
#include "Token.hpp"
#endif

#ifndef  AFNIX_INPUTSTREAM_HPP
#include "InputStream.hpp"
#endif

namespace afnix {

  /// The Lexer class implements the functionality of a lexical analyzer for 
  /// the afnix system. The lexical analyzer consumes character from an input 
  /// stream and generate the appropriate object when one has been constructed.
  /// @author amaury darsch

  class Lexer : public virtual Object {
  private:
    /// the current line number
    long   d_lnum;
    /// the accumulation buffer
    Buffer d_cbuf;
    /// the input stream
    InputStream* p_is;

  public:
    /// create a default lexical analyzer
    Lexer (void);

    /// create a new lexical analyzer with a string
    /// @param sval the string to parse
    Lexer (const String& sval);

    /// create a new lexical analyzer
    /// @param is the input stream
    Lexer (InputStream* is);

    /// destroy this lexer
    ~Lexer (void);

    /// @return the class name
    String repr (void) const;

    /// @return the next token from this stream
    Token get (void);

    /// @return the lexer line number
    long getlnum (void) const;

  private:
    // make the copy constructor private
    Lexer (const Lexer&);
    // make the assignement operator private
    Lexer& operator = (const Lexer&);

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
