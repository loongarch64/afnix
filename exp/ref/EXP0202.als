# ----------------------------------------------------------------------------
# - EXP0202.als                                                              -
# - afnix example : chapter 2 : example 02                                   -
# ----------------------------------------------------------------------------
# - This program is  free software;  you can  redistribute it and/or  modify -
# - it provided that this copyright notice is kept intact.                   -
# -                                                                          -
# - This  program  is  distributed in the hope  that it  will be useful, but -
# - without  any   warranty;  without  even   the   implied    warranty   of -
# - merchantability  or fitness for a particular purpose. In not event shall -
# - the copyright holder be  liable for  any direct, indirect, incidental or -
# - special damages arising in any way out of the use of this software.      -
# ----------------------------------------------------------------------------
# - copyright (c) 1999-2021 amaury darsch                                    -
# ----------------------------------------------------------------------------

# legendre polynome order 0 and 1
const lp-0 (x) 1
const lp-1 (x) x

# legendre polynom of order n
const lp-n (n) (
    if (> n 1) {
      const lp-n-1 (lp-n (- n 1))
      const lp-n-2 (lp-n (- n 2))
      gamma (x) (n lp-n-1 lp-n-2) (/ (- (* (* (- (* 2 n) 1) x) (lp-n-1 x))
                                     (* (- n 1) (lp-n-2 x))) n)
     } (if (== n 1) lp-1 lp-0)
)

# generate order 2 polynom
const lp-2 (lp-n 2)

# print lp-2 (2)
println "lp2 (2) = " (lp-2 2)
