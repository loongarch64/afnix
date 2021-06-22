# ---------------------------------------------------------------------------
# - DIP0100.als                                                             -
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

# @info   netpbm test unit
# @author amaury darsch

# get the service
interp:library "afnix-dip"

# create an image by file
const pgm (afnix:dip:netpbm-read "DIP0100.pgm")
assert true (afnix:dip:pixmap-p pgm)

# check format dipmetry
assert afnix:dip:Pixel:PFMT-BYTE (pgm:get-format)
assert 16 (pgm:get-width)
assert 16 (pgm:get-height)

# check content
loop (trans y 0) (< y 16) (y:++) {
  loop (trans x 0) (< x 16) (x:++) {
    # get color value
    trans col (+ (* y 16) x)
    # get pixmap pixel
    trans pixl (pgm:get-pixel x y)
    # check components
    assert col (pixl:get-component)
  }
}

# create an image by file
const ppm (afnix:dip:netpbm-read "DIP0100.ppm")
assert true (afnix:dip:pixmap-p ppm)

# check format dipmetry
assert afnix:dip:Pixel:PFMT-RGBA (ppm:get-format)
assert 16 (ppm:get-width)
assert 16 (ppm:get-height)

# check content
loop (trans y 0) (< y 16) (y:++) {
  loop (trans x 0) (< x 16) (x:++) {
    # get color value
    trans col (+ (* y 16) x)
    # get pixmap pixel
    trans pixl (ppm:get-pixel x y)
    # check components
    assert col (pixl:get-component 0)
    assert col (pixl:get-component 1)
    assert col (pixl:get-component 2)
    assert 255 (pixl:get-component 3)
  }
}
