# ---------------------------------------------------------------------------
# - AXI0020.als                                                              -
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

# @info   boolean class and builtin method
# @author amaury darsch

const f  (Boolean)
const fl (Boolean false)
const fs (Boolean "false")

const t  (Boolean true)
const ts (Boolean "true")

assert false f
assert false fl
assert false fs

assert true  t
assert true  ts

assert "true"  (t:to-string)
assert "false" (f:to-string)
