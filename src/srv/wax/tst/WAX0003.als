# ---------------------------------------------------------------------------
# - WAX0003.als                                                             -
# - afnix:wax module test unit                                              -
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

# @info   xhtml html test unit
# @author amaury darsch

# get the module
interp:library "afnix-xml"
interp:library "afnix-wax"

# the title node
const title "afnix"

# create a document
const  html (afnix:wax:XhtmlHtml title)
assert true (afnix:xml:node-p html)
assert true (afnix:wax:xhtml-html-p html)

# get the head node
const head (html:get-child "head")
# get the title node
const node (head:get-child "title")

# get the xval string and check against title
const text (node:get-child 0)
assert title (text:get-xval)

# release the node
html:release
