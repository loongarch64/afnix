# ----------------------------------------------------------------------------
# - EXP0203.als                                                              -
# - afnix example : chapter 2 : example 03                                   -
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

# compute string hashid
const hashid (s) {
  const len (s:length)
  trans cnt 0
  trans val 0
  trans sht 17
  do {
    # compute the hash value
    trans i (Integer (s:get cnt))
    val:= (val:xor (i:shl sht))
    # adjust shift index
    if (< (sht:-= 7) 0) (sht:+= 24)
  } (< (cnt:++) len)
  eval val
}

# test our favorite string
const hello "hello world"
println "hello world (builtin) : " (hello:hashid)
println "hello world (afnix)   : " (hashid hello)
