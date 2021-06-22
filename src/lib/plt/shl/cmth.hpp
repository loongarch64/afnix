// ---------------------------------------------------------------------------
// - cmth.hpp                                                                -
// - standard platform library - c math native function definition           -
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

#ifndef  AFNIX_CMTH_HPP
#define  AFNIX_CMTH_HPP

#ifndef  AFNIX_CCNF_HPP
#include "ccnf.hpp"
#endif

namespace afnix {
  /// This file contains the procedures that manipulate basic mathematical
  /// functions. Most of theses functions operate with real types.
  /// @author amaury darsch

  /// @return true if the t_real is nan
  bool c_isnan (const t_real x);

  /// @return the system real nan value
  t_real c_nan (void);

  /// @return true if the t_real is infinite
  bool c_isinf (const t_real x);

  /// @return a positive infinite value
  t_real c_pinf (void);

  /// @return a negative infinite value
  t_real c_ninf (void);

  /// @return the ceiling of the argument
  t_real c_ceiling (const t_real x);

  /// @return the floor of the argument
  t_real c_floor (const t_real x);

  /// @return the round of the argument
  t_real c_round (const t_real x);
  
  /// @return the absolute value of the argument
  t_real c_abs (const t_real x);

  /// @return the remainder of x divided by y
  t_real c_mod (const t_real x, const t_real y);

  /// @return the square root of the number
  t_real c_sqrt (const t_real x, bool& status);

  /// @return the natural logarithm of the number
  t_real c_ln (const t_real x, bool& status);

  /// @return the exponential of the argument
  t_real c_exp (const t_real x);

  /// @return the x**y value of the arguments
  t_real c_pow (const t_real x, const t_real y);

  /// @return the sine of the argument in radian
  t_real c_sin (const t_real x);

  /// @return the cosine of the argument in radian
  t_real c_cos (const t_real x);

  /// @return the tangent of the argument in radian
  t_real c_tan (const t_real x);

  /// @return the arc sine of the argument
  t_real c_asin (const t_real x, bool& status);

  /// @return the arc cosine of the argument in radian
  t_real c_acos (const t_real x, bool& status);

  /// @return the tangent of the argument in radian
  t_real c_atan (const t_real x, bool& status);

  /// @return the hyperbolic sine of the argument
  t_real c_sinh (const t_real x, bool& status);

  /// @return the hyperbolic cosine of the argument
  t_real c_cosh (const t_real x, bool& status);

  /// @return the hyperbolic tangent of the argument
  t_real c_tanh (const t_real x, bool& status);

  /// @return the hyperbolic arc sine of the argument
  t_real c_asinh (const t_real x, bool& status);

  /// @return the hyperbolic arc cosine of the argument
  t_real c_acosh (const t_real x, bool& status);

  /// @return the hyperbolic arc tangent of the argument
  t_real c_atanh (const t_real x, bool& status);
}

#endif
