# ---------------------------------------------------------------------------
# - AXI0050.als                                                              -
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

# @info   enumeration test module
# @author amaury darsch

# create a new enumeration
const  e (enum E1 E2 E3)
assert true   (enum-p e)
assert "Enum" (e:repr)

# check for item
assert true   (item-p e:E1)
assert "Item" (e:E1:repr)

# check for item equality
const i1 e:E1
const i2 e:E2
assert true  (i1:== i1)
assert false (== i1 i2)

# get back the enumeration
assert true (enum-p (i1:get-enum))

# add a new item by string
e:add "E4"
assert true (item-p e:E4)
