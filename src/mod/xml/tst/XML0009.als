# ---------------------------------------------------------------------------
# - XML0009.als                                                             -
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

# @info   entity reference node test unit
# @author amaury darsch

# get the module
interp:library "afnix-xml"

# the reference test value
const xref "nbsp"

# create a eref node
trans  node (afnix:xml:XmlEref xref)
assert true (afnix:xml:node-p      node)
assert true (afnix:xml:reference-p node)
assert true (afnix:xml:eref-p      node)

# check the value and string
assert xref (node:get-xref)

# create a reader
const xmlr  (afnix:xml:XmlReader)

# parse a simple reference
trans  node (xmlr:get-node "&nbsp;") 
#assert true (afnix:xml:eref-p node)
#assert xref (node:get-xref)

# parse a more complex text
#trans  node  (afnix:xml:XmlTag "p")
#node:parse   "&lt;i&gt;"
#assert "<i>" (node:to-text)
#node:release

# parse a regular text
#trans  node  (afnix:xml:XmlTag "p")
#node:parse   "afnix&amp;company"
#assert "afnix&company" (node:to-text)
#node:release
