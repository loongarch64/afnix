# ---------------------------------------------------------------------------
# - SIO0006.als                                                             -
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

# @info   afnix:sio pathlist test unit
# @author amaury darsch

# get the module
interp:library "afnix-sio"

# create a default path name
trans  plst (afnix:sio:Pathlist true)
assert true (afnix:sio:pathlist-p plst)

# check the size and local search flag
assert 0    (plst:length)
assert true (plst:local-p)

# check for a local file
assert true (plst:file-p "Makefile")

# disable the local search and check
plst:set-local-search false
assert false (plst:local-p)
assert false (plst:file-p "Makefile")

# add a path component and check
plst:add-path "."
assert 1    (plst:length)
assert true (plst:file-p "Makefile")
