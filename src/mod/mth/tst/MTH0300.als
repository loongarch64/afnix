# ---------------------------------------------------------------------------
# - MTH0300.als                                                             -
# - afnix:mth module point test unit                                      -
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

# @info   integer plane point test unit
# @author amaury darsch

# get the module
interp:library "afnix-mth"

# create a default point
const p (afnix:mth:Ippd)

# check predicate
assert true (afnix:mth:ippd-p p)

# check representation
assert "Ippd" (p:repr)

# check accessors
assert 0 (p:get-x)
assert 0 (p:get-y)

p:set-x 1
p:set-y 2
assert 1 (p:get-x)
assert 2 (p:get-y)

p:set 3 4
assert 3 (p:get-x)
assert 4 (p:get-y)
