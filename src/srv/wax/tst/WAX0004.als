# ---------------------------------------------------------------------------
# - WAX0004.als                                                             -
# - afnix:wax service test unit                                             -
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

# @info   xhtml root test unit
# @author amaury darsch

# get the module
interp:library "afnix-xml"
interp:library "afnix-wax"

# the title node
const title "afnix"

# create a document
const  root (afnix:wax:XhtmlRoot title)
assert true (afnix:xml:node-p root)
assert true (afnix:wax:xhtml-root-p root)

# get the html node
const  html (root:get-child "html")
assert true (afnix:wax:xhtml-html-p html)
# get the head node
const  head (html:get-child "head")
assert true (afnix:wax:xhtml-head-p head)
# get the body node
const  body (html:get-child "body")
assert true (afnix:wax:xhtml-body-p body)

# get the title node
const node   (head:get-child "title")
# get the xval string and check against title
const text   (node:get-child 0)
assert title (text:get-xval)

# release the node
root:release
