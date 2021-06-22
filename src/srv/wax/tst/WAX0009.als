# ---------------------------------------------------------------------------
# - WAX0009.als                                                             -
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

# @info   xhtm para/emph test unit
# @author amaury darsch

# get the module
interp:library "afnix-xml"
interp:library "afnix-wax"

# create a para element
const  para (afnix:wax:XhtmlPara)
assert true (afnix:xml:node-p para)
assert true (afnix:wax:xhtml-para-p para)

# create an emph element
const  emph (afnix:wax:XhtmlEmph)
assert true (afnix:xml:node-p emph)
assert true (afnix:wax:xhtml-emph-p emph)

# create a text italic element
const  txti (afnix:wax:XhtmlTxti)
assert true (afnix:xml:node-p txti)
assert true (afnix:wax:xhtml-i-p txti)

# create a text bold element
const  txtb (afnix:wax:XhtmlTxtb)
assert true (afnix:xml:node-p txtb)
assert true (afnix:wax:xhtml-b-p txtb)

# create a text teletype element
const  txtt (afnix:wax:XhtmlTxtt)
assert true (afnix:xml:node-p txtt)
assert true (afnix:wax:xhtml-tt-p txtt)
