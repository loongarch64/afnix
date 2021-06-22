# ----------------------------------------------------------------------------
# - EXP0110.als                                                              -
# - afnix example : chapter 1 : example 10                                   -
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

# print the program arguments
const argc (interp:argv:length)
loop (trans i 0) (< i argc) (i:++) {
  trans    arg (interp:argv:get i)
  println "argv[" i "] = " arg
}
