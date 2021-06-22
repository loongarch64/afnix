# ---------------------------------------------------------------------------
# - SVG1006.als                                                             -
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

# @info   svg polyline test unit - svg 1.1 example
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
const pv (Vector 
  (afnix:mth:Ippd   50 375) (afnix:mth:Ippd  150 375)
  (afnix:mth:Ippd  150 325) (afnix:mth:Ippd  250 325)
  (afnix:mth:Ippd  250 375) (afnix:mth:Ippd  350 375)
  (afnix:mth:Ippd  350 250) (afnix:mth:Ippd  450 250)
  (afnix:mth:Ippd  450 375) (afnix:mth:Ippd  550 375)
  (afnix:mth:Ippd  550 175) (afnix:mth:Ippd  650 175)
  (afnix:mth:Ippd  650 375) (afnix:mth:Ippd  750 375)
  (afnix:mth:Ippd  750 100) (afnix:mth:Ippd  850 100)
  (afnix:mth:Ippd  850 375) (afnix:mth:Ippd  950 375)
  (afnix:mth:Ippd  950  25) (afnix:mth:Ippd 1050  25)
  (afnix:mth:Ippd 1050 375) (afnix:mth:Ippd 1150 375))
  
# create a svg polyline
trans poly (afnix:svg:SvgPolyline pv )
poly:set-fill   "none"
poly:set-stroke "sienna"
poly:set-stroke-width 10
frag:add-child poly

# create an output file and write
const os (afnix:sio:OutputFile "SVG1006.svg")
# add it to the root node
root:write os
os:close

# release the node
root:release
