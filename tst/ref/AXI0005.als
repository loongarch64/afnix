# ---------------------------------------------------------------------------
# - AXI0005.als                                                              -
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

# @info   complex argument closure test
# @author amaury darsch

# test with self
const gfact (n) (if (== n 1) 1 (* n (self (- n 1))))
trans lfact (n) (if (== n 1) 1 (* n (self (- n 1))))
assert (gfact 4) (lfact 4)

# test with recursive arguments
const x 1
trans f (x) (+ (g) x)
trans g nil (+ x 2)
assert 16 (f 7)
