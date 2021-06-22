# ---------------------------------------------------------------------------
# - CSM0060.als                                                             -
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

# @info   realmzone test unit
# @author amaury darsch

# get the module
interp:library "afnix-csm"

# create a temporary realm
const relm (afnix:csm:RealmZone)
assert true (afnix:csm:realm-p relm)
assert true (afnix:csm:realmzone-p relm)
assert "RealmZone" (relm:repr)

# assert a non existent authority
assert false (relm:exists-p "guest")

# create a guest authority
const rid  "guest"
const name "guest"
const info "guest authority"
const auth (afnix:csm:Authority rid name info)
assert name (auth:get-name)
assert info (auth:get-info)
assert rid  (auth:get-rid)
# get the authority kid
const kid (auth:get-kid)

# create a credential
const cred (afnix:csm:Credential)
cred:add afnix:csm:Credential:PASS "guest"
auth:set-credential cred

# update the realm
relm:update auth
assert true (relm:exists-p (kid:to-string))

# validate the authority
assert true (relm:valid-p (kid:to-string) cred)
