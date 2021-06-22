# ---------------------------------------------------------------------------
# - AXI0043.als                                                              -
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

# @info   builtin regex test module
# @author amaury darsch

# simple regex first
assert true (== [hello] "hello")
assert true (== [$d+]    2000)
assert true (== [($d$d):($d$d)] "12:31")

# another with sub group
assert true  (== [$d$d[$d$d]+] 2000)
assert false (== [$d$d[$d$d]*] 20000)

# simple symbol binding
if (== (const re [($d$d):($d$d)]) "12:31") {
  assert true (regex-p re)
  assert 2    (re:length)
  assert "12" (re:get 0)
  assert "31" (re:get 1)
}

# backtracking fun
assert true (== [$d+$d]   666)
assert true (== [$d+$d$d] 666)
