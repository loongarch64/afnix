# ---------------------------------------------------------------------------
# - SEC0030.als                                                             -
# - afnix:sec module test unit                                              -
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

# @info   ecc point test unit
# @author amaury darsch

# get the module
interp:library "afnix-sec"

# create a null point
trans  p     (afnix:sec:Ecp)
assert true  (afnix:sec:ecp-p p)
assert "Ecp" (p:repr)

# check for null point
assert 0R (p:get-x)
assert 0R (p:get-y)
assert true (p:nil-p)

# bind the point
p:set 1 2R
assert 1R (p:get-x)
assert 2R (p:get-y)

p:set-x 2R
p:set-y 1
assert 2R (p:get-x)
assert 1R (p:get-y)
