# ---------------------------------------------------------------------------
# - AXI0046.als                                                              -
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

# @info   expression argument check
# @author amaury darsch

# check with no arguments
const proc-nilp (args) {
  trans result 0
  for (i) (args) (result:+= i)
  eval result
}

assert 3 (proc-nilp 1 2)
assert 7 (proc-nilp 1 2 4)
assert 9 (proc-nilp 1 2 4 2)

# check with arguments
const proc-args (a b args) {
  trans result (+ a b)
  for (i) (args) (result:+= i)
  eval result
}

assert 3 (proc-args 1 2)
assert 7 (proc-args 1 2 4)
assert 9 (proc-args 1 2 4 2)

# check with const arguments
const proc-const-args (a b (const args)) {
  trans result (+ a b)
  for (i) (args) (result:+= i)
  eval result
}

assert 3 (proc-const-args 1 2)
assert 7 (proc-const-args 1 2 4)
assert 9 (proc-const-args 1 2 4 2)
