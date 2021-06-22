# ---------------------------------------------------------------------------
# - AXI0007.als                                                              -
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

# @info   trans closure test module
# @author amaury darsch

trans add (lambda (y) (+ x y))
trans x 1

assert 2 (add 1)
assert 3 (add 2)

# test now the closure

trans gen  (lambda (x) (lambda (y) (x) (+ x y)))
trans mak  (gen 3)
assert 4 (mak 1)
