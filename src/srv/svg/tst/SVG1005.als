# ---------------------------------------------------------------------------
# - SVG1005.als                                                             -
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

# @info   svg line test unit - svg 1.1 example
# @author amaury darsch

# get the module
interp:library "afnix-sio"
interp:library "afnix-xml"
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

# create a svg line
trans line (afnix:svg:SvgLine 100 300 300 100)
line:set-stroke "sienna"
line:set-stroke-width 5
frag:add-child line

trans line (afnix:svg:SvgLine 300 300 500 100)
line:set-stroke "sienna"
line:set-stroke-width 10
frag:add-child line

trans line (afnix:svg:SvgLine 500 300 700 100)
line:set-stroke "sienna"
line:set-stroke-width 15
frag:add-child line

trans line (afnix:svg:SvgLine 700 300 900 100)
line:set-stroke "sienna"
line:set-stroke-width 20
frag:add-child line

trans line (afnix:svg:SvgLine 900 300 1100 100)
line:set-stroke "sienna"
line:set-stroke-width 25
frag:add-child line

# create an output file and write
const os (afnix:sio:OutputFile "SVG1005.svg")
# add it to the root node
root:write os
os:close

# release the node
root:release
