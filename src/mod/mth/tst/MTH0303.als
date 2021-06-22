# ---------------------------------------------------------------------------
# - MTH0303.als                                                             -
# - afnix:mth module - real givens matrix test unit                         -
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

# @info   real givens matrix test unit
# @author amaury darsch

# get the module
interp:library "afnix-mth"

# create a givens array
const ga (afnix:mth:Rgivens)

# check predicate
assert true (afnix:mth:r-givens-p ga)
# check representation
assert "Rgivens" (ga:repr)

# check content
assert 0 (ga:length)

# add and check
assert 0   (ga:add 0 1 1 0)
assert 0   (ga:get-row 0)
assert 1   (ga:get-col 0)
assert 0.0 (ga:get-c 0)
assert 1.0 (ga:get-s 0)
