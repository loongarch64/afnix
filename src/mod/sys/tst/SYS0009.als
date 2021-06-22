# ---------------------------------------------------------------------------
# - SYS0009.als                                                             -
# - afnix:sys module test unit                                              -
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

# @info   performance meter unit
# @author amaury darsch

# get the module
interp:library "afnix-sys"

# create a default performance meter
trans  perf (afnix:sys:Meter)
assert "Meter" (perf:repr)

# set name and info
const name "meter"
const info "afnix performance meter"
perf:set-name-info name info
assert name (perf:get-name)
assert info (perf:get-info)

# check default time/counter numbers
assert 1 (perf:get-meter-number)
assert 0 (perf:get-counter-number)

# check reference time
assert 0    (perf:get-reference-time 0)
trans  tref (perf:set-reference-time)
assert tref (perf:get-reference-time 0)

trans  tmtr (perf:stamp 0)
assert tmtr (perf:get-meter 0)

# create a new meter
trans  perf (afnix:sys:Meter 1 0)
# check default time/counter numbers
assert 0 (perf:get-meter-number)
assert 1 (perf:get-counter-number)

# check counter
assert 0 (perf:get-counter 0)
trans  pcnt (perf:mark 0)
assert 1    pcnt
assert pcnt (perf:get-counter 0)
