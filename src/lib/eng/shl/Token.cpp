// ---------------------------------------------------------------------------
// - Token.cpp                                                               -
// - afnix engine - token class implementation                               -
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

#include "Real.hpp"
#include "Token.hpp"
#include "Regex.hpp"
#include "Strvec.hpp"
#include "Boolean.hpp"
#include "Relatif.hpp"
#include "Lexical.hpp"
#include "Constant.hpp"
#include "Reserved.hpp"
#include "Qualified.hpp"
#include "Character.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - private section                                                       -
  // -------------------------------------------------------------------------

  // bind the reserved keywords
  static Strvec afx_tok_resv;

  // -------------------------------------------------------------------------
  // - public section                                                        -
  // -------------------------------------------------------------------------

  // this procedure adds a reserved keyword
  void Token::mkrsv (const String& name) {
    if (afx_tok_resv.exists (name) == true) return;
    afx_tok_resv.add (name);
  }

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a default token
 
  Token::Token (void) {
    d_type   = Token::ERROR;
    d_lnum   = 0;
    p_object = nullptr;
  }

  // create a token with a type and a line number

  Token::Token (t_token type, const long lnum) {
    d_type   = type;
    d_lnum   = lnum;
    p_object = nullptr;
  }

  // create a token with a type, a value and a line number

  Token::Token (t_token type, const String& value, const long lnum) {
    d_type   = type;
    d_token  = value;
    d_lnum   = lnum;
    
    // create the object value based on the type
    switch (d_type) {
    case Token::LEXICAL:
      if (afx_tok_resv.exists (d_token) == true)
	p_object = new Reserved (d_token, d_lnum);
      else
	p_object = new Lexical (d_token, d_lnum);
      break;
    case Token::QUALIFIED:
      p_object = new Qualified (d_token, d_lnum);
      break;
    case Token::BOOLEAN:
      p_object = new Constant (new Boolean (value));
      break;
    case Token::INTEGER:
      p_object = new Constant (new Integer (value));
      break;
    case Token::RELATIF:
      p_object = new Constant (new Relatif (value));
      break;
    case Token::REAL:
      p_object = new Constant (new Real (value));
      break;
    case Token::STRING:
      p_object = new Constant (new String (value));
      break;
    case Token::CHARACTER:
      p_object = new Constant (new Character (value));
      break;
    case Token::REGEX:
      p_object = new Constant (new Regex (value));
      break;
    default:
      p_object = nullptr;
      break;
    }
    Object::iref (p_object);
  }

  // copy constructor for this token

  Token::Token (const Token& that) {
    that.rdlock ();
    try {
      d_type   = that.d_type;
      d_token  = that.d_token;
      d_lnum   = that.d_lnum;
      p_object = Object::iref (that.p_object);
      that.unlock ();
    } catch (...) {
      that.unlock ();
      throw;
    }
  }

  // destroy this token

  Token::~Token (void) {
    Object::dref (p_object);
  }

  // return the class name

  String Token::repr (void) const {
    return "Token";
  }
  
  // return a clone of this object

  Object* Token::clone (void) const {
    return new Token (*this);
  }

  // assign a token to this one

  Token& Token::operator = (const Token& that) {
    // check for self assignation
    if (this == &that) return *this;
    // lock and assign
    wrlock ();
    that.rdlock ();
    try {
      Object::dref (p_object);
      d_type   = that.d_type;
      d_token  = that.d_token;
      d_lnum   = that.d_lnum;
      p_object = Object::iref (that.p_object);
      unlock ();
      that.unlock ();
      return *this;
    } catch (...) {
      unlock ();
      that.unlock ();
      throw;
    }
  }

  // return the token line number
  
  long Token::getlnum (void) const {
    rdlock ();
    try {
      long result = d_lnum;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return the token type

  Token::t_token Token::gettid (void) const {
    rdlock ();
    try {
      Token::t_token result = d_type;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  //  return the token string value

  String Token::getval (void) const {
    rdlock ();
    try {
      String result = d_token;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
   
  // return the token associated object
  
  Object* Token::getobj (void) const {
    rdlock ();
    try {
      Object* result = (p_object == nullptr) ? nullptr : p_object->clone ();
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
}
