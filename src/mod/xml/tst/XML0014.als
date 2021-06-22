# ---------------------------------------------------------------------------
# - XML0013.als                                                             -
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

# @info   element node test unit
# @author amaury darsch

# get the module
interp:library "afnix-xml"

# the element name and value
const name "form"
const attr "method"

# create an element node
trans  node (afnix:xml:XmlAttlist name attr)
assert true (afnix:xml:node-p    node)
assert true (afnix:xml:attlist-p node)

# check the node name and value
assert name (node:get-element-name)
assert attr (node:get-attribute-name)

# create a reader and check entity node
const xmlr  (afnix:xml:XmlReader)

# parse a simple general entity
trans  node (xmlr:get-node "<!ATTLIST form method CDATA #IMPLIED>") 
assert true (afnix:xml:attlist-p node)
assert name (node:get-element-name)
assert attr (node:get-attribute-name)

# parse a simple general entity
trans  node (xmlr:get-node "<!ATTLIST form method ID #REQUIRED>") 
assert true (afnix:xml:attlist-p node)
assert name (node:get-element-name)
assert attr (node:get-attribute-name)

# parse a simple general entity
trans  node (xmlr:get-node "<!ATTLIST form method (GET|POST) #REQUIRED>") 
assert true (afnix:xml:attlist-p node)
assert name (node:get-element-name)
assert attr (node:get-attribute-name)

# parse a simple general entity
trans  node (xmlr:get-node "<!ATTLIST form method (GET|POST) \"GET\">") 
assert true (afnix:xml:attlist-p node)
assert name (node:get-element-name)
assert attr (node:get-attribute-name)

# parse a simple general entity
trans  node (xmlr:get-node "<!ATTLIST form method NOTATION (G|P) \"P\">") 
assert true (afnix:xml:attlist-p node)
assert name (node:get-element-name)
assert attr (node:get-attribute-name)

# parse a simple general entity
trans  node (xmlr:get-node "<!ATTLIST form method CDATA #FIXED \"POST\">") 
assert true (afnix:xml:attlist-p node)
assert name (node:get-element-name)
assert attr (node:get-attribute-name)

