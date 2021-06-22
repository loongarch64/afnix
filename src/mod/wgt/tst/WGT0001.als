# ---------------------------------------------------------------------------
# - WGT0001.als                                                             -
# - afnix:wgt module test unit                                              -
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

# @info   conditional test unit
# @author amaury darsch

# get the module
interp:library "afnix-wgt"

# the conditional name and info
const name "test"
const info "conditional test"

# check creation
const cond (afnix:wgt:Conditional name info)
assert true (afnix:wgt:conditional-p cond)
assert "Conditional" (cond:repr)

# check name and info
assert name (cond:get-name)
assert info (cond:get-info)

# check active
assert false (cond:get-active)
cond:set-active true
assert true (cond:get-active)

# check conditional expression
cond:set "not" true
assert "not" (cond:get-operator)
assert true  (cond:get-lhs)

cond:set "<" 1 2.0
assert "<" (cond:get-operator)
assert 1   (cond:get-lhs)
assert 2.0 (cond:get-rhs)
