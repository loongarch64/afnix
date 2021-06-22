# ----------------------------------------------------------------------------
# - EXP0109.als                                                              -
# - afnix example : chapter 1 : example 09                                   -
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

# compute the scalar product of two vectors

const scalar-product (u v) {
  trans result 0
  for (x y) (u v) (result:+= (* x y))
  eval result
}

# define 2 vector
const v1 (Vector 1 2 3)
const v2 (Vector 2 4 6)
println "scalar product [1 2 3][2 4 6] = " (scalar-product v1 v2)
