# ---------------------------------------------------------------------------
# - WAX0015.als                                                             -
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

# @info   xhtml link test unit
# @author amaury darsch

# get the module
interp:library "afnix-xml"
interp:library "afnix-wax"

# create a meta node
const  node (afnix:wax:XhtmlLink "style.css" "text/css" "stylesheet")
assert true (afnix:xml:node-p node)
assert true (afnix:wax:xhtml-link-p node)

# check attribute as a property
assert 3 (node:attribute-length)

# check attribute values
assert "style.css"  (node:get-attribute-value "href")
assert "text/css"   (node:get-attribute-value "type")
assert "stylesheet" (node:get-attribute-value "rel")
