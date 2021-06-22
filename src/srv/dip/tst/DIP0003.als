# ---------------------------------------------------------------------------
# - DIP0003.als                                                             -
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

# @info   pixmap test unit
# @author amaury darsch

# get the service
interp:library "afnix-dip"

# create a new pixmap by format and dipmetry
trans pixm (afnix:dip:Pixmap afnix:dip:Pixel:PFMT-RGBF 16 16)
assert afnix:dip:Pixel:PFMT-RGBF (pixm:get-format)
assert 16 (pixm:get-width)
assert 16 (pixm:get-height)

# fill the pixmap array
loop (trans x 0) (< x 16) (x:++) {
  loop (trans y 0) (< y 16) (y:++) {
    trans pixl (afnix:dip:Pixel)
    pixl:set-format afnix:dip:Pixel:PFMT-RGBF
    pixl:set-component 0 (Real x)
    pixl:set-component 1 (Real y)
    pixl:set-component 2 (Real (+ x y))
    pixl:set-component 3 1.0
    pixm:set-pixel x y pixl
  }
}

# check pixmap pixels
loop (trans x 0) (< x 16) (x:++) {
  loop (trans y 0) (< y 16) (y:++) {
    trans pixl (pixm:get-pixel x y)
    assert (pixl:get-component 0) (Real x)
    assert (pixl:get-component 1) (Real y)
    assert (pixl:get-component 2) (Real (+ x y))
    assert (pixl:get-component 3) 1.0
  }
}
