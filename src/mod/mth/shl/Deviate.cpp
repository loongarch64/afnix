// ---------------------------------------------------------------------------
// - Deviate.cpp                                                             -
// - afnix:mth module - distribution deviate class implementation            -
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
#include "Deviate.hpp"
#include "Utility.hpp"
#include "Exception.hpp"
 
namespace afnix {

  // -------------------------------------------------------------------------
  // - private section                                                        -
  // -------------------------------------------------------------------------

  // this procedure compute the absolute value
  static inline t_real abs (const t_real x) {
    return x < 0.0 ? -x : x;
  }

  // -------------------------------------------------------------------------
  // - public section                                                        -
  // -------------------------------------------------------------------------

  // get a uniform deviate

  t_real Deviate::uniform (void) {
    t_real result = 2.0 * Utility::realrnd (true) - 1.0;
    return result;
  }

  // get a uniforme deviate by mean and deviation

  t_real Deviate::uniform (const t_real mu, const t_real sg) {
    t_real result = mu + sg * Deviate::uniform ();
    return result;
  }
  
  // get a normal deviate [Leva (1992)]

  t_real Deviate::normal (void) {
    t_real u = 0.0;
    t_real v = 0.0;
    t_real q = 0.0;
    do {
      // P (u,v) uniform deviate
      u = Utility::realrnd (true);
      v = Utility::realrnd (true);
      // set v in [-r, r]
      v = 1.7156 * (v - 0.5);
      // evalute Q
      t_real x = u - 0.449871;
      t_real y = abs (v) + 0.386595;
      q = x*x + y*(0.19600*y - 0.25472*x);
    } while ((q > 0.27597) && ((q > 0.27846) || (v*v > -4.0*u*u*Math::ln(u))));
    t_real result = v / u;
    return result;
  }

  // get a normal deviate by mean and deviation

  t_real Deviate::normal (const t_real mu, const t_real sg) {
    t_real result = mu + sg * Deviate::normal ();
    return result;
  }
}
