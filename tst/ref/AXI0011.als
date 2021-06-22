# ---------------------------------------------------------------------------
# - AXI0011.als                                                              -
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

# @info   logical operation test module
# @author amaury darsch

# negation
assert true  (not false)
assert false (not true)

# and operator
assert true  (and true  true)
assert false (and true  false)

# or operator
assert true  (or  true  false)
assert false (or  false false)

# self assign
const  bval  false
assert false bval
bval:= true
assert false (not bval)
