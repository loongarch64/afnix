# ---------------------------------------------------------------------------
# - AXI0075.als                                                             -
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

# @info   binary tree object test
# @author amaury darsch

# create an empty tree
const tree (Tree)
assert true (tree-p tree)
assert "Tree" (tree:repr)
assert 0 (tree:length)

# add objects and check
tree:add 0 "zero"
assert 1 (tree:length)
assert true (tree:exists-p 0)
assert "zero" (tree:find 0)

tree:add -1 -1
assert 2 (tree:length)
assert true (tree:exists-p -1)
assert -1 (tree:find -1)

tree:add 1 "one"
assert 3 (tree:length)
assert true (tree:exists-p 1)
assert "one" (tree:find 1)

tree:add 3 3
assert 4 (tree:length)
assert true (tree:exists-p 3)
assert 3 (tree:find 3)

