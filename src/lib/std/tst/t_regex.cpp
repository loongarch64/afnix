// ---------------------------------------------------------------------------
// - t_regex.cpp                                                             -
// - standard object library - regex class tester                            -
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

#include "Regex.hpp"
#include "String.hpp"
#include "Unicode.hpp"
#include "InputString.hpp"

// simple char testing
static bool re_check_char (void) {
  using namespace afnix;

  // simple bracket notation
  if (Regex ("[a]") != "a")  return false;
  if (Regex ("[a]") != "a")  return false;
  if (Regex ("[a]") == "b")  return false;
  if (Regex ("[a]") == "ab") return false;

  // direct notation
  if (Regex ("a") != "a")    return false;
  if (Regex ("a") != "a")    return false;
  if (Regex ("a") == "b")    return false;
  if (Regex ("a") == "ab")   return false;
  return true;
}

// simple meta-character testing
static bool re_check_meta (void) {
  using namespace afnix;

  if (Regex ("[$a]") != "0")  return false;
  if (Regex ("[$a]") != "9")  return false;
  if (Regex ("[$a]") != "a")  return false;
  if (Regex ("[$a]") != "z")  return false;
  if (Regex ("[$a]") != "A")  return false;

  if (Regex ("[$d]") != "0")  return false;
  if (Regex ("[$d]") != "9")  return false;

  if (Regex ("[$l]") != "a")  return false;
  if (Regex ("[$l]") != "z")  return false;

  if (Regex ("[$s]") != "a")  return false;
  if (Regex ("[$s]") != "z")  return false;
  if (Regex ("[$s]") != "A")  return false;
  if (Regex ("[$s]") != "Z")  return false;

  if (Regex ("[$u]") != "A")  return false;
  if (Regex ("[$u]") != "Z")  return false;

  if (Regex ("[$k]") != "[")  return false;
  if (Regex ("[$k]") != "]")  return false;
  return true;
}

// simple string testing
static bool re_check_strg (void) {
  using namespace afnix;

  // continuous test
  if (Regex ("\"hello\"")   != "hello") return false;
  if (Regex ("[\"hello\"]") != "hello") return false;
  return true;
}

// simple + operator check
static bool re_check_plus (void) {
  using namespace afnix;

  // simple digit first
  if (Regex ("0+") != "0")  return false;
  if (Regex ("0+") != "00") return false;
  if (Regex ("0+") == "01") return false;

  // control digit now
  if (Regex ("[$d+]") != "0")   return false;
  if (Regex ("[$d+]") != "01")  return false;
  if (Regex ("[$d+]") == "0A")  return false;
  if (Regex ("[$d+]") == "01A") return false;

  // block control
  if (Regex ("[$d[$d]+]")     != "00")    return false;
  if (Regex ("[$d[$d]+]")     != "2000")  return false;
  if (Regex ("$d$d[$d$d]+")   != "2000")  return false;
  if (Regex ("[$d$d[$d$d]+]") != "2000")  return false;
  if (Regex ("$d$d[$d$d]+")   == "20000") return false;

  // simple backtracking
  if (Regex ("[$d+$d]")       != "2000") return false;
  if (Regex ("$d+$d$d")       != "2000") return false;
  if (Regex ("[$d$d]+$d$d")   != "2000") return false;
  if (Regex ("$d+0")          == "11")   return false;

  // complex backtracking
  if (Regex ("[[[a]+[b]+]+[c]+]")   != "aabbabcc")    return false;
  if (Regex ("[[[a|c]+[b]+]+[c]+]") != "aabbabccabc") return false;
  return true;
}

// simple * operator check
static bool re_check_mult (void) {
  using namespace afnix;

  // simple digit first
  if (Regex ("0*") != "")    return false;
  if (Regex ("0*") != "0")   return false;
  if (Regex ("0*") == "01")  return false;

  // simple digit first
  if (Regex ("$d*") != "")   return false;
  if (Regex ("$d*") != "0")  return false;
  if (Regex ("$d*") != "01") return false;
  return true;
}

// simple ? operator check
static bool re_check_zone (void) {
  using namespace afnix;

  // simple digit first
  if (Regex ("0?") != "")       return false;
  if (Regex ("0?") != "0")      return false;
  if (Regex ("0?") == "01")     return false;
  
  // simple number
  if (Regex ("-?$d+") != "0")   return false;
  if (Regex ("-?$d+") != "-1")  return false;
  if (Regex ("-?$d+") != "-10") return false;
  return true;
}

// simple | operator check
static bool re_check_altn (void) {
  using namespace afnix;

  // simple digit first
  if (Regex ("0|1") != "0")     return false;
  if (Regex ("0|1") != "1")     return false;
  if (Regex ("0|1") == "a")     return false;

  // simple block check
  if (Regex ("[0]|[1]")      != "0")     return false;
  if (Regex ("$d+|[hello]")  != "hello") return false;
  if (Regex ("$d+|hello")    != "1ello") return false;
  if (Regex ("[$d+|h]ello")  != "1ello") return false;
  if (Regex ("[$d+|h]?ello") != "1ello") return false;
  if (Regex ("[$d+|h]?ello") != "hello") return false;
  if (Regex ("[$d+|h]?ello") != "ello")  return false;

  // root alternate testing
  if (Regex ("[$d+]|[0x$x+]") != "0x2000") return false;
  return true;
}


// group matching check
static bool re_check_group (void) {
  using namespace afnix;

  // simple date check
  Regex re ("($d$d):($d$d):($d$d)");
  if (re != "12:31:00")      return false;
  if (re.length ()  != 3)    return false;
  if (re.getstring (0) != "12") return false;
  if (re.getstring (1) != "31") return false;
  if (re.getstring (2) != "00") return false;
  return true;
}

// check for character set
static bool re_check_cset (void) {
  using namespace afnix;

  // simple set first
  if (Regex ("<$d+-*/>+") != "1+2")       return false;
  if (Regex ("<$a+-*/>+") != "a-b+c*d/0") return false;
  if (Regex ("<$d+-*/>+") == "a+b")       return false;
  if (Regex ("<$a+-*/>+") == "a~b")       return false;
  
  // complement set
  if (Regex ("<^$d>+")     != "a+b")       return false;
  if (Regex ("<^$d+-*/>+") == "1+2")       return false;
  if (Regex ("<^$a+-*/>+") == "a-b+c*d/0") return false;  
  return true;
}

// check for a real number
static bool re_check_real (void) {
  using namespace afnix;

  // simple real
  if (Regex ("$d*.$d+") != ".0")     return false;
  if (Regex ("$d*.$d+") != "2000.0") return false;
  if (Regex ("$d*.$d+") == ".a")     return false;
  if (Regex ("$d*.$d+") == "a.0")    return false;
  return true;
}  

// check for combining characters
static bool re_check_can (void) {
  using namespace afnix;

  // prepare the test string
  const char*   cs = "noe\314\210l";
  const t_quad* fs = Unicode::decode (Encoding::EMOD_UTF8, cs);
  String xmas (fs);
  delete [] fs;
  // combining test
  if (Regex ("$c+") != xmas) return false;
  if (Regex ("$a$a$a$c$a") != xmas) return false;
  return true;
}

// check for a complex url
static bool re_check_url (void) {
  using namespace afnix;

  // afnix uri
  if (Regex ("($l$a+):(<$a-_.!~*'()/>+):($d+)") != 
      "http://www.afnix.org:80") return false;
  if (Regex ("($l$a+):(<$a-_.!~*'()/>+):($d+)") == 
      "http://www.afnix.org") return false;
  return true;
}

// check for an input stream
static bool re_check_is (void) {
  using namespace afnix;
  // separator test
  Regex       r0 ("<$b$n>+");
  InputString is ("\n");
  if (r0.match (&is) != "\n") return false;

  // simple digit
  Regex r1 ("$d+");
  is.set ("0");
  if (r1.match (&is) != "0") return false;
  is.set ("2000");
  if (r1.match (&is) != "2000") return false;
  is.set ("2000 hello world");
  if (r1.match (&is) != "2000") return false;

  // afnix qualified names
  Regex r2 ("$v+[:$v+]+");
  is.set ("interp:library \"afnix-txt\"");
  if (r2.match (&is) != "interp:library") return false;
  is.set (":library \"afnix-txt\"");
  if (r2.match (&is, "interp") != "interp:library") return false;

  // string delimiters
  Regex r3 ("\"\\\"\"$N*\"\\\"\"");
  is.set ("\"afnix-txt\"");
  if (r3.match (&is) != "\"afnix-txt\"") return false;
  is.set ("\"afnix \\\"language\" quoted in the text");
  if (r3.match (&is) != "\"afnix \\\"language\"") return false;

  // success
  return true;
}

// full blown test
int main (int, char**) {
  using namespace afnix;

  try {
    // simple checks first
    if (re_check_char () == false) return 1;
    if (re_check_meta () == false) return 1;
    if (re_check_strg () == false) return 1;
    if (re_check_plus () == false) return 1;
    if (re_check_mult () == false) return 1;
    if (re_check_zone () == false) return 1;
    if (re_check_altn () == false) return 1;
    if (re_check_cset () == false) return 1;
    
    // complex check
    if (re_check_group () == false) return 1;
    if (re_check_real  () == false) return 1;
    if (re_check_url   () == false) return 1;
    if (re_check_can   () == false) return 1;
    
    // check input stream
    if (re_check_is    () == false) return 1;
  } catch (...) {
    return 2;
  }
  // ok so far
  return 0;
}
