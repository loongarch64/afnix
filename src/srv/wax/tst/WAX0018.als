# ---------------------------------------------------------------------------
# - WAX0018.als                                                             -
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

# @info   xhtml list test unit
# @author amaury darsch

# get the module
interp:library "afnix-xml"
interp:library "afnix-wax"

# create list item element
const  item (afnix:wax:XhtmlLi)
assert true (afnix:xml:node-p item)
assert true (afnix:wax:xhtml-li-p item)

# create a list element
const  list (afnix:wax:XhtmlUl)
assert true (afnix:xml:node-p list)
assert true (afnix:wax:xhtml-ul-p list)
