# ---------------------------------------------------------------------------
# - MTH0200.als                                                             -
# - afnix:mth module function test unit                                     -
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

# @info   real function test unit
# @author amaury darsch

# get the module
interp:library "afnix-mth"

# create a null function
const f (afnix:mth:Rfunction)

# check predicate
assert true (afnix:mth:rfi-p f)
assert true (afnix:mth:r-function-p f)

# check representation
assert "Rfunction" (f:repr)

# check compute at 0.0 and 1.0
assert 0.0 (f:compute 0.0)
assert 0.0 (f:compute 1.0)

# check derivate at 0.0 and 1.0
assert 0.0 (f:derivate 0.0)
assert 0.0 (f:derivate 1.0)

