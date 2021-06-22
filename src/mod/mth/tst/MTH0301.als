# ---------------------------------------------------------------------------
# - MTH0301.als                                                             -
# - afnix:mth module point test unit                                      -
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

# @info   real samples array test unit
# @author amaury darsch

# get the module
interp:library "afnix-mth"

# create a default samples
trans rsa (afnix:mth:Rsamples)

# check predicate ands representation
assert true (afnix:mth:r-samples-p rsa)
assert "Rsamples" (rsa:repr)

# check default rows and columns
assert 1 (rsa:get-columns)
assert 0 (rsa:get-rows)

# restart with 3 columns
trans cols 3
trans reps 0.001
trans rsa  (afnix:mth:Rsamples cols)
rsa:set-number-precision 5

# check default rows and columns
assert cols (rsa:get-columns)
assert 0    (rsa:get-rows)

# check min/max signed time
trans mt (rsa:min-signed-time)
assert true (mt:nan-p)
trans mt (rsa:max-signed-time)
assert true (mt:nan-p)
# check min/max signed column
trans mc (rsa:min-signed-column 0)
assert true (mc:nan-p)
trans mc (rsa:max-signed-column 2)
assert true (mc:nan-p)

# create a new row at time 0
trans row (rsa:new-row 0.0)
assert 0 row
loop (trans i 0) (< i cols) (i:++) {
  trans r (rsa:get row i)
  assert true (r:nan-p)
}
# check min/max signed time
assert 0.0 (rsa:min-signed-time)
assert 0.0 (rsa:max-signed-time)

# add element in row
loop (trans i 0) (< i cols) (i:++) {
  trans cval (* (Real i) reps)
  rsa:set row i cval
  assert cval (rsa:get row i)
}
# create a new row and do it again
trans  row (rsa:new-row)
assert 1   row
# check time in row 1
const  t    (rsa:get-time 1)
assert true (t:nan-p)
# add element in row 1
loop (trans i 0) (< i cols) (i:++) {
  trans cval (+ (* (Real i) reps) reps)
  rsa:set row i cval
  assert cval (rsa:get row i)
}
# add in row 2
trans  row (rsa:new-row 0.2)
loop (trans i 0) (< i cols) (i:++) {
  trans cval (+ (* (Real i) reps) reps)
  rsa:set row i cval
  assert cval (rsa:get row i)
}
# add in row 3
trans  row (rsa:new-row 0.3)
loop (trans i 0) (< i cols) (i:++) {
  trans cval (+ (* (Real i) reps) reps)
  rsa:set row i cval
  assert cval (rsa:get row i)
}
# add in row 4
trans  row (rsa:new-row 0.4)
loop (trans i 0) (< i cols) (i:++) {
  trans cval (+ (* (Real i) reps) reps)
  rsa:set row i cval
  assert cval (rsa:get row i)
}

# check size
assert 5 (rsa:get-rows)
assert cols (rsa:get-columns)

# smooth the array
const  rss (rsa:smooth 0.01)
assert 3   (rss:get-columns)
 
# clear and check again
rsa:clear
assert 0 (rsa:get-rows)
assert cols (rsa:get-columns)

