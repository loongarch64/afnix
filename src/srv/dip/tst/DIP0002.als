# ---------------------------------------------------------------------------
# - DIP0002.als                                                             -
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

# create a pixmap object
trans pixm (afnix:dip:Pixmap)
assert true (afnix:dip:slice-p pixm)
assert true (afnix:dip:pixmap-p pixm)
assert "Pixmap" (pixm:repr)

# check for null dipmetry
assert afnix:dip:Pixel:PFMT-NONE (pixm:get-format)
assert 0 (pixm:get-width)
assert 0 (pixm:get-height)

# create a new pixmap by format and dipmetry
trans pixm (afnix:dip:Pixmap afnix:dip:Pixel:PFMT-RGBA 1920 1080)
assert afnix:dip:Pixel:PFMT-RGBA (pixm:get-format)
assert 1920 (pixm:get-width)
assert 1080 (pixm:get-height)

# check for a null pixel
trans pixl (pixm:get-pixel 0 0)
assert 0 (pixl:get-component 0)
assert 0 (pixl:get-component 1)
assert 0 (pixl:get-component 2)
assert 0 (pixl:get-component 3)

# set a pixel and check
pixl:set-component 0 64
pixl:set-component 1 128
pixl:set-component 2 184
pixl:set-component 3 255
pixm:set-pixel 0 0 pixl
trans pixl (pixm:get-pixel 0 0)
assert 64  (pixl:get-component 0)
assert 128 (pixl:get-component 1)
assert 184 (pixl:get-component 2)
assert 255 (pixl:get-component 3)

pixm:set-pixel 1919 1079 pixl
trans pixl (pixm:get-pixel 1919 1079)
assert 64  (pixl:get-component 0)
assert 128 (pixl:get-component 1)
assert 184 (pixl:get-component 2)
assert 255 (pixl:get-component 3)
