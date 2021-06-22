# ---------------------------------------------------------------------------
# - AXI0042.als                                                              -
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

# @info   regex test module
# @author amaury darsch

# simple regex with characters
const  r1    (Regex "hello")
assert true  (== r1 "hello")
assert false (== r1 "hello-world")

# simple regex with digit
const  r2    (Regex "$d")
assert true  (== r2 "0")
assert false (== r2 "hello-world")

# simple regex with operator
const  r3    (Regex "$d+")
assert true  (== r3 "0")
assert true  (== r3 "0123456789")
assert false (== r3 "hello")
assert false (== r3 "000-hello")

# simple regex with brackets
const  r4    (Regex "[$d]")
assert true  (== r4 "0")
assert false (== r4 "hello-world")

# simple regex with brackets
const  r5    (Regex "[$d[$d]+]")
assert true  (== r5 "2000")
assert false (== r5 "hello-world")

# another sub-expression
const  r6    (Regex "$d$d[$d$d]+")
assert true  (== r6 2000)
assert false (== r6 20000)

# simple regex with group
const  r7    (Regex "($d$d):($d$d):($d$d)")
assert true  (== r7 "00:12:31")
assert false (== r7 "hello:world:2000")

# simple regex with combining alphanumeric
const r8     (Regex "$c+")
assert true  (== r8 "noël")

