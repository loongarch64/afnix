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

# @info   qr solver test unit
# @author amaury darsch

# get the module
interp:library "afnix-mth"
# load matrix generator
interp:load "MTHXXXX"

# this procedure checks the qr solver with a matrix
const mth-qr-ckt-3 (p) {
  # create 3x3 matrix
  const am (afnix:mth:Rmatrix 3)
  mth-set-3x3 am
  # create the initial vector
  const bv (afnix:mth:Rvector 3)
  bv:set 0 5.0
  # create a qr solver with a permutation
  trans qr (afnix:mth:Qr am)
  qr:set-permutation p
  # solve the system
  trans xv (qr:solve bv)
  # verify result
  assert true (qr:verify bv xv)
}

# this procedure checks the qr solver with a matrix
const mth-qr-ckt-5 (p) {
  # create 5x5 matrix
  const am (afnix:mth:Rmatrix 5)
  mth-set-5x5 am
  # create the initial vector
  const bv (afnix:mth:Rvector 5)
  bv:set 2 0.15
  # create a qr solver with a permutation
  trans qr (afnix:mth:Qr am)
  qr:set-permutation p
  # solve the system
  trans xv (qr:solve bv)
  # verify result
  assert true (qr:verify bv xv)
}

# this procedure checks the qr factorization with a matrix
const mth-qr-ckt-9 nil {
  # create 9x9 matrix
  const am (afnix:mth:Rmatrix 9)
  mth-set-9x9 am
  # create the initial vector
  const bv (afnix:mth:Rvector 9)
  bv:set 4 4.0
  # solve the system with the qr solver
  trans qr (afnix:mth:Qr am)
  trans xv (qr:solve bv)
  # verify result
  assert true (qr:verify bv xv)
}

# this procedure checks the qr solver with a random matrix
const mth-qr-rnd (size) {
  const imax (* size 10)
  
  # create a random matrix and vector
  const am (afnix:mth:get-random-r-matrix size 100.0 true)
  const rv (afnix:mth:get-random-r-vector size 100.0)
  
  # check matrix and vector size
  assert size (am:get-row-size)
  assert size (am:get-col-size)
  assert size (rv:get-size)
  
  # create an linear system vector
  const bv (* am rv)
  # solve the system with the qr solver
  trans qr (afnix:mth:Qr am)
  trans xv (qr:solve bv)
  # verify result
  assert true (qr:verify bv xv)
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

# check the qr solver
mth-qr-ckt-3 nil
mth-qr-ckt-3 p3
mth-qr-ckt-5 nil
mth-qr-ckt-5 p5
mth-qr-ckt-9
mth-qr-rnd 15
mth-qr-rnd 16
mth-qr-rnd 17
mth-qr-rnd 32
mth-qr-rnd 100
