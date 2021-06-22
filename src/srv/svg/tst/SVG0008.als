# ---------------------------------------------------------------------------
# - SVG0008.als                                                             -
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

# @info   svg polygon test unit
# @author amaury darsch

# get the module
interp:library "afnix-xml"
interp:library "afnix-mth"
interp:library "afnix-svg"

# create a point vector
const pv (Vector (afnix:mth:Ippd 375 75) (afnix:mth:Ippd 379 161))

# create a svg polygon
const  poly (afnix:svg:SvgPolygon pv)
assert true (afnix:xml:node-p poly)
assert true (afnix:svg:svg-styling-p poly)
assert true (afnix:svg:svg-polygon-p poly)

# set some styling
poly:set-fill   "lightcoral"
poly:set-stroke "sienna"
poly:set-stroke-width 10
