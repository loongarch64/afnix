# ---------------------------------------------------------------------------
# - MTH0700.als                                                             -
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

# @info   numeral vector test unit
# @author amaury darsch

# get the module
interp:library "afnix-mth"

# create a simple vector
const nv (afnix:mth:Nvector 3)

# check predicate
assert true (afnix:mth:avi-p nv)
assert true (afnix:mth:nvi-p nv)
assert true (afnix:mth:n-vector-p nv)
# check representation
assert "Nvector" (nv:repr)

# check vector length and predicate
assert 3 (nv:get-size)
assert true (nv:nil-p)

# check accessors
trans nz (Numeral 0)
assert 0 (nv:get 0)
nv:set 0 nz
assert nz (nv:get 0)

trans no (Numeral 1)
assert 0 (nv:get 1)
nv:set 1 no
assert no (nv:get 1)

trans nt (Numeral 0)
assert 0 (nv:get 2)
nv:set 2 nt
assert nt (nv:get 2)

