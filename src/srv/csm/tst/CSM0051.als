# ---------------------------------------------------------------------------
# - CSM0051.als                                                             -
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

# @info   carrier test unit
# @author amaury darsch

# get the service
interp:library "afnix-csm"

# create a nil carrier
trans aco (afnix:csm:Carrier)

# check predicate and representation
assert true  (afnix:csm:carrier-p aco)
assert "Carrier" (aco:repr)

# test a carrier by object and name
const name "ONE"
const info "ONE DGIT"
const aobj 1
trans  aco (afnix:csm:Carrier aobj name)
assert name (aco:get-name)
assert ""   (aco:get-info)
assert aobj (aco:get-object)
assert ""   (aco:get-rid)

# test a carrier by object, name and info
trans  aco (afnix:csm:Carrier aobj name info)
assert name (aco:get-name)
assert info (aco:get-info)
assert aobj (aco:get-object)
assert ""   (aco:get-rid)

# test the rid
aco:set-rid "RID"
assert true  (aco:rid-p "RID")
assert "RID" (aco:get-rid)

