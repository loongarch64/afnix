// ---------------------------------------------------------------------------
// - t_utility.cpp                                                           -
// - standard object library - trie class tester                             -
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

#include "Utility.hpp"

int main (int, char**) {
  using namespace afnix;

  // block nullification
  t_byte nbuf[] = {0x01U, 0x02U, 0x03U};
  Utility::tonull (nbuf, sizeof(nbuf));
  for (long k = 0L; k < (long) sizeof(nbuf); k++) {
    if (nbuf[k] != 0x00U) return 1;
  }

  // block copy
  t_byte cbuf[sizeof(nbuf)];
  Utility::tobcpy (cbuf, sizeof(cbuf), nbuf);
  for (long k = 0L; k < (long) sizeof(nbuf); k++) {
    if (cbuf[k] != nbuf[k]) return 1;
  }

  // boolean conversion
  if (Utility::tobool ("true")  != true)  return 1;
  if (Utility::tobool ("TRUE")  != true)  return 1;
  if (Utility::tobool ("false") != false) return 1;
  if (Utility::tobool ("FALSE") != false) return 1;

  if (Utility::toboil ("0")     != false) return 1;
  if (Utility::toboil ("1")     != true)  return 1;
  if (Utility::toboil ("true")  != true)  return 1;
  if (Utility::toboil ("TRUE")  != true)  return 1;
  if (Utility::toboil ("false") != false) return 1;
  if (Utility::toboil ("FALSE") != false) return 1;

  // byte conversion
  if (Utility::tobyte('0') != 0x00U) return 1;
  if (Utility::tobyte('1') != 0x01U) return 1;
  if (Utility::tobyte('2') != 0x02U) return 1;
  if (Utility::tobyte('3') != 0x03U) return 1;
  if (Utility::tobyte('4') != 0x04U) return 1;
  if (Utility::tobyte('5') != 0x05U) return 1;
  if (Utility::tobyte('6') != 0x06U) return 1;
  if (Utility::tobyte('7') != 0x07U) return 1;
  if (Utility::tobyte('8') != 0x08U) return 1;
  if (Utility::tobyte('9') != 0x09U) return 1;
  if (Utility::tobyte('a') != 0x0AU) return 1;
  if (Utility::tobyte('b') != 0x0BU) return 1;
  if (Utility::tobyte('c') != 0x0CU) return 1;
  if (Utility::tobyte('d') != 0x0DU) return 1;
  if (Utility::tobyte('e') != 0x0EU) return 1;
  if (Utility::tobyte('f') != 0x0FU) return 1;

  // integer conversion
  if (Utility::tolong ("0")    !=    0L) return 1;
  if (Utility::tolong ("10")   !=   10L) return 1;
  if (Utility::tolong ("1024") != 1024L) return 1;
  if (Utility::tolong ("0x00") !=    0L) return 1;
  if (Utility::tolong ("0xFF") !=  255L) return 1;

  // real conversion
  if (Utility::toreal ("0.0")  !=  0.0)  return 1;
  if (Utility::toreal ("0.5")  !=  0.5)  return 1;

  // string conversion
  if (Utility::tostring (true)  != "true")  return 1;
  if (Utility::tostring (false) != "false") return 1;

  if (Utility::tostring (0L)    != "0")    return 1;
  if (Utility::tostring (10L)   != "10")   return 1;
  if (Utility::tostring (1024L) != "1024") return 1;

  if (Utility::tostring (0L,    1) != "0.0")    return 1;
  if (Utility::tostring (10L,   2) != "10.00")   return 1;
  if (Utility::tostring (1024L, 3) != "1024.000") return 1;

  // hashq
  if (Utility::hashq ((const t_byte*) "", 0)  != 0x00000000U) return 1;
  if (Utility::hashq ((const t_byte*) "a", 1) != 0x050C5D7EU) return 1;
  if (Utility::hashq ((const t_byte*) "b", 1) != 0x050C5D7DU) return 1;
  // hasho
  if (Utility::hasho ((const t_byte*) "", 0)  != 0x0000000000000000U) return 1;
  if (Utility::hasho ((const t_byte*) "a", 1) != 0xAF63BD4C8601B7BEU) return 1;
  if (Utility::hasho ((const t_byte*) "b", 1) != 0xAF63BD4C8601B7BDU) return 1;
  
  // ok - everything is fine
  return 0;
}
