# ---------------------------------------------------------------------------
# - SIO0008.als                                                             -
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

# @info   afnix:sio file info test unit
# @author amaury darsch

# get the module
interp:library "afnix-sio"

# create a file info object
trans  info (afnix:sio:FileInfo "SIO0008.als")
assert true (afnix:sio:file-info-p info)

# check the size
assert 1441 (info:length)
