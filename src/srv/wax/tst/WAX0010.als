# ---------------------------------------------------------------------------
# - WAX0010.als                                                             -
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

# @info   xhtm hr/br test unit
# @author amaury darsch

# get the module
interp:library "afnix-xml"
interp:library "afnix-wax"

# create a hr element
const  hr   (afnix:wax:XhtmlHr)
assert true (afnix:xml:node-p hr)
assert true (afnix:wax:xhtml-hr-p hr)

# create a br element
const  br   (afnix:wax:XhtmlBr)
assert true (afnix:xml:node-p br)
assert true (afnix:wax:xhtml-br-p br)
