# ---------------------------------------------------------------------------
# - AXI0032.als                                                              -
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

# @info   vector class test module
# @author amaury darsch

const  v (Vector 1 2 3)
assert true (vector-p v)

assert 3    (v:length)
assert 1    (v:get 0)
assert 2    (v:get 1)
assert 3    (v:get 2)

# compute internal value
const it    (v:iterator)
trans val   0
while (it:valid-p) {
  val:+= (it:get-object)
  it:next
}
assert  val 6

# a a new intermediate value
v:add 1 -1
assert  4 (v:length) 
assert  1 (v:get 0)
assert -1 (v:get 1)
assert  2 (v:get 2)
assert  3 (v:get 3)
