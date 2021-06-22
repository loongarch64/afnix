# ---------------------------------------------------------------------------
# - XNE0002.als                                                             -
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

# @info   xne condition test unit
# @author amaury darsch

# get the module
interp:library "afnix-xml"

# create a xne condition
const  xcnd (afnix:xml:XneCond)
assert true (afnix:xml:xne-cond-p xcnd)

# create a tag node
trans  node (afnix:xml:XmlTag "afnix")
assert true (xcnd:valid-p node)

# add a tag condition
xcnd:add afnix:xml:Xne:TAG
assert true (xcnd:valid-p node)

# add a condition name
xcnd:add afnix:xml:Xne:NAME "afnix"
assert true (xcnd:valid-p node)

# add a condition by id
xcnd:add afnix:xml:Xne:ID "hello"
assert false (xcnd:valid-p node)

# add the id attribute and check
node:set-attribute "id" "hello"
assert true (xcnd:valid-p node)
