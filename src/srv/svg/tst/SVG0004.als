# ---------------------------------------------------------------------------
# - SVG0004.als                                                             -
# - afnix:svg service test unit                                             -
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

# @info   svg circle test unit
# @author amaury darsch

# get the module
interp:library "afnix-xml"
interp:library "afnix-svg"

# create a svg circle
const  circ (afnix:svg:SvgCircle 100 600 200)
assert true (afnix:xml:node-p circ)
assert true (afnix:svg:svg-styling-p circ)
assert true (afnix:svg:svg-circle-p  circ)

# set some styling
circ:set-fill   "lightcoral"
circ:set-stroke "sienna"
circ:set-stroke-width 10
