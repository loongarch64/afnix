# ---------------------------------------------------------------------------
# - CSM0023.als                                                             -
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

# create an afnix authority
const anam "afnix"
const anfo "afnix authority"
trans auth (afnix:csm:Authority anam anfo)

# create a wanix identity
const wnam "wanix"
const wnfo "wanix identity"
trans wdty (afnix:csm:Identity wnam wnfo)

# create an act
const act (afnix:csm:Act auth wdty)

# check predicate and representation
assert true  (afnix:csm:act-p act)
assert "Act" (act:repr)

# check validity time
act:set-before-validity-time 1
assert 1 (act:get-before-validity-time)
act:set-after-validity-time 2
assert 2 (act:get-after-validity-time)
