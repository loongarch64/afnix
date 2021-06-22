# ---------------------------------------------------------------------------
# - SPS0003.als                                                             -
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

# @info   sheet test unit
# @author amaury darsch

# get the module
interp:library "afnix-sps"

# create a new sheet
const  sht  (afnix:sps:Sheet)
assert true (afnix:sps:sheet-p sht)
assert 0    (sht:length)
assert 0    (sht:column-length)

# set and check name
sht:set-name    "sheet"
assert "sheet"  (sht:get-name)

# set and check info
sht:set-info    "info"
assert "info"   (sht:get-info)

# add a marker and check
sht:add-marker 0
assert 0 (sht:get-marker)

# set a signature and check
sht:set-signature "sps"
assert "sps" (sht:get-signature)

# add some tags and check
sht:add-tag "hello" "world" 2000
assert 3 (sht:tag-length)
assert "hello" (sht:get-tag 0)
assert "world" (sht:get-tag 1)
assert "2000"  (sht:get-tag 2)
assert true    (sht:tag-p "hello")
assert true    (sht:tag-p "world")
assert true    (sht:tag-p "2000")

# add in the sheet header and check
sht:add-header "head" 2000
trans  cell   (sht:get-header 0)
assert "head" (cell:get)
assert "head" (sht:map-header 0)
trans  cell   (sht:get-header 1)
assert 2000   (cell:get)
assert 2000   (sht:map-header 1)

# add in the sheet footer and check
sht:add-footer 2000 "foot"
trans  cell   (sht:get-footer 0)
assert 2000   (cell:get)
assert 2000   (sht:map-footer 0)
trans  cell   (sht:get-footer 1)
assert "foot" (cell:get)
assert "foot" (sht:map-footer 1)

# add data to the sheet
sht:add-data   "hello" "world"
assert 1       (sht:length)
assert true    (sht:row-p 0 "hello")
assert 0       (sht:find-row 0   "hello")
assert 0       (sht:lookup-row 1 "world")
sht:add-data   2000 3000
assert 2       (sht:length)
assert 1       (sht:find-row   0 2000)
assert 1       (sht:lookup-row 1 3000)

# check for row, column access
trans  cell    (sht:get 0 0)
assert "hello" (cell:get)
trans  cell    (sht:get 0 1)
assert "world" (cell:get)
trans  cell    (sht:get 1 0)
assert 2000    (cell:get)
trans  cell    (sht:get 1 1)
assert 3000    (cell:get)

# link a column
const nsh (afnix:sps:Sheet)
nsh:link-column sht 1
assert 2       (nsh:length)
assert 1       (nsh:column-length)
trans  cell    (nsh:get 0 0)
assert "world" (cell:get)
trans  cell    (nsh:get 1 0)
assert 3000    (cell:get)
