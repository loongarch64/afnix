# ----------------------------------------------------------------------------
# - EXP0803.als                                                              -
# - afnix example : chapter 8 : example 03                                   -
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

# SKI combinators example = SKK = I

# the K combinator
const K (x) (gamma (y) (x) (eval x))

# the S combinator
const S (f) (gamma (g) (f) (gamma (x) (f g) ((f x) (g x))))

# the test
println "SKK combinators with  0 : result = " (((S K) K) 0)
