# ---------------------------------------------------------------------------
# - CSM0020.als                                                             -
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

# @info   credential test unit
# @author amaury darsch

# get the service
interp:library "afnix-csm"

# create a nil credential
trans cred (afnix:csm:Credential)

# check predicate and representation
assert true  (afnix:csm:credential-p cred)
assert "Credential" (cred:repr)

# add a credential
cred:add afnix:csm:Credential:NONE ""
cred:add afnix:csm:Credential:PASS "afnix"
# check length
assert 2 (cred:length)

# check content
assert afnix:csm:Credential:NONE (cred:get-type 0)
assert afnix:csm:Credential:PASS (cred:get-type 1)

