# ---------------------------------------------------------------------------
# - WAX0006.als                                                             -
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

# @info   xhtm image test unit
# @author amaury darsch

# get the module
interp:library "afnix-xml"
interp:library "afnix-wax"

# the image information
const src "afnix.png"
const alt "afnix logo"
const wth "100"
const hgt "100"

# create an image (img) element
const  img  (afnix:wax:XhtmlImg src alt)
assert true (afnix:xml:node-p img)
assert true (afnix:wax:xhtml-img-p img)

# check the image attribute
assert src (img:get-attribute-value "src")
assert alt (img:get-attribute-value "alt")

# set the image geometry and check
img:set-geometry wth hgt
assert wth (img:get-attribute-value "width")
assert hgt (img:get-attribute-value "height")
