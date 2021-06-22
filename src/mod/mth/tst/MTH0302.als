# ---------------------------------------------------------------------------
# - MTH0302.als                                                             -
# - afnix:mth module - ream matrix datum test unit                          -
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

# @info   real matrix datum test unit
# @author amaury darsch

# get the module
interp:library "afnix-mth"

# create a default datum
const d (afnix:mth:Rmd)

# check predicate
assert true (afnix:mth:rmd-p d)

# check representation
assert "Rmd" (d:repr)

# check accessors
assert 0   (d:get-row)
assert 0   (d:get-col)
assert 0.0 (d:get-val)

# check mutators
d:set 1 2 3
assert 1   (d:get-row)
assert 2   (d:get-col)
assert 3.0 (d:get-val)

