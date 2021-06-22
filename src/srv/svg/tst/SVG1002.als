# ---------------------------------------------------------------------------
# - SVG1002.als                                                             -
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

# @info   svg root rectangle test unit - svg 1.1 example
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

# create a svg rectangle
const  rect (afnix:svg:SvgRect 400 200 100 100)
rect:set-fill "green"
rect:set-rounding-x 50
frag:add-child rect

# create an output file and write
const os (afnix:sio:OutputFile "SVG1002.svg")
# add it to the root node
root:write os
os:close

# release the node
root:release
