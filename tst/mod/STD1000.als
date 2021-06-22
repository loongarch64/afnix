# ---------------------------------------------------------------------------
# - STD1000.als                                                             -
# - afnix:std extended test unit                                       -
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

# @info   elliptic curve arithmetic test
# @author amaury darsch

# y^2 = x^3 + 4x + 20 (29) a = 4, b = 20, n = 29

# verify elliptic condition
const a 4R
const b 20R
const n 29R

# this function validates a point
const ecc-point-p (x y) {
  const lhs (* y y)
  const rhs (+ (+ (x:pow 3) (* a x)) b)
  assert (lhs:mod n) (rhs:mod n)
}

# this function compute the sum of two points
const ecc-point-sum (xp yp xq yq) {
  # compute delta
  const dy (- yq yp)
  const dx (- xq xp)
  # get fraction with modular inverse
  const df (* (dy:mod n) (dx:mmi n))
  # compute point sum
  const tx (- (- (df:pow 2) xp) xq)
  const xr (if (< tx 0) (+ (tx:mod n) n) (tx:mod n))
  const ty (- (* df (- xp xr)) yp)
  const yr (if (< ty 0) (+ (ty:mod n) n) (ty:mod n))
  # evalute result
  Cons xr yr
}

# this function compute a point doubling
const ecc-point-dbl (xp yp) {
  # compute modular invers
  const dp (* 2R yp)
  const mi (dp:mmi n)
  const df (* (+ (* 3R (xp:pow 2)) a) mi)
  const tx (- (df:pow 2) (* 2R xp))
  const xr (if (< tx 0) (+ (tx:mod n) n) (tx:mod n))
  const ty (- (* df (- xp xr)) yp)
  const yr (if (< ty 0) (+ (ty:mod n) n) (ty:mod n))
  # evalute result
  Cons xr yr
}

# compute discriminant
const d (* 16R (+ (* 4R (a:pow 3)) (* 27R (b:pow 2))))
assert 176896R d
assert false (== 0R (d:mod 29R))

# validate points
const x1 5R
const y1 22R
ecc-point-p x1 y1

const x2 16R
const y2 27R
ecc-point-p x2 y2

# validate point sum
const ps (ecc-point-sum x1 y1 x2 y2)
assert 13R (ps:get-car)
assert 6R  (ps:get-cadr)

# validate point double
const pd (ecc-point-dbl x1 y1)
assert 14R (pd:get-car)
assert 6R  (pd:get-cadr)
