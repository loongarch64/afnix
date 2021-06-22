# ---------------------------------------------------------------------------
# - MTH0102.als                                                             -
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

# @info   real block matrix test unit
# @author amaury darsch

# get the module
interp:library "afnix-mth"

# create a simple square matrix
const rm (afnix:mth:Rblock 2)

# check predicate
assert true (afnix:mth:rmi-p rm)
assert true (afnix:mth:r-block-p rm)

# check representation
assert "Rblock" (rm:repr)

# check matrix size and elements
assert 2    (rm:get-row-size)
assert 2    (rm:get-col-size)
assert true (rm:nil-p)

# check accessors
assert 0.0 (rm:get 0 0)
rm:set 0 0 1.0
assert 1.0 (rm:get 0 0)

# check compress and null
assert false (rm:nil-p)
assert false (rm:compress)
assert false (rm:nil-p)

assert 0.0 (rm:get 0 1)
assert 0.0 (rm:get 1 0)

assert 0.0 (rm:get 1 1)
rm:set 1 1 1.0
assert 1.0 (rm:get 1 1)

# check row and column vectors
trans v (rm:copy-row 0)
assert 1.0 (v:get 0)
assert 0.0 (v:get 1)
trans v (rm:copy-row 1)
assert 0.0 (v:get 0)
assert 1.0 (v:get 1)

trans v (rm:copy-column 0)
assert 1.0 (v:get 0)
assert 0.0 (v:get 1)
trans v (rm:copy-column 1)
assert 0.0 (v:get 0)
assert 1.0 (v:get 1)

# create a working vector
const xv (afnix:mth:Rvector 2)
xv:set 0 2.0
xv:set 0 3.0

# test the matrix operators
const bv (* rm xv)
assert true (xv:?= bv)

# nulify and test
rm:set 0 0 0.0
rm:set 1 1 0.0
assert true  (rm:nil-p)
assert false (rm:compress)
assert true  (rm:nil-p)

# create a matrix by vector product
const u (afnix:mth:Rvector 3)
u:set 0 1.0
u:set 1 2.0
u:set 2 3.0
const v (afnix:mth:Rvector 3)
v:set 0 4.0
v:set 1 5.0
v:set 2 6.0

const  m (afnix:mth:Rblock u v)
assert 3 (m:get-row-size)
assert 3 (m:get-col-size)

assert  4.0 (m:get 0 0)
assert  5.0 (m:get 0 1)
assert  6.0 (m:get 0 2)
assert  8.0 (m:get 1 0)
assert 10.0 (m:get 1 1)
assert 12.0 (m:get 1 2)
assert 12.0 (m:get 2 0)
assert 15.0 (m:get 2 1)
assert 18.0 (m:get 2 2)

# create a permutation
const p (afnix:mth:Permute 3)
p:set 0 2
p:set 1 0
p:set 2 1

# permutate the matrix
const pm (m:permutate p)
assert 12.0 (pm:get 0 0)
assert 15.0 (pm:get 0 1)
assert 18.0 (pm:get 0 2)
assert  4.0 (pm:get 1 0)
assert  5.0 (pm:get 1 1)
assert  6.0 (pm:get 1 2)
assert  8.0 (pm:get 2 0)
assert 10.0 (pm:get 2 1)
assert 12.0 (pm:get 2 2)

# do a reverse permutation
const mp (pm:reverse p)
assert true (m:?= mp)
