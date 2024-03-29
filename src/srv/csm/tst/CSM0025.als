# ---------------------------------------------------------------------------
# - CSM0025.als                                                             -
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

# @info   act test unit
# @author amaury darsch

# get the service
interp:library "afnix-csm"

# create an afnix notary
const name "afnix"
const info "afnix notary"
trans ntry (afnix:csm:Notary name info)

# check predicate and representation
assert true  (afnix:csm:nameable-p ntry)
assert true  (afnix:csm:notary-p ntry)
assert "Notary" (ntry:repr)
