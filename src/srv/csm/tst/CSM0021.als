# ---------------------------------------------------------------------------
# - CSM0021.als                                                             -
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

# @info   identity test unit
# @author amaury darsch

# get the service
interp:library "afnix-csm"

# create a guest identity
const name "guest"
const info "guest identity"
trans idty (afnix:csm:Identity name info)

# check predicate and representation
assert true  (afnix:csm:blob-p idty)
assert true  (afnix:csm:identity-p idty)
assert "Identity" (idty:repr)

# check name and info
assert name (idty:get-name)
assert info (idty:get-info)

