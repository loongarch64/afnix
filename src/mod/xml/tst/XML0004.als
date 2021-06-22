# ---------------------------------------------------------------------------
# - XML0004.als                                                             -
# - afnix:xml module test unit                                              -
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

# @info   text node test unit
# @author amaury darsch

# get the module
interp:library "afnix-xml"

# create a text node
const  node (afnix:xml:XmlText)
assert true (afnix:xml:node-p node)
assert true (afnix:xml:text-p node)

# set the text value and check
const text "this is an xml text"
node:set-xval text
assert text (node:get-xval)
