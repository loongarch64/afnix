# ---------------------------------------------------------------------------
# - MTH0109.als                                                             -
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

# @info   real block matrix iterator test unit
# @author amaury darsch

# get the module
interp:library "afnix-mth"

# create a simple matrix
trans rm (afnix:mth:Rblock 4 3)
rm:set 0 0  1.0
rm:set 0 2  3.0
rm:set 1 1  5.0
rm:set 1 2  6.0
rm:set 2 0  7.0
rm:set 2 1  8.0
rm:set 2 2  9.0
rm:set 3 0 10.0
rm:set 3 1 11.0

# compute global sum
trans sum 0.0 
for (val) (rm) (sum:+= (val:get-val))
assert 60.0 sum

# get an iterator
trans it (rm:iterator)

# loop in sequential mode
trans sum 0.0 
while (it:valid-p) {
  trans rmd (it:get-object)
  sum:+= (rmd:get-val)
  it:next
}
assert 60.0 sum

# test column 0
trans sum 0.0
it:move 0 0
it:set-type afnix:mth:Rmit:ROW
while (it:valid-p) {
  trans rmd (it:get-object)
  sum:+= (rmd:get-val)
  it:next
}
assert 18.0 sum

# test column 1
trans sum 0.0
it:move 0 1
while (it:valid-p) {
  trans rmd (it:get-object)
  sum:+= (rmd:get-val)
  it:next
}
assert 24.0 sum

# test column 2
trans sum 0.0
it:move 0 2
while (it:valid-p) {
  trans rmd (it:get-object)
  sum:+= (rmd:get-val)
  it:next
}
assert 18.0 sum

# test row 0
trans sum 0.0
it:move 0 0
it:set-type afnix:mth:Rmit:COL
while (it:valid-p) {
  trans rmd (it:get-object)
  sum:+= (rmd:get-val)
  it:next
}
assert 4.0 sum

# test row 1
trans sum 0.0
it:move 1 0
while (it:valid-p) {
  trans rmd (it:get-object)
  sum:+= (rmd:get-val)
  it:next
}
assert 11.0 sum

# test row 2
trans sum 0.0
it:move 2 0
while (it:valid-p) {
  trans rmd (it:get-object)
  sum:+= (rmd:get-val)
  it:next
}
assert 24.0 sum

# test row 3
trans sum 0.0
it:move 3 0
while (it:valid-p) {
  trans rmd (it:get-object)
  sum:+= (rmd:get-val)
  it:next
}
assert 21.0 sum

# create a generix 32x32 matrix
trans rm (afnix:mth:Rblock 32 32)
loop (trans row 0) (< row 32) (row:++) {
  loop (trans col 0) (< col 32) (col:++) {
    rm:set row col (Real (+ (* row 32) col))
  }
}

# get an iterator - and loop by row
trans it (rm:iterator)
it:set-type afnix:mth:Rmit:COL

loop (trans row 0) (< row 32) (row:++) {
  it:move row 0
  while (it:valid-p) {
    trans rmd (it:get-object)
    trans row (rmd:get-row)
    trans col (rmd:get-col)
    trans val (Real (+ (* row 32) col))
    assert val (rmd:get-val)
    it:next
  }
}

# create another 32x32 matrix
trans rm (afnix:mth:Rblock 32 32)
rm:set 30 16 1.0

trans it (rm:iterator)
it:set-type afnix:mth:Rmit:COL
it:move  30 0

trans rmd (it:get-object)
trans val (rmd:get-val)
if (== val 0.0) (it:next)

if (it:valid-p) {
  trans rmd (it:get-object)
  trans row (rmd:get-row)
  trans col (rmd:get-col)
  trans val (rmd:get-val)
  assert row 30
  assert col 16
  assert val 1.0
}
