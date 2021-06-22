// ---------------------------------------------------------------------------
// - t_string.cpp                                                            -
// - standard object library - string class tester                           -
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

#include "String.hpp"
#include "Exception.hpp"

int main (int, char**) {
  using namespace afnix;

  // create a default null string
  String s1;

  // check for length equal to 0
  if (s1.length () != 0) return 1;

  // check for null string definition
  if (s1 != "") return 1;

  // add the magic string into it
  s1 = "hello world";

  // check for the length again
  if (s1.length () != 11) return 1;

  // create a second string
  String s2 = "hello world";

  // check for the length again
  if (s2.length () != 11) return 1;

  // compare the two string
  if ((s1 == s2) != true)            return 1;
  if ((s1 == "hello world") != true) return 1;
  if (s1 != s2)                      return 1;

  // compare for less
  String s3 = "hello";
  if ((s1 < "ifmmp!xpsme") != true) return 1;  
  if ((s3 < "ifmmp!xpsme") != true) return 1;

  // compare for less equal
  if ((s1 <= "ifmmp!xpsme") != true) return 1;  
  if ((s3 <= "ifmmp!xpsme") != true) return 1;
  if ((s1 <= s2)            != true) return 1;

  // compare more greater
  if ((s3 >  "world") != false) return 1;
  if ((s3 >= "world") != false) return 1;
  if ((s3 >  "hell")  != true)  return 1;
  if ((s3 >= "hell")  != true)  return 1;
  if ((s3 >  "hellx") != false) return 1;
  if ((s3 >= "hellx") != false) return 1;
  if ((s3 >  "hello") != false) return 1;
  if ((s3 >= "hello") != true)  return 1;

  // check for concatenation
  String hello = "hello";
  hello = hello + ':' + "world";
  if (hello != "hello:world") return 1;

  // check for character access
  if (s1[0] != 'h') return 1;

  // check for substring
  String label = "@label";
  if (label.rsubstr (1) != "label") return 1;
  String ccase = "lx";
  if (ccase.rsubstr (1) != "x") return 1;

  // check for lfill
  String lf = "hello";
  if (lf.lfill ('0', 10) != "00000hello") return 1;

  // check for quote
  String sq = "'hello'";
  if (sq.rmquote () != "hello") return 1;
  String dq = "\"hello\"";
  if (dq.rmquote () != "hello") return 1;

  //check for normal form (U+00C0 = U+0041 + U+0300)
  String nc = (char) 0xC0;
  String nf = nc.tonfd ();
  // check length
  if (nf.length () != 2) return 1;
  if (nf.ncclen () != 1) return 1;
  // check content
  if (nf[0] != 0x00000041U) return 1;
  if (nf[1] != 0x00000300U) return 1;

  // case insensitive compare
  if (s1.strcic ("Hello World") != true) return 1;

  // everything is fine
  return 0;
}
