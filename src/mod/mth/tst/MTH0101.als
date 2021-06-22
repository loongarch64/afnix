# ---------------------------------------------------------------------------
# - MTH0101.als                                                             -
# - afnix:mth module test unit                                              -
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

# @info   real vector test unit
# @author amaury darsch

# get the module
interp:library "afnix-mth"

# create a random vector
const rv (afnix:mth:get-random-r-vector 10 100.0)

# check predicate
assert true (afnix:mth:rvi-p rv)
assert true (afnix:mth:r-vector-p rv)

# check vector size
assert 10 (rv:get-size)

# clone this vector
const cv (rv:clone)
# do some arithmetic
rv:+= cv
cv:*= 2.0
assert true (rv:?= cv)
