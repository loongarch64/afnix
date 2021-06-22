# ----------------------------------------------------------------------------
# - EXP0104.als                                                              -
# - afnix example : chapter 1 : example 04                                   -
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

# a lambda which creates a lambda
const gen (n) (
  lambda (x) (n) (+ x n))

# create a function which add 2 to its argument
const add-2 (gen 2)

# call add-2 with an argument and check
println "result (2+3) = " (add-2 3)
