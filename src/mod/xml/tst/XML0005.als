# ---------------------------------------------------------------------------
# - XML0005.als                                                             -
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

# @info   pi node test unit
# @author amaury darsch

# get the module
interp:library "afnix-xml"

# create a pi node
trans  node (afnix:xml:XmlPi "afnix")
assert true (afnix:xml:node-p node)
assert true (afnix:xml:pi-p   node)

# check the node name
assert "afnix" (node:get-name)

# set the pi value and check
const text "this is an xml pi node"
node:set-xval text
assert text (node:get-xval)

# create a reader
const xmlr (afnix:xml:XmlReader)

# parse a processing instruction
trans  node    (xmlr:get-node "<?afnix?>")
assert true    (afnix:xml:pi-p node)
assert "afnix" (node:get-name)

# reset and parse with text
xmlr:reset
trans  node    (xmlr:get-node "<?afnix mode=\"en\"?>")
assert true    (afnix:xml:pi-p node)
assert "afnix" (node:get-name)
assert "mode=\"en\"" (node:get-xval)

# expand attributes
trans plst  (node:map-xval)
assert   1  (plst:length)
assert true (plst:exists-p "mode")
assert "en" (plst:get-value "mode")
