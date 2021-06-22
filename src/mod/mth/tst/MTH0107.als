# ---------------------------------------------------------------------------
# - MTH0107.als                                                             -
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

# @info   bcs krylov solver test unit
# @author amaury darsch

# get the module
interp:library "afnix-mth"

# set the system precision
interp:set-absolute-precision 1.0E-6
interp:set-relative-precision 1.0E-16

# this procedure checks the bcs solver with a matrix
const mth-bcs-ckt-3 nil {
  # create 3x3 matrix
  const am (afnix:mth:Rmatrix 3)
  am:set 0 2     1.0
  am:set 1 1 -1000.0
  am:set 1 2     1.0
  am:set 2 0     1.0
  am:set 2 1     1.0
  # create the initial vector
  const bv (afnix:mth:Rvector 3)
  bv:set 0 5.0
  # create the result vector
  const rv (afnix:mth:Rvector 3)
  rv:set 0 -0.005
  rv:set 1  0.005
  rv:set 2  5.000
  # create a bcs solver
  trans slv (afnix:mth:Bcs am)
  slv:set-max-iterations 6
  # solve the system
  trans xv (slv:solve bv)
  # verify result
  assert true (xv:?= rv)
  # create a preconditioner vector
  const mv (afnix:mth:Rvector 3)
  mv:set 0  1.0
  mv:set 1 -0.001
  mv:set 2  1.0
  # create a bcs solver
  trans slv (afnix:mth:Bcs am mv)
  slv:set-max-iterations 6
  # solve the system
  trans xv (slv:solve bv)
  # verify result
  assert true (xv:?= rv)
}

# this procedure checks the bcs with a matrix
const mth-bcs-ckt-9 nil {
  # create 9x9 matrix
  const am (afnix:mth:Rmatrix 9)
  am:set 0 0 -1000.0
  am:set 0 5     1.0
  am:set 0 6    -1.0
  am:set 1 1 -1000.0
  am:set 1 6     1.0
  am:set 1 7    -1.0
  am:set 2 2 -1000.0
  am:set 2 7     1.0
  am:set 2 8    -1.0
  am:set 3 3 -1000.0
  am:set 3 8     1.0
  am:set 4 5     1.0
  am:set 5 0     1.0
  am:set 5 4     1.0
  am:set 6 0    -1.0
  am:set 6 1     1.0
  am:set 7 1    -1.0
  am:set 7 2     1.0
  am:set 8 2    -1.0
  am:set 8 3     1.0
  # create the initial vector
  const bv (afnix:mth:Rvector 9)
  bv:set 4 4.0
  # create the result vector
  const rv (afnix:mth:Rvector 9)
  rv:set 0  0.001
  rv:set 1  0.001
  rv:set 2  0.001
  rv:set 3  0.001
  rv:set 4 -0.001
  rv:set 5  4.000
  rv:set 6  3.000
  rv:set 7  2.000
  rv:set 8  1.000
  # create a bcs solver
  trans slv (afnix:mth:Bcs am)
  slv:set-max-iterations 18
  # solve the system
  trans xv (slv:solve bv)
  # verify result
  assert true (xv:?= rv)
  # create a preconditioner vector
  const mv (afnix:mth:Rvector 9)
  mv:set 0 -0.001
  mv:set 1 -0.001
  mv:set 2 -0.001
  mv:set 3 -0.001
  mv:set 4  1.0
  mv:set 5  1.0
  mv:set 6  1.0
  mv:set 7  1.0
  mv:set 8  1.0
  # create a bcs solver
  trans slv (afnix:mth:Bcs am mv)
  slv:set-max-iterations 18
  # solve the system
  trans xv (slv:solve bv)
  # verify result
  assert true (xv:?= rv)
}

# this procedure checks the bcs with a random matrix
const mth-bcs-rnd (size) {
  const mni (* size 10)
  
  # create a random matrix and vector
  const am (afnix:mth:get-random-r-matrix size 100.0 true)
  const rv (afnix:mth:get-random-r-vector size 100.0)
  
  # check matrix and vector size
  assert size (am:get-row-size)
  assert size (am:get-col-size)
  assert size (rv:get-size)
  
  # create an linear system vector
  const bv (* am rv)
  # create a bcs solver
  trans slv (afnix:mth:Bcs am)
  slv:set-max-iterations mni
  # check solver
  assert true  (afnix:mth:bcs-p slv)
  assert true  (afnix:mth:solver-p slv)
  assert true  (afnix:mth:iterative-p slv)
  assert "Bcs" (slv:repr)
  # solve the system
  trans xv (slv:solve bv)
  # check the solution
  assert true (rv:?= xv)
}

# check the bcs
mth-bcs-ckt-3
mth-bcs-ckt-9
mth-bcs-rnd 100
  
