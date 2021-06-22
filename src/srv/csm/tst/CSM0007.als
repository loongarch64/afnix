# ---------------------------------------------------------------------------
# - CSM0007.als                                                             -
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

# @info   localspace test unit
# @author amaury darsch

# get the module
interp:library "afnix-csm"

# create a default localspace
const lspc (afnix:csm:LocalSpace)
assert true (afnix:csm:workspace-p  lspc)
assert true (afnix:csm:localspace-p lspc)
# check the name
assert "LocalSpace" (lspc:repr)

# create a new zone
const znam "csm-test-zone"
const zone (lspc:add-zone znam)
# check zone existence
assert true (lspc:zone-p znam)

# add a new entry
trans name "csm-test-file"
trans zuri "zone:///csm-test-file"

trans os (lspc:get-output-stream znam name)
os:write "hello"
os:close

# check existence
assert true (lspc:exists-p znam name)
assert true (lspc:exists-p znam zuri)

# get the zone content
trans zcnt  (lspc:get-entity-list znam)
assert 1    (zcnt:length)
assert name (zcnt:get 0)

# remove the zone
assert true (lspc:remove znam)
