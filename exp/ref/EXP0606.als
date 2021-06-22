# ----------------------------------------------------------------------------
# - EXP0606.als                                                              -
# - afnix example : chapter 6 : example 06                                   -
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

# shared variable access
const var 0

const decr nil (while true (var:= (- var 1)))
const incr nil (while true (var:= (+ var 1)))
const prtv nil (while true (println "value = " var))

launch (prtv)
launch (decr)
launch (incr)
