# ---------------------------------------------------------------------------
# - AXI0024.als                                                              -
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

# @info   builtin predicates test module
# @author amaury darsch 

# simple predicate test
assert true  (nil-p       nil)
assert true  (real-p      0.0)
assert true  (string-p    "hello")
assert true  (integer-p   0)
assert true  (boolean-p   true)
assert true  (character-p 'a')

assert true  (real-p      (Real 0.0))
assert true  (string-p    (String "hello"))
assert true  (integer-p   (Integer 0))
assert true  (numeral-p   (Numeral 1))
assert true  (numeral-p   (Numeral 1.0))
assert true  (boolean-p   (Boolean true))
assert true  (character-p (Character 'a'))

assert true  (nameset-p   .)
assert true  (nameset-p   ...)

assert true  (eval-p      .)
assert true  (eval-p      ...)
assert false (eval-p      les-chaussettes-de-l-archiduchesse-sont-elles-seches)

# bind and test
const  x    1
assert x    1
assert true (eval-p x)

unref  x
assert false (eval-p x)
