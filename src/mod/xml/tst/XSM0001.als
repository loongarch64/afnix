# ---------------------------------------------------------------------------
# - XSM0001.als                                                             -
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

# @info   xsm node test unit
# @author amaury darsch

# get the module
interp:library "afnix-xml"

# create a text node
trans  node (afnix:xml:XsmNode "afnix")
assert true (afnix:xml:xsm-node-p node)

# check the value
assert "afnix" (node:to-string)
assert "afnix" (node:to-literal)
assert  false  (node:reserved-p)

# create a tag text node
trans  node (afnix:xml:XsmNode afnix:xml:XsmNode:TAG "meta hello=world")
assert true (afnix:xml:xsm-node-p node) 

# check the value
assert "meta hello=world"   (node:to-string)
assert "<meta hello=world>" (node:to-literal)
assert  false (node:reserved-p)
assert "meta" (node:get-name)

# check the attribute list
const  alst (node:get-attribute-list)
assert 1    (alst:length)

