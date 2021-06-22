# ---------------------------------------------------------------------------
# - SPS0005.als                                                             -
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

# @info   index test unit
# @author amaury darsch

# get the module
interp:library "afnix-sps"

# create a new index
const  indx (afnix:sps:Index)
assert true (afnix:sps:index-p indx)

# check length
assert 0    (indx:length)

# add  element
indx:add 9 9 9
assert 1 (indx:length)
assert 9 (indx:get-index-cell   0)
assert 9 (indx:get-index-record 0)
assert 9 (indx:get-index-sheet  0)

# set element
indx:set-index-cell 0 0
assert 0 (indx:get-index-cell 0)
indx:set-index-record 0 1
assert 1 (indx:get-index-record 0)
indx:set-index-sheet 0 2
assert 2 (indx:get-index-sheet 0)

