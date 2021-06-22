# ---------------------------------------------------------------------------
# - MTH0000.als                                                             -
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

# @info   mth matrix reference test unit
# @author amaury darsch

# get the module
interp:library "afnix-mth"

# set a 3x3 matrix
const mth-set-3x3 (m) {
  m:clear
  m:set 0 2     1.0
  m:set 1 1 -1000.0
  m:set 1 2     1.0
  m:set 2 0     1.0
  m:set 2 1     1.0
}

# set a 5x5 matrix
const mth-set-5x5 (m) {
  m:clear
  m:set 0 0 -1000.0
  m:set 0 3     1.0
  m:set 0 4    -1.0
  m:set 1 1     1.0
  m:set 1 4    -0.0001
  m:set 2 3     1.0
  m:set 3 0     1.0
  m:set 3 2     1.0
  m:set 4 0    -1.0
  m:set 4 1     1.0
}

# set a 9x9 matrix
const mth-set-9x9 (m) {
  m:clear
  m:set 0 0 -1000.0
  m:set 0 5     1.0
  m:set 0 6    -1.0
  m:set 1 1 -1000.0
  m:set 1 6     1.0
  m:set 1 7    -1.0
  m:set 2 2 -1000.0
  m:set 2 7     1.0
  m:set 2 8    -1.0
  m:set 3 3 -1000.0
  m:set 3 8     1.0
  m:set 4 4     1.0
  m:set 5 0     1.0
  m:set 5 4     1.0
  m:set 6 0    -1.0
  m:set 6 1     1.0
  m:set 7 1    -1.0
  m:set 7 2     1.0
  m:set 8 2    -1.0
  m:set 8 3     1.0
}

# check a 3x3 matrix
const mth-chk-3x3 (m) {
  assert     0.0 (m:get 0 0)
  assert     0.0 (m:get 0 1)
  assert     1.0 (m:get 0 2)
  assert     0.0 (m:get 1 0)
  assert -1000.0 (m:get 1 1)
  assert     1.0 (m:get 1 2)
  assert     1.0 (m:get 2 0)
  assert     1.0 (m:get 2 1)
  assert     0.0 (m:get 2 2)
}
