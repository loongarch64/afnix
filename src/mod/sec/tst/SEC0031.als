# ---------------------------------------------------------------------------
# - SEC0031.als                                                             -
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

# @info   ecc curve test unit
# @author amaury darsch

# get the module
interp:library "afnix-sec"

# create a null curve
trans  c     (afnix:sec:Secp)
assert true  (afnix:sec:secp-p c)
assert "Secp" (c:repr)

# check for invalid
assert false (c:valid-p)

# create a valid curve
trans g (afnix:sec:Ecp 0R 7R)
trans c (afnix:sec:Secp 29R 4R 20R g 26R 1R)
assert true (c:valid-p)
assert true (c:valid-p g)
# validate the points
assert true (c:valid-p (afnix:sec:Ecp  0R  0R))
assert true (c:valid-p (afnix:sec:Ecp  0R 22R))
assert true (c:valid-p (afnix:sec:Ecp  1R  5R))
assert true (c:valid-p (afnix:sec:Ecp  1R 24R))
assert true (c:valid-p (afnix:sec:Ecp  2R  6R))
assert true (c:valid-p (afnix:sec:Ecp  2R 23R))
assert true (c:valid-p (afnix:sec:Ecp  3R  1R))
assert true (c:valid-p (afnix:sec:Ecp  3R 28R))
assert true (c:valid-p (afnix:sec:Ecp  4R 10R))
assert true (c:valid-p (afnix:sec:Ecp  4R 19R))
assert true (c:valid-p (afnix:sec:Ecp  5R  7R))
assert true (c:valid-p (afnix:sec:Ecp  5R 22R))
assert true (c:valid-p (afnix:sec:Ecp  6R 12R))
assert true (c:valid-p (afnix:sec:Ecp  6R 17R))
assert true (c:valid-p (afnix:sec:Ecp  8R 10R))
assert true (c:valid-p (afnix:sec:Ecp  8R 19R))
assert true (c:valid-p (afnix:sec:Ecp 10R  4R))
assert true (c:valid-p (afnix:sec:Ecp 10R 25R))
assert true (c:valid-p (afnix:sec:Ecp 13R  6R))
assert true (c:valid-p (afnix:sec:Ecp 13R 23R))
assert true (c:valid-p (afnix:sec:Ecp 14R  6R))
assert true (c:valid-p (afnix:sec:Ecp 14R 23R))
assert true (c:valid-p (afnix:sec:Ecp 15R  2R))
assert true (c:valid-p (afnix:sec:Ecp 15R 27R))
assert true (c:valid-p (afnix:sec:Ecp 15R 27R))
assert true (c:valid-p (afnix:sec:Ecp 16R 27R))
assert true (c:valid-p (afnix:sec:Ecp 17R 10R))
assert true (c:valid-p (afnix:sec:Ecp 17R 19R))
assert true (c:valid-p (afnix:sec:Ecp 19R 13R))
assert true (c:valid-p (afnix:sec:Ecp 19R 16R))
assert true (c:valid-p (afnix:sec:Ecp 20R  3R))
assert true (c:valid-p (afnix:sec:Ecp 20R 26R))
assert true (c:valid-p (afnix:sec:Ecp 24R  7R))
assert true (c:valid-p (afnix:sec:Ecp 24R 22R))
assert true (c:valid-p (afnix:sec:Ecp 27R  2R))
assert true (c:valid-p (afnix:sec:Ecp 27R 27R))

# add two points
trans px (afnix:sec:Ecp  5R 22R)
trans py (afnix:sec:Ecp 16R 27R)
trans ps (c:add px py)
assert 13R (ps:get-x)
assert  6R (ps:get-y)
assert true (c:valid-p ps)

# double two points
trans px (afnix:sec:Ecp  5R 22R)
trans ps (c:add px px)
assert 14R (ps:get-x)
assert  6R (ps:get-y)
assert true (c:valid-p ps)

# negate points
assert true (c:valid-p (c:neg px))
assert true (c:valid-p (c:neg py))
assert true (c:valid-p (c:neg ps))

# multiply point
trans px (afnix:sec:Ecp 5R 22R)
trans pm (c:mul 3 px)
trans pa (c:add px (c:add px px))
assert (pm:get-x) (pa:get-x)
assert (pm:get-y) (pa:get-y)
