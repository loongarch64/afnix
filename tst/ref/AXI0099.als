# ---------------------------------------------------------------------------
# - AXI0099.als                                                              -
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

# @info   corner cases test module
# @author amaury darsch

# nil argument in function call
const f (x y) {
  if (nil-p x) (return y)
  if (nil-p y) (return x)
  + x y
}
assert 2 (f nil 2)
assert 1 (f 1 nil)

# nil argument in method call
const c (class)
trans c:f (x y) {
  if (nil-p x) (return y)
  if (nil-p y) (return x)
  + x y
}
const  i (c)
assert 2 (i:f nil 2)
assert 1 (i:f 1 nil)
