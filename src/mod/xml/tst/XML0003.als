# ---------------------------------------------------------------------------
# - XML0003.als                                                             -
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

# @info   comment node test unit
# @author amaury darsch

# get the module
interp:library "afnix-xml"

# create a comment node
trans  node (afnix:xml:XmlComment)
assert true (afnix:xml:node-p node)
assert true (afnix:xml:comment-p node)

# set the x-value and check
const text "this is the afnix xml module"
node:set-xval text
assert text (node:get-xval)

# create a reader
const xmlr (afnix:xml:XmlReader)

# parse a comment
trans  node      (xmlr:get-node "<!-- afnix -->")
assert true      (afnix:xml:comment-p node)
assert " afnix " (node:get-xval)

# check for error
try {
  # parse an unterminated comment
  xmlr:parse "<!-- afnix --"
  # we shoud never be here
  assert true false
} (assert "xml-error" what:eid)
try {
  # parse an nivalid sequence comment
  xmlr:parse "<!-- afnix -- >"
  # we shoud never be here
  assert true false
} (assert "xml-error" what:eid)
