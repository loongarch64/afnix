# ---------------------------------------------------------------------------
# - AXI0041.als                                                              -
# - afnix engine test module                                                -
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

# @info   bitset test module
# @author amaury darsch

# create a bitset
const  bs   (Bitset 8)
assert true (bitset-p bs)
assert 8    (bs:length)

# check, mark and clear
assert false (bs:marked-p 0)
bs:mark 0
assert true  (bs:marked-p 0)
bs:clear 0
assert false (bs:marked-p 0)

# mark and check
bs:mark 0
bs:mark 2
bs:mark 4
bs:mark 6

# set a bitset by binary value
bs:set "010101"
assert 6 (bs:length)

# clamp by 1 and check
bs:clamp true
assert 5 (bs:length)

# set a bitset by hexadecimal value
bs:reset
assert 0 (bs:length)
bs:set "0x12345678"
assert 32 (bs:length)

assert (Byte 0x78) (bs:to-byte 0)
assert (Byte 0x12) (bs:to-byte 24)

# extract subset
trans sb (bs:subset 4)
assert (Byte 0x08) (sb:to-byte 0)
