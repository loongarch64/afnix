# ---------------------------------------------------------------------------
# - AXI0063.als                                                              -
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

# @info   class deference test module
# @author amaury darsch

# the base class
const bc (class)
# the defer class
const dc (class)

# bind some value and check
const dc:x 1
const bc:y 2
assert 1 dc:x
assert 2 bc:y

# bind the defer class
const bc:defer dc
assert 1 bc:x
assert 2 bc:y

# here is the tricky part
# bind a method in the defer class that reference a symbol in the base class
trans dc:get-y nil (eval this:y)

# create an instance and check the values  
# note that the method 'get-y' is called from the instance but is bound
# in the defer class (aka virtual data member access)
const i (bc)
assert 1 i:x
assert 2 i:y
assert 2 (i:get-y)
