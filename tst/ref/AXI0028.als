# ---------------------------------------------------------------------------
# - AXI0028.als                                                              -
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

# @info   simple class definition test module
# @author amaury darsch

# bind class symbols
const c        (class)
const c:a      0
trans c:b      "hello"
const c:c      (x) (eval x)

# verify class symbols
assert 0       c:a
assert "hello" c:b
assert 1       (c:c 1)

# mute class symbols
c:b:=  "world"
assert "world" c:b
assert "Class" (c:repr)

# assert class bindings
assert true    (eval-p c:a)
unref c:a
assert true    (eval-p  c:a)
assert true    (combo-p c:a)
