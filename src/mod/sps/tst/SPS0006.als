# ---------------------------------------------------------------------------
# - SPS0006.als                                                             -
# - afnix:sps mdoule test unit                                              -
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

# @info   xref table test unit
# @author amaury darsch

# get the module
interp:library "afnix-sps"

# create a new xref table
const  xref (afnix:sps:Xref)
assert true (afnix:sps:xref-p xref)

# check length
assert 0    (xref:length)

# add indexes
const T0 "test 0"
xref:add T0 0
assert 1    (xref:length)
assert true (xref:exists-p T0)

# get the index
trans  indx (xref:get T0)
assert 1    (indx:length)
assert 0    (indx:get-index-cell 0)

# add a new entry and check the index
xref:add T0 9 9 9
assert 2    (indx:length)
assert 9    (indx:get-index-cell 1)

# add indexes
const T1 "test 1"
xref:add T1 1 2
assert 2    (xref:length)
assert true (xref:exists-p T1)

# get the index
trans  indx (xref:get T1)
assert 1    (indx:length)
assert 1    (indx:get-index-cell 0)

# add a new entry and check the index
xref:add T1 9 9 9
assert 2    (indx:length)
assert 9    (indx:get-index-cell 1)

# add indexes
const T2 "test 2"
xref:add T2 2 3 4
assert 3    (xref:length)
assert true (xref:exists-p T2)

# get the index
trans  indx (xref:get T2)
assert 1    (indx:length)
assert 2    (indx:get-index-cell 0)

# add a new entry and check the index
xref:add T2 9 9 9
assert 2    (indx:length)
assert 9    (indx:get-index-cell 1)
