# ---------------------------------------------------------------------------
# - AXI0019.als                                                              -
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

# @info   real builtin method
# @author amaury darsch

const r0   0.0
const r1   (Real 1.0)
const r2   (Real 2)
const rn   (Real "-2.0")
const rq   (r2:sqrt)
const pi   3.1415926535
const p2   (/ pi 2)

assert 1.0 (rq:floor)
assert 2.0 (rq:ceiling)
assert 2.0 (rn:abs)
assert 0.0 (r1:ln)
assert 1.0 (r0:exp)
assert 0.0 (r0:sin)
assert 1.0 (r0:cos)
assert 0.0 (r0:tan)
assert 0.0 (r0:asin)
assert 3.0 (pi:floor)
assert 4.0 (pi:ceiling)
assert 3.0 (pi:round)

trans  r   0.0
assert 1.0 (r:++)
assert 0.0 (r:--)
assert 4.0 (r:+ 4.0)
assert 4.0 (r:= 4.0)
assert 3.0 (r:- 1.0)
assert 8.0 (r:* 2.0)
assert 2.0 (r:/ 2.0)
assert 5.0 (r:+= 1.0)
assert 4.0 (r:-= 1.0)
assert 8.0 (r:*= 2.0)
assert 4.0 (r:/= 2.0)
assert 0.0 (r:mod 2.0)
assert 4.0 (r:mod 5.0)

assert 1.0  (r:=  1.0)
assert true (r:== 1.0)
assert true (r:!= 0.0)
assert true (r:>  0.0)
assert true (r:>= 0.0)
assert true (r:<  2.0)
assert true (r:<= 2.0)

