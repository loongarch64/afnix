# ---------------------------------------------------------------------------
# - WAX0017.als                                                             -
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

# @info   xhtml script test unit
# @author amaury darsch

# get the module
interp:library "afnix-xml"
interp:library "afnix-wax"

# create a script node
const  node (afnix:wax:XhtmlScript "text/javascript" "hello.js")
assert true (afnix:xml:node-p node)
assert true (afnix:wax:xhtml-script-p node)

# check attribute as a property
assert 2 (node:attribute-length)

# check attribute values
assert "text/javascript" (node:get-attribute-value "type")
assert "hello.js"        (node:get-attribute-value "src")
