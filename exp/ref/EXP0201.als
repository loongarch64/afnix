# ----------------------------------------------------------------------------
# - EXP0201.als                                                              -
# - afnix example : chapter 2 : example 01                                   -
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

# redefine fibo
const fib-it (gamma (n acc1 acc2) (
    if (== n 1) acc2 (fib-it (- n 1) acc2 (+ acc1 acc2))))

const fibo (gamma (n) (
    if (== n 0) 0 (fib-it n 0 1)))

# compute again
println "fibo 10 = " (fibo 10)
