# ---------------------------------------------------------------------------
# - CSM0055.als                                                             -
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

# @info   principal test unit
# @author amaury darsch

# get the module
interp:library "afnix-csm"

# create an principal by name
const name "guest"
const info "guest principal"
const pcpl (afnix:csm:Principal name info)

# check predicate and representation
assert true (afnix:csm:principal-p pcpl)
assert "Principal" (pcpl:repr)

# check the principal name and info
assert name (pcpl:get-name)
assert info (pcpl:get-info)

# sign this principal
pcpl:sign
assert true (pcpl:verify)
