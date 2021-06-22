# ---------------------------------------------------------------------------
# - MTH0115.als                                                             -
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

# @info   triangular matrix test unit
# @author amaury darsch

# get the module
interp:library "afnix-mth"

# this procedure checks the upper triangular operator
const mth-upper-triangular (size) {
  # create a random matrix and vector
  const m (afnix:mth:get-random-r-matrix size 100.0 true)
  const x (afnix:mth:get-random-r-vector size 100.0)
  
  # check matrix and vector size
  assert size (m:get-row-size)
  assert size (m:get-col-size)
  assert size (x:get-size)
  
  # perform a multiply/divide
  const r (m:upper-triangular-mul x)
  const y (m:upper-triangular-div r)

  # check the solution
  assert true (x:?= y)
}

# this procedure checks the lower triangular operator
const mth-lower-triangular (size) {
  # create a random matrix and vector
  const m (afnix:mth:get-random-r-matrix size 100.0 true)
  const x (afnix:mth:get-random-r-vector size 100.0)
  
  # check matrix and vector size
  assert size (m:get-row-size)
  assert size (m:get-col-size)
  assert size (x:get-size)
  
  # perform a multiply/divide
  const r (m:lower-triangular-mul x)
  const y (m:lower-triangular-div r)

  # check the solution
  assert true (x:?= y)
}

# check the upper triangular part
mth-upper-triangular 3
mth-upper-triangular 16
mth-upper-triangular 255

# check the lower triangular part
mth-lower-triangular 4
mth-lower-triangular 15
mth-lower-triangular 127
  
