# ---------------------------------------------------------------------------
# - AXI0015.als                                                              -
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

# @info   switch special form test module
# @author amaury darsch

# test with lexical
const  a 1
switch a (
  (1    (assert a 1))
  (else (assert true (!= a 1)))
)

switch a (
  (0    (assert a 0))
  (else (assert true (!= a 0)))
)

switch (+ a 1) (
  (2    (assert a 1))
  (else (assert true (!= a 1)))
)

# test with a symbol
const A 1
switch a (
  (A    (assert a 1))
  (else (assert true (!= a 1)))
)

# test with a nameset
const N (nameset ...)
const N:T 2
switch (+ a 1) (
  (N:T  (assert a 1))
  (else (assert true (!= a 1)))
)

# test with nothing
const n nil
switch n (
  ("nil" (assert n "nil"))
  (else (assert n nil))
)
