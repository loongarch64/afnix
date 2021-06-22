# ---------------------------------------------------------------------------
# - AXI0031.als                                                              -
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

# @info   list class test module
# @author amaury darsch

const  list (List 1 2 3)
assert true (list-p list)

assert 3    (list:length)
assert 1    (list:get 0)
assert 2    (list:get 1)
assert 3    (list:get 2)

const it    (list:iterator)
trans val   0
while (it:valid-p) {
  val:+= (it:get-object)
  it:next
}
assert  val 6
