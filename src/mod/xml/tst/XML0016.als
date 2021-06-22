# ---------------------------------------------------------------------------
# - XML0016.als                                                             -
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

# @info   root node test unit
# @author amaury darsch

# get the module
interp:library "afnix-xml"

# create a root node
trans  node (afnix:xml:XmlRoot)
assert true (afnix:xml:node-p node)
assert true (afnix:xml:root-p node)

# create a texter and parse a text
const  txtr (afnix:xml:XmlTexter)
txtr:parse "<b>hello</b> world"

# get the root node and check
const  root (txtr:get-root)
assert 2    (root:child-length)

# get the declaration node
trans  node (root:get-child 0)
assert true (afnix:xml:node-p node)
assert true (afnix:xml:decl-p node)

# get the text node
trans  node (root:get-child 1)
assert true (afnix:xml:node-p node)
assert true (afnix:xml:text-p node)

# check escape text
const  tesc "<b>hello</b> world"
assert tesc (node:get-xval)

