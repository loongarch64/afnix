# ---------------------------------------------------------------------------
# - SIO0001.als                                                             -
# - afnix:sio module test unit                                              -
# ---------------------------------------------------------------------------
# - This program is free software;  you can redistribute it  and/or  modify -
# - it provided that this copyright notice is kept intact.                  -
# -                                                                         -
# - This program  is  distributed in  the hope  that it will be useful, but -
# - without  any  warranty;  without  even   the   implied    warranty   of -
# - merchantability or fitness for a particular purpose.  In no event shall -
# - the copyright holder be liable for any  direct, indirect, incidental or -
# - special damages arising in any way out of the use of this software.     -
# ---------------------------------------------------------------------------
# - copyright (c) 1999-2021 amaury darsch                                   -
# ---------------------------------------------------------------------------

# @info   afnix:sio directory test unit
# @author amaury darsch

# get the module
interp:library "afnix-sio"

# create a temporary directory
const tmp (afnix:sio:Directory (afnix:sio:absolute-path "tmp"))
const exp (tmp:mkdir "examples")

# test the predicates
assert true (afnix:sio:directory-p tmp)
assert true (afnix:sio:directory-p exp)

# check the directories
assert true (afnix:sio:dir-p (tmp:get-name))
assert true (afnix:sio:dir-p (exp:get-name))

# check for files
const lsf (exp:get-files)
assert  0 (lsf:length)
tmp:rmdir "examples"

