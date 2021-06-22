# ---------------------------------------------------------------------------
# - CDA0201                                                             -
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

# @info   sps streamer test unit
# @author amaury darsch

# load the cda generator
interp:load "CDA020X"

# check a sps streamer
const sps (new-sps-streamer 1)
assert true (afnix:cda:sps-streamer-p sps)

# check boundary
assert 0.0 (sps:departure)
assert 9.0 (sps:arrival)

# move and check
assert 0.0 (sps:begin)
assert 0   (sps:get-integer)
assert 1.0 (sps:next)
assert 1   (sps:get-integer)
assert 9.0 (sps:end)
assert 9   (sps:get-integer)

# check forward
assert (sps:departure) (sps:begin)
assert 4.0 (sps:forward 4)
assert 6.0 (sps:forward 6.5)
assert 9.0 (sps:forward 9.0)
assert 9.0 (sps:forward true)

# check backward
assert (sps:arrival) (sps:end)
assert 4.0 (sps:backward 4)
assert 2.0 (sps:backward 2.5)
assert (sps:departure) (sps:backward 0.0)
assert (sps:departure) (sps:backward true)
