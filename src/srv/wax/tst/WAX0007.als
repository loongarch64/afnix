# ---------------------------------------------------------------------------
# - WAX0007.als                                                             -
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

# @info   xhtm div/pre test unit
# @author amaury darsch

# get the module
interp:library "afnix-xml"
interp:library "afnix-wax"

# the div/pre class
const cls "test"

# create a div element
const  div  (afnix:wax:XhtmlDiv cls)
assert true (afnix:xml:node-p div)
assert true (afnix:wax:xhtml-div-p div)
# check the div attribute
assert cls  (div:get-attribute-value "class")

# create a pre element
const  pre  (afnix:wax:XhtmlPre cls)
assert true (afnix:xml:node-p div)
assert true (afnix:wax:xhtml-pre-p pre)
# check the div attribute
assert cls  (pre:get-attribute-value "class")
