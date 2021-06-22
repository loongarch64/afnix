# ---------------------------------------------------------------------------
# - SYS0004.als                                                             -
# - afnix:sys module test unit                                              -
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

# @info   complex test unit
# @author amaury darsch

# get the module
interp:library "afnix-sys"

# shared vector of threads descriptors
const thr-group (Vector)

# this expression waits until all threads in the group are finished
const wait-all nil (for (thr) (thr-group) (thr:wait))

# this expression initialize a matrix with unique numbers
# the matrix is a square one with its size as an argument
const init-matrix (n) {
  trans i (Integer 0)
  const m (Vector)
  do {
    trans v (m:add (Vector))
    trans j (Integer)
    do {
      v:add (afnix:sys:get-unique-id)
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

# set a simple size (3)
const n 3
mult-matrix (init-matrix n) (init-matrix n)

# wait for all threads to complete
wait-all

# make sure we have the right number
assert (* n n) (thr-group:length)
