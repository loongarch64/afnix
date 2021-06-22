// ---------------------------------------------------------------------------
// - Math.hpp                                                                -
// - standard object library - math functions definition                     -
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

#ifndef  AFNIX_MATH_HPP
#define  AFNIX_MATH_HPP

#ifndef  AFNIX_STRING_HPP
#include "String.hpp"
#endif

namespace afnix {

  /// The math class is a collection of static mathematic functions. There
  /// purposes is to group general purpose functions into a single umbrella.
  /// Additionnaly, the standard mathematical values are also defined here.
  /// @author amaury darsch

  class Math {
  public:
    /// the system real nan value
    static const t_real CV_NAN;
    /// the positive infinite value
    static const t_real CV_PINF;
    /// the negative infinite value
    static const t_real CV_NINF;

    /// the natural exponent
    static const t_real CV_E;
    /// the famous pi
    static const t_real CV_PI;
    /// the even more famous 2*PI
    static const t_real CV_TP;
    /// the even more more famous 1/2PI
    static const t_real CV_HP;    
    /// the radian fraction 2*PI/360
    static const t_real CV_TR;

    /// the system absolute precision
    static t_real d_aeps;
    /// the system relative precision
    static t_real d_reps;
    /// the system function step
    static t_real d_seps;

  public:
    /// check if the argument is a nan
    /// @param x the real argument
    static bool isnan (const t_real x);

    /// check if the argument is infinite
    /// @param x the real argument
    static bool isinf (const t_real x);

    /// compare two numbers by absolute precision
    /// @param x the reference argument
    /// @param y the argument to compare
    static bool acmp (const t_real x, const t_real y);

    /// compare two numbers by absolute precision
    /// @param x    the reference argument
    /// @param y    the argument to compare
    /// @param aeps the absolute precision
    static bool acmp (const t_real x, const t_real y, const t_real aeps);
    
    /// compare two numbers by relative precision
    /// @param x the reference argument
    /// @param y the argument to compare
    static bool rcmp (const t_real x, const t_real y);

    /// compare two numbers by relative precision
    /// @param x the reference argument
    /// @param y the argument to compare
    /// @param reps the relative precision
    static bool rcmp (const t_real x, const t_real y, const t_real reps);

    /// compute the argument absolute value
    /// @param x the real argument
    static t_real abs (const t_real x);

    /// compute the argument square root
    /// @param x the real argument
    static t_real sqrt (const t_real x);
    
    /// compute the argument floor
    /// @param x the real argument
    static t_real floor (const t_real x);

    /// compute the argument ceiling
    /// @param x the real argument
    static t_real ceiling (const t_real x);

    /// compute the argument round
    /// @param x the real argument
    static t_real round (const t_real x);

    /// compute the argument natural logarithm
    /// @param x the real argument
    static t_real ln (const t_real x);

    /// compute the exponential of the argument
    /// @param x the real argument
    static t_real exp (const t_real x);

    /// compute the argument sine
    /// @param x the real argument
    static t_real sin (const t_real x);

    /// compute the argument cosine
    /// @param x the real argument
    static t_real cos (const t_real x);

    /// compute the argument tangent
    /// @param x the real argument
    static t_real tan (const t_real x);

    /// compute the argument arc sine
    /// @param x the real argument
    static t_real asin (const t_real x);

    /// compute the argument arc cosine
    /// @param x the real argument
    static t_real acos (const t_real x);

    /// compute the argument arc tangent
    /// @param x the real argument
    static t_real atan (const t_real x);

    /// compute the argument hyperbolic sine
    /// @param x the real argument
    static t_real sinh (const t_real x);

    /// compute the argument hyperbolic cosine
    /// @param x the real argument
    static t_real cosh (const t_real x);

    /// compute the argument hyperbolic tangent
    /// @param x the real argument
    static t_real tanh (const t_real x);

    /// compute the argument arc hyperbolic sine
    /// @param x the real argument
    static t_real asinh (const t_real x);

    /// compute the argument arc hyperbolic cosine
    /// @param x the real argument
    static t_real acosh (const t_real x);

    /// compute the argument arc hyperbolic tangent
    /// @param x the real argument
    static t_real atanh (const t_real x);

    /// compute the argument power
    /// @param x the real argument
    /// @param y the real power
    static t_real pow (const t_real x, const t_real y);

    /// compute the argument remainder
    /// @param x the real argument
    /// @param m the real modulo
    static t_real mod (const t_real x, const t_real m);

    /// compute the hypotenuse of the two arguments
    /// @param x the x argument
    /// @param y the y argument
    static t_real hypot (const t_real x, const t_real y);
  };
}

#endif
