// ---------------------------------------------------------------------------
// - cmth.cxx                                                                -
// - standard platform library - c math function implementation              -
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
#include "cmth.hxx"

namespace afnix {

  // -------------------------------------------------------------------------
  // - private section                                                       -
  // -------------------------------------------------------------------------

  // the nan value
  static const t_real MTH_NAN_VAL = NAN;
  // the infinite values
  static const t_real MTH_INF_POS = +INFINITY;
  static const t_real MTH_INF_NEG = -INFINITY;

  // -------------------------------------------------------------------------
  // - public section                                                       -
  // -------------------------------------------------------------------------

  // return true if the number is nan

  bool c_isnan (const t_real x) {
    return (isnan (x) == 0) ? false : true;
  }

  // return the real nan value
  
  t_real c_nan (void) {
    return MTH_NAN_VAL;
  }

  // return true if the number is infinite

  bool c_isinf (const t_real x) {
    return (isinf (x) == 0) ? false : true;
  }

  // return a positive infinite value
  
  t_real c_pinf (void) {
    return MTH_INF_POS;
  }

  // return a positive infinite value
  
  t_real c_ninf (void) {
    return MTH_INF_NEG;
  }

  // return the ceiling of the argument

  t_real c_ceiling (const t_real x) {
    return ceil (x);
  }

  // return the floor of the argument

  t_real c_floor (const t_real x) {
    return floor (x);
  }

  // return the round of the argument

  t_real c_round (const t_real x) {
    return round (x);
  }

  // return the absolute value of the argument

  t_real c_abs (const t_real x) {
    return fabs (x);
  }

  // return the remainder of x divided by y

  t_real c_mod (const t_real x, const t_real y) {
    return fmod (x,y);
  }

  // return the square root of the real
 
  t_real c_sqrt (const t_real x, bool& status) {
    if (x < 0.0) {
      status = false;
      return 0.0;
    }
    errno  = 0;
    t_real result = sqrt (x);
    if (errno != 0) {
      status = false;
      return 0.0;
    }
    status = true;
    return result;
  }

  // return the natural logarithm of the argument
 
  t_real c_ln (const t_real x, bool& status) {
    errno  = 0;
    t_real result = log (x);
    if (errno != 0) {
      status = false;
      return 0.0;
    }
    status = true;
    return result;
  }

  // return the exponential of the argument
 
  t_real c_exp (const t_real x) {
    return exp (x);
  }

  // return the power of x with y
 
  t_real c_pow (const t_real x, const t_real y) {
    return pow (x,y);
  }

  // return the sine of x
 
  t_real c_sin (const t_real x) {
    return sin (x);
  }

  // return the cosine of x
 
  t_real c_cos (const t_real x) {
    return cos (x);
  }

  // return the tangent of x
 
  t_real c_tan (const t_real x) {
    return tan (x);
  }

  // return the arc sine of the argument
 
  t_real c_asin (const t_real x, bool& status) {
    errno  = 0;
    t_real result = asin (x);
    if (errno != 0) {
      status = false;
      return 0.0;
    }
    status = true;
    return result;
  }

  // return the arc cosine of the argument
 
  t_real c_acos (const t_real x, bool& status) {
    errno  = 0;
    t_real result = acos (x);
    if (errno != 0) {
      status = false;
      return 0.0;
    }
    status = true;
    return result;
  }

  // return the arc tangent of the argument
 
  t_real c_atan (const t_real x, bool& status) {
    errno  = 0;
    t_real result = atan (x);
    if (errno != 0) {
      status = false;
      return 0.0;
    }
    status = true;
    return result;
  }

  // return the hyperbolic sine of the argument
 
  t_real c_sinh (const t_real x, bool& status) {
    errno  = 0;
    t_real result = sinh (x);
    if (errno != 0) {
      status = false;
      return 0.0;
    }
    status = true;
    return result;
  }

  // return the hyperbolic cosine of the argument
 
  t_real c_cosh (const t_real x, bool& status) {
    errno  = 0;
    t_real result = cosh (x);
    if (errno != 0) {
      status = false;
      return 0.0;
    }
    status = true;
    return result;
  }

  // return the hyperbolic tangent of the argument
 
  t_real c_tanh (const t_real x, bool& status) {
    errno  = 0;
    t_real result = tanh (x);
    if (errno != 0) {
      status = false;
      return 0.0;
    }
    status = true;
    return result;
  }

  // return the hyperbolic arc sine of the argument
 
  t_real c_asinh (const t_real x, bool& status) {
    errno  = 0;
    t_real result = asinh (x);
    if (errno != 0) {
      status = false;
      return 0.0;
    }
    status = true;
    return result;
  }

  // return the hyperbolic arc cosine of the argument
 
  t_real c_acosh (const t_real x, bool& status) {
    errno  = 0;
    t_real result = acosh (x);
    if (errno != 0) {
      status = false;
      return 0.0;
    }
    status = true;
    return result;
  }

  // return the hyperbolic arc tangent of the argument
 
  t_real c_atanh (const t_real x, bool& status) {
    errno  = 0;
    t_real result = atanh (x);
    if (errno != 0) {
      status = false;
      return 0.0;
    }
    status = true;
    return result;
  }
}
