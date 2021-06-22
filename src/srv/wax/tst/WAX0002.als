# ---------------------------------------------------------------------------
# - WAX0002.als                                                             -
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

# @info   xhtml body test unit
# @author amaury darsch

# get the module
interp:library "afnix-xml"
interp:library "afnix-wax"

# the title node
const title "afnix"

# create a document
const  body (afnix:wax:XhtmlBody)
assert true (afnix:xml:node-p body)
assert true (afnix:wax:xhtml-body-p body)
