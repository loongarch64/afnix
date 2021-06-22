# ---------------------------------------------------------------------------
# - SVG1007.als                                                             -
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

# @info   svg polygon test unit - svg 1.1 example
# @author amaury darsch

# get the module
interp:library "afnix-sio"
interp:library "afnix-xml"
interp:library "afnix-mth"
interp:library "afnix-svg"

# create a root node
const root (afnix:svg:SvgRoot 1200 400)
const frag (root:get-svg)
frag:set-width  "12cm"
frag:set-height "4cm"

# create a svg rectangle
trans rect (afnix:svg:SvgRect 1198 398 1 1)
rect:set-fill   "none"
rect:set-stroke "blue"
rect:set-stroke-width 2
frag:add-child rect

# create a vector points
trans pv (Vector 
  (afnix:mth:Ippd 350  75) (afnix:mth:Ippd 379 161)
  (afnix:mth:Ippd 469 161) (afnix:mth:Ippd 397 215)
  (afnix:mth:Ippd 423 301) (afnix:mth:Ippd 350 250)
  (afnix:mth:Ippd 277 301) (afnix:mth:Ippd 303 215)
  (afnix:mth:Ippd 231 161) (afnix:mth:Ippd 321 161))
  
# create a svg polygon
trans poly (afnix:svg:SvgPolygon pv )
poly:set-fill   "lightcoral"
poly:set-stroke "sienna"
poly:set-stroke-width 10
frag:add-child poly

# create a vector points
trans pv (Vector 
  (afnix:mth:Ippd 850  75) (afnix:mth:Ippd 958 137)
  (afnix:mth:Ippd 958 262) (afnix:mth:Ippd 850 325)
  (afnix:mth:Ippd 742 262) (afnix:mth:Ippd 742 137))

# create a svg polygon
trans poly (afnix:svg:SvgPolygon pv )
poly:set-fill   "lightcoral"
poly:set-stroke "sienna"
poly:set-stroke-width 10
frag:add-child poly

# create an output file and write
const os (afnix:sio:OutputFile "SVG1007.svg")
# add it to the root node
root:write os
os:close

# release the node
root:release
