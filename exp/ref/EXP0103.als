# ----------------------------------------------------------------------------
# - EXP0103.als                                                              -
# - afnix example : chapter 1 : example 03                                   -
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

# define the factorial expression
const fact (n) (
  if (== n 1) 1 (* n (fact (- n 1))))

# compute factorial 5
println "factorial 5 = " (fact 5)
