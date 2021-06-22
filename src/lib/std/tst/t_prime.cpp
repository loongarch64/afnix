// ---------------------------------------------------------------------------
// - t_prime.cpp                                                             -
// - standard object library - prime number tester                           -
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

#include "Prime.hpp"

int main (int, char**) {
  using namespace afnix;
  
  // check threshold
  if (Prime::mkthrp (0)     != 37)     return 1;
  if (Prime::mkthrp (70000) != 70001)  return 1;

  // check little fermat theorem
  if (Prime::fermat (2, 7)   == false) return 1;
  if (Prime::fermat (2, 561) == false) return 1;

  // check miller-rabin primality test
  if (Prime::mrpt (2, 7)     == false) return 1;
  if (Prime::mrpt (2, 561)   == true)  return 1;
  if (Prime::mrpt (2, 221)   == true)  return 1;
  if (Prime::mrpt (174, 221) == false) return 1;

  // base probable test
  if (Prime::isprobable (0) == true)      return 1;
  if (Prime::isprobable (1) == false)     return 1;
  if (Prime::isprobable (2) == false)     return 1;
  if (Prime::isprobable (17863) == false) return 1;
  if (Prime::isprobable (17864) == true)  return 1;

  // extra probable test
  if (Prime::isprobable (130003) == false)  return 1;
  if (Prime::isprobable (270001) == false)  return 1;
  if (Prime::isprobable (570001) == false)  return 1;

  // random prime generation
  Relatif p = Prime::random (128);
  if (p.getmsb () != 128) return 1;
  if (Prime::isprobable (p) == false) return 1;

  // safe prime generation
  Relatif s = Prime::safe (128);
  if (s.getmsb () != 128) return 1;
  if (Prime::isprobable (s) == false) return 1;
  if (Prime::issafe (s) == false) return 1;

  // success
  return 0;
}
