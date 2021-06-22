# ----------------------------------------------------------------------------
# - EXP0804.als                                                              -
# - afnix example : chapter 8 : example 04                                   -
# ----------------------------------------------------------------------------
# - This program is  free software;  you can  redistribute it and/or  modify -
# - it provided that this copyright notice is kept intact.                   -
# -                                                                          -
# - This  program  is  distributed in the hope  that it  will be useful, but -
# - without  any   warranty;  without  even   the   implied    warranty   of -
# - merchantability  or fitness for a particular purpose. In not event shall -
# - the copyright holder be  liable for  any direct, indirect, incidental or -
# - special damages arising in any way out of the use of this software.      -
# ----------------------------------------------------------------------------
# - copyright (c) 1999-2021 amaury darsch                                    -
# ----------------------------------------------------------------------------

# SKI combinators exampl, form transformation (+ x x)

# the S combinator
const S (f) (gamma (g) (f) (gamma (x) (f g) ((f x) (g x))))

# the K combinator
const K (x) (gamma (y) (x) (eval x))

# the I combinator
const I (x) (eval x)

# curried '+' operator
const c+ (x) (gamma (y) (x) (+ x y))

# testing the reduction (+ x x) => (S ((S (K +)) I)) I)
println "((+ x x) 512) = " (((S ((S (K c+)) I)) I) 512)
