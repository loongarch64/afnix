# ---------------------------------------------------------------------------
# - MTH0131.als                                                             -
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

# @info   mgs direct solver test unit
# @author amaury darsch

# get the module
interp:library "afnix-mth"
# load matrix generator
interp:load "MTHXXXX"

# create a mgs solver
const mgs (afnix:mth:Mgs)  

# check representation
assert true (afnix:mth:mgs-p mgs)
assert true (afnix:mth:direct-p mgs)
assert true (afnix:mth:solver-p mgs)
assert "Mgs" (mgs:repr)

# test a mgs factorization
const mth-mgs-test (m) {
  # set lhs and factorize
  mgs:set-lhs m
  mgs:factorize
  # get the mgs block and compare
  trans  mgsm (mgs:get-qr)
  assert true (mgsm:?= m)
}

# test a 3x3 block
const mth-mgs-ckt-3 (m) {
  mth-set-3x3  m
  mth-mgs-test m
  
  # create 3x3 block
  mth-set-3x3 m
  # create a 3x3 permutation
  trans p (afnix:mth:Permute 3)
  p:set 0 2
  p:set 1 0
  p:set 2 1
  # set lhs and factorize
  mgs:set-lhs m
  mgs:set-permutation p
  mgs:factorize
  # get the mgs block and compare
  trans mgsm (mgs:get-qr)
  assert true (mgsm:?= m)
}

# test a 5x5 block
const mth-mgs-ckt-5 (m) {
  mth-set-5x5  m
  mth-mgs-test m
}

# test a 9x9 block
const mth-mgs-ckt-9 (m) {
  mth-set-9x9 m
  mth-mgs-test m
}

trans m (afnix:mth:Rblock 3) 
mth-mgs-ckt-3 m
trans m (afnix:mth:Rblock 5) 
mth-mgs-ckt-5 m
trans m (afnix:mth:Rblock 9) 
mth-mgs-ckt-9 m

trans m (afnix:mth:Rmatrix 3) 
mth-mgs-ckt-3 m
trans m (afnix:mth:Rmatrix 5) 
mth-mgs-ckt-5 m
trans m (afnix:mth:Rmatrix 9) 
mth-mgs-ckt-9 m

# create a random block
trans m (afnix:mth:get-random-r-block  17 100.0 true)
mth-mgs-test m
trans m (afnix:mth:get-random-r-matrix 33 100.0 true)
mth-mgs-test m

# create a random block
trans m (afnix:mth:get-random-r-block  64 100.0 true)
mth-mgs-test m
trans m (afnix:mth:get-random-r-matrix 64 100.0 true)
mth-mgs-test m

# create a random block
trans m (afnix:mth:get-random-r-block  128 100.0 true)
mth-mgs-test m
trans m (afnix:mth:get-random-r-matrix 128 100.0 true)
mth-mgs-test m

