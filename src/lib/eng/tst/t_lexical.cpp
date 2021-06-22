// ---------------------------------------------------------------------------
// - t_lexical.cpp                                                           -
// - afnix engine - lexical class tester module                              -
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

#include "Unicode.hpp"
#include "Lexical.hpp"

int main (int, char**) {
  using namespace afnix;

  // create a simple lexical name
  Lexical lex ("hello");
  if (lex.repr () != "Lexical") return 1;

  // check for members
  if (lex.toliteral () != "hello") return 1;
  if (lex.tostring  () != "hello") return 1;
  if (lex.getlnum   () != 0)       return 1;

  // create a unicode lexical
  const char*   cs = "noe\314\210l";
  const t_quad* fs = Unicode::decode (Encoding::EMOD_UTF8, cs);
  Lexical lux (fs);
  
  // check for members
  if (lux.toliteral () != fs) return 1;
  if (lux.tostring  () != fs) return 1;
  if (lux.getlnum   () != 0)  return 1;
  // cleanup
  delete [] fs;

  // success
  return 0;
}
