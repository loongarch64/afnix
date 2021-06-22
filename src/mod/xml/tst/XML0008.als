# ---------------------------------------------------------------------------
# - XML0008.als                                                             -
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

# @info   character reference node test unit
# @author amaury darsch

# get the module
interp:library "afnix-xml"

# the character test value
const xref "x1234"
const cval (Character "U+1234")
const sval (String cval)

# create a cref node
trans  node (afnix:xml:XmlCref cval)
assert true (afnix:xml:node-p      node)
assert true (afnix:xml:reference-p node)
assert true (afnix:xml:cref-p      node)

# check the value and string
assert cval (node:get-value)
assert xref (node:get-xref)

# create a reader
const xmlr  (afnix:xml:XmlReader)

# parse a simple character reference
trans  node (xmlr:get-node "&#x1234;") 
assert true (afnix:xml:cref-p node)
assert sval (node:to-text)


