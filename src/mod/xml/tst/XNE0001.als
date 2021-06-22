# ---------------------------------------------------------------------------
# - XNE0001.als                                                             -
# - afnix:xml module test unit                                              -
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

# @info   xne tree test unit
# @author amaury darsch

# get the module
interp:library "afnix-xml"

# create a xne tree
const  tree (afnix:xml:XneTree)
assert true (afnix:xml:xne-tree-p tree)

# check the depth
assert 0    (tree:depth)

# create a root node
const root (afnix:xml:XmlRoot)

# add two tag nodes
root:add-child (afnix:xml:XmlTag "hello")
root:add-child (afnix:xml:XmlTag "world")

# attach the node
tree:set-node root
# check the depth
assert 1 (tree:depth)
# set the tree qith uid
tree:generate-id
# normalize the tree
tree:normalize

# release the root node
root:release
