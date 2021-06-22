# ---------------------------------------------------------------------------
# - AXI0025.als                                                              -
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

# @info   fibonacci iterator test module (from Springer & Friedman)
# @author amaury darsch (for the afnix version)

# this test is excellent to check for the fp consistency

const fib-it (n acc1 acc2) (
    if (== n 1) acc2 (fib-it (- n 1) acc2 (+ acc1 acc2)))

const fiboi (n) (
    if (== n 0) 0 (fib-it n 0 1))

const fibos (n) (
  if (< n 2) n (+ (self (- n 1)) (self (- n 2))))

assert (fibos 10) (fiboi 10)
