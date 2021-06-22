# ----------------------------------------------------------------------------
# - EXP0602.als                                                              -
# - afnix example : chapter 6 : example 02                                   -
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
trans cnt 0

const decr nil {
  while (< (cnt:++) max) (println "decrease:" (var:--))
}

const incr nil {
  while (< (cnt:++) max) (println "increase:" (var:++))
}

# get the maximum value argument
const max (if (== (interp:argv:length) 0) 0xFFFFFFFF 
  (Integer (interp:argv:get 0)))

launch (decr)
launch (incr)
