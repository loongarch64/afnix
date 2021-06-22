# ---------------------------------------------------------------------------
# - XNE0003.als                                                             -
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

# @info   xne tree select test unit
# @author amaury darsch

# get the module
interp:library "afnix-xml"

# create a xne condition
const  xcnd (afnix:xml:XneCond)
assert true (afnix:xml:xne-cond-p xcnd)

# create a tag node
trans  node (afnix:xml:XmlTag "afnix")
assert true (xcnd:valid-p  node)

# create a tree with the node
const  tree (afnix:xml:XneTree node)

# add a condition name
xcnd:add afnix:xml:Xne:NAME "afnix"
assert true (xcnd:valid-p node)

# select the node by condition
trans  xset  (tree:select xcnd)
assert 1     (xset:length)
