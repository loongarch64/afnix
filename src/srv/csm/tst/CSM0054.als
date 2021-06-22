# ---------------------------------------------------------------------------
# - CSM0054.als                                                             -
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

# @info   authority test unit
# @author amaury darsch

# get the module
interp:library "afnix-csm"

# create an authority by name
const name "guest"
const info "guest authority"
const auth (afnix:csm:Authority name info)

# check predicate and representation
assert true (afnix:csm:authority-p auth)
assert "Authority" (auth:repr)

# check the authority name and info
assert name (auth:get-name)
assert info (auth:get-info)

# create a credential
const cred (afnix:csm:Credential)
cred:add afnix:csm:Credential:PASS "guest"

#  set the credential and check
auth:set-credential cred
assert true (auth:valid-p cred)
# get the credential and validate
trans  c (auth:get-credential)
assert true (afnix:csm:credential-p c)
assert true (c:valid-p cred)
