# ---------------------------------------------------------------------------
# - AXI0004.als                                                              -
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

# @info   simple lambda expression test module
# @author amaury darsch

const  one (lambda nil (+ 1 0))
assert 1   ((lambda nil (+ 1 0)))
assert 1   (one)

const  add (x y) (+ x y)
assert 2   ((lambda (x y) (+ x y)) 1 1)
assert 2   (add 1 1)
assert 2   (add 1 1)

const  x 1
assert 2   (+ x 1)
assert 2   (add 1 1)
assert 2   (add x 1)
assert (+ x 1) (add x 1)

