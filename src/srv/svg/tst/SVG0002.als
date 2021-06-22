# ---------------------------------------------------------------------------
# - SVG0002.als                                                             -
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

# @info   svg root test unit
# @author amaury darsch

# get the module
interp:library "afnix-xml"
interp:library "afnix-svg"

# create a svg root
const  root (afnix:svg:SvgRoot 100 100)
assert true (afnix:xml:node-p root)
assert true (afnix:svg:svg-root-p root)

# check the mime type
assert "image/svg+xml" (root:get-mime)

# get the svg fragment and check
const  frag (root:get-svg)
assert true (afnix:svg:svg-fragment-p frag)

# release the node
root:release
