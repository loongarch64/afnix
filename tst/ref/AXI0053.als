# ---------------------------------------------------------------------------
# - AXI0053.als                                                              -
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

# @info   set iteration test module
# @author amaury darsch

const scalar-product (u v) {
  trans result 0
  for (x y) (u v) (result:+= (* x y))
  eval result
}

# define 2 sets
const  s1 (Set 1 2 3)
const  s2 (Set 2 4 6)
assert  3 (s1:length)
assert  3 (s2:length)
assert 28 (scalar-product s1 s2)

# merge the sets
s1:merge s2
assert 6 (s1:length)

# get a random subset
const  rs   (s1:get-random-subset 2)
assert true (<= (rs:length) 2)
