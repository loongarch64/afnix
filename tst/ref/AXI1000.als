# ---------------------------------------------------------------------------
# - AXI1000.als                                                              -
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

# @info   hash value comparision
# @author amaury darsch

# compute string hashid - 32 bit form
const hashid-32 nil {
  trans s "foo"
  eval (== (s:hashid) 0x408f5e13)
}

# compute string hashid - 64 bit form
const hashid-64 nil {
  trans s "foo"
  eval (== (s:hashid) (- 0xd8cbc7186ba13533))
}

# compute a string hashid
const hashid nil {
  if (== interp:machine-size "32") (return (hashid-32))
  if (== interp:machine-size "64") (return (hashid-64))
  throw "test-error" "invalid machine size"
}

# validate testing
assert true (hashid)
