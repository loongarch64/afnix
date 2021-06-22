# ---------------------------------------------------------------------------
# - AXI0062.als                                                              -
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

# @info   instance namespace test module
# @author amaury darsch

# class A tester
const  A (class)

trans A:preset nil {
  trans this:xa "xa"
}

# class B tester
const  B (class)

# B inference from A
const  A:infer B

trans B:preset (x y) {
  this:super:preset
  trans this:xb "xb"
}

# this piece of code is the tricky one here
# it detects whether or not the "this" symbol is evaluated
# inside the calling namespace rather that the callee namespace
# this is why afnix is not a call-by-value langage

trans B:new-c nil {
  const c (C this)
  eval c
}

# class C tester
const  C    (class)
assert true (class-p C)

trans C:preset (cb) {
  trans this:cb cb
  assert cb:xb "xb"
  assert cb:xa "xa"
}

# get b instance
const  b   (A 1 2)
assert "xb" b:xb
assert "xa" b:xa

# get c instance
const c (b:new-c)
assert "xb" c:cb:xb
assert "xa" c:cb:xa
