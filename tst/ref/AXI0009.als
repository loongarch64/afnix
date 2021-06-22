# ---------------------------------------------------------------------------
# - AXI0009.als                                                              -
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

# @info   if special form test module
# @author amaury darsch

assert true  (if true true)
assert false (if true false)
assert true  (if false false true)

assert 0     (if true 0)
assert 1     (if (== 0 0) 1)

# simple complex evaluation

const  a 1
assert 2 (if (== a 1) (+ a 1))
