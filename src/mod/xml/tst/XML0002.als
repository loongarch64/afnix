# ---------------------------------------------------------------------------
# - XML0002.als                                                             -
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

# @info   reader test unit
# @author amaury darsch

# get the module
interp:library "afnix-xml"

# create a reader
const xmlr (afnix:xml:XmlReader)

# parse a tag node
trans  node (xmlr:get-node "<html xmlns=\"http://www.w3.org/1999/xhtml\">")
assert true (afnix:xml:tag-p node)

# check the node name
assert "html" (node:get-name)

# check attribute as a property
assert 1 (node:attribute-length)
trans attr (node:get-attribute 0)
assert "xmlns" (attr:get-name)
assert "http://www.w3.org/1999/xhtml" (attr:get-value)
assert "http://www.w3.org/1999/xhtml" (node:get-attribute-value "xmlns")


