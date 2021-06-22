# ---------------------------------------------------------------------------
# - MTH0100.als                                                             -
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

# create a simple vector
const rv (afnix:mth:Rvector 3)
# check predicate
assert true (afnix:mth:rvi-p rv)
assert true (afnix:mth:r-vector-p rv)
# check representation
assert "Rvector" (rv:repr)
# check vector length and predicate
assert 3 (rv:get-size)
assert true (rv:nil-p)

# check accessors
assert 0.0 (rv:get 0)
rv:set 0 0.0
assert 0.0 (rv:get 0)

assert 0.0 (rv:get 1)
rv:set 1 3.0
assert 3.0 (rv:get 1)

assert 0.0 (rv:get 2)
rv:set 2 4.0
assert 4.0 (rv:get 2)

# check dedicated methods
assert 5.0 (rv:norm)

# test the vector operators
rv:*= 2.0
assert 10.0 (rv:norm)

rv:+= 1.0
rv:-= 1.0
rv:/= 2.0
assert 5.0 (rv:norm)

# create another vector
const nv (afnix:mth:Rvector 3)
nv:set 0 1.0
nv:set 1 1.0
nv:set 2 1.0

# check the dot product
assert 7.0 (rv:dot nv)

# check the operators
trans av (+ rv nv)
assert 1.0 (av:get 0)
assert 4.0 (av:get 1)
assert 5.0 (av:get 2)

trans sv (- av nv)
assert true (sv:?= rv)

trans av (* rv 2.0)
assert 10.0 (av:norm)
trans av (/ av 2.0)
assert 5.0 (av:norm)

assert 0.0 (rv:get 0)
assert 3.0 (rv:get 1)
assert 4.0 (rv:get 2)

# create a permutation
const p (afnix:mth:Permute 3)
p:set 0 2
p:set 1 0
p:set 2 1

# permutate the vector
const pv (rv:permutate p)
assert 4.0 (pv:get 0)
assert 0.0 (pv:get 1)
assert 3.0 (pv:get 2)

# do a reverse permutation
const vp (pv:reverse p)
assert true (rv:?= vp)
