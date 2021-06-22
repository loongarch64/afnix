# ---------------------------------------------------------------------------
# - CSM0053.als                                                             -
# - afnix:csm service test unit                                             -
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

# @info   collection test unit
# @author amaury darsch

# get the service
interp:library "afnix-csm"

# create a nil collection
trans coll (afnix:csm:Collection)

# check predicate and representation
assert true (afnix:csm:collection-p coll)
assert "Collection" (coll:repr)

# check for empty
assert true  (coll:empty-p)
assert 0     (coll:length)

# add a test part
const part (afnix:csm:Part "part" "test part")
trans uuid (coll:add-part part)

# validate the part
assert 1 (coll:length)
# set-part and check
assert true (coll:set-part part)
# get the part by kid
const sprt (coll:get-by-kid uuid)
assert true (sprt:swappable-p part)
