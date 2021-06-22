// ---------------------------------------------------------------------------
// - t_unicode.cpp                                                           -
// - standard object library - unicode class tester                          -
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

namespace afnix {

  // -------------------------------------------------------------------------
  // - utf section                                                           -
  // -------------------------------------------------------------------------

  // UTF-8 encoding: 0x00000000
  static const char U00000000[] = {'\x00','\x00','\x00','\x00'};
  static const char U00000001[] = {'\x01','\x00','\x00','\x00'};
  static const char U0000007F[] = {'\x7F','\x00','\x00','\x00'};
  static const char U00000080[] = {'\xC2','\x80','\x00','\x00'};
  static const char U000000FF[] = {'\xC3','\xBF','\x00','\x00'};
  static const char U00000100[] = {'\xC4','\x80','\x00','\x00'};
  static const char U000007FF[] = {'\xDF','\xBF','\x00','\x00'};
  static const char U00000800[] = {'\xE0','\xA0','\x80','\x00'};
  static const char U00000FFF[] = {'\xE0','\xBF','\xBF','\x00'};
  static const char U0000FFFF[] = {'\xEF','\xBF','\xBF','\x00'};
  static const char U00010000[] = {'\xF0','\x90','\x80','\x80'};

  // this procedure check a basic encoding
  static bool utf_test_08 (void) {
    // utf validation
    if (Unicode::valid (Encoding::EMOD_UTF8, U00000000, 1) == false)
      return false;
    if (Unicode::valid (Encoding::EMOD_UTF8, U00000001, 1) == false)
      return false;
    if (Unicode::valid (Encoding::EMOD_UTF8, U0000007F, 1) == false)
      return false;
    if (Unicode::valid (Encoding::EMOD_UTF8, U00000080, 1) == true)
      return false;
    if (Unicode::valid (Encoding::EMOD_UTF8, U00000080, 2) == false)
      return false;
    if (Unicode::valid (Encoding::EMOD_UTF8, U000000FF, 1) == true)
      return false;
    if (Unicode::valid (Encoding::EMOD_UTF8, U000000FF, 2) == false)
      return false;
    if (Unicode::valid (Encoding::EMOD_UTF8, U00000100, 1) == true)
      return false;
    if (Unicode::valid (Encoding::EMOD_UTF8, U00000100, 2) == false)
      return false;
    if (Unicode::valid (Encoding::EMOD_UTF8, U00000100, 3) == true)
      return false;
    if (Unicode::valid (Encoding::EMOD_UTF8, U000007FF, 1) == true)
      return false;
    if (Unicode::valid (Encoding::EMOD_UTF8, U000007FF, 2) == false)
      return false;
    if (Unicode::valid (Encoding::EMOD_UTF8, U000007FF, 3) == true)
      return false;
    if (Unicode::valid (Encoding::EMOD_UTF8, U00000800, 1) == true)
      return false;
    if (Unicode::valid (Encoding::EMOD_UTF8, U00000800, 2) == true)
      return false;
    if (Unicode::valid (Encoding::EMOD_UTF8, U00000800, 3) == false)
      return false;
    if (Unicode::valid (Encoding::EMOD_UTF8, U00000800, 4) == true)
      return false;
    if (Unicode::valid (Encoding::EMOD_UTF8, U00000FFF, 1) == true)
      return false;
    if (Unicode::valid (Encoding::EMOD_UTF8, U00000FFF, 2) == true)
      return false;
    if (Unicode::valid (Encoding::EMOD_UTF8, U00000FFF, 3) == false)
      return false;
    if (Unicode::valid (Encoding::EMOD_UTF8, U00000FFF, 4) == true)
      return false;
    if (Unicode::valid (Encoding::EMOD_UTF8, U0000FFFF, 1) == true)
      return false;
    if (Unicode::valid (Encoding::EMOD_UTF8, U0000FFFF, 2) == true)
      return false;
    if (Unicode::valid (Encoding::EMOD_UTF8, U0000FFFF, 3) == false)
      return false;
    if (Unicode::valid (Encoding::EMOD_UTF8, U0000FFFF, 4) == true)
      return false;
    if (Unicode::valid (Encoding::EMOD_UTF8, U00010000, 1) == true)
      return false;
    if (Unicode::valid (Encoding::EMOD_UTF8, U00010000, 2) == true)
      return false;
    if (Unicode::valid (Encoding::EMOD_UTF8, U00010000, 3) == true)
      return false;
    if (Unicode::valid (Encoding::EMOD_UTF8, U00010000, 4) == false)
      return false;

    // UTF-8 conversion
    if (Unicode::decode (U00000000) != 0x00000000UL) return false;
    if (Unicode::decode (U00000001) != 0x00000001UL) return false;
    if (Unicode::decode (U0000007F) != 0x0000007FUL) return false;
    if (Unicode::decode (U00000080) != 0x00000080UL) return false;
    if (Unicode::decode (U000000FF) != 0x000000FFUL) return false;
    if (Unicode::decode (U00000100) != 0x00000100UL) return false;
    if (Unicode::decode (U000007FF) != 0x000007FFUL) return false;
    if (Unicode::decode (U00000800) != 0x00000800UL) return false;
    if (Unicode::decode (U00000FFF) != 0x00000FFFUL) return false;
    if (Unicode::decode (U0000FFFF) != 0x0000FFFFUL) return false;
    if (Unicode::decode (U00010000) != 0x00010000UL) return false;
    // success
    return true;
  }

  // utf test function
  static bool utf_main_test (void) {
    // test the single code point first
    if (utf_test_08 () == false) return false;
    // success
    return true;
  }

  // -------------------------------------------------------------------------
  // - string section                                                        -
  // -------------------------------------------------------------------------

  // str test function
  static bool str_main_test (void) {
    // check stob function
    long    slen = 0;
    t_byte* sbuf = Unicode::stob (slen, "0123456789ABCDEF");
    if (slen != 8)       return false;
    if (sbuf[0] != 0x01) return false;
    if (sbuf[1] != 0x23) return false;
    if (sbuf[2] != 0x45) return false;
    if (sbuf[3] != 0x67) return false;
    if (sbuf[4] != 0x89) return false;
    if (sbuf[5] != 0xAB) return false;
    if (sbuf[6] != 0xCD) return false;
    if (sbuf[7] != 0xEF) return false;
    delete [] sbuf;

    sbuf = Unicode::stob (slen, "0x0123456789ABCDEF");
    if (slen != 8)       return false;
    if (sbuf[0] != 0x01) return false;
    if (sbuf[1] != 0x23) return false;
    if (sbuf[2] != 0x45) return false;
    if (sbuf[3] != 0x67) return false;
    if (sbuf[4] != 0x89) return false;
    if (sbuf[5] != 0xAB) return false;
    if (sbuf[6] != 0xCD) return false;
    if (sbuf[7] != 0xEF) return false;
    delete [] sbuf;

    // check stripl/stripr function
    static char SEP[] = {'\t', ' ', nilc};
    t_quad* scl = Unicode::stripl ("  A", SEP);
    t_quad* scr = Unicode::stripr ("a  ", SEP);
    if (scl[0] != Unicode::toquad ('A')) return false;
    if (scr[0] != Unicode::toquad ('a')) return false;
    delete [] scl;
    delete [] scr;
    
    // check tolower function
    t_quad* lcm = Unicode::tolower ("A");
    if (lcm[0] != Unicode::toquad ('a')) return false;
    delete [] lcm;
    
    // check toupper function
    t_quad* ucm = Unicode::toupper ("a");
    if (ucm[0] != Unicode::toquad ('A')) return false;
    delete [] ucm;
    
    // check for lower case value
    if (Unicode::islower (Unicode::toquad ('a')) == false) return false;
    if (Unicode::islower (Unicode::toquad ('A')) == true)  return false;
    
    // check for upper case value
    if (Unicode::isupper (Unicode::toquad ('Z')) == false) return false;
    if (Unicode::isupper (Unicode::toquad ('z')) == true)  return false;
    
    // check for letter
    if (Unicode::isletter (Unicode::toquad ('l')) == false) return false;
    if (Unicode::isletter (Unicode::toquad ('0')) == true)  return false;
    
    // check for digit
    if (Unicode::isdigit (Unicode::toquad ('0')) == false) return false;
    if (Unicode::isdigit (Unicode::toquad ('l')) == true)  return false;
    
    // check for terminal
    if (Unicode::isterm (Unicode::toquad ('l')) == false) return false;
    if (Unicode::isterm (Unicode::toquad ('U')) == false) return false;
    if (Unicode::isterm (Unicode::toquad ('0')) == false) return false;
    if (Unicode::isterm (Unicode::toquad ('|')) == false) return false;
    if (Unicode::isterm (Unicode::toquad (' ')) == false) return false;
    
    // check length vs not combining length (U+00C0 = U+0041 + U+0300)
    t_quad* cs = Unicode::strmak ('\xC0');
    if (Unicode::strlen (cs) != 1) return false;
    if (Unicode::ncclen (cs) != 1) return false;
    t_quad* ns = Unicode::strnrm (cs);
    if (Unicode::strlen (ns) != 2) return false;
    if (Unicode::ncclen (ns) != 1) return false;
    delete [] cs;
    delete [] ns;
    // success
    return true;
  }
}

int main (int, char**) {
  using namespace afnix;

  // utf test
  if (utf_main_test () == false) return 1;
  // str test
  if (str_main_test () == false) return 1;

  // done
  return 0;
}
