# ---------------------------------------------------------------------------
# - SPS0004.als                                                             -
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

# @info   folio test unit
# @author amaury darsch

# get the module
interp:library "afnix-sps"

# the folio name and info
const name "planets"
const info "the solar system planets"

# create a new folio
const  sps  (afnix:sps:Folio name info)
assert true (afnix:sps:folio-p sps)

# check the folio name and info
assert name (sps:get-name)
assert info (sps:get-info)

# check the folio properties
assert 0 (sps:property-length)
sps:add-property "library" "sps"

assert 1     (sps:property-length)
assert true  (sps:property-p "library")
assert "sps" (sps:get-property-value "library")

# create and add a sheet data
const sht (afnix:sps:Sheet "data")
sps:add sht

# fill the sheet with data
sht:add-data "Mercury"   4840  "1407:36"
sht:add-data "Venus"    12400  "5819:51"
sht:add-data "Earth"    12756  "23:56"
sht:add-data "Mars"      6800  "24:37"
sht:add-data "Jupiter" 142800  "9:50"
sht:add-data "Saturn"  120800  "10:14"
sht:add-data "Uranus"   47600  "10:49"
sht:add-data "Neptune"  44600  "15:40"
sht:add-data "Pluto"     5850  "153:17"
sht:add-data "Sedna"     1800  "960:00"
