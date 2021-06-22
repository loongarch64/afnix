# ---------------------------------------------------------------------------
# - AXI0003.als                                                              -
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

# @info   integer and real builtin operators test module
# @author amaury darsch

assert 2000   (+ 1999 1)
assert 2000R  (+ 1999R 1)
assert 2000R  (+ 1999R 1R)
assert 2000.0 (+ 1999.0 1)
assert 2000.0 (+ 1999 1.0)
assert 2000.0 (+ 1999.0 1.0)

assert -1999  (- 1999)
assert 1999   (- 2000 1)
assert 1999R  (- 2000R 1)
assert 1999R  (- 2000R 1R)
assert 1999.0 (- 2000.0 1)
assert 1999.0 (- 2000 1.0)
assert 1999.0 (- 2000.0 1.0)

assert 2000   (* 1000 2)
assert 2000R  (* 1000R 2)
assert 2000R  (* 1000R 2R)
assert 2000.0 (* 1000.0 2)
assert 2000.0 (* 1000 2.0)
assert 2000.0 (* 1000.0 2.0)

assert 1000   (/ 2000 2)
assert 1000R  (/ 2000R 2)
assert 1000R  (/ 2000R 2R)
assert 1000.0 (/ 2000.0 2)
assert 1000.0 (/ 2000 2.0)
assert 1000.0 (/ 2000.0 2.0)

assert true   (== 0 0)
assert true   (== 0 0.0)
assert true   (== 0.0 0)
assert true   (== 0.0 0.0)

assert true   (== 2000 2000)
assert true   (== 2000R 2000)
assert true   (== 2000R 2000R)
assert true   (== 2000 2000.0)
assert true   (== 2000.0 2000)
assert true   (== 2000.0 2000.0)

assert true   (!= 2000 1999)
assert true   (!= 2000R 1999)
assert true   (!= 2000R 1999R)
assert true   (!= 2000 1999.0)
assert true   (!= 2000.0 1999)
assert true   (!= 2000.0 1999.0)
 
assert true   (< 1999 2000)
assert true   (< 1999R 2000)
assert true   (< 1999R 2000R)
assert true   (< 1999 2000.0)
assert true   (< 1999.0 2000)
assert true   (< 1999.0 2000.0)

assert true   (<= 1999 2000)
assert true   (<= 1999R 2000)
assert true   (<= 1999R 2000R)
assert true   (<= 1999 2000.0)
assert true   (<= 1999.0 2000)
assert true   (<= 1999.0 2000.0)
assert true   (<= 2000 2000)
assert true   (<= 2000 2000.0)
assert true   (<= 2000.0 2000)
assert true   (<= 2000.0 2000.0)

assert true   (> 2000 1999)
assert true   (> 2000R 1999)
assert true   (> 2000R 1999R)
assert true   (> 2000 1999.0)
assert true   (> 2000.0 1999)
assert true   (> 2000.0 1999.0)
assert true   (>= 2000 2000)
assert true   (>= 2000 2000.0)
assert true   (>= 2000.0 2000)
assert true   (>= 2000.0 2000.0)

# integer test
trans  i     0
assert 1     (i:++)
assert 0     (i:--)
assert 4     (i:+ 4)
assert 4     (i:= 4)
assert 3     (i:- 1)
assert 8     (i:* 2)
assert 2     (i:/ 2)
assert 5     (i:+= 1)
assert 4     (i:-= 1)
assert 8     (i:*= 2)
assert 4     (i:/= 2)

assert 1     (i:=  1)
assert true  (i:== 1)
assert true  (i:!= 0)
assert true  (i:>  0)
assert true  (i:>= 0)
assert true  (i:<  2)
assert true  (i:<= 2)

assert 0     (i:= 0)
assert true  (i:zero-p)
assert 2     (i:= 2)
assert true  (i:even-p)
assert 3     (i:= 3)
assert true  (i:odd-p)
assert 1     (i:mod 2)

assert -1    (i:= -1)
assert  1    (i:abs)
assert "-1"  (i:to-string)
assert "-1"  (i:to-literal)

# relatif test
trans  i     0R
assert 1R    (i:++)
assert 0R    (i:--)
assert 4R    (i:+ 4)
assert 4R    (i:= 4)
assert 3R    (i:- 1)
assert 8R    (i:* 2)
assert 2R    (i:/ 2)
assert 5R    (i:+= 1)
assert 4R    (i:-= 1)
assert 8R    (i:*= 2)
assert 4R    (i:/= 2)

assert 0R    (i:= 0R)
assert true  (i:zero-p)
assert 2R    (i:= 2)
assert true  (i:even-p)
assert 3R    (i:= 3)
assert true  (i:odd-p)
assert 1R    (i:mod 2)

assert -1R   (i:= -1R)
assert  1R   (i:abs)
assert "-1"  (i:to-string)
assert "-1R" (i:to-literal)
