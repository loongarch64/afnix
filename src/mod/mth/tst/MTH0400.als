# ---------------------------------------------------------------------------
# - MTH0400.als                                                             -
# - afnix:mth mean test unit                                      -
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

# @info   samples mean unit
# @author amaury darsch

# get the module
interp:library "afnix-mth"

# create a default mean
const m (afnix:mth:Mean)

# check predicate
assert true (afnix:mth:mean-p m)

# check representation
assert "Mean" (m:repr)

# check accessors
assert 0.0 (m:get-cumulative-value)
assert 0   (m:get-samples)
assert 0.0 (m:get-mean)

# push and test
m:push 1.0
assert 1.0 (m:get-cumulative-value)
assert 1   (m:get-samples)
assert 1.0 (m:get-mean)

m:push 2.0
assert 3.0 (m:get-cumulative-value)
assert 2   (m:get-samples)
assert 1.5 (m:get-mean)

m:push 3.0
assert 6.0 (m:get-cumulative-value)
assert 3   (m:get-samples)
assert 2.0 (m:get-mean)
