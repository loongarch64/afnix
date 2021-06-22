# ----------------------------------------------------------------------------
# - EXP0806.als                                                              -
# - afnix example : chapter 8 : example 06                                   -
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

# Y combinator with factorial

# factorial - as a combinator
const c-fact (f) {
  println "creating factorial gamma expression"
  gamma (n) (f) {
    println "evaluating the factorial gamma with n = " n
    if (== n 1) 1 (* n (f (- n 1)))
  }
}

# the Y combinator
const Y (f) ((gamma (g) (f) (f (gamma (x) (g) ((g g) x)))) 
             (gamma (g) (f) (f (gamma (x) (g) ((g g) x)))))

# let do the math
println "fact 5 = " ((Y c-fact) 5)
