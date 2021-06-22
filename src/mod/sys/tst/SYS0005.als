# ---------------------------------------------------------------------------
# - SYS0005.als                                                             -
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

# @info   bubble sort on integers test unit
# @author amaury darsch

# get the module
interp:library "afnix-sys"

# initialize the vector
const vector-init (n) {
  trans result (Vector)
  trans i 0
  do (result:add (afnix:sys:get-unique-id)) (< (i:++) n)
  eval result
}

# swap two argument in a vector
const swap (v i) {
  trans vc (v:get i)
  trans vn (v:get (+ i 1))
  v:set i vn
  v:set (+ i 1) vc
}

# sort the vector
const bubble-sort (v) {
  trans bound (- (v:length) 1)
  trans t     (Integer)
  do {
    trans j (Integer)
    do {
      if (> (v:get j) (v:get (+ j 1))) {
        swap  v j
        trans t (Integer j)
      }
    } (< (j:++) bound)
    trans bound (Integer t)
    trans t     (Integer)
  } (!= bound 0)
  eval v
}

# check for sorted
const vector-check (v) {
  trans n (- (v:length) 1)
  trans i 0
  while (< i n) (assert true (<= (v:get i) (v:get (i:++))))
}

# initialize the vector and sort i
vector-check (bubble-sort (vector-init 10))

