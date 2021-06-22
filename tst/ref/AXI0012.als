# ---------------------------------------------------------------------------
# - AXI0012.als                                                              -
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

# @info   predicates test module
# @author amaury darsch

# nil validtion
assert true  (nil-p nil)
assert false (nil-p true)

# object validation
assert false (object-p nil)
assert true  (object-p true)

# base literal objects
assert true  (real-p      1.0)
assert true  (real-p      6.626068E-34)
assert true  (string-p    "hello")
assert true  (integer-p   0)
assert true  (relatif-p   0R)
assert true  (boolean-p   true)
assert true  (literal-p   "world")
assert true  (character-p 'a')

# number predicate
assert true  (number-p    1)
assert true  (number-p    1R)
assert true  (number-p    1.0)

  
