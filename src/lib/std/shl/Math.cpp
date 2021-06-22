// ---------------------------------------------------------------------------
// - Math.cpp                                                                -
// - standard object library - math function implementation                  -
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
#include "cmth.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - private section                                                       -
  // -------------------------------------------------------------------------

  // this procedure compute the minimum of two numbers
  static inline t_real min (const t_real x, const t_real y) {
    return (x < y) ? x : y;
  }

  // this procedure compute the maximum of two numbers
  static inline t_real max (const t_real x, const t_real y) {
    return (x < y) ? y : x;
  }

  // -------------------------------------------------------------------------
  // - public section                                                        -
  // -------------------------------------------------------------------------

  // initialize math constants
  const t_real Math::CV_NAN  = c_nan  ();
  const t_real Math::CV_PINF = c_pinf ();
  const t_real Math::CV_NINF = c_ninf ();

  // standard constants
  const t_real Math::CV_E    = 2.7182818284590452354;
  const t_real Math::CV_PI   = 3.1415926535897932384;
  const t_real Math::CV_TP   = 2.0 * Math::CV_PI;
  const t_real Math::CV_HP   = 0.5 * Math::CV_PI;
  const t_real Math::CV_TR   = Math::CV_TP / 360.0;

  // the system absolute precision
  t_real Math::d_aeps        = 1.0E-5;
  // the system relative precision
  t_real Math::d_reps        = 1.0E-10;
  // the system function step
  t_real Math::d_seps        = 1.0E-6;
  
  // check if a number is a nan

  bool Math::isnan (const t_real x) {
    return c_isnan (x);
  }

  // check if a number is a infinite

  bool Math::isinf (const t_real x) {
    return c_isinf (x);
  }

  // compare two numbers by absolute precision

  bool Math::acmp (const t_real x, const t_real y) {
    return Math::abs (x - y) < Math::d_aeps;
  }

  // compare two numbers by absolute precision

  bool Math::acmp (const t_real x, const t_real y, const t_real aeps) {
    return Math::abs (x - y) < aeps;
  }

  // compare two numbers by relative precision

  bool Math::rcmp (const t_real x, const t_real y) {
    // not valid with null reference
    if (x == 0.0) return (abs (y) < Math::d_reps);
    // compare relatively
    return Math::abs ((x - y) / x) < Math::d_reps;
  }

  // compare two numbers by relative precision

  bool Math::rcmp (const t_real x, const t_real y, const t_real reps) {
    // not valid with null reference
    if (x == 0.0) return (abs (y) < Math::d_aeps);
    // compare relatively
    return Math::abs ((x - y) / x) < reps;
  }

  // compute the argument absolute value

  t_real Math::abs (const t_real x) {
    return c_abs (x);
  }

  // compute the argument square root

  t_real Math::sqrt (const t_real x) {
    bool   status = false;
    t_real result = c_sqrt (x, status);
    if (status == false) {
      throw Exception ("math-error", "math error with sqrt call");
    }
    return result;
  }

  // compute the argument floor

  t_real Math::floor (const t_real x) {
    return c_floor (x);
  }

  // compute the argument ceiling

  t_real Math::ceiling (const t_real x) {
    return c_ceiling (x);
  }

  // compute the argument round

  t_real Math::round (const t_real x) {
    return c_round (x);
  }

  // compute the natural logarithm

  t_real Math::ln (const t_real x) {
    bool   status = false;
    t_real result = c_ln (x, status);
    if (status == false) {
      throw Exception ("math-error", "math error with ln call");
    }
    return result;
  }

  // compute the exponential of the argument

  t_real Math::exp (const t_real x) {
    return c_exp (x);
  }

  // compute the argument sine

  t_real Math::sin (const t_real x) {
    return c_sin (x);
  }

  // compute the argument cosine

  t_real Math::cos (const t_real x) {
    return c_cos (x);
  }

  // compute the argument tangent

  t_real Math::tan (const t_real x) {
    return c_tan (x);
  }

  // compute the argument arc sine

  t_real Math::asin (const t_real x) {
    bool   status = false;
    t_real result = c_asin (x, status);
    if (status == false) {
      throw Exception ("math-error", "math error with asin call");
    }
    return result;
  }

  // compute the argument arc cosine

  t_real Math::acos (const t_real x) {
    bool   status = false;
    t_real result = c_acos (x, status);
    if (status == false) {
      throw Exception ("math-error", "math error with acos call");
    }
    return result;
  }

  // compute the argument arc tangent

  t_real Math::atan (const t_real x) {
    bool   status = false;
    t_real result = c_atan (x, status);
    if (status == false) {
      throw Exception ("math-error", "math error with atan call");
    }
    return result;
  }

  // compute the argument hyperbolic sine

  t_real Math::sinh (const t_real x) {
    bool   status = false;
    t_real result = c_sinh (x, status);
    if (status == false) {
      throw Exception ("math-error", "math error with sinh call");
    }
    return result;
  }

  // compute the argument hyperbolic cosine

  t_real Math::cosh (const t_real x) {
    bool   status = false;
    t_real result = c_cosh (x, status);
    if (status == false) {
      throw Exception ("math-error", "math error with cosh call");
    }
    return result;
  }

  // compute the argument hyperbolic tangent

  t_real Math::tanh (const t_real x) {
    bool   status = false;
    t_real result = c_tanh (x, status);
    if (status == false) {
      throw Exception ("math-error", "math error with tanh call");
    }
    return result;
  }

  // compute the argument arc hyperbolic sine

  t_real Math::asinh (const t_real x) {
    bool   status = false;
    t_real result = c_asinh (x, status);
    if (status == false) {
      throw Exception ("math-error", "math error with asinh call");
    }
    return result;
  }

  // compute the argument arc hyperbolic cosine

  t_real Math::acosh (const t_real x) {
    bool   status = false;
    t_real result = c_acosh (x, status);
    if (status == false) {
      throw Exception ("math-error", "math error with acosh call");
    }
    return result;
  }

  // compute the argument arc hyperbolic tangent

  t_real Math::atanh (const t_real x) {
    bool   status = false;
    t_real result = c_atanh (x, status);
    if (status == false) {
      throw Exception ("math-error", "math error with atanh call");
    }
    return result;
  }

  // compute the power of the argument

  t_real Math::pow (const t_real x, const t_real y) {
    return c_pow (x, y);
  }

  // compute the argument remainder

  t_real Math::mod (const t_real x, const t_real m) {
    return c_mod (x, m);
  }

  // compute the hypotenuse of the two arguments

  t_real Math::hypot (const t_real x, const t_real y) {
    // trivial case
    if (x == 0.0) return Math::abs (y);
    if (y == 0.0) return Math::abs (x);
    // get absolute and minimum
    t_real ax = Math::abs (x);
    t_real ay = Math::abs (y);
    t_real mi = min (ax, ay);
    t_real mx = max (ax, ay);
    t_real tr = mi / mx;
    return mx * Math::sqrt (1.0 + tr * tr);
  }
}
