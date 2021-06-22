# ----------------------------------------------------------------------------
# - EXP0805.als                                                              -
# - afnix example : chapter 8 : example 05                                   -
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

# combinator with factorial

# factorial - as a combinator
const c-fact (f) (gamma (n) (f) (if (== n 1) 1 (* n ((f f) (- n 1)))))

# the U combinator
const U (f) (f f)

# the generated factorial
const fact (U c-fact)

# let do the math
println "fact 5 = " (fact 5)
