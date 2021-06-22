# ---------------------------------------------------------------------------
# - MTH0720.als                                                             -
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

# @info   integer vector test unit
# @author amaury darsch

# get the module
interp:library "afnix-mth"

# create a simple vector
const iv (afnix:mth:Ivector 3)

# check predicate
assert true (afnix:mth:avi-p iv)
assert true (afnix:mth:ivi-p iv)
assert true (afnix:mth:i-vector-p iv)

# check representation
assert "Ivector" (iv:repr)

# check vector length and elements
assert 3 (iv:get-size)

# check accessors
assert 0 (iv:get 0)
iv:set 0 0
assert 0 (iv:get 0)

assert 0 (iv:get 1)
iv:set 1 3
assert 3 (iv:get 1)

assert 0 (iv:get 2)
iv:set 2 4
assert 4 (iv:get 2)

# test the vector operators
iv:*= 2
assert 0 (iv:get 0)
assert 6 (iv:get 1)
assert 8 (iv:get 2)

iv:+= 1
assert 1 (iv:get 0)
assert 7 (iv:get 1)
assert 9 (iv:get 2)
iv:-= 1
assert 0 (iv:get 0)
assert 6 (iv:get 1)
assert 8 (iv:get 2)

# create another vector
const nv (afnix:mth:Ivector 3)
nv:set 0 1
nv:set 1 1
nv:set 2 1

# check the operators
trans av (+ iv nv)
assert 1 (av:get 0)
assert 7 (av:get 1)
assert 9 (av:get 2)

trans sv (- av nv)
assert true (sv:== iv)

assert 0 (iv:get 0)
assert 6 (iv:get 1)
assert 8 (iv:get 2)

# create a permutation
const p (afnix:mth:Permute 3)
p:set 0 2
p:set 1 0
p:set 2 1

# permutate the vector
const pv (iv:permutate p)
assert 8 (pv:get 0)
assert 0 (pv:get 1)
assert 6 (pv:get 2)

# do a reverse permutation
const vp (pv:reverse p)
assert true (iv:== vp)
