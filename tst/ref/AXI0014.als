# ---------------------------------------------------------------------------
# - AXI0014.als                                                              -
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

# @info   eval / protect / return test function
# @author amaury darsch

assert 0 (interp:eval 0)
const  k 1
assert k (interp:eval k)

const  i (protect 0)
assert 0 (eval i)
assert 0 (interp:eval i)

const  g (x) (return x)
assert 0 (g 0)

trans  l (x) (return x)
assert 0 (l 0)
