// ---------------------------------------------------------------------------
// - t_math.cpp                                                              -
// - standard object library - math class tester module                      -
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

#include "Math.hpp"
#include "Exception.hpp"

int main (int, char**) {
  using namespace afnix;

  // check for nan
  if (Math::isnan (Math::CV_NAN) == false) return 1;
  // check for infinite
  if (Math::isinf (Math::CV_PINF) == false) return 1;
  if (Math::isinf (Math::CV_NINF) == false) return 1;
  
  // check for absolute precision
  if (Math::acmp (1.0, 1.0 + Math::d_aeps / 10.0) == false) return 1;

  // check for absolute
  if (Math::abs ( 1.0) != 1.0) return 1;
  if (Math::abs (-1.0) != 1.0) return 1;
  
  // check for sqrt
  if (Math::acmp (Math::sqrt (4.0), 2.0) == false) return 1;

  // check for hypotenuse
  if (Math::hypot (1.0, 0.0) != 1.0) return 1;
  if (Math::hypot (3.0, 4.0) != 5.0) return 1;

  // checkf or floor/ceiling/round
  if (Math::floor   (1.3) != 1.0) return 1;
  if (Math::ceiling (1.3) != 2.0) return 1;
  if (Math::round   (1.3) != 1.0) return 1;
  if (Math::round   (1.7) != 2.0) return 1;
  
  // success
  return 0;
}
