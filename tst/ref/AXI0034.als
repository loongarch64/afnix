# ---------------------------------------------------------------------------
# - AXI0034.als                                                              -
# - afnix engine test module                                                -
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

# @info   instance meta class test module
# @author amaury darsch

const Color (class)

# class initializer
trans Color:preset (red green blue) {
  const this:red   red
  const this:green green
  const this:blue  blue
}

# this function check that two colors are equals
trans Color:== (color) {
  if (== Color color:meta) {
    if (!= this:red   color:red)   (return false)    
    if (!= this:green color:green) (return false)    
    if (!= this:blue  color:blue)  (return false)
    eval true
  } false
}

# create a new yellow color
const yellow (Color 255 255 0)
assert true  (yellow:== (Color 255 255 0))
