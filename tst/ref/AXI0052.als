# ---------------------------------------------------------------------------
# - AXI0052.als                                                              -
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

# @info   set class test module
# @author amaury darsch

# create an empty set
trans s (Set)
assert true (set-p s)
assert "Set" (s:repr)

# fill a new set
trans  s (Set 1 2 3)
# check the cardinal
assert 3 (s:length)

# check the iterator
const it  (s:iterator)
trans val 0
while (it:valid-p) {
  val:+= (it:get-object)
  it:next
}
assert  val 6
