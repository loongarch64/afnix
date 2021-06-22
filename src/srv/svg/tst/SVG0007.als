# ---------------------------------------------------------------------------
# - SVG0007.als                                                             -
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

# @info   svg polyline test unit
# @author amaury darsch

# get the module
interp:library "afnix-xml"
interp:library "afnix-mth"
interp:library "afnix-svg"

# create a point vector
const pv (Vector (afnix:mth:Ippd 50 375) (afnix:mth:Ippd 150 375))

# create a svg polyline
const  poly (afnix:svg:SvgPolyline pv)
assert true (afnix:xml:node-p poly)
assert true (afnix:svg:svg-styling-p poly)
assert true (afnix:svg:svg-polyline-p poly)

# set some styling
poly:set-stroke "sienna"
poly:set-stroke-width 10
