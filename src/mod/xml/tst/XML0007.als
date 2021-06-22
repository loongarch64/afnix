# ---------------------------------------------------------------------------
# - XML0007.als                                                             -
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

# @info   document type node test unit
# @author amaury darsch

# get the module
interp:library "afnix-xml"

# the public and system id
const xval "axmxa"
const publ "-//AFNIX//DTD AXMXA//EN"
const sysl "http://www.afnix.org/dtd/axmxa.dtd"

# create a document type node
trans  node (afnix:xml:XmlDoctype xval publ sysl)
assert true (afnix:xml:node-p node)
assert true (afnix:xml:doctype-p node)

# create a reader and check doctype node
const xmlr  (afnix:xml:XmlReader)

# parse a simple document type
trans  node (xmlr:get-node "<!DOCTYPE axmxa>") 
assert true (afnix:xml:doctype-p node)
assert xval (node:get-xval)

# parse a system document type
trans  node (xmlr:get-node "<!DOCTYPE axmxa SYSTEM 
  \"http://www.afnix.org/dtd/axmxa.dtd\">")
assert true (afnix:xml:doctype-p node)
assert xval (node:get-xval)
assert sysl (node:get-system-id)

# parse a public document type
trans node  (xmlr:get-node "<!DOCTYPE axmxa PUBLIC 
  \"-//AFNIX//DTD AXMXA//EN\" 
  \"http://www.afnix.org/dtd/axmxa.dtd\">")
assert true (afnix:xml:doctype-p node)
assert xval (node:get-xval)
assert publ (node:get-public-id)
assert sysl (node:get-system-id)

# parse a public document type
trans node  (xmlr:get-node "<!DOCTYPE axmxa PUBLIC 
  \"-//AFNIX//DTD AXMXA//EN\"
  \"http://www.afnix.org/dtd/axmxa.dtd\"
  [<!ELEMENT greeting (#PCDATA)>]>")
assert true (afnix:xml:doctype-p node)
assert xval (node:get-xval)
assert publ (node:get-public-id)
assert sysl (node:get-system-id)


