# ---------------------------------------------------------------------------
# - AXI0038.als                                                             -
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

# @info   imbricated for loops
# @author amaury darsch

# two vectors for test
const v1 (Vector 1 1 1)
const v2 (Vector 1 2 3)

# loop and assert
trans re 0
for (x) (v1) {
  for (y) (v2) {
    re:+= (* x y)
  }
}
assert re 18
