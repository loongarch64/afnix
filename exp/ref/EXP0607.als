# ----------------------------------------------------------------------------
# - EXP0607.als                                                              -
# - afnix example : chapter 6 : example 07                                   -
# ----------------------------------------------------------------------------
# - This program is  free software;  you can  redistribute it and/or  modify -
# - it provided that this copyright notice is kept intact.                   -
# -                                                                          -
# - This  program  is  distributed in the hope  that it  will be useful, but -
# - without  any   warranty;  without  even   the   implied    warranty   of -
# - merchantability  or fitness for a particular purpose. In not event shall -
# - the copyright holder be  liable for  any direct, indirect, incidental or -
# - special damages arising in any way out of the use of this software.      -
# ----------------------------------------------------------------------------
# - copyright (c) 1999-2021 amaury darsch                                    -
# ----------------------------------------------------------------------------

# initialize the shared library
interp:library "afnix-sys"
interp:library "afnix-mth"

# shared vector of threads descriptors
const thr-group (Vector)

# this expression waits until all threads in the group are finished
const wait-all nil (for (thr) (thr-group) (thr:wait))

# this expression initializes a matrix with random numbers
# the matrix is a square one with its size as an argument
const init-matrix (n) {
  trans i (Integer 0)
  const m (Vector)
  do {
    trans v (m:add (Vector))
    trans j (Integer)
    do {
      v:add (afnix:mth:get-random-integer)
    } (< (j:++) n)
  } (< (i:++) n)
  eval m
}

# this expression multiply one line with one column
const mult-line-column (u v) {
  assert (u:length) (v:length)
  trans result 0
  for (x y) (u v) (result:+= (* x y))
  eval result
}

# this expression multiply two vectors assuming one is a line and one is
# a column coming from the matrix
const mult-matrix (mx my) {
  for (lv) (mx) {
    assert true (vector-p lv)
    for (cv) (my) {
      assert true (vector-p cv)
      thr-group:add (launch (mult-line-column lv cv))
    }
  }
}

# check for some arguments
if (== 0 (interp:argv:length)) {
  errorln "usage: axi EXP0607 size"
  afnix:sys:exit 1
}

# get the integer and multiply
const n (Integer (interp:argv:get 0))
mult-matrix (init-matrix n) (init-matrix n)

# wait for all threads to complete
wait-all

# make sure we have the right number
assert (* n n) (thr-group:length)

