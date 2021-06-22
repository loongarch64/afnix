# ---------------------------------------------------------------------------
# - SPS0001.als                                                             -
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

# @info   cell test unit
# @author amaury darsch

# get the module
interp:library "afnix-sps"

# create a new cell
const  cell-0 (afnix:sps:Cell)
assert true   (afnix:sps:cell-p cell-0)
assert nil    (cell-0:get)

# create a new cell
const  cell-1 (afnix:sps:Cell 2001)
assert 2001   (cell-1:get)

# create a new cell
const  cell-2  (afnix:sps:Cell "hello" 2001)
assert "hello" (cell-2:get-name)
assert 2001    (cell-2:get)

# change the cell name and value
cell-2:set-name "world"
assert     "world" (cell-2:get-name)
cell-2:set "edi"
assert     "edi"   (cell-2:get)

# check operator
cell-2:= 0
assert 0 (cell-2:get)
