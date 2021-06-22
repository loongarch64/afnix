# ---------------------------------------------------------------------------
# - MTH0104.als                                                             -
# - afnix:mth module  test unit                                             -
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

# @info   real large sparse matrix test unit
# @author amaury darsch

# get the module
interp:library "afnix-mth"

# create a large matrix
const rm (afnix:mth:Rmatrix 10000000000 4000000000)
# check matrix size and elements
assert 10000000000 (rm:get-row-size)
assert 4000000000  (rm:get-col-size)

# check top top-left
assert 0.0 (rm:get 0 0)
rm:set 0 0 1
assert 1.0 (rm:get 0 0)

# check bottom-right
assert 0.0 (rm:get 9999999999 3999999999)
rm:set 9999999999 3999999999 1
assert 1.0 (rm:get 9999999999 3999999999)

# check the matrix by summation
trans sum 0
for (val) (rm) (sum:+= (val:get-row))
assert 9999999999 sum

trans sum 0
for (val) (rm) (sum:+= (val:get-col))
assert 3999999999 sum

trans sum 0.0 
for (val) (rm) (sum:+= (val:get-val))
assert 2.0 sum

rm:set 1 0 2.0

# clear lower-triangular
rm:clear-lower-triangular
assert 0.0 (rm:get 9999999999 3999999999)
