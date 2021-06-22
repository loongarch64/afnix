# ---------------------------------------------------------------------------
# - AXI0027.als                                                              -
# - afnix engine test module                                                -
# ---------------------------------------------------------------------------
# - This program is free software;  you can redistribute it  and/or  modify -
# - it provided that this copyright notice is kept intact.                  -
# -                                                                         -
# - This program  is  distributed in  the hope  that it will be useful, but -
# - without  any  warranty;  without  even   the   implied    warranty   of -
# - merchantability or fitness for a particular purpose.  In no event shall -
# - the copyright holder be liable for any  direct, indirect, incidental or -
# - special damages arising in any way out of the use of this software.     -
# ---------------------------------------------------------------------------
# - copyright (c) 1999-2021 amaury darsch                                   -
# ---------------------------------------------------------------------------

# @info   legendre polynom generation
# @author amaury darsch

# legendre polynome order 0 and 1
const lp-0 (x) 1.0
const lp-1 (x) x

# legendre polynom of order n
const lp-n (n) (
    if (> n 1) {
      const lp-n-1 (lp-n (- n 1))
      const lp-n-2 (lp-n (- n 2))
      gamma (x) (n lp-n-1 lp-n-2) (/ (- (* (* (- (* 2.0 n) 1.0) x) (lp-n-1 x))
                                     (* (- n 1.0) (lp-n-2 x))) (Real n))
     } (if (== n 1) lp-1 lp-0)
)

# generate order 2 polynome
const lp-2 (lp-n 2)

# assert order 2 function is 0.5 * (3x**2 - 1)
const alp2 (x) (* 0.5 (- (* 3.0 (* x x)) 1.0))

# do some assert
assert (alp2 0.0) (lp-2 0.0)
assert (alp2 1.0) (lp-2 1.0)
assert (alp2 2.0) (lp-2 2.0)
assert (alp2 3.0) (lp-2 3.0)
assert (alp2 4.0) (lp-2 4.0)
assert (alp2 5.0) (lp-2 5.0)
assert (alp2 6.0) (lp-2 6.0)
assert (alp2 7.0) (lp-2 7.0)
assert (alp2 8.0) (lp-2 8.0)
assert (alp2 9.0) (lp-2 9.0)
