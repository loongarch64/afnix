# ---------------------------------------------------------------------------
# - CSM0006.als                                                             -
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

# @info   localzone test unit
# @author amaury darsch

# get the module
interp:library "afnix-csm"

# create a temporary localzone
const lzon (afnix:csm:LocalZone)
assert true (afnix:csm:workzone-p  lzon)
assert true (afnix:csm:localzone-p lzon)
# check the name
assert "LocalZone" (lzon:repr)

# get the zone content
trans zlst (lzon:get-entity-list)
assert 0 (zlst:length)

# add a new entry
trans name "csm-test-file"
trans zuri "zone:///csm-test-file"

trans os (lzon:get-output-stream zuri)
os:write "hello"
os:close

# check existence
assert true (lzon:exists-p name)
assert true (lzon:exists-p zuri)

# get the zone content
trans zlst  (lzon:get-entity-list)
assert 1    (zlst:length)
assert name (zlst:get 0)
