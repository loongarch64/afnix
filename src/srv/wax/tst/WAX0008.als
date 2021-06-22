# ---------------------------------------------------------------------------
# - WAX0008.als                                                             -
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

# @info   xhtm ref test unit
# @author amaury darsch

# get the module
interp:library "afnix-xml"
interp:library "afnix-wax"

# the ref uri
const uri interp:afnix-uri

# create a ref element
const  ref  (afnix:wax:XhtmlRef uri)
assert true (afnix:xml:node-p ref)
assert true (afnix:wax:xhtml-ref-p ref)

# check the href attribute
assert uri  (ref:get-attribute-value "href")
