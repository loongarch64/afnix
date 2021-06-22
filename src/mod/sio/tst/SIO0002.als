# ---------------------------------------------------------------------------
# - SIO0002.als                                                             -
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

# @info   afnix:sio file test unit
# @author amaury darsch

# get the module
interp:library "afnix-sio"

# create a temporary file
const tname (afnix:sio:absolute-path "tmp" (afnix:sio:tmp-name))
trans tfile (afnix:sio:OutputFile tname)

# open the input file and copy
const ifile (afnix:sio:InputFile "SIO0002.als")
const ilen  (ifile:length)
while (ifile:valid-p) (tfile:write (ifile:read))
ifile:close
tfile:close

# reopen the temporary file and grab the length
trans tfile (afnix:sio:InputFile tname)
const tlen  (tfile:length)
tfile:close

# check for size and delete
assert ilen tlen
afnix:sio:rmfile tname


