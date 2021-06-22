# ---------------------------------------------------------------------------
# - MTH0401.als                                                             -
# - afnix:mth covariance test unit                                          -
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

# @info   samples covariance unit
# @author amaury darsch

# get the module
interp:library "afnix-mth"
interp:set-absolute-precision 0.0001

# create a default covariance
const c (afnix:mth:Cov)
# check predicate
assert true (afnix:mth:cov-p c)
# check representation
assert "Cov" (c:repr)

# check accessors
assert 0.0 (c:get-x-cumulative-value)
assert 0.0 (c:get-y-cumulative-value)
assert 0.0 (c:get-xx-cumulative-value)
assert 0.0 (c:get-yy-cumulative-value)
assert 0.0 (c:get-xy-cumulative-value)
assert 0   (c:get-samples)
assert 0.0 (c:get-covariance)

# push and test
c:push 1.0 4.0
assert 1.0  (c:get-x-cumulative-value)
assert 4.0  (c:get-y-cumulative-value)
assert 1.0  (c:get-xx-cumulative-value)
assert 16.0 (c:get-yy-cumulative-value)
assert 4.0  (c:get-xy-cumulative-value)
assert 1    (c:get-samples)
trans  xv   (c:get-x-variance)
trans  yv   (c:get-y-variance)
trans  cov (c:get-covariance)
assert true (xv:?= 0.0)
assert true (yv:?= 0.0)
assert true (cov:?= 0.0)

# push and test
c:push 2.0  5.0
assert 3.0  (c:get-x-cumulative-value)
assert 9.0  (c:get-y-cumulative-value)
assert 5.0  (c:get-xx-cumulative-value)
assert 41.0 (c:get-yy-cumulative-value)
assert 14.0 (c:get-xy-cumulative-value)
assert 2    (c:get-samples)
trans  xv   (c:get-x-variance)
trans  yv   (c:get-y-variance)
trans  cov  (c:get-covariance)
assert true (xv:?= 0.25)
assert true (yv:?= 0.25)
assert true (cov:?= 0.25)

# push and test
c:push 3.0 6.0
assert 6.0  (c:get-x-cumulative-value)
assert 15.0 (c:get-y-cumulative-value)
assert 14.0 (c:get-xx-cumulative-value)
assert 77.0 (c:get-yy-cumulative-value)
assert 3    (c:get-samples)
trans  xv   (c:get-x-variance)
trans  yv   (c:get-y-variance)
trans  cov  (c:get-covariance)
assert true (xv:?= 0.6666)
assert true (yv:?= 0.6666)
assert true (cov:?= 0.6666)

