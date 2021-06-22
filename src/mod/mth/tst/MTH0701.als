# ---------------------------------------------------------------------------
# - MTH0701.als                                                             -
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

# @info   floating point test unit
# @author amaury darsch

# get the module
interp:library "afnix-mth"

# create a simple point
const np (afnix:mth:Npoint 3)

# check predicate
assert true (afnix:mth:npi-p np)
assert true (afnix:mth:n-point-p np)

# check representation
assert "Npoint" (np:repr)

# check point length and elements
assert 3 (np:get-size)

# check accessors
trans nz (Numeral 0)
assert nz (np:get 0)
np:set 0 nz
assert nz (np:get 0)

trans no (Numeral 1)
assert nz (np:get 1)
np:set 1 no
assert no (np:get 1)

trans nt (Numeral 0)
assert nz (np:get 2)
np:set 2 nt
assert nt (np:get 2)
