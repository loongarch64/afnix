// ---------------------------------------------------------------------------
// - t_character.cpp                                                         -
// - standard object library - character class test module                   -
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

#include "Character.hpp"
#include "Exception.hpp"

int main (int, char**) {
  using namespace afnix;

  // create a default character and check for value
  Character u1;
  if (u1.isnil () == false)    return 1;

  // create an ascii character
  Character u2 ('a');
  if (u2 != 'a')               return 1;
  if (u2.toliteral() != "'a'") return 1;
  if (u2.isalpha () == false)  return 1;
  if (u2.isdigit () == true)   return 1;
  if (u2.isblank () == true)   return 1;

  // create a character from string and compare
  Character u3 ("a");
  Character u4 ("'a'");
  if (u3 != u4) return 1;
  if (u3 != u2) return 1;
  
  // check for operators
  if (u2 <  u1) return 1;
  if (u1 >  u2) return 1;
  if (u2 <= u1) return 1;
  if (u1 >= u2) return 1;

  // create an unicode character
  Character u5 ("U+0061");
  if (u5 != u3) return 1    ;

  // everything is fine
  return 0;
}
