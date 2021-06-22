# ---------------------------------------------------------------------------
# - CSM0005.als                                                             -
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

# @info   session set test unit
# @author amaury darsch

# get the service
interp:library "afnix-csm"

# the session name
const name "session"

# create a new session set
const sset (afnix:csm:SessionSet)

# check predicate and representation
assert true (afnix:csm:session-set-p sset)
assert "SessionSet" (sset:repr)

# check content
assert 0 (sset:length)

# create a new session
const so (afnix:csm:Session name)
sset:add so

# check content
assert 1 (sset:length)
assert true (sset:exists-p (so:get-hash-id))
