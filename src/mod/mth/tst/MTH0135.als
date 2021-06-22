# ---------------------------------------------------------------------------
# - MTH0105.als                                                             -
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

# @info   mgs solver test unit
# @author amaury darsch

# get the module
interp:library "afnix-mth"
# load matrix generator
interp:load "MTHXXXX"

# this procedure checks the mgs solver with a matrix
const mth-mgs-ckt-3 (p) {
  # create 3x3 matrix
  const am (afnix:mth:Rblock 3)
  mth-set-3x3 am
  # create the initial vector
  const bv (afnix:mth:Rvector 3)
  bv:set 0 5.0
  # create a mgs solver with a permutation
  trans mgs (afnix:mth:Mgs am)
  mgs:set-permutation p
  # solve the system
  trans xv (mgs:solve bv)
  # verify result
  assert true (mgs:verify bv xv)
}

# this procedure checks the mgs solver with a matrix
const mth-mgs-ckt-5 (p) {
  # create 5x5 matrix
  const am (afnix:mth:Rblock 5)
  mth-set-5x5 am
  # create the initial vector
  const bv (afnix:mth:Rvector 5)
  bv:set 2 0.15
  # create a mgs solver with a permutation
  trans mgs (afnix:mth:Mgs am)
  mgs:set-permutation p
  # solve the system
  trans xv (mgs:solve bv)
  # verify result
  assert true (mgs:verify bv xv)
}

# this procedure checks the mgs factorization with a matrix
const mth-mgs-ckt-9 nil {
  # create 9x9 matrix
  const am (afnix:mth:Rblock 9)
  mth-set-9x9 am
  # create the initial vector
  const bv (afnix:mth:Rvector 9)
  bv:set 4 4.0
  # solve the system with the mgs solver
  trans mgs (afnix:mth:Mgs am)
  trans xv (mgs:solve bv)
  # verify result
  assert true (mgs:verify bv xv)
}

# this procedure checks the mgs with a random matrix
const mth-mgs-rnd (size) {
  const imax (* size 10)
  
  # create a random matrix and vector
  const am (afnix:mth:get-random-r-block  size 100.0 true)
  const rv (afnix:mth:get-random-r-vector size 100.0)
  
  # check matrix and vector size
  assert size (am:get-row-size)
  assert size (am:get-col-size)
  assert size (rv:get-size)
  
  # create an linear system vector
  const bv (* am rv)
  # solve the system with the mgs solver
  trans mgs (afnix:mth:Mgs am)
  trans xv (mgs:solve bv)
  # verify result
  assert true (mgs:verify bv xv)
  assert true (xv:?= rv)
}

# create a permutation
const p3 (afnix:mth:Permute 3)
p3:set 0 2
p3:set 1 0
p3:set 2 1

# create a permutation
const p5 (afnix:mth:Permute 5)
p5:set 0 2
p5:set 1 0
p5:set 2 1
p5:set 3 4
p5:set 4 3

# check the mgs solver
mth-mgs-ckt-3 nil
mth-mgs-ckt-3 p3
mth-mgs-ckt-5 nil
mth-mgs-ckt-5 p5
mth-mgs-ckt-9
mth-mgs-rnd 15
mth-mgs-rnd 16
mth-mgs-rnd 17
mth-mgs-rnd 32
mth-mgs-rnd 100
