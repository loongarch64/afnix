# ---------------------------------------------------------------------------
# - MTH0010                                                             -
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

# @info   permutation test unit
# @author amaury darsch

# get the module
interp:library "afnix-mth"

# create a permutation by size
const p (afnix:mth:Permute 3)

# check representation
assert true  (afnix:mth:cpi-p p)
assert true  (afnix:mth:permute-p p)
assert "Permute" (p:repr)

# check identity
assert 0 (p:get 0)
assert 1 (p:get 1)
assert 2 (p:get 2)

assert 0 (p:reverse 0)
assert 1 (p:reverse 1)
assert 2 (p:reverse 2)

# change permutation
p:set 0 2
p:set 1 0
p:set 2 1

assert 2 (p:get 0)
assert 0 (p:get 1)
assert 1 (p:get 2)

assert 1 (p:reverse 0)
assert 2 (p:reverse 1)
assert 0 (p:reverse 2)


