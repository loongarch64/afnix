# ---------------------------------------------------------------------------
# - MTH1000.als                                                             -
# - afnix:mth module global test unit                                       -
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

# @info   real samples array serialization test
# @author amaury darsch

# get the module
interp:library "afnix-mth"
interp:library "afnix-sio"

# the test dimension
const COLS 16
const ROWS 128

# create a test samples
trans ro (afnix:mth:Rsamples COLS)

# add several rows
loop (trans row 0) (< row ROWS) (row:++) {
  # create a new row
  assert row (ro:new-row row)
  # fill the column
  loop (trans col 0) (< col COLS) (col:++) {
    ro:set row col col
  }
}

# simple verifications
assert COLS (ro:get-columns)
assert ROWS (ro:get-rows)

# the io buffer transit
const io (afnix:sio:InputOutput true)

# serialize the array
ro:write-serial io

# create a default array
const ri (afnix:mth:Rsamples)
# deserialize the array
ri:read-serial io

# and now compare
assert COLS (ri:get-columns)
assert ROWS (ri:get-rows)

loop (trans row 0) (< row ROWS) (row:++) {
  # fill the column
  loop (trans col 0) (< col COLS) (col:++) {
    #println row ' ' col ' ' (ri:get row col) ' ' (ro:get row col)
    assert (ri:get row col) (ro:get row col)
  }
}
