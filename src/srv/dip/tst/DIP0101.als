# ---------------------------------------------------------------------------
# - DIP0101.als                                                             -
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
const ppm (afnix:dip:netpbm-read "DIP0101.ppm")
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

# write the raw file
assert true (afnix:dip:netpbm-write "DIP0101.raw" ppm true)
# read again the file
const raw (afnix:dip:netpbm-read "DIP0101.raw")

# compare the file
assert (raw:get-format) (ppm:get-format)
assert (raw:get-width)  (ppm:get-width)
assert (raw:get-height) (ppm:get-height)

# check content
loop (trans y 0) (< y 16) (y:++) {
  loop (trans x 0) (< x 16) (x:++) {
    # get pixmap pixel
    trans spix (ppm:get-pixel x y)
    trans rpix (raw:get-pixel x y)
    # check components
    assert (rpix:get-component 0) (spix:get-component 0)
    assert (rpix:get-component 1) (spix:get-component 1)
    assert (rpix:get-component 2) (spix:get-component 2)
    assert (rpix:get-component 3) (spix:get-component 3)
  }
}

# write the text file
assert true (afnix:dip:netpbm-write "DIP0101.txt" ppm false)
# read again the file
const txt (afnix:dip:netpbm-read "DIP0101.txt")

# compare the file
assert (txt:get-format) (ppm:get-format)
assert (txt:get-width)  (ppm:get-width)
assert (txt:get-height) (ppm:get-height)

# check content
loop (trans y 0) (< y 16) (y:++) {
  loop (trans x 0) (< x 16) (x:++) {
    # get pixmap pixel
    trans spix (ppm:get-pixel x y)
    trans tpix (txt:get-pixel x y)
    # check components
    assert (tpix:get-component 0) (spix:get-component 0)
    assert (tpix:get-component 1) (spix:get-component 1)
    assert (tpix:get-component 2) (spix:get-component 2)
    assert (tpix:get-component 3) (spix:get-component 3)
  }
}
