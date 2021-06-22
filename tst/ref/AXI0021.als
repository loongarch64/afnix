# ---------------------------------------------------------------------------
# - AXI0021.als                                                              -
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

# @info   integer builtin method
# @author amaury darsch

const i0  0
const i1  (Integer 1)
const i2  (Integer 2)
const i5  (Integer 5)
const in  (Integer "-2")

assert 2     (in:abs)
assert 1     (i0:++)
assert 0     (i0:--)
assert "0"   (i0:to-string)
assert "5"   (i5:to-string)
assert 1     (i5:mod 2)
assert true  (i0:even-p)
assert false (i5:even-p)
assert true  (i5:odd-p)
assert false (i2:odd-p)
