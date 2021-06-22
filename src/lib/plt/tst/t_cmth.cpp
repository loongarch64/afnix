// ---------------------------------------------------------------------------
// - t_mth.cpp                                                               -
// - standard paltform library - math system call tester module              -
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

#include "cmth.hpp"

int main (int, char**) {
  using namespace afnix;

  // check for nan
  if (c_isnan (0.0) == true)       return 1;
  if (c_isnan (c_nan ()) == false) return 1;

  // check for positive infinite
  if (c_isinf (c_pinf ()) == false) return 1;
  if (c_isinf (c_ninf ()) == false) return 1;
  // check for infinite ordering
  if ((0 < c_pinf ()) == false) return 1;
  if ((0 > c_ninf ()) == false) return 1;
  if ((c_ninf () < c_pinf ()) == false) return 1;

  // check for ceiling/floor/round
  if (c_ceiling (1.3) != 2.0) return 1;
  if (c_floor   (1.3) != 1.0) return 1;
  if (c_round   (1.3) != 1.0) return 1;
  if (c_round   (1.7) != 2.0) return 1;

  // check for abs
  if (c_abs (-2.0) != 2.0) return 1;

  // check for mod
  if (c_mod (4.0, 2.0) != 0.0) return 1;

  // check for sqrt
  bool status = false;
  if ((c_sqrt (4.0,status) != 2.0) || (status == false)) return 1;

  // check for logarithm
  status = false;
  if ((c_ln (1.0, status) != 0.0) || (status == false)) return 1;

  // check for exp
  if (c_exp (0.0) != 1.0) return 1;

  // check for pow
  if (c_pow (2.0, 2.0) != 4.0) return 1;

  // check for sin, cos, tan
  if (c_sin (0.0) != 0.0) return 1;
  if (c_cos (0.0) != 1.0) return 1;
  if (c_tan (0.0) != 0.0) return 1;

  // ok - enough for now
  return 0;
}
