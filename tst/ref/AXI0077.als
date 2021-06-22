# ---------------------------------------------------------------------------
# - AXI0077.als                                                              -
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

# @info   class extra testing
# @author amaury darsch

# bind class and closure
const c (class)
trans c:preset nil {
 trans this:tl (lambda (x y) (+ x y))
}

trans c:compute (x y) (this:tl x y)
# create an instance and check
const i (c)
assert true (combo-p i:tl)
assert 3 (i:tl 1 2)
assert (i:compute 1 2) (i:tl 1 2)


