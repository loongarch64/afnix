# ---------------------------------------------------------------------------
# - AXI0061.als                                                              -
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

# @info   class inference test module
# @author amaury darsch

# class A tester
const  A    (class)
assert true (class-p A)

trans A:preset (x y) {
  trans this:xa (+ x 1)
  trans this:ya (+ y 1)
}

# check direct creation
const  a     (A 0 0)
assert true  (instance-p a)
assert A     a:meta
assert 1     a:xa
assert 1     a:ya

# class B tester
const  B    (class)
assert true (class-p B)

const  A:infer B

trans B:preset (x y) {
  trans this:xb x
  trans this:yb y
  if (== A this:super:meta) (this:super:preset x y)
}

# check direct creation
const  b     (A 0 0)
assert true  (instance-p b)
assert B     b:meta
assert 0     b:xb
assert 0     b:yb

# check base class
assert true  (instance-p b:super)
assert A     b:super:meta
assert 1     b:xa
assert 1     b:ya
assert 1     b:super:xa
assert 1     b:super:ya
