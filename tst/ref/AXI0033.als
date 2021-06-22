# ---------------------------------------------------------------------------
# - AXI0033.als                                                              -
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

# @info   vector iteration test module
# @author amaury darsch

const scalar-product (u v) {
  trans result 0
  for (x y) (u v) (result:+= (* x y))
  eval result
}

# define 2 vectors
const  v1 (Vector 1 2 3)
const  v2 (Vector 2 4 6)
assert 28 (scalar-product v1 v2)
