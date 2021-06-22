# ---------------------------------------------------------------------------
# - XML0006.als                                                             -
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

# @info   declaration node test unit
# @author amaury darsch

# get the module
interp:library "afnix-xml"

# the version id
const vers "1.0"

# create a declaration node
trans  node (afnix:xml:XmlDecl vers)
assert true (afnix:xml:node-p node)
assert true (afnix:xml:decl-p node)

# check the node name
assert "xml" (node:get-name)

# create a reader and check pi node
const xmlr (afnix:xml:XmlReader)

# parse a declaration
trans  node    (xmlr:get-node "<?xml version=\"1.0\" encoding=\"UTF-8\"?>")
assert true    (afnix:xml:decl-p node)
assert "xml"   (node:get-name)
assert "1.0"   (node:get-version)
assert "UTF-8" (node:get-encoding)
