# ---------------------------------------------------------------------------
# - SYS0002.als                                                             -
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

# @info   time class test unit
# @author amaury darsch

# get the module
interp:library "afnix-sys"

# simple time class
const  time (afnix:sys:Time 0)
assert true (afnix:sys:time-p time)

# check access methods
assert 0 (time:get-time)
assert 0 (time:hours)
assert 0 (time:minutes)
assert 0 (time:seconds)
# check the format
assert "00:00:00"  (time:format true)
assert "00:00:00Z" (time:to-iso true)

# add some time
time:add-hours   13
time:add-minutes 10
time:add         35

# check new time
assert 13 (time:hours)
assert 10 (time:minutes)
assert 35 (time:seconds)
# check the format
assert "13:10:35"  (time:format true)
assert "13:10:35Z" (time:to-iso true)

# add 2 dayt and check again
time:add-days 2
assert "13:10:35" (time:format true)
