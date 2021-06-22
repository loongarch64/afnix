# ---------------------------------------------------------------------------
# - SVG2001.als                                                             -
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

# @info   svg plot2d test unit
# @author amaury darsch

# get the module
interp:library "afnix-sio"
interp:library "afnix-mth"
interp:library "afnix-svg"

# create a 1 column samples
const rsa (afnix:mth:Rsamples 1)
loop (trans i -9) (< i 10) (i:++) {
  trans row (rsa:new-row (Real i))
  rsa:set row 0 (* (Real i) (Real i))
}

# create a plot2d object
const ptd (afnix:svg:Plot2d rsa)

# create a time plot at column 0
ptd:time-plot 0

# create a svg root node
const root (afnix:svg:SvgRoot ptd)
 
# create an output file and write
const os (afnix:sio:OutputFile "SVG2001.svg")
root:write os
os:close

# release the node
root:release
