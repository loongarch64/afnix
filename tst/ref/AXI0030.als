# ---------------------------------------------------------------------------
# - AXI0030.als                                                              -
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

# @info   cons class test module
# @author amaury darsch

# create a list of cons cell
const  c      (Cons 1 2)
assert true   (cons-p c)
assert "Cons" (c:repr)

# basic check
assert 2  (c:length)
assert 1  (c:get-car)
assert 2  (c:get-cadr)

# check the iterator
const it    (c:iterator)
trans val   0
while (it:valid-p) {
  val:+= (it:get-object)
  it:next
}
assert val 3

# check for additional
c:add  3
assert 3  (c:get-caddr)
it:begin
trans val 0
while (it:valid-p) {
  val:+= (it:get-object)
  it:next
}
assert  val 6

# check for circular
const cell (c:get-cdr)
cell:set-cdr c

assert 2 (cell:length)
c:release
