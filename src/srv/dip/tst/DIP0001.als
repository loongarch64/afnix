# ---------------------------------------------------------------------------
# - DIP0001.als                                                             -
# - afnix:dip service test unit                                             -
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

# @info   pixel test unit
# @author amaury darsch

# get the service
interp:library "afnix-dip"

# create a pixel object
trans  pixl (afnix:dip:Pixel)
assert true (afnix:dip:pixel-p pixl)
assert "Pixel" (pixl:repr)

# check format
assert afnix:dip:Pixel:PFMT-NONE (pixl:get-format)
pixl:set-format afnix:dip:Pixel:PFMT-RGBA
assert afnix:dip:Pixel:PFMT-RGBA (pixl:get-format)

# check value
assert 0 (pixl:get-component 0)
assert 0 (pixl:get-component 1)
assert 0 (pixl:get-component 2)
assert 0 (pixl:get-component 3)

pixl:set-component 0 63
pixl:set-component 1 127
pixl:set-component 2 183
pixl:set-component 3 255

assert 63  (pixl:get-component 0)
assert 127 (pixl:get-component 1)
assert 183 (pixl:get-component 2)
assert 255 (pixl:get-component 3)

# convert to real
trans pixr (pixl:convert afnix:dip:Pixel:PFMT-RGBR)
assert (pixr:get-component 0) (/ (Real (pixl:get-component 0)) 255.0)
assert (pixr:get-component 1) (/ (Real (pixl:get-component 1)) 255.0)
assert (pixr:get-component 2) (/ (Real (pixl:get-component 2)) 255.0)
assert (pixr:get-component 3) 1.0

# convert back to bgra instead
trans pixi (pixr:convert afnix:dip:Pixel:PFMT-BGRA)
assert (pixi:get-component 0) (pixl:get-component 2)
assert (pixi:get-component 1) (pixl:get-component 1)
assert (pixi:get-component 2) (pixl:get-component 0)
assert (pixi:get-component 3) (pixl:get-component 3)


