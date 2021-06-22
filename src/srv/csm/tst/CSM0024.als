# ---------------------------------------------------------------------------
# - CSM0024.als                                                             -
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

# @info   visa test unit
# @author amaury darsch

# get the service
interp:library "afnix-csm"

# create an afnix authority
const anam "afnix"
const anfo "afnix authority"
trans auth (afnix:csm:Authority anam anfo)

# create a wanix identity
const wnam "wanix"
const wnfo "wanix identity"
trans wdty (afnix:csm:Identity wnam wnfo)

# create a guest identity
const inam "guest"
const info "guest identity"
trans idty (afnix:csm:Identity inam info)

# create a visa
const visa (afnix:csm:Visa auth wdty idty)

# check predicate and representation
assert true  (afnix:csm:act-p visa)
assert true  (afnix:csm:visa-p visa)
assert "Visa" (visa:repr)
