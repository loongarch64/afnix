// ---------------------------------------------------------------------------
// - Token.hpp                                                               -
// - afnix engine - token class definition                                   -
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

#ifndef  AFNIX_TOKEN_HPP
#define  AFNIX_TOKEN_HPP

#ifndef  AFNIX_STRING_HPP
#include "String.hpp"
#endif

namespace afnix {

  /// The Token class is the class used by the afnix lexer when a token has 
  /// been recognized. The token holds the string value, the line number,
  /// and eventually a constructed object.
  /// @author amaury darsch

  class Token : public Object {
  public:
    /// the various token type
    enum t_token {
      ERROR,
      EOL,
      EOS,
      RFB,
      RFE,
      BFB,
      BFE,
      REAL,
      REGEX,
      STRING,
      LEXICAL,
      BOOLEAN,
      INTEGER,
      RELATIF,
      QUALIFIED,
      CHARACTER
    };

    /// add a new reserved name
    /// @param name the name to add
    static void mkrsv (const String& name);

  private:
    /// the token type
    t_token d_type;
    /// the token string
    String d_token;
    /// the line number
    long d_lnum;
    /// the associated object
    Object* p_object;

  public:
    /// create a null token
    Token (void);

    /// create a new token with a type and line number
    /// @param type the token type
    /// @param line the line number
    Token (t_token type, const long lnum);

    /// create a new token with a type, string value and line number
    /// @param type the token type
    /// @param value the token string value
    /// @param line the line number
    Token (t_token type, const String& name, const long lnum);

    /// copy constructor for this token
    /// @param that the lexical to copy
    Token (const Token& that);

    /// destroy this token
    ~Token (void);

    /// @return the class name
    String repr (void) const override;

    /// @return a clone of this object
    Object* clone (void) const override;

    /// assign a token to this one
    /// @param that the token to assign
    Token& operator = (const Token& that);

    /// @return the token line number
    long getlnum (void) const;

    /// @return the token type
    t_token gettid (void) const;

    /// @return the token string value
    String getval (void) const;

    /// @return the token associated object
    Object* getobj (void) const;
  };
}

#endif
