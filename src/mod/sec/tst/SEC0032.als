# ---------------------------------------------------------------------------
# - SEC0032.als                                                             -
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
trans  c     (afnix:sec:Sect)
assert true  (afnix:sec:sect-p c)
assert "Sect" (c:repr)

# check for invalid
assert false (c:valid-p)

# create a valid curve
trans g (afnix:sec:Ecp  0x08R 0x01R)
trans c (afnix:sec:Sect 0x13R 0x08R 0x09R g 11R 1R)
assert true (c:valid-p)
assert true (c:valid-p g)

# validate the points
assert true (c:valid-p (afnix:sec:Ecp  0x00R 0x00R))
assert true (c:valid-p (afnix:sec:Ecp  0x00R 0x0BR))
assert true (c:valid-p (afnix:sec:Ecp  0x01R 0x00R))
assert true (c:valid-p (afnix:sec:Ecp  0x01R 0x01R))
assert true (c:valid-p (afnix:sec:Ecp  0x02R 0x0DR))
assert true (c:valid-p (afnix:sec:Ecp  0x02R 0x0FR))
assert true (c:valid-p (afnix:sec:Ecp  0x03R 0x0CR))
assert true (c:valid-p (afnix:sec:Ecp  0x03R 0x0FR))
assert true (c:valid-p (afnix:sec:Ecp  0x05R 0x00R))
assert true (c:valid-p (afnix:sec:Ecp  0x05R 0x05R))
assert true (c:valid-p (afnix:sec:Ecp  0x07R 0x0BR))
assert true (c:valid-p (afnix:sec:Ecp  0x07R 0x0CR))
assert true (c:valid-p (afnix:sec:Ecp  0x08R 0x01R))
assert true (c:valid-p (afnix:sec:Ecp  0x08R 0x09R))
assert true (c:valid-p (afnix:sec:Ecp  0x09R 0x06R))
assert true (c:valid-p (afnix:sec:Ecp  0x09R 0x0FR))
assert true (c:valid-p (afnix:sec:Ecp  0x0BR 0x02R))
assert true (c:valid-p (afnix:sec:Ecp  0x0BR 0x09R))
assert true (c:valid-p (afnix:sec:Ecp  0x0CR 0x00R))
assert true (c:valid-p (afnix:sec:Ecp  0x0CR 0x0CR))
assert true (c:valid-p (afnix:sec:Ecp  0x0FR 0x04R))
assert true (c:valid-p (afnix:sec:Ecp  0x0FR 0x0BR))

# add two points
trans px (afnix:sec:Ecp 0x02R 0x0FR)
trans py (afnix:sec:Ecp 0x0CR 0x0CR)
trans ps (c:add px py)
assert 0x01R (ps:get-x)
assert 0x01R (ps:get-y)
assert true (c:valid-p ps)

# double two points
trans px (afnix:sec:Ecp 0x02R 0x0FR)
trans ps (c:add px px)
assert 0x0BR (ps:get-x)
assert 0x02R (ps:get-y)
assert true (c:valid-p ps)

# negate points
assert true (c:valid-p (c:neg px))
assert true (c:valid-p (c:neg py))
assert true (c:valid-p (c:neg ps))

# multiply point
trans px (afnix:sec:Ecp 0x02R 0x0FR)
trans pm (c:mul 3 px)
trans pa (c:add px (c:add px px))
assert (pm:get-x) (pa:get-x)
assert (pm:get-y) (pa:get-y)
