# ---------------------------------------------------------------------------
# - MTH0050.als                                                             -
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

# @info   real point test unit
# @author amaury darsch

# get the module
interp:library "afnix-mth"

# create a simple point
const rp (afnix:mth:Rpoint 3)

# check predicate
assert true (afnix:mth:rpi-p rp)
assert true (afnix:mth:r-point-p rp)

# check representation
assert "Rpoint" (rp:repr)

# check point length and elements
assert 3 (rp:get-size)

# check accessors
assert 0.0 (rp:get 0)
rp:set 0 0.0
assert 0.0 (rp:get 0)

assert 0.0 (rp:get 1)
rp:set 1 3.0
assert 3.0 (rp:get 1)

assert 0.0 (rp:get 2)
rp:set 2 4.0
assert 4.0 (rp:get 2)
