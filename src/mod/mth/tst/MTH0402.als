# ---------------------------------------------------------------------------
# - MTH0402.als                                                             -
# - afnix:mth univariate fitter test unit                                   -
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

# @info   lufit test unit
# @author amaury darsch

# get the module
interp:library "afnix-mth"

# create a default fitter
const fit (afnix:mth:Lufit)

# check predicate
assert true (afnix:mth:fit-p fit)
assert true (afnix:mth:lufit-p fit)

# check representation
assert "Lufit" (fit:repr)

# check size
assert 1 (fit:get-size)

# add some predictor/response
fit:add-predictor 0
fit:add-response  0
fit:add-predictor 1
fit:add-response  1
fit:add-predictor 2
fit:add-response  2

# factorize the effects
assert true (fit:factorize)

# evaluate some regression
assert 0.0 (fit:regress 0.0)
assert 1.0 (fit:regress 1.0)
assert 2.0 (fit:regress 2.0)
assert 3.0 (fit:regress 3.0)
