# ---------------------------------------------------------------------------
# - AXI0059.als                                                              -
# - afnix engine test module                                                -
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

# @info   property class test module
# @author amaury darsch

# create a property
const  prop (Property "hello" "world")
assert true (property-p prop)

# check name and value
assert "hello" (prop:get-name)
assert "world" (prop:get-value)

# set name and value
const name "afnix"
const pval "programming language"
prop:set name pval

assert name (prop:get-name)
assert pval (prop:get-value)

# create a boolean property by info
const  bprp (Property "boolean" "boolean value" true)
assert true (bprp:get-boolean-value)

# create an integer property by info
const  iprp (Property "integer" "integer value" 0)
assert 0    (iprp:get-integer-value)

# create a real property by info
const  rprp (Property "real" "real value" 0.0)
assert 0.0  (rprp:get-real-value)
