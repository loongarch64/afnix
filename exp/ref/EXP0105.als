# ----------------------------------------------------------------------------
# - EXP0105.als                                                              -
# - afnix example : chapter 1 : example 05                                   -
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

# the famous fibonacci
const fibo (gamma (n) (
  if (< n 2) n (+ (fibo (- n 1)) (fibo (- n 2)))))

# now do it
println "fibo 10 = " (fibo 10)
