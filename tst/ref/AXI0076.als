# ---------------------------------------------------------------------------
# - AXI0076.als                                                              -
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

# @info   numeral builtin method
# @author amaury darsch

const i0   (Numeral 0)
assert true (numeral-p i0)
assert "Numeral" (i0:repr)
assert Numeral:INTEGER (i0:get-type)

const t0   (Numeral Numeral:INTEGER)
assert true (numeral-p t0)
assert "Numeral" (t0:repr)
assert Numeral:INTEGER (t0:get-type)
 
const r0   (Numeral 0.0)
const r1   (Numeral 1.0)
const r2   (Numeral 2.0)
const rq   (r2:sqrt)
const pi   (Numeral 3.1415926535)
const p2   (/ pi 2)

assert true (numeral-p r0)
assert "Numeral" (r0:repr)
assert Numeral:FLOAT (r0:get-type)

assert 1.0 (rq:floor)
assert 2.0 (rq:ceiling)
assert 2.0 (r2:abs)
assert 0.0 (r1:ln)
assert 1.0 (r0:exp)
assert 0.0 (r0:sin)
assert 1.0 (r0:cos)
assert 0.0 (r0:tan)
assert 0.0 (r0:asin)
assert 3.0 (pi:floor)
assert 4.0 (pi:ceiling)

assert (Numeral 1.0) (rq:floor)
assert (Numeral 2.0) (rq:ceiling)
assert (Numeral 2.0) (r2:abs)
assert (Numeral 0.0) (r1:ln)
assert (Numeral 1.0) (r0:exp)
assert (Numeral 0.0) (r0:sin)
assert (Numeral 1.0) (r0:cos)
assert (Numeral 0.0) (r0:tan)
assert (Numeral 0.0) (r0:asin)
assert (Numeral 3.0) (pi:floor)
assert (Numeral 4.0) (pi:ceiling)

assert (rq:floor) 1.0
assert (rq:ceiling) 2.0
assert (r2:abs) 2.0
assert (r1:ln) 0.0
assert (r0:exp) 1.0
assert (r0:sin) 0.0
assert (r0:cos) 1.0
assert (r0:tan) 0.0
assert (r0:asin) 0.0
assert (pi:floor) 3.0
assert (pi:ceiling) 4.0

trans  r 0.0
assert true (r:zero-p)
trans  r (Numeral 0.0)
assert true (r:zero-p)

assert (Numeral 1.0) (r:++)
assert (Numeral 0.0) (r:--)
assert (Numeral 4.0) (r:+ 4.0)
assert (Numeral 4.0) (r:= 4.0)
assert (Numeral 3.0) (r:- 1.0)
assert (Numeral 8.0) (r:* 2.0)
assert (Numeral 2.0) (r:/ 2.0)
assert (Numeral 5.0) (r:+= 1.0)
assert (Numeral 4.0) (r:-= 1.0)
assert (Numeral 8.0) (r:*= 2.0)
assert (Numeral 4.0) (r:/= 2.0)
assert (Numeral 0.0) (r:mod 2.0)
assert (Numeral 4.0) (r:mod 5.0)

assert (Numeral 1.0) (r:=  1.0)
assert true (r:== 1.0)
assert true (r:!= 0.0)
assert true (r:>  0.0)
assert true (r:>= 0.0)
assert true (r:<  2.0)
assert true (r:<= 2.0)

# check mapper
const ni (Numeral 1)
assert (ni:to-integer) 1

const nr (Numeral 1.0)
assert (ni:to-real) 1.0
