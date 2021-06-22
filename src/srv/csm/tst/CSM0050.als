# ---------------------------------------------------------------------------
# - CSM0050.als                                                             -
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

# @info   blob test unit
# @author amaury darsch

# get the service
interp:library "afnix-csm"

# create a nil blob
trans blob (afnix:csm:Blob)

# check predicate and representation
assert true  (afnix:csm:blob-p blob)
assert "Blob" (blob:repr)

# create an blob by name and info
trans  blob (afnix:csm:Blob "ONE" "ONE DIGIT")
assert ""    (blob:get-rid)
assert "ONE" (blob:get-name)
assert "ONE DIGIT" (blob:get-info)

# test the rid
blob:set-rid "RID"
assert true  (blob:rid-p "RID")
assert "RID" (blob:get-rid)

# check self swappability
assert true (blob:swappable-p blob)

# test the whatis
const wais (blob:whatis)
assert (blob:get-rid) (wais:get-rid)
assert (blob:get-creation-time)  (wais:get-creation-time)
assert (blob:get-modification-time) (wais:get-modification-time)
