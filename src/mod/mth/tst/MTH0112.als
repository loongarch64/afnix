# ---------------------------------------------------------------------------
# - MTH0112.als                                                             -
# - afnix:mth matrix iterator test unit                                     -
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

# @info   real large sparse matrix test unit
# @author amaury darsch

# get the module
interp:library "afnix-mth"

# compute a direct matrix column sum

const get-col-sum (m row) {
  const cols (m:get-col-size)
  trans sum 0.0
  loop (trans k 0) (< k cols) (k:++) {
    sum:+= (rm:get row k)
  }
  eval sum
}

# compute an iterated direct matrix column sum

const itr-dir-sum (m row) {
  const it (m:iterator)
  it:set-type afnix:mth:Rmit:COL
  it:move row 0
  trans sum 0.0
  while (it:valid-p) {
    trans rmd (it:get-object)
    sum:+= (rmd:get-val)
    it:next
  }
  eval sum
}

# create a 16x16 random matrix
trans rm (afnix:mth:get-random-r-matrix 16 10.0 true)
assert (get-col-sum rm  0) (itr-dir-sum rm  0)
assert (get-col-sum rm 15) (itr-dir-sum rm 15)

# create a 17x17 random matrix
trans rm (afnix:mth:get-random-r-matrix 17 10.0 true)
assert (get-col-sum rm  0) (itr-dir-sum rm  0)
assert (get-col-sum rm 15) (itr-dir-sum rm 15)
assert (get-col-sum rm 16) (itr-dir-sum rm 16)

# create a 32x32 random matrix
trans rm (afnix:mth:get-random-r-matrix 32 10.0 true)
assert (get-col-sum rm  0) (itr-dir-sum rm  0)
assert (get-col-sum rm 15) (itr-dir-sum rm 15)
assert (get-col-sum rm 31) (itr-dir-sum rm 31)

# create a 128x128 random matrix
trans rm (afnix:mth:get-random-r-matrix 128 10.0 true)
assert (get-col-sum rm   0) (itr-dir-sum rm   0)
assert (get-col-sum rm  15) (itr-dir-sum rm  15)
assert (get-col-sum rm  31) (itr-dir-sum rm  31)
assert (get-col-sum rm  32) (itr-dir-sum rm  32)
assert (get-col-sum rm  63) (itr-dir-sum rm  63)
assert (get-col-sum rm  64) (itr-dir-sum rm  64)
assert (get-col-sum rm 127) (itr-dir-sum rm 127)
