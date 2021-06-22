# ---------------------------------------------------------------------------
# - SPS0007.als                                                             -
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

# @info   bundle test unit
# @author amaury darsch

# get the module
interp:library "afnix-sps"

# create a new bundle
const  bndl (afnix:sps:Bundle)
assert true (afnix:sps:bundle-p bndl)

# check length
assert 0 (bndl:length)

# test the boolean bundle
bndl:add true
assert 1 (bndl:length)
assert true (bndl:get 0)
bndl:add false
assert 2 (bndl:length)
assert false (bndl:get 1)
bndl:set 0 false
bndl:set 1 true
assert 2 (bndl:length)
assert false (bndl:get 0)
assert true  (bndl:get 1)
assert false (bndl:first)
assert true  (bndl:last)

# test the integer bundle
bndl:clear
bndl:add 0
assert 1 (bndl:length)
assert 0 (bndl:get 0)
bndl:add 1
assert 2 (bndl:length)
assert 1 (bndl:get 1)
bndl:set 0 2
bndl:set 1 3
assert 2 (bndl:length)
assert 2 (bndl:get 0)
assert 3 (bndl:get 1)
assert 2 (bndl:first)
assert 3 (bndl:last)

# check iterator
trans sum 0.
for (x) (bndl) (sum:+= x)
assert 5 sum

# test the real bundle
bndl:clear
bndl:add 0.0
assert 1 (bndl:length)
assert 0.0 (bndl:get 0)
bndl:add 1.0
assert 2 (bndl:length)
assert 1.0 (bndl:get 1)
bndl:set 0 2.0
bndl:set 1 3.0
assert 2 (bndl:length)
assert 2.0 (bndl:get 0)
assert 3.0 (bndl:get 1)
assert 2.0 (bndl:first)
assert 3.0 (bndl:last)

# check iterator
trans sum 0.0
for (x) (bndl) (sum:+= x)
assert 5.0 sum

# test the bundle
bndl:clear
bndl:add 0R
assert 1  (bndl:length)
assert 0R (bndl:get 0)
bndl:add 1R
assert 2  (bndl:length)
assert 1R (bndl:get 1)
bndl:set 0 2R
bndl:set 1 3R
assert 2  (bndl:length)
assert 2R (bndl:get 0)
assert 3R (bndl:get 1)
assert 2R (bndl:first)
assert 3R (bndl:last)
