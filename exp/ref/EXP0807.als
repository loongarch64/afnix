# ----------------------------------------------------------------------------
# - EXP0807.als                                                              -
# - afnix example : chapter 8 : example 07                                   -
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

# Y combinator with fibonacci

# factorial - as a combinator
const c-fibo (f) (gamma (n) (f) (if (< n 2) n (+ (f (- n 1)) (f (- n 2)))))

# the Y combinator
const Y (f) ((gamma (g) (f) (f (gamma (x) (g) ((g g) x)))) 
             (gamma (g) (f) (f (gamma (x) (g) ((g g) x)))))

# let do the math
println "fibo 10 = " ((Y c-fibo) 10)
