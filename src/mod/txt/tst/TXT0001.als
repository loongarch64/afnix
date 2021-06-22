# ---------------------------------------------------------------------------
# - TXT0001.als                                                             -
# - afnix:txt module test unit                                              -
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

# @info   sort test unit
# @author amaury darsch

# get the module
interp:library "afnix-txt"

# this fucntion checks that a vector is sorted in ascending order
const check-vector-ascent (v) {
  trans elem (v:get 0)
  for (i) (v) {
    assert (>= i elem) true
    elem:= i
  }
}

# create an unsorted vector
const v-i (Vector 7 5 3 4 1 8 0 9 2 6)
const v-c (Vector 'h' 'e' 'l' 'l' 'o')
const v-s (Vector "world" "hello" "bonjour")

# sort and check integer
afnix:txt:sort-ascent  v-i
check-vector-ascent    v-i

# sort and check characters
afnix:txt:sort-ascent  v-c
check-vector-ascent    v-c

# sort and check strings
afnix:txt:sort-ascent  v-s
check-vector-ascent    v-s
