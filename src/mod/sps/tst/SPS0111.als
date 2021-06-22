# ---------------------------------------------------------------------------
# - SPS0111.als                                                             -
# - afnix:sps module test unit                                              -
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

# @info   transit/structure test unit
# @author amaury darsch

# get the module
interp:library "afnix-sps"

# create a new sps transit
const  st  (afnix:sps:SpsTransit)
assert true (afnix:sps:transit-p st)

# import a csv into a sheet
st:import "SPS011X.csv"
const sht (st:get-sheet)

# check sheet data
assert true (afnix:sps:sheet-p sht)
assert 18   (sht:length)

# create a structure to fill
trans s (Structure)
s:add-locale "L=1&O=INTG&X=NONE"
s:add-locale "L=2&O=REAL&X=NONE"
s:add-locale "L=3&O=REAL&X=NONE"
assert true (st:fill s)
assert 432 (s:length)

# create a structure to fill
trans s (Structure)
s:add-locale "L=1&O=INTG&X=SHRT"
s:add-locale "L=2&O=REAL&X=SHRT"
s:add-locale "L=3&O=REAL&X=NONE"
assert true (st:fill s)
assert 288 (s:length)

# create a structure to fill
trans s (Structure)
s:add-locale "L=1&O=INTG&X=SHRT"
s:add-locale "L=2&O=REAL&X=SHRT"
s:add-locale "L=3&O=REAL&X=SHRT"
assert true (st:fill s)
assert 216 (s:length)

# create a structure to fill
trans s (Structure)
s:add-locale "L=1&O=INTG&X=SHRT"
s:add-locale "L=2&O=REAL&X=SHRT"
s:add-locale "L=3&O=REAL&X=SHRT"
s:set-padding 16
assert true (st:fill s)
assert 288 (s:length)
