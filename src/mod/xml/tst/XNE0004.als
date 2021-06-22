# ---------------------------------------------------------------------------
# - XNE0004.als                                                             -
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

# @info   xne tree replace test unit
# @author amaury darsch

# get the module
interp:library "afnix-xml"

# create a xne condition
const  xcnd (afnix:xml:XneCond)
assert true (afnix:xml:xne-cond-p xcnd)

# create a root node
const root (afnix:xml:XmlRoot)

# create a pi node
trans rnod (afnix:xml:XmlPi "afnix")
root:add-child rnod

# add a condition by type
xcnd:add afnix:xml:Xne:PI
assert true (xcnd:valid-p rnod)

# reset the condition by name
xcnd:reset
xcnd:add afnix:xml:Xne:PI "afnix"
assert true (xcnd:valid-p rnod)

# create a tree with the node
trans tree (afnix:xml:XneTree root)

# select the node by condition
trans  xset (tree:select xcnd)
assert 1    (xset:length)

# create a replace node
trans  snod (afnix:xml:XmlTag "afnix")
assert true (root:replace rnod snod)

# select the node by condition
trans  tree (afnix:xml:XneTree root)
trans  xset (tree:select xcnd)
assert 0    (xset:length)

# reset the condition
xcnd:reset
xcnd:add afnix:xml:Xne:TAG

# select the node by condition
trans  xset  (tree:select xcnd)
assert 1     (xset:length)

# release the root node
root:release
