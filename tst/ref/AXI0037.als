# ---------------------------------------------------------------------------
# - AXI0037.als                                                              -
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

# @info   force and delay test module
# @author amaury darsch

# basic tests
const     a (delay (+ 1 2))
assert true (promise-p a)
assert    3 (force a)

# delay with a lambda
trans   y 3
const   l ((lambda (x) (+ x y)) 1)
assert  4 (force l)
trans   y 0
assert  4 (force l)

# double call
assert  0 (force (delay 0))

# delayed call with evaluation
trans   y 1
trans   d (delay (lambda (x) (+ x 1)) y)
assert  2 (force d)

trans   y 2
trans   z 3
trans   d (delay (lambda (xy xz) (+ xy xz)) y z)
assert  5 (force d)
