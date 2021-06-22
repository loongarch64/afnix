# ---------------------------------------------------------------------------
# - CSM0052.als                                                             -
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

# @info   delegate test unit
# @author amaury darsch

# get the service
interp:library "afnix-csm"

# create a nil delegate
trans dlg (afnix:csm:Delegate)

# check predicate and representation
assert true  (afnix:csm:delegate-p dlg)
assert "Delegate" (dlg:repr)

# test a delegate by object and name
const rid  "0"
const name "ONE"
const info "ONE DGIT"
const aobj 1
const addr "ADDR"

trans  dlg (afnix:csm:Delegate aobj name)
assert name (dlg:get-name)
assert ""   (dlg:get-info)
assert aobj (dlg:get-object)
assert ""   (dlg:get-rid)
assert ""   (dlg:get-address)

# test a delegate by object, name and info
trans  dlg (afnix:csm:Delegate aobj name info)
assert name (dlg:get-name)
assert info (dlg:get-info)
assert aobj (dlg:get-object)
assert ""   (dlg:get-rid)
assert ""   (dlg:get-address)

# test a delegate by object, name and info
trans  dlg (afnix:csm:Delegate aobj name info)
assert name (dlg:get-name)
assert info (dlg:get-info)
assert aobj (dlg:get-object)
assert ""   (dlg:get-rid)
assert ""   (dlg:get-address)

# test the address
dlg:set-address addr
assert addr (dlg:get-address)

# test a delegate by object, rid, name, info and address
trans  dlg (afnix:csm:Delegate aobj rid name info addr)
assert name (dlg:get-name)
assert info (dlg:get-info)
assert aobj (dlg:get-object)
assert rid  (dlg:get-rid)
assert addr (dlg:get-address)


