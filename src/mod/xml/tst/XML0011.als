# ---------------------------------------------------------------------------
# - XML0011.als                                                             -
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

# @info   general entity node test unit
# @author amaury darsch

# get the module
interp:library "afnix-xml"

# the public and system id
trans name "status"
const xval "300 OK"

# create a general entity
trans  node (afnix:xml:XmlGe name xval)
assert true (afnix:xml:node-p   node)
assert true (afnix:xml:entity-p node)
assert true (afnix:xml:ge-p     node)

# check the node name and value
assert name (node:get-name)
assert xval (node:get-xval)

# create a reader and check entity node
const xmlr  (afnix:xml:XmlReader)

# parse a simple general entity
trans  node (xmlr:get-node "<!ENTITY status \"300 OK\">") 
assert true (afnix:xml:entity-p node)
assert true (afnix:xml:ge-p     node)
assert name (node:get-name)
assert xval (node:get-xval)

# check with a system id
trans name  "axmxa"
const sysl  "http://www.afnix.org/axmxa.xml"
const data  "xml"
trans node  (xmlr:get-node "<!ENTITY axmxa SYSTEM 
                              \"http://www.afnix.org/axmxa.xml\">")
assert true (afnix:xml:entity-p node)
assert true (afnix:xml:ge-p     node)
assert name (node:get-name)
assert sysl (node:get-system-id)

trans node  (xmlr:get-node "<!ENTITY axmxa SYSTEM 
                              \"http://www.afnix.org/axmxa.xml\" NDATA xml>")
assert true (afnix:xml:entity-p node)
assert true (afnix:xml:ge-p     node)
assert name (node:get-name)
assert sysl (node:get-system-id)
assert data (node:get-data)

# check with a public id
trans name  "axmxa"
const publ  "-//AFNIX//DTD AXMXA//EN"

trans node  (xmlr:get-node "<!ENTITY axmxa PUBLIC
                              \"-//AFNIX//DTD AXMXA//EN\" 
                              \"http://www.afnix.org/axmxa.xml\">")
assert name (node:get-name)
assert publ (node:get-public-id)
assert sysl (node:get-system-id)

trans node  (xmlr:get-node "<!ENTITY axmxa PUBLIC
                              \"-//AFNIX//DTD AXMXA//EN\" 
                              \"http://www.afnix.org/axmxa.xml\" NDATA xml>")
assert name (node:get-name)
assert publ (node:get-public-id)
assert sysl (node:get-system-id)
assert data (node:get-data)
