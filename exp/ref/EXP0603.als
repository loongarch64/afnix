# ----------------------------------------------------------------------------
# - EXP0603.als                                                              -
# - afnix example : chapter 6 : example 03                                   -
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
const max 1000
trans cnt 0

const decr nil {
  while (< (cnt:++) max) (var:--)
}

const incr nil {
  while (< (cnt:++) max) (var:++)
}

const prtv nil {
  while (< (cnt:++) max) (println "value = " var)
}

launch (prtv)
launch (decr)
launch (incr)
