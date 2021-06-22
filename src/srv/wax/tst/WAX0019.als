# ---------------------------------------------------------------------------
# - WAX0019.als                                                             -
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

# @info   xhtml heading test unit
# @author amaury darsch

# get the module
interp:library "afnix-xml"
interp:library "afnix-wax"

# the h(x) class
const cls "test"

# create a h1 element
const  h1   (afnix:wax:XhtmlH1 cls)
assert true (afnix:xml:node-p h1)
assert true (afnix:wax:xhtml-h1-p h1)
# check the heading attribute
assert cls  (h1:get-attribute-value "class")

# create a h2 element
const  h2   (afnix:wax:XhtmlH2 cls)
assert true (afnix:xml:node-p h2)
assert true (afnix:wax:xhtml-h2-p h2)
# check the heading attribute
assert cls  (h2:get-attribute-value "class")

