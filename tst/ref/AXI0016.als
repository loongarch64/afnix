# ---------------------------------------------------------------------------
# - AXI0016.als                                                              -
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

# @info   while and do loop test module
# @author amaury darsch

# test while and do loop
trans i 0
while (< i 10) {
  assert true (< i 10)
  trans i (+ i 1)
}
assert i 10

do {
  assert true (> i 0)
  trans i (- i 1)
} (> i 0)
assert i 0

# test loop
trans x 0
loop (trans j 0) (< j 10) (j:++) {
  assert x j
  x:++
}

# two loops - (yes it can break)
loop (trans k 0) (< k 10) (k:++) {
  loop (trans r 0) (< r 10) (r:++) {
  }
}
