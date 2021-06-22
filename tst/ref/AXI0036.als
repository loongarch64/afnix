# ---------------------------------------------------------------------------
# - AXI0036.als                                                              -
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

# @info   integer hexadecimal and binary test module
# @author amaury darsch

# basic tests
assert 255 0xff
assert 255 0b11111111

assert -255 -0x0000_00ff
assert -255 -0b1111_1111

# invariant integer
assert 0x8000_0000_0000_0000 -0x8000_0000_0000_0000

# biggest negative
assert -1 0xffff_ffff_ffff_ffff

