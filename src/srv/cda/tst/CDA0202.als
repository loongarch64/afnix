# ---------------------------------------------------------------------------
# - CDA0202                                                             -
# - afnix:cda module test unit                                              -
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

# @info   linear streamer test unit
# @author amaury darsch

# load the cda generator
interp:load "CDA020X"

# check a sps streamer
const lnr (new-lnr-streamer 1)
assert true (afnix:cda:linear-streamer-p lnr)

# check boundary
assert 0.0 (lnr:departure)
assert 9.0 (lnr:arrival)

# check interpolation
assert 0.0 (lnr:move 0.0)
assert 0   (lnr:get-integer)
assert 0.5 (lnr:move 0.5)
assert 0.5 (lnr:locate)
assert 0   (lnr:get-integer)
assert 0.5 (lnr:get-real)
assert 1.0 (lnr:next)
assert 1.0 (lnr:locate)
assert 1   (lnr:get-integer)
assert 1.0 (lnr:get-real)
assert 1.4 (lnr:next 0.4)
assert 1.4 (lnr:locate)
assert 1   (lnr:get-integer)
assert 1.4 (lnr:get-real)
assert 1.2 (lnr:previous 0.2)
assert 1.2 (lnr:locate)
assert 1   (lnr:get-integer)
assert 1.2 (lnr:get-real)
assert 0.0 (lnr:previous)
assert 9.0 (lnr:move 10.0)
assert 9   (lnr:get-integer)
assert 9.0 (lnr:get-real)
assert 9.0 (lnr:move 10.5)
assert 9   (lnr:get-integer)
assert 9.0 (lnr:get-real)
assert 0.0 (lnr:move -10.0)

# check forward
assert (lnr:departure) (lnr:begin)
assert 4.0 (lnr:forward 4)
assert 4.5 (lnr:forward 4.5)
assert 6.0 (lnr:forward 6)
assert (lnr:arrival) (lnr:forward 9.5)

# check backward
assert (lnr:arrival) (lnr:end)
assert 4.0 (lnr:backward 4)
assert 2.5 (lnr:backward 2.5)
assert (lnr:departure) (lnr:backward 0.0)
assert (lnr:departure) (lnr:backward true)

