// ---------------------------------------------------------------------------
// - t_token.cpp                                                             -
// - afnix engine - token class tester module                                -
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
#include "Lexical.hpp"
#include "Constant.hpp"

int main (int, char**) {
  using namespace afnix;

  // create a simple token
  Token token (Token::LEXICAL, "hello", 0);
  if (token.repr () != "Token") return 1;

  // check for members
  if (token.gettid () != Token::LEXICAL) return 1;
  if (token.getval () != "hello") return 1;
  if (token.getlnum () != 0) return 1;
  Object* obj = token.getobj ();
  if (dynamic_cast <Lexical*> (obj) == nullptr) return 1;

  // check other tokens
  Token tokint (Token::INTEGER, "123", 0);
  if (dynamic_cast <Constant*> (tokint.getobj ()) == nullptr) return 1;
  Token tokreal (Token::REAL, "123.0", 0);
  if (dynamic_cast <Constant*> (tokreal.getobj ()) == nullptr) return 1;
  Token tokstr (Token::STRING, "world", 0);
  if (dynamic_cast <Constant*> (tokstr.getobj ()) == nullptr) return 1;

  // success
  return 0;
}
