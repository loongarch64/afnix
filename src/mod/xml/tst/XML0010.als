# ---------------------------------------------------------------------------
# - XML0010.als                                                             -
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

# @info   cdata node test unit
# @author amaury darsch

# get the module
interp:library "afnix-xml"

const text "cdata <hello/> some tags &lt;world&gt;"
const tref "cdata <hello/> some tags <world>"

# create a text node
trans  node (afnix:xml:XmlData text)
assert true (afnix:xml:node-p  node)
assert true (afnix:xml:data-p  node)

# check the text
assert text (node:get-xval)

# set the text value and check
const data  (+ (+ "<![CDATA[" text) "]]>")

# create a reader
const xmlr  (afnix:xml:XmlReader)

# parse the data text and check
trans  node (xmlr:get-node data)
assert true (afnix:xml:data-p node)
assert tref (node:get-xval)
