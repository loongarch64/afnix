# ---------------------------------------------------------------------------
# - SPS0002.als                                                             -
# - afnix:sps module test unit                                              -
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

# @info   record test unit
# @author amaury darsch

# get the module
interp:library "afnix-sps"

# create a new record
const  rcd  (afnix:sps:Record)
assert true (afnix:sps:record-p rcd)

# set and check name
rcd:set-name    "record"
assert "record" (rcd:get-name)

# set and check transaction number
assert -1 (rcd:get-transaction-number)
rcd:set-transaction-number 0
assert  0 (rcd:get-transaction-number)


# add data to the record
rcd:add 0 1 2 3 4 5 6 7 8 9
assert 10 (rcd:length)
loop (trans i 0) (< i 10) (i:++) {
  trans cell (rcd:get i)
  assert i (cell:get)
}

# set the cell name and check
loop (trans i 0) (< i 10) (i:++) {
  rcd:set-name i (String i)
  trans cell (rcd:get i)
  assert (String i) (cell:get-name)
}

# check by direct evaluation
assert 0 rcd:0
assert 1 rcd:1
assert 2 rcd:2
assert 3 rcd:3
assert 4 rcd:4
assert 5 rcd:5

