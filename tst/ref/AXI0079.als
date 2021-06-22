# ---------------------------------------------------------------------------
# - AXI0079.als                                                             -
# - afnix engine test module                                                -
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

# @info   structure object test
# @author amaury darsch

# create a structure
trans s (Structure)
assert true (structure-p s)
assert "Structure" (s:repr)

# define the structure by locale
assert true (s:add-locale "N=LONG-NONE&L=0&O=INTG&X=NONE")
assert true (s:add-locale "N=LONG-SHRT&L=1&O=INTG&X=SHRT")
assert true (s:add-locale "N=REAL-NONE&L=2&O=REAL&X=NONE")
assert true (s:add-locale "N=REAL-SHRT&L=3&O=REAL&X=SHRT")

# check buffer size
assert 0 (s:length)

# open for recording
s:open
# build structure
assert true (s:add 1)
assert true (s:add -1)
assert true (s:add 1.0)
assert true (s:add -1.0)
# close recording
s:close
assert 24 (s:length)

if interp:big-endian-p {
  assert "0000000000000001FFFFFFFF3FF0000000000000BF800000" (s:format)
} {
  assert "0100000000000000FFFFFFFF000000000000F03F000080BF" (s:format)
}
assert  1   (s:get 0 0)
assert -1   (s:get 0 1)
assert  1.0 (s:get 0 2)
assert -1.0 (s:get 0 3)

# open for recording
s:open
# build structure
assert true (s:add 2)
assert true (s:add -2)
assert true (s:add 2.0)
assert true (s:add -2.0)
# close recording
s:close
assert 48 (s:length)

assert  2   (s:get 1 0)
assert -2   (s:get 1 1)
assert  2.0 (s:get 1 2)
assert -2.0 (s:get 1 3)

# create a new structure
trans s (Structure)
assert true (s:add-locale "N=ITEM&L=0&O=INTG&X=SHRT")
assert true (s:add-locale "N=QUTY&L=1&O=INTG&X=SHRT")
assert true (s:add-locale "N=UNIT&L=2&O=REAL&X=SHRT")

trans cntr (Counter 0 10)
while (cntr:valid-p) {
  s:open
  s:add (cntr:++)
  s:add 8
  s:add 3.141592
  s:close
}
trans cntr (Counter 0 10)
while (cntr:valid-p) {
  assert (cntr:get) (s:get (cntr:++) 0)
}
